#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(IIDM_SOURCES
    PowsyblException.cpp

    iidm/Branch.cpp
    iidm/Bus.cpp
    iidm/BusAdder.cpp
    iidm/BusbarSection.cpp
    iidm/BusbarSectionAdder.cpp
    iidm/BusBreakerVoltageLevel.cpp
    iidm/BusTerminal.cpp
    iidm/CalculatedBus.cpp
    iidm/ConfiguredBus.cpp
    iidm/Connectable.cpp
    iidm/ConnectableType.cpp
    iidm/Container.cpp
    iidm/Country.cpp
    iidm/DanglingLine.cpp
    iidm/Generator.cpp
    iidm/HvdcConverterStation.cpp
    iidm/Identifiable.cpp
    iidm/Injection.cpp
    iidm/LccConverterStation.cpp
    iidm/Line.cpp
    iidm/Load.cpp
    iidm/LoadAdder.cpp
    iidm/MergedBus.cpp
    iidm/MultipleStateContext.cpp
    iidm/Network.cpp
    iidm/NodeBreakerVoltageLevel.cpp
    iidm/NodeTerminal.cpp
    iidm/ObjectStore.cpp
    iidm/ShuntCompensator.cpp
    iidm/StateManager.cpp
    iidm/StaticVarCompensator.cpp
    iidm/Substation.cpp
    iidm/SubstationAdder.cpp
    iidm/Switch.cpp
    iidm/Terminal.cpp
    iidm/ThreeWindingsTransformer.cpp
    iidm/TieLine.cpp
    iidm/TwoWindingsTransformer.cpp
    iidm/ValidationException.cpp
    iidm/ValidationUtils.cpp
    iidm/VoltageLevel.cpp
    iidm/VoltageLevelAdder.cpp
    iidm/VscConverterStation.cpp

    logging/ConsoleLogger.cpp
    logging/Level.cpp
    logging/LoggerFactory.cpp
    logging/NoopLogger.cpp

    stdcxx/demangle.cpp)

add_library(iidm ${IIDM_SOURCES})
