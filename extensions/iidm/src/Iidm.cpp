/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <memory>
#include <vector>

#include <boost/dll/alias.hpp>

#include <powsybl/iidm/extensions/iidm/ActivePowerControlXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/CoordinatedReactiveControlXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/HvdcAngleDroopActivePowerControlXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/HvdcOperatorActivePowerRangeXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerPhaseAngleClockXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerPhaseAngleClockXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/VoltagePerReactivePowerControlXmlSerializer.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

std::vector<std::unique_ptr<ExtensionProvider>> create() {
    std::vector<std::unique_ptr<ExtensionProvider>> serializers;
    serializers.emplace_back(stdcxx::make_unique<ActivePowerControlXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<CoordinatedReactiveControlXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<HvdcAngleDroopActivePowerControlXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<HvdcOperatorActivePowerRangeXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<ThreeWindingsTransformerPhaseAngleClockXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<TwoWindingsTransformerPhaseAngleClockXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<VoltagePerReactivePowerControlXmlSerializer>());

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
