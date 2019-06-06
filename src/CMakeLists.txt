#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(IIDM_SOURCES
    AssertionError.cpp
    PowsyblException.cpp

    iidm/Battery.cpp
    iidm/BatteryAdder.cpp
    iidm/Branch.cpp
    iidm/Bus.cpp
    iidm/BusAdder.cpp
    iidm/BusbarSection.cpp
    iidm/BusbarSectionAdder.cpp
    iidm/BusBreakerVoltageLevel.cpp
    iidm/BusBreakerVoltageLevelBusCache.cpp
    iidm/BusBreakerVoltageLevelTopology.cpp
    iidm/BusBreakerVoltageLevelVariant.cpp
    iidm/BusBreakerVoltageLevelViews.cpp
    iidm/BusTerminal.cpp
    iidm/BusTerminalViews.cpp
    iidm/CalculatedBus.cpp
    iidm/ConfiguredBus.cpp
    iidm/Connectable.cpp
    iidm/ConnectableType.cpp
    iidm/Container.cpp
    iidm/Country.cpp
    iidm/CurrentLimits.cpp
    iidm/DanglingLine.cpp
    iidm/DanglingLineAdder.cpp
    iidm/Generator.cpp
    iidm/GeneratorAdder.cpp
    iidm/HvdcConverterStation.cpp
    iidm/HvdcLine.cpp
    iidm/HvdcLineAdder.cpp
    iidm/Identifiable.cpp
    iidm/Injection.cpp
    iidm/InternalConnectionAdder.cpp
    iidm/LccConverterStation.cpp
    iidm/LccConverterStationAdder.cpp
    iidm/Line.cpp
    iidm/LineAdder.cpp
    iidm/LineCharacteristics.cpp
    iidm/Load.cpp
    iidm/LoadAdder.cpp
    iidm/LoadType.cpp
    iidm/MergedBus.cpp
    iidm/MinMaxReactiveLimits.cpp
    iidm/MinMaxReactiveLimitsAdder.cpp
    iidm/MultipleVariantContext.cpp
    iidm/Network.cpp
    iidm/NetworkIndex.cpp
    iidm/NodeBreakerVoltageLevel.cpp
    iidm/NodeBreakerVoltageLevelBusCache.cpp
    iidm/NodeBreakerVoltageLevelBusNamingStrategy.cpp
    iidm/NodeBreakerVoltageLevelTopology.cpp
    iidm/NodeBreakerVoltageLevelVariant.cpp
    iidm/NodeBreakerVoltageLevelViews.cpp
    iidm/NodeTerminal.cpp
    iidm/NodeTerminalViews.cpp
    iidm/PhaseTapChanger.cpp
    iidm/PhaseTapChangerAdder.cpp
    iidm/PhaseTapChangerStep.cpp
    iidm/Properties.cpp
    iidm/RatioTapChanger.cpp
    iidm/RatioTapChangerAdder.cpp
    iidm/RatioTapChangerStep.cpp
    iidm/ReactiveCapabilityCurve.cpp
    iidm/ReactiveCapabilityCurveAdder.cpp
    iidm/ReactiveLimitsHolder.cpp
    iidm/ShuntCompensator.cpp
    iidm/ShuntCompensatorAdder.cpp
    iidm/StaticVarCompensator.cpp
    iidm/StaticVarCompensatorAdder.cpp
    iidm/Substation.cpp
    iidm/SubstationAdder.cpp
    iidm/Switch.cpp
    iidm/SwitchAdder.cpp
    iidm/Terminal.cpp
    iidm/TerminalBuilder.cpp
    iidm/ThreeWindingsTransformer.cpp
    iidm/ThreeWindingsTransformerAdder.cpp
    iidm/ThreeWindingsTransformerLeg2.cpp
    iidm/ThreeWindingsTransformerLeg3.cpp
    iidm/TieLine.cpp
    iidm/TieLineAdder.cpp
    iidm/TopologyKind.cpp
    iidm/TwoWindingsTransformer.cpp
    iidm/TwoWindingsTransformerAdder.cpp
    iidm/ValidationException.cpp
    iidm/ValidationUtils.cpp
    iidm/VariantContextGuard.cpp
    iidm/VariantManager.cpp
    iidm/VoltageLevel.cpp
    iidm/VoltageLevelAdder.cpp
    iidm/VscConverterStation.cpp
    iidm/VscConverterStationAdder.cpp
    iidm/util/LimitViolationUtils.cpp

    logging/ConsoleLogger.cpp
    logging/ContainerLogger.cpp
    logging/Level.cpp
    logging/LoggerFactory.cpp
    logging/LogMessage.cpp
    logging/NoopLogger.cpp

    network/EurostagFactory.cpp

    stdcxx/DateTime.cpp
    stdcxx/demangle.cpp
    stdcxx/put_time.cpp
)

add_library(iidm ${IIDM_SOURCES})
target_include_directories(iidm
    PRIVATE ${CMAKE_HOME_DIRECTORY}/include
)
