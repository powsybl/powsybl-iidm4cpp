/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/network/DanglingLineNetworkFactory.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Country.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

namespace powsybl {

namespace network {

iidm::Network DanglingLineNetworkFactory::create() {
    iidm::Network network = createBase();
    iidm::DanglingLine& danglingLine = network.getVoltageLevel("VL").newDanglingLine()
                .setId("DL")
                .setBus("BUS")
                .setR(10.0)
                .setX(1.0)
                .setB(10e-6)
                .setG(10e-5)
                .setP0(50.0)
                .setQ0(30.0)
                .add();
    createDanglingLineCurrentLimits(danglingLine);
    return network;
}

iidm::Network DanglingLineNetworkFactory::createWithGeneration() {
    iidm::Network network = createBase();
    iidm::DanglingLine& danglingLine = network.getVoltageLevel("VL").newDanglingLine()
                .setId("DL")
                .setBus("BUS")
                .setR(10.0)
                .setX(1.0)
                .setB(10e-6)
                .setG(10e-5)
                .setP0(50.0)
                .setQ0(30.0)
                .newGeneration()
                    .setTargetP(440)
                    .setMaxP(900)
                    .setMinP(0)
                    .setTargetV(101)
                    .setVoltageRegulationOn(true)
                    .add()
                .add();
    danglingLine.getGeneration().get().newReactiveCapabilityCurve()
                .beginPoint()
                    .setP(0.0)
                    .setMinQ(-59.3)
                    .setMaxQ(60.0)
                .endPoint()
                .beginPoint()
                    .setP(70.0)
                    .setMinQ(-54.55)
                    .setMaxQ(46.25)
                .endPoint()
                .add();
    createDanglingLineCurrentLimits(danglingLine);
    return network;
}

iidm::Network DanglingLineNetworkFactory::createBase() {
    iidm::Network network("fictitious", "test");

    iidm::Substation& substation = network.newSubstation()
                .setId("S")
                .setCountry(iidm::Country::FR)
                .add();
    iidm::VoltageLevel& voltageLevel = substation.newVoltageLevel()
                .setId("VL")
                .setNominalV(100.0)
                .setLowVoltageLimit(80.0)
                .setHighVoltageLimit(120.0)
                .setTopologyKind(iidm::TopologyKind::BUS_BREAKER)
                .add();
    voltageLevel.getBusBreakerView().newBus()
                .setId("BUS")
                .add();
    voltageLevel.newGenerator()
                .setId("G")
                .setMinP(0.0)
                .setMaxP(100.0)
                .setVoltageRegulatorOn(true)
                .setTargetV(100.0)
                .setTargetP(50.0)
                .setTargetQ(30.0)
                .setBus("BUS")
                .add();
    return network;
}

void DanglingLineNetworkFactory::createDanglingLineCurrentLimits(iidm::DanglingLine& danglingLine) {
    danglingLine.newCurrentLimits()
                .setPermanentLimit(100.0)
                .beginTemporaryLimit()
                .setName("20'")
                .setValue(120.0)
                .setAcceptableDuration(20 * 60)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("10'")
                .setValue(140.0)
                .setAcceptableDuration(10 * 60)
                .endTemporaryLimit()
                .add();
}

}  // namespace network

}  // namespace powsybl
