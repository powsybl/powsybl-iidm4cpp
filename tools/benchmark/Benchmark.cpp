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

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Network.hpp>
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

int main(int argc, char** argv) {
    const char* const INPUT_FILE = "input-file";
    const char* const OUTPUT_FILE = "output-file";
    const char* const EXT_PATH = "ext-path";

    boost::program_options::options_description desc("Options");
    desc.add_options()
        (INPUT_FILE, boost::program_options::value<std::string>()->required())
        (OUTPUT_FILE, boost::program_options::value<std::string>()->required())
        (EXT_PATH, boost::program_options::value<std::string>()->default_value(""));

    try {
        // Use the RAII to load/unload LibXml2
        LibXml2 libxml2;

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        // Setup the logger
        auto logger = stdcxx::make_unique<powsybl::logging::ConsoleLogger>(powsybl::logging::Level::DEBUG);
        powsybl::logging::LoggerFactory::getInstance().addLogger("powsybl::iidm::converter::xml::NetworkXml", std::move(logger));

        // Load the XIIDM extensions
        loadExtensions(vm[EXT_PATH].as<std::string>());

        const auto& inputFile = vm[INPUT_FILE].as<std::string>();
        std::ifstream inputStream(inputFile);
        if (!inputStream.is_open()) {
            std::cerr << stdcxx::format("Unable to open file '%1%' for reading", inputFile) << std::endl;
            return EXIT_FAILURE;
        }

        const auto& outputFile = vm[OUTPUT_FILE].as<std::string>();
        std::ofstream outputStream(outputFile);
        if (!outputStream.is_open()) {
            std::cerr << stdcxx::format("Unable to open file '%1%' for writing", outputFile) << std::endl;
            return EXIT_FAILURE;
        }

        const powsybl::iidm::Network& network = powsybl::iidm::Network::readXml(inputStream);
        powsybl::iidm::Network::writeXml(outputStream, network);

        inputStream.close();
        outputStream.close();
    } catch (const boost::program_options::error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
        std::cerr << desc << std::endl;
        return EXIT_FAILURE;
    } catch (const powsybl::PowsyblException& e) {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "ERROR: unexpected exception" << std::endl << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

