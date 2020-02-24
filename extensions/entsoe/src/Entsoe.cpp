/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/dll/alias.hpp>

#include <powsybl/iidm/extensions/entsoe/EntsoeAreaXmlSerializer.hpp>
#include <powsybl/iidm/extensions/entsoe/MergedXnodeXmlSerializer.hpp>
#include <powsybl/iidm/extensions/entsoe/XnodeXmlSerializer.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

const std::initializer_list<std::string>& getNames() {
    static std::initializer_list<std::string> s_extensionNames = {
        EntsoeAreaXmlSerializer().getExtensionName(),
        MergedXnodeXmlSerializer().getExtensionName(),
        XnodeXmlSerializer().getExtensionName()
    };
    return s_extensionNames;
}

// NOLINTNEXTLINE BOOST_DLL_ALIAS uses reinterpret_cast
BOOST_DLL_ALIAS(
    getNames,
    getExtensionsNames
)

std::unique_ptr<ExtensionProvider> createSerializer(const std::string& extensionName) {
    if (extensionName == EntsoeAreaXmlSerializer().getExtensionName()) {
        return stdcxx::make_unique<EntsoeAreaXmlSerializer>();
    }
    if (extensionName == MergedXnodeXmlSerializer().getExtensionName()) {
        return stdcxx::make_unique<MergedXnodeXmlSerializer>();
    }
    if (extensionName == XnodeXmlSerializer().getExtensionName()) {
        return stdcxx::make_unique<XnodeXmlSerializer>();
    }

    throw powsybl::PowsyblException(logging::format("Unknown provider for extension %1%", extensionName));
}

// NOLINTNEXTLINE BOOST_DLL_ALIAS uses reinterpret_cast
BOOST_DLL_ALIAS(
    createSerializer,
    create
)

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

