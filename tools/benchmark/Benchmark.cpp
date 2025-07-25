/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <regex>

#include <boost/algorithm/string.hpp>
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/dll/shared_library.hpp>
#include <boost/program_options.hpp>

#include <libxml/parser.h>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>
#include <powsybl/logging/ConsoleLogger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

/**
 * Helper class to load/unload properly LibXml2
 */
class LibXml2 {
public:
    LibXml2() {
        xmlInitParser();
    }

    ~LibXml2() {
        xmlCleanupParser();
    }
};

void loadExtensions(const std::string& strPaths) {
    std::regex fileRegex(stdcxx::format(".*\\%1%.*", boost::dll::shared_library::suffix().string()));

    // Load extensions from libraries found in the same directory that this binary
    powsybl::iidm::ExtensionProviders<powsybl::iidm::converter::xml::ExtensionXmlSerializer>::getInstance().loadExtensions(boost::dll::program_location().parent_path().string(), fileRegex);

    if (!strPaths.empty()) {
#if defined(BOOST_WINDOWS_API)
        char pathSeparator = ';';
#elif defined(BOOST_POSIX_API)
        char pathSeparator = ':';
#endif

        std::vector<std::string> paths;
        boost::algorithm::split(paths, strPaths, [pathSeparator](char c) { return c == pathSeparator; });
        for (const auto& path : paths) {
            powsybl::iidm::ExtensionProviders<powsybl::iidm::converter::xml::ExtensionXmlSerializer>::getInstance().loadExtensions(path, fileRegex);
        }
    }
}

void readOptions(stdcxx::Properties& properties, const boost::program_options::variable_value& options) {
    if (!options.empty()) {
        for (const auto& property : options.as<std::vector<std::string>>()) {
            std::vector<std::string> result;
            boost::split(result, property, boost::is_any_of("="));
            if (result.size() == 2) {
                properties.set(result[0], result[1]);
            } else {
                throw powsybl::PowsyblException(stdcxx::format("Malformed option: %1%", property));
            }
        }
    }
}

int main(int argc, char** argv) {
    const char* const INPUT_FILE = "input-file";
    const char* const OUTPUT_FILE = "output-file";
    const char* const EXT_PATH = "ext-path";
    const char* const EXPORT_OPTION = "-E";
    const char* const IMPORT_OPTION = "-I";

    boost::program_options::options_description desc("Options");
    desc.add_options()
        (INPUT_FILE, boost::program_options::value<std::string>()->required())
        (OUTPUT_FILE, boost::program_options::value<std::string>()->required())
        (EXT_PATH, boost::program_options::value<std::string>()->default_value(""))
        (",E", boost::program_options::value<std::vector<std::string>>())
        (",I", boost::program_options::value<std::vector<std::string>>());

    try {
        // Use the RAII to load/unload LibXml2
        LibXml2 libxml2;

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        stdcxx::Properties options;
        readOptions(options, vm[EXPORT_OPTION]);
        readOptions(options, vm[IMPORT_OPTION]);

        // Setup the logger
        auto logger = stdcxx::make_unique<powsybl::logging::ConsoleLogger>(powsybl::logging::Level::DEBUG);
        powsybl::logging::LoggerFactory::getInstance().addLogger("powsybl::iidm::converter::xml::NetworkXml", std::move(logger));

        // Load the XIIDM extensions
        loadExtensions(vm[EXT_PATH].as<std::string>());

        const powsybl::iidm::Network& network = powsybl::iidm::Network::readXml(vm[INPUT_FILE].as<std::string>(), powsybl::iidm::converter::ImportOptions(options));
        powsybl::iidm::Network::writeXml(vm[OUTPUT_FILE].as<std::string>(), network, powsybl::iidm::converter::ExportOptions(options));

    } catch (const boost::program_options::error& e) {
        std::cerr << "ERROR: " << e.what() << "\n\n";
        std::cerr << desc << '\n' << std::flush;
        return EXIT_FAILURE;
    } catch (const powsybl::PowsyblException& e) {
        std::cerr << "ERROR: " << e.what() << "\n\n" << std::flush;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n\n" << std::flush;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "ERROR: unexpected exception" << "\n\n" << std::flush;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

