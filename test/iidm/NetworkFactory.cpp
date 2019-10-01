/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NetworkFactory.hpp"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
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
        .setPowerFactor(2.0)
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

    Substation& substation2 = network.newSubstation()
        .setId("S2")
        .setName("S2_NAME")
        .setCountry(Country::FR)
        .setTso("TSO")
        .add();

    VoltageLevel& vl2 = substation2.newVoltageLevel()
        .setId("VL2")
        .setName("VL2_NAME")
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .setNominalVoltage(380.0)
        .setLowVoltageLimit(340.0)
        .setHighVoltageLimit(420.0)
        .add();

    Bus& vl2Bus1 = vl2.getBusBreakerView().newBus()
        .setId("VL2_BUS1")
        .add();

    vl2.newLccConverterStation()
        .setId("LCC2")
        .setName("LCC2_NAME")
        .setBus(vl2Bus1.getId())
        .setConnectableBus(vl2Bus1.getId())
        .setLossFactor(6.0)
        .setPowerFactor(7.0)
        .add();

    vl2.newVscConverterStation()
        .setId("VSC2")
        .setName("VSC2_NAME")
        .setBus(vl2Bus1.getId())
        .setConnectableBus(vl2Bus1.getId())
        .setLossFactor(8.0)
        .setVoltageRegulatorOn(true)
        .setVoltageSetpoint(9.0)
        .setReactivePowerSetpoint(10.0)
        .add();

    network.newHvdcLine()
        .setId("HVDC1")
        .setName("HVDC1_NAME")
        .setActivePowerSetpoint(11.0)
        .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_RECTIFIER_SIDE_2_INVERTER)
        .setConverterStationId1("LCC1")
        .setConverterStationId2("LCC2")
        .setMaxP(12.0)
        .setNominalVoltage(13.0)
        .setR(14.0)
        .add();

    network.newHvdcLine()
        .setId("HVDC2")
        .setName("HVDC2_NAME")
        .setActivePowerSetpoint(15.0)
        .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER)
        .setConverterStationId1("VSC1")
        .setConverterStationId2("VSC2")
        .setMaxP(16.0)
        .setNominalVoltage(17.0)
        .setR(18.0)
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

        vl.getNodeBreakerView().setNodeCount(1);

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
