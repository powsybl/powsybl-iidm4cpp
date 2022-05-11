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
#include <powsybl/iidm/extensions/iidm/BranchObservabilityXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/CoordinatedReactiveControlXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurementsXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorShortCircuitXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/HvdcAngleDroopActivePowerControlXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/HvdcOperatorActivePowerRangeXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/IdentifiableShortCircuitXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/InjectionObservabilityXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/MeasurementsXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerPhaseAngleClockXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerToBeEstimatedXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerPhaseAngleClockXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerToBeEstimatedXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/VoltagePerReactivePowerControlXmlSerializer.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

std::vector<std::unique_ptr<ExtensionProvider>> create() {
    std::vector<std::unique_ptr<ExtensionProvider>> serializers;
    serializers.emplace_back(stdcxx::make_unique<ActivePowerControlXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<BranchObservabilityXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<CoordinatedReactiveControlXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<DiscreteMeasurementsXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<GeneratorShortCircuitXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<HvdcAngleDroopActivePowerControlXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<HvdcOperatorActivePowerRangeXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<IdentifiableShortCircuitXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<InjectionObservabilityXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<MeasurementsXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<ThreeWindingsTransformerPhaseAngleClockXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<ThreeWindingsTransformerToBeEstimatedXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<TwoWindingsTransformerPhaseAngleClockXmlSerializer>());
    serializers.emplace_back(stdcxx::make_unique<TwoWindingsTransformerToBeEstimatedXmlSerializer>());
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
