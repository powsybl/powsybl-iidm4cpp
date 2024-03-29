/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/network/EurostagFactory.hpp>

#include <powsybl/iidm/ActivePowerLimits.hpp>
#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimits.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>

namespace powsybl {

namespace network {

iidm::Network EurostagFactory::createTutorial1Network() {
    iidm::Network network("sim1", "test");
    iidm::Substation& p1 = network.newSubstation()
        .setId("P1")
        .setCountry(iidm::Country::FR)
        .setTso("RTE")
        .setGeographicalTags({"A"})
        .add();
    iidm::Substation& p2 = network.newSubstation()
        .setId("P2")
        .setCountry(iidm::Country::FR)
        .setTso("RTE")
        .setGeographicalTags({"B"})
        .add();
    iidm::VoltageLevel& vlgen = p1.newVoltageLevel()
        .setId("VLGEN")
        .setNominalV(24.0)
        .setTopologyKind(iidm::TopologyKind::BUS_BREAKER)
        .add();
    iidm::VoltageLevel& vlhv1 = p1.newVoltageLevel()
        .setId("VLHV1")
        .setNominalV(380.0)
        .setTopologyKind(iidm::TopologyKind::BUS_BREAKER)
        .add();
    iidm::VoltageLevel& vlhv2 = p2.newVoltageLevel()
        .setId("VLHV2")
        .setNominalV(380.0)
        .setTopologyKind(iidm::TopologyKind::BUS_BREAKER)
        .add();
    iidm::VoltageLevel& vlload = p2.newVoltageLevel()
        .setId("VLLOAD")
        .setNominalV(150.0)
        .setTopologyKind(iidm::TopologyKind::BUS_BREAKER)
        .add();
    iidm::Bus& ngen = vlgen.getBusBreakerView().newBus()
        .setId("NGEN")
        .add();
    iidm::Bus& nhv1 = vlhv1.getBusBreakerView().newBus()
        .setId("NHV1")
        .add();
    iidm::Bus& nhv2 = vlhv2.getBusBreakerView().newBus()
        .setId("NHV2")
        .add();
    iidm::Bus& nload = vlload.getBusBreakerView().newBus()
        .setId("NLOAD")
        .add();
    iidm::LineAdder adder = network.newLine()
        .setVoltageLevel1(vlhv1.getId())
        .setBus1(nhv1.getId())
        .setConnectableBus1(nhv1.getId())
        .setVoltageLevel2(vlhv2.getId())
        .setBus2(nhv2.getId())
        .setConnectableBus2(nhv2.getId())
        .setR(3.0)
        .setX(33.0)
        .setG1(0.0)
        .setB1(386E-6 / 2)
        .setG2(0.0)
        .setB2(386E-6 / 2);
    adder.setId("NHV1_NHV2_1").add();
    adder.setId("NHV1_NHV2_2").add();

    int zb380 = 380 * 380 / 100;
    p1.newTwoWindingsTransformer()
        .setId("NGEN_NHV1")
        .setVoltageLevel1(vlgen.getId())
        .setBus1(ngen.getId())
        .setConnectableBus1(ngen.getId())
        .setRatedU1(24.0)
        .setVoltageLevel2(vlhv1.getId())
        .setBus2(nhv1.getId())
        .setConnectableBus2(nhv1.getId())
        .setRatedU2(400.0)
        .setR(0.24 / 1300 * zb380)
        .setX(std::sqrt(10 * 10 - 0.24 * 0.24) / 1300 * zb380)
        .setG(0.0)
        .setB(0.0)
        .add();
    int zb150 = 150 * 150 / 100;
    iidm::TwoWindingsTransformer& nhv2Nload = p2.newTwoWindingsTransformer()
        .setId("NHV2_NLOAD")
        .setVoltageLevel1(vlhv2.getId())
        .setBus1(nhv2.getId())
        .setConnectableBus1(nhv2.getId())
        .setRatedU1(400.0)
        .setVoltageLevel2(vlload.getId())
        .setBus2(nload.getId())
        .setConnectableBus2(nload.getId())
        .setRatedU2(158.0)
        .setR(0.21 / 1000 * zb150)
        .setX(std::sqrt(18 * 18 - 0.21 * 0.21) / 1000 * zb150)
        .setG(0.0)
        .setB(0.0)
        .add();
    double a = (158.0 / 150.0) / (400.0 / 380.0);
    nhv2Nload.newRatioTapChanger()
        .beginStep()
        .setRho(0.85 * a)
        .setR(0.0)
        .setX(0.0)
        .setG(0.0)
        .setB(0.0)
        .endStep()
        .beginStep()
        .setRho(a)
        .setR(0.0)
        .setX(0.0)
        .setG(0.0)
        .setB(0.0)
        .endStep()
        .beginStep()
        .setRho(1.15 * a)
        .setR(0.0)
        .setX(0.0)
        .setG(0.0)
        .setB(0.0)
        .endStep()
        .setTapPosition(1)
        .setLoadTapChangingCapabilities(true)
        .setRegulating(true)
        .setTargetV(158.0)
        .setTargetDeadband(0.0)
        .setRegulationTerminal(stdcxx::ref(nhv2Nload.getTerminal2()))
        .add();

    vlload.newLoad()
        .setId("LOAD")
        .setBus(nload.getId())
        .setConnectableBus(nload.getId())
        .setP0(600.0)
        .setQ0(200.0)
        .add();
    iidm::Generator& generator = vlgen.newGenerator()
        .setId("GEN")
        .setBus(ngen.getId())
        .setConnectableBus(ngen.getId())
        .setMinP(-9999.99)
        .setMaxP(9999.99)
        .setVoltageRegulatorOn(true)
        .setTargetV(24.5)
        .setTargetP(607.0)
        .setTargetQ(301.0)
        .add();
    generator.newMinMaxReactiveLimits()
        .setMinQ(-9999.99)
        .setMaxQ(9999.99)
        .add();

    return network;
}

iidm::Network EurostagFactory::createWithCurrentLimits() {
    iidm::Network network = EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2018-01-01T11:00:00+01:00"));

    network.getSubstation("P2").setCountry(iidm::Country::BE);

    network.getVoltageLevel("VLGEN").newGenerator()
        .setId("GEN2")
        .setBus("NGEN")
        .setConnectableBus("NGEN")
        .setMinP(-9999.99)
        .setMaxP(9999.99)
        .setVoltageRegulatorOn(true)
        .setTargetV(24.5)
        .setTargetP(607.0)
        .setTargetQ(301.0)
        .add();

    network.get<iidm::Bus>("NHV1").setV(380).getVoltageLevel().setLowVoltageLimit(400).setHighVoltageLimit(500);
    network.get<iidm::Bus>("NHV2").setV(380).getVoltageLevel().setLowVoltageLimit(300).setHighVoltageLimit(500);

    iidm::Line& line = network.getLine("NHV1_NHV2_1");
    line.getTerminal1().setP(560.0).setQ(550.0);
    line.getTerminal2().setP(560.0).setQ(550.0);
    line.newCurrentLimits1().setPermanentLimit(500).add();
    line.newCurrentLimits2()
        .setPermanentLimit(1100)
        .beginTemporaryLimit()
        .setName("10'")
        .setAcceptableDuration(10 * 60)
        .setValue(1200)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("1'")
        .setAcceptableDuration(60)
        .setValue(1500)
        .endTemporaryLimit()
        .add();

    iidm::Line& line2 = network.getLine("NHV1_NHV2_2");
    line2.getTerminal1().setP(560.0).setQ(550.0);
    line2.getTerminal2().setP(560.0).setQ(550.0);
    line2.newCurrentLimits1()
        .setPermanentLimit(1100)
        .beginTemporaryLimit()
        .setName("20'")
        .setAcceptableDuration(20 * 60)
        .setValue(1200)
        .endTemporaryLimit()
        .add();
    line2.newCurrentLimits2().setPermanentLimit(500).add();

    return network;
}

iidm::Network EurostagFactory::createWithFixedLimits() {
    iidm::Network network = createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2018-01-01T11:00:00+01:00"));

    network.getSubstation("P2").setCountry(iidm::Country::BE);

    network.getVoltageLevel("VLGEN").newGenerator()
        .setId("GEN2")
        .setBus("NGEN")
        .setConnectableBus("NGEN")
        .setMinP(-9999.99)
        .setMaxP(9999.99)
        .setVoltageRegulatorOn(true)
        .setTargetV(24.5)
        .setTargetP(607.0)
        .setTargetQ(301.0)
        .add();

    auto& bus1 = static_cast<iidm::Bus&>(network.getIdentifiable("NHV1"));
    bus1.setV(380).getVoltageLevel().setLowVoltageLimit(400).setHighVoltageLimit(500);
    auto& bus2 = static_cast<iidm::Bus&>(network.getIdentifiable("NHV2"));
    bus2.setV(380).getVoltageLevel().setLowVoltageLimit(300).setHighVoltageLimit(500);

    iidm::Line& line = network.getLine("NHV1_NHV2_1");
    line.getTerminal1().setP(560.0).setQ(550.0);
    line.getTerminal2().setP(560.0).setQ(550.0);
    line.newActivePowerLimits1().setPermanentLimit(500).add();
    line.newActivePowerLimits2()
        .setPermanentLimit(1100)
        .beginTemporaryLimit()
            .setName("10'")
            .setAcceptableDuration(10 * 60)
            .setValue(1200)
        .endTemporaryLimit()
        .beginTemporaryLimit()
            .setName("1'")
            .setAcceptableDuration(60)
            .setValue(1500)
        .endTemporaryLimit()
        .beginTemporaryLimit()
            .setName("N/A")
            .setAcceptableDuration(0)
            .setValue(std::numeric_limits<double>::max())
        .endTemporaryLimit()
        .add();

    line.newApparentPowerLimits1().setPermanentLimit(500).add();
    line.newApparentPowerLimits2()
        .setPermanentLimit(1100)
        .beginTemporaryLimit()
            .setName("10'")
            .setAcceptableDuration(10 * 60)
            .setValue(1200)
        .endTemporaryLimit()
        .beginTemporaryLimit()
            .setName("1'")
            .setAcceptableDuration(60)
            .setValue(1500)
        .endTemporaryLimit()
        .beginTemporaryLimit()
            .setName("N/A")
            .setAcceptableDuration(0)
            .setValue(std::numeric_limits<double>::max())
        .endTemporaryLimit()
        .add();

    iidm::Line& line2 = network.getLine("NHV1_NHV2_2");
    line2.getTerminal1().setP(560.0).setQ(550.0);
    line2.getTerminal2().setP(560.0).setQ(550.0);
    line2.newActivePowerLimits1()
        .setPermanentLimit(1100)
        .beginTemporaryLimit()
            .setName("20'")
            .setAcceptableDuration(20 * 60)
            .setValue(1200)
        .endTemporaryLimit()
        .beginTemporaryLimit()
            .setName("N/A")
            .setAcceptableDuration(60)
            .setValue(std::numeric_limits<double>::max())
        .endTemporaryLimit()
        .add();
    line2.newActivePowerLimits2().setPermanentLimit(500).add();

    line2.newApparentPowerLimits1()
        .setPermanentLimit(1100)
        .beginTemporaryLimit()
            .setName("20'")
            .setAcceptableDuration(20 * 60)
            .setValue(1200)
        .endTemporaryLimit()
        .beginTemporaryLimit()
            .setName("N/A")
            .setAcceptableDuration(60)
            .setValue(std::numeric_limits<double>::max())
        .endTemporaryLimit()
        .add();
    line2.newApparentPowerLimits2().setPermanentLimit(500).add();

    return network;
}

}  // namespace network

}  // namespace powsybl
