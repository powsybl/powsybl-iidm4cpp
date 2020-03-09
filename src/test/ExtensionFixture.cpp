/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/test/ExtensionFixture.hpp>

#include <regex>

#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/dll/shared_library.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>

namespace powsybl {

namespace test {

ExtensionFixture::ExtensionFixture() {
    std::regex fileRegex(logging::format(".*\\%1%.*", boost::dll::shared_library::suffix().string()));
    iidm::ExtensionProviders<iidm::converter::xml::ExtensionXmlSerializer>::getInstance().loadExtensions(boost::dll::program_location().parent_path().string(), fileRegex);
}

}  // namespace test

}  // namespace powsybl

