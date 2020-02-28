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

std::map<std::string, std::unique_ptr<ExtensionProvider>> create() {
    std::map<std::string, std::unique_ptr<ExtensionProvider>> serializers;
    serializers.emplace(std::make_pair(EntsoeAreaXmlSerializer().getExtensionName(), stdcxx::make_unique<EntsoeAreaXmlSerializer>()));
    serializers.emplace(std::make_pair(MergedXnodeXmlSerializer().getExtensionName(), stdcxx::make_unique<MergedXnodeXmlSerializer>()));
    serializers.emplace(std::make_pair(XnodeXmlSerializer().getExtensionName(), stdcxx::make_unique<XnodeXmlSerializer>()));
    return serializers;
}

// NOLINTNEXTLINE BOOST_DLL_ALIAS uses reinterpret_cast
BOOST_DLL_ALIAS(
    create,
    createSerializers
)

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

