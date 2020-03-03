/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/dll/alias.hpp>

#include <powsybl/iidm/extensions/iidm/ActivePowerControlXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/CoordinatedReactiveControlXmlSerializer.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

std::set<std::unique_ptr<ExtensionProvider>> create() {
    std::set<std::unique_ptr<ExtensionProvider>> serializers;
    serializers.insert(stdcxx::make_unique<ActivePowerControlXmlSerializer>());
    serializers.insert(stdcxx::make_unique<CoordinatedReactiveControlXmlSerializer>());
    return serializers;
}

// NOLINTNEXTLINE BOOST_DLL_ALIAS uses reinterpret_cast
BOOST_DLL_ALIAS(
    create,
    createSerializers
)

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

