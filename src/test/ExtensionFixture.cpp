/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/test/ExtensionFixture.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>

namespace powsybl {

namespace test {

ExtensionFixture::ExtensionFixture() {
    boost::program_options::options_description desc;
    desc.add_options()
        ("resources", boost::program_options::value<std::string>()->required(),
         "Path where the test resources are stored")
        ("ext-path", boost::program_options::value<std::string>()->required(),
         "Path where extensions are stored")
        ("files", boost::program_options::value<std::string>()->required(),
         "Identify files to be loaded in the directory");

    parse(desc);

    if (!getOptionValue("ext-path").as<std::string>().empty()) {
        const std::string& directory = getOptionValue("ext-path").as<std::string>();
        const std::string& filesRegex = getOptionValue("files").as<std::string>();
        iidm::ExtensionProviders<iidm::converter::xml::ExtensionXmlSerializer>::addExtensions(directory, boost::regex(filesRegex));
    }
}

}  // namespace test

}  // namespace powsybl

