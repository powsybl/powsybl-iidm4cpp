/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <memory>
#include <vector>

#include <boost/dll/alias.hpp>

#include <powsybl/iidm/extensions/sld/BusbarSectionPositionXmlSerializer.hpp>
#include <powsybl/iidm/extensions/sld/ConnectablePositionXmlSerializer.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

std::vector<std::unique_ptr<ExtensionProvider>> create() {
    std::vector<std::unique_ptr<ExtensionProvider>> serializers;
    serializers.emplace_back(stdcxx::make_unique<BusbarSectionPositionXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<ConnectablePositionXmlSerializer>());

    return serializers;
}

// NOLINTNEXTLINE BOOST_DLL_ALIAS uses reinterpret_cast
BOOST_DLL_ALIAS(
    create,
    createSerializers
)

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
