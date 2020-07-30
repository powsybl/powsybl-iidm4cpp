/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NetworkFactory.hpp"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>

namespace powsybl {

namespace iidm {

static Network network2("test2", "test2");

Network createHvdcConverterStationTestNetwork() {
    Network network("test", "test");
    Substation& substation = network.newSubstation()
        .setId("S1")
        .setName("S1_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setName("VL1_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl1Bus1 = vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS1")
        .add();

    vl1.newLccConverterStation()
        .setId("LCC1")
        .setName("LCC1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setLossFactor(1.0)
        .setPowerFactor(1.0)
        .add();

    vl1.newVscConverterStation()
        .setId("VSC1")
        .setName("VSC1_NAME")
        .setBus(vl1Bus1.getId())
        .setConnectableBus(vl1Bus1.getId())
        .setLossFactor(3.0)
        .setVoltageRegulatorOn(true)
        .setVoltageSetpoint(4.0)
        .setReactivePowerSetpoint(5.0)
        .add();

    return network;
}

Network createNetwork() {
    Network network("test", "test");
    Substation& substation = network.newSubstation()
        .setId("S1")
        .setName("S1_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl1 = substation.newVoltageLevel()
        .setId("VL1")
        .setName("VL1_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    vl1.getBusBreakerView().newBus()
        .setId("VL1_BUS1")
        .add();

    vl1.newLoad()
        .setId("LOAD1")
        .setBus("VL1_BUS1")
        .setConnectableBus("VL1_BUS1")
        .setName("LOAD1_NAME")
        .setLoadType(LoadType::UNDEFINED)
        .setP0(50.0)
        .setQ0(40.0)
        .add();

    substation.newVoltageLevel()
        .setId("VL2")
        .setName("VL2_NAME")
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .setNominalVoltage(225.0)
        .setLowVoltageLimit(200.0)
        .setHighVoltageLimit(260.0)
        .add();

    return network;
}

Terminal& getTerminalFromNetwork2() {
    if(network2.getSubstationCount() == 0) {
        Substation& s = network2.newSubstation()
            .setId("S")
            .setCountry(Country::FR)
            .add();

        VoltageLevel& vl = s.newVoltageLevel()
            .setId("VL")
            .setTopologyKind(TopologyKind::NODE_BREAKER)
            .setNominalVoltage(400.0)
            .setLowVoltageLimit(380.0)
            .setHighVoltageLimit(420.0)
            .add();

        vl.newLoad()
            .setId("LOAD1")
            .setNode(0)
            .setName("LOAD1_NAME")
            .setLoadType(LoadType::UNDEFINED)
            .setP0(50.0)
            .setQ0(40.0)
            .add();
    }

    return network2.getLoad("LOAD1").getTerminal();
}

}  // namespace iidm

}  // namespace powsybl
