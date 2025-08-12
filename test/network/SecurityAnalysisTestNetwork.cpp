/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/range/algorithm.hpp>
#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ActivePowerLimits.hpp>
#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/SwitchKind.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace network {

BOOST_AUTO_TEST_SUITE(SecurityanalysisTestSuite)

void createSwitch(iidm::VoltageLevel &vl, std::string id, iidm::SwitchKind kind, bool open, int node1, int node2)
{
    vl.getNodeBreakerView().newSwitch()
        .setId(id)
        .setName(id)
        .setKind(kind)
        .setRetained(kind == iidm::SwitchKind::BREAKER)
        .setOpen(open)
        .setFictitious(false)
        .setNode1(node1)
        .setNode2(node2)
        .add();
}

iidm::Network createNetwork() {
    iidm::Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2022-12-18T16:00:00.000+01:00"));
    network.setForecastDistance(0);

        // first substation
        iidm::Substation& s1 = network.newSubstation()
                .setId("S1")
                .add();

        iidm::VoltageLevel& s1vl1 = s1.newVoltageLevel()
                .setId("S1VL1")
                .setNominalV(400)
                .setTopologyKind(iidm::TopologyKind::NODE_BREAKER)
                .add();

        s1vl1.getNodeBreakerView().newBusbarSection()
                .setId("S1VL1_BBS")
                .setName("S1VL1_BBS")
                .setNode(0)
                .add();

        iidm::VoltageLevel& s1vl2 = s1.newVoltageLevel()
                .setId("S1VL2")
                .setNominalV(225)
                .setTopologyKind(iidm::TopologyKind::NODE_BREAKER)
                .add();
        s1vl2.getNodeBreakerView().newBusbarSection()
                .setId("S1VL2_BBS1")
                .setName("S1VL2_BBS1")
                .setNode(0)
                .add();

        // second substation
        iidm::Substation& s2 = network.newSubstation()
                .setId("S2")
                .add();

        iidm::VoltageLevel& s2vl1 = s2.newVoltageLevel()
                .setId("S2VL1")
                .setNominalV(400)
                .setTopologyKind(iidm::TopologyKind::NODE_BREAKER)
                .add();
        s2vl1.getNodeBreakerView().newBusbarSection()
                .setId("S2VL1_BBS1")
                .setName("S2VL1_BBS1")
                .setNode(0)
                .add();
        iidm::VoltageLevel& s2vl2 = s2.newVoltageLevel()
                .setId("S2VL2")
                .setNominalV(225)
                .setTopologyKind(iidm::TopologyKind::NODE_BREAKER)
                .add();
        s2vl2.getNodeBreakerView().newBusbarSection()
                .setId("S2VL2_BBS1")
                .setName("S2VL2_BBS1")
                .setNode(0)
                .add();

        // generator
        createSwitch(s1vl1, "S1VL1_BBS1_GEN_DISCONNECTOR", iidm::SwitchKind::DISCONNECTOR, false, 0, 1);
        createSwitch(s1vl1, "S1VL1_BBS1_GEN_BREAKER", iidm::SwitchKind::BREAKER, false, 1, 2);
        s1vl1.newGenerator()
                .setId("GEN")
                .setEnergySource(iidm::EnergySource::OTHER)
                .setMinP(0)
                .setMaxP(150)
                .setVoltageRegulatorOn(true)
                .setTargetV(400)
                .setTargetP(100.0)
                .setNode(2)
                .add();

        // Loads
        createSwitch(s1vl2, "S1VL2_BBS1_LD1_DISCONNECTOR", iidm::SwitchKind::DISCONNECTOR, false, 0, 3);
        createSwitch(s1vl2, "S1VL2_BBS1_LD1_BREAKER", iidm::SwitchKind::BREAKER, false, 3, 4);
        createSwitch(s2vl2, "S2VL2_BBS1_LD2_DISCONNECTOR", iidm::SwitchKind::DISCONNECTOR, false, 0, 3);
        createSwitch(s2vl2, "S2VL2_BBS1_LD2_BREAKER", iidm::SwitchKind::BREAKER, false, 3, 4);
        s1vl2.newLoad()
                .setId("LD1")
                .setLoadType(iidm::LoadType::UNDEFINED)
                .setP0(50)
                .setQ0(4)
                .setNode(4)
                .add();
        s2vl2.newLoad()
                .setId("LD2")
                .setLoadType(iidm::LoadType::UNDEFINED)
                .setP0(50)
                .setQ0(4)
                .setNode(4)
                .add();

        // lines
        createSwitch(s1vl1, "S1VL2_BBS_LINES1S2V1_1_DISCONNECTOR", iidm::SwitchKind::DISCONNECTOR, false, 0, 16);
        createSwitch(s1vl1, "S1VL2_LINES1S2V1_1_BREAKER", iidm::SwitchKind::BREAKER, false, 16, 17);
        createSwitch(s2vl1, "S2VL2_BBS_LINES1S2V1_1_DISCONNECTOR", iidm::SwitchKind::DISCONNECTOR, false, 0, 16);
        createSwitch(s2vl1, "S2VL2_LINES1S2V1_1_BREAKER", iidm::SwitchKind::BREAKER, false, 16, 17);
        network.newLine()
                .setId("LINE_S1S2V1_1")
                .setR(0.01)
                .setX(50)
                .setG1(0.0)
                .setB1(0.0)
                .setG2(0.0)
                .setB2(0.0)
                .setNode1(17)
                .setVoltageLevel1("S1VL1")
                .setNode2(17)
                .setVoltageLevel2("S2VL1")
                .add();
        createSwitch(s1vl1, "S1VL2_BBS_LINES1S2V1_2_DISCONNECTOR", iidm::SwitchKind::DISCONNECTOR, false, 0, 14);
        createSwitch(s1vl1, "S1VL2_LINES1S2V1_2_BREAKER", iidm::SwitchKind::BREAKER, false, 14, 15);
        createSwitch(s2vl1, "S2VL2_BBS_LINES1S2V1_2_DISCONNECTOR", iidm::SwitchKind::DISCONNECTOR, false, 0, 14);
        createSwitch(s2vl1, "S2VL2_LINES1S2V1_2_BREAKER", iidm::SwitchKind::BREAKER, false, 14, 15);
        network.newLine()
                .setId("LINE_S1S2V1_2")
                .setR(0.01)
                .setX(50)
                .setG1(0.0)
                .setB1(0.0)
                .setG2(0.0)
                .setB2(0.0)
                .setNode1(15)
                .setVoltageLevel1("S1VL1")
                .setNode2(15)
                .setVoltageLevel2("S2VL1")
                .add();
        createSwitch(s1vl2, "S1VL2_BBS_LINES1S2_DISCONNECTOR", iidm::SwitchKind::DISCONNECTOR, false, 0, 12);
        createSwitch(s1vl2, "S1VL2_LINES1S2_BREAKER", iidm::SwitchKind::BREAKER, false, 12, 13);
        createSwitch(s2vl2, "S2VL2_BBS_LINES1S2_DISCONNECTOR", iidm::SwitchKind::DISCONNECTOR, false, 0, 12);
        createSwitch(s2vl2, "S2VL2_LINES1S2_BREAKER", iidm::SwitchKind::BREAKER, false, 12, 13);
        network.newLine()
                .setId("LINE_S1S2V2")
                .setR(0.01)
                .setX(50)
                .setG1(0.0)
                .setB1(0.0)
                .setG2(0.0)
                .setB2(0.0)
                .setNode1(13)
                .setVoltageLevel1("S1VL2")
                .setNode2(13)
                .setVoltageLevel2("S2VL2")
                .add();

        // transformers
        createSwitch(s1vl1, "S1VL1_BBS_TWT_DISCONNECTOR", iidm::SwitchKind::DISCONNECTOR, false, 0, 10);
        createSwitch(s1vl1, "S1VL1_TWT_BREAKER", iidm::SwitchKind::BREAKER, false, 10, 11);
        createSwitch(s1vl2, "S1VL2_BBS_TWT_DISCONNECTOR", iidm::SwitchKind::DISCONNECTOR, false, 0, 10);
        createSwitch(s1vl2, "S1VL2_TWT_BREAKER", iidm::SwitchKind::BREAKER, false, 10, 11);
        s1.newTwoWindingsTransformer()
                .setId("TWT")
                .setR(2.0)
                .setX(25)
                .setG(0.0)
                .setB(3.2E-5)
                .setRatedU1(400.0)
                .setRatedU2(225.0)
                .setNode1(11)
                .setVoltageLevel1("S1VL1")
                .setNode2(11)
                .setVoltageLevel2("S1VL2")
                .add();
        createSwitch(s2vl1, "S2VL1_BBS_TWT_DISCONNECTOR", iidm::SwitchKind::DISCONNECTOR, false, 0, 10);
        createSwitch(s2vl1, "S2VL1_TWT_BREAKER", iidm::SwitchKind::BREAKER, false, 10, 11);
        createSwitch(s2vl2, "S2VL2_BBS_TWT_DISCONNECTOR", iidm::SwitchKind::DISCONNECTOR, false, 0, 10);
        createSwitch(s2vl2, "S2VL2_TWT_BREAKER", iidm::SwitchKind::BREAKER, false, 10, 11);
        s2.newTwoWindingsTransformer()
                .setId("TWT2")
                .setR(2.0)
                .setX(50)
                .setG(0.0)
                .setB(3.2E-5)
                .setRatedU1(400.0)
                .setRatedU2(225.0)
                .setNode1(11)
                .setVoltageLevel1("S2VL1")
                .setNode2(11)
                .setVoltageLevel2("S2VL2")
                .add();

    return network;
}

iidm::Network createNetworkWithCurrentLimits() {
    iidm::Network network = createNetwork();

    network.getLine("LINE_S1S2V1_1").newCurrentLimits2()
                .setPermanentLimit(75)
                .add();
        network.getLine("LINE_S1S2V1_1").newCurrentLimits1()
                .setPermanentLimit(75)
                .beginTemporaryLimit()
                .setName("10'")
                .setAcceptableDuration(10 * 60)
                .setValue(80)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("1'")
                .setAcceptableDuration(60)
                .setValue(85)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("Undefined")
                .setAcceptableDuration(0)
                .setValue(std::numeric_limits<double>::max())
                .endTemporaryLimit()
                .add();
        network.getLine("LINE_S1S2V1_2").newCurrentLimits2()
            .setPermanentLimit(75)
            .add();
        network.getLine("LINE_S1S2V1_2").newCurrentLimits1()
                .setPermanentLimit(75)
                .beginTemporaryLimit()
                .setName("10'")
                .setAcceptableDuration(10 * 60)
                .setValue(80)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("1'")
                .setAcceptableDuration(60)
                .setValue(85)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("Undefined")
                .setAcceptableDuration(0)
                .setValue(std::numeric_limits<double>::max())
                .endTemporaryLimit()
                .add();
        network.getLine("LINE_S1S2V2").newCurrentLimits1()
                .setPermanentLimit(60)
                .beginTemporaryLimit()
                .setName("10'")
                .setAcceptableDuration(10 * 60)
                .setValue(80)
                .endTemporaryLimit()
                .add();
        network.getTwoWindingsTransformer("TWT2").newCurrentLimits1().setPermanentLimit(90)
                .beginTemporaryLimit()
                .setName("10'")
                .setAcceptableDuration(10 * 60)
                .setValue(100)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("1'")
                .setAcceptableDuration(60)
                .setValue(110)
                .endTemporaryLimit()
                .add();
        network.getTwoWindingsTransformer("TWT").newCurrentLimits1().setPermanentLimit(92)
                .beginTemporaryLimit()
                .setName("10'")
                .setAcceptableDuration(10 * 60)
                .setValue(100)
                .endTemporaryLimit()
                .beginTemporaryLimit()
                .setName("1'")
                .setAcceptableDuration(60)
                .setValue(110)
                .endTemporaryLimit()
                .add();

    return network;
}

iidm::Network createNetworkWithFixedPowerLimits() {
    iidm::Network network = createNetwork();

    network.getTwoWindingsTransformer("TWT").newActivePowerLimits1().setPermanentLimit(71).add();
        network.getTwoWindingsTransformer("TWT2").newActivePowerLimits1().setPermanentLimit(55).add();
        network.getLine("LINE_S1S2V1_1").newActivePowerLimits1().setPermanentLimit(55).add();
        network.getLine("LINE_S1S2V1_2").newActivePowerLimits1().setPermanentLimit(55).add();
        network.getLine("LINE_S1S2V2").newActivePowerLimits1().setPermanentLimit(30).add();
        return network;

    return network;
}

BOOST_AUTO_TEST_CASE(testCreate)
{
    iidm::Network network = createNetwork();

    iidm::Line &lineS1S2V11 = network.getLine("LINE_S1S2V1_1");
    BOOST_CHECK_CLOSE(0.01, lineS1S2V11.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(50, lineS1S2V11.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, lineS1S2V11.getG1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, lineS1S2V11.getG2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, lineS1S2V11.getB1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, lineS1S2V11.getB2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("S1VL1", lineS1S2V11.getTerminal1().getVoltageLevel().getId());
    BOOST_CHECK_EQUAL("S2VL1", lineS1S2V11.getTerminal2().getVoltageLevel().getId());

    iidm::Line &lineS1S2V12 = network.getLine("LINE_S1S2V1_2");
    BOOST_CHECK_CLOSE(0.01, lineS1S2V12.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(50, lineS1S2V12.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, lineS1S2V12.getG1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, lineS1S2V12.getG2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, lineS1S2V12.getB1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, lineS1S2V12.getB2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("S1VL1", lineS1S2V11.getTerminal1().getVoltageLevel().getId());
    BOOST_CHECK_EQUAL("S2VL1", lineS1S2V11.getTerminal2().getVoltageLevel().getId());

    iidm::Line &lineS1S2V2 = network.getLine("LINE_S1S2V2");
    BOOST_CHECK_CLOSE(0.01, lineS1S2V2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(50, lineS1S2V2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, lineS1S2V2.getG1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, lineS1S2V2.getG2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, lineS1S2V2.getB1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, lineS1S2V2.getB2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("S1VL2", lineS1S2V2.getTerminal1().getVoltageLevel().getId());
    BOOST_CHECK_EQUAL("S2VL2", lineS1S2V2.getTerminal2().getVoltageLevel().getId());

    iidm::TwoWindingsTransformer &twt = network.getTwoWindingsTransformer("TWT");
    BOOST_CHECK_CLOSE(2.0, twt.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(25, twt.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, twt.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.2E-5, twt.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(400, twt.getRatedU1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(225, twt.getRatedU2(), std::numeric_limits<double>::epsilon());

    iidm::TwoWindingsTransformer &twt2 = network.getTwoWindingsTransformer("TWT2");
    BOOST_CHECK_CLOSE(2.0, twt2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(50, twt2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, twt2.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.2E-5, twt2.getB(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(400, twt2.getRatedU1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(225, twt2.getRatedU2(), std::numeric_limits<double>::epsilon());

    iidm::Load &load1 = network.getLoad("LD1");
    BOOST_CHECK_EQUAL(iidm::LoadType::UNDEFINED, load1.getLoadType());
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4, load1.getQ0(), std::numeric_limits<double>::epsilon());

    iidm::Load &load2 = network.getLoad("LD2");
    BOOST_CHECK_EQUAL(iidm::LoadType::UNDEFINED, load2.getLoadType());
    BOOST_CHECK_CLOSE(50, load2.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4, load2.getQ0(), std::numeric_limits<double>::epsilon());

    iidm::Generator &generator = network.getGenerator("GEN");
    BOOST_CHECK_EQUAL(iidm::EnergySource::OTHER, generator.getEnergySource());
    BOOST_CHECK_CLOSE(0, generator.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(150, generator.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(generator.isVoltageRegulatorOn());
    BOOST_CHECK_CLOSE(400, generator.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(100, generator.getTargetP(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(withCurrentLimits)
{

    iidm::Network network = createNetworkWithCurrentLimits();

    BOOST_CHECK(static_cast<bool>(network.getLine("LINE_S1S2V1_1").getCurrentLimits1()));
    BOOST_CHECK_CLOSE(75, network.getLine("LINE_S1S2V1_1").getCurrentLimits1().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(3, boost::size(network.getLine("LINE_S1S2V1_1").getCurrentLimits1().get().getTemporaryLimits()));
    auto temporaryLimits = network.getLine("LINE_S1S2V1_1").getCurrentLimits1().get().getTemporaryLimits();
    iidm::CurrentLimits::TemporaryLimit tl1 = network.getLine("LINE_S1S2V1_1").getCurrentLimits1().get().getTemporaryLimit(600);
    iidm::CurrentLimits::TemporaryLimit tl2 = network.getLine("LINE_S1S2V1_1").getCurrentLimits1().get().getTemporaryLimit(60);
    iidm::CurrentLimits::TemporaryLimit tl3 = network.getLine("LINE_S1S2V1_1").getCurrentLimits1().get().getTemporaryLimit(0);

    BOOST_CHECK_EQUAL("10'", tl1.getName());
    BOOST_CHECK_CLOSE(80, tl1.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("1'", tl2.getName());
    BOOST_CHECK_CLOSE(85, tl2.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("Undefined", tl3.getName());
    BOOST_CHECK_CLOSE(std::numeric_limits<double>::max(), tl3.getValue(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(static_cast<bool>(network.getLine("LINE_S1S2V1_1").getCurrentLimits2()));
    BOOST_CHECK_CLOSE(75, network.getLine("LINE_S1S2V1_1").getCurrentLimits2().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(static_cast<bool>(network.getLine("LINE_S1S2V1_2").getCurrentLimits1()));
    BOOST_CHECK_CLOSE(75, network.getLine("LINE_S1S2V1_2").getCurrentLimits1().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(3, boost::size(network.getLine("LINE_S1S2V1_2").getCurrentLimits1().get().getTemporaryLimits()));
    temporaryLimits = network.getLine("LINE_S1S2V1_2").getCurrentLimits1().get().getTemporaryLimits();
    tl1 = network.getLine("LINE_S1S2V1_2").getCurrentLimits1().get().getTemporaryLimit(600);
    tl2 = network.getLine("LINE_S1S2V1_2").getCurrentLimits1().get().getTemporaryLimit(60);
    tl3 = network.getLine("LINE_S1S2V1_2").getCurrentLimits1().get().getTemporaryLimit(0);
    BOOST_CHECK_EQUAL("10'", tl1.getName());
    BOOST_CHECK_CLOSE(80, tl1.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("1'", tl2.getName());
    BOOST_CHECK_CLOSE(85, tl2.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("Undefined", tl3.getName());
    BOOST_CHECK_CLOSE(std::numeric_limits<double>::max(), tl3.getValue(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(static_cast<bool>(network.getLine("LINE_S1S2V1_2").getCurrentLimits2()));
    BOOST_CHECK_CLOSE(75, network.getLine("LINE_S1S2V1_2").getCurrentLimits2().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(static_cast<bool>(network.getLine("LINE_S1S2V2").getCurrentLimits1()));
    BOOST_CHECK_CLOSE(60, network.getLine("LINE_S1S2V2").getCurrentLimits1().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(1, boost::size(network.getLine("LINE_S1S2V2").getCurrentLimits1().get().getTemporaryLimits()));
    temporaryLimits = network.getLine("LINE_S1S2V2").getCurrentLimits1().get().getTemporaryLimits();
    tl1 = network.getLine("LINE_S1S2V2").getCurrentLimits1().get().getTemporaryLimit(600);
    BOOST_CHECK_EQUAL("10'", tl1.getName());
    BOOST_CHECK_CLOSE(80, tl1.getValue(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(static_cast<bool>(network.getTwoWindingsTransformer("TWT").getCurrentLimits1()));
    BOOST_CHECK_CLOSE(92, network.getTwoWindingsTransformer("TWT").getCurrentLimits1().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2, boost::size(network.getTwoWindingsTransformer("TWT").getCurrentLimits1().get().getTemporaryLimits()));
    temporaryLimits = network.getTwoWindingsTransformer("TWT").getCurrentLimits1().get().getTemporaryLimits();
    tl1 = network.getTwoWindingsTransformer("TWT").getCurrentLimits1().get().getTemporaryLimit(600);
    tl2 = network.getTwoWindingsTransformer("TWT").getCurrentLimits1().get().getTemporaryLimit(60);
    BOOST_CHECK_EQUAL("10'", tl1.getName());
    BOOST_CHECK_CLOSE(100, tl1.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("1'", tl2.getName());
    BOOST_CHECK_CLOSE(110, tl2.getValue(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK(static_cast<bool>(network.getTwoWindingsTransformer("TWT2").getCurrentLimits1()));
    BOOST_CHECK_CLOSE(90, network.getTwoWindingsTransformer("TWT2").getCurrentLimits1().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2, boost::size(network.getTwoWindingsTransformer("TWT2").getCurrentLimits1().get().getTemporaryLimits()));
    temporaryLimits = network.getTwoWindingsTransformer("TWT2").getCurrentLimits1().get().getTemporaryLimits();
    tl1 = network.getTwoWindingsTransformer("TWT2").getCurrentLimits1().get().getTemporaryLimit(600);
    tl2 = network.getTwoWindingsTransformer("TWT2").getCurrentLimits1().get().getTemporaryLimit(60);
    BOOST_CHECK_EQUAL("10'", tl1.getName());
    BOOST_CHECK_CLOSE(100, tl1.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL("1'", tl2.getName());
    BOOST_CHECK_CLOSE(110, tl2.getValue(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(withFixedPowerLimits)
{

    iidm::Network network = createNetworkWithFixedPowerLimits();
    BOOST_CHECK(static_cast<bool>(network.getLine("LINE_S1S2V1_1").getActivePowerLimits1()));
    BOOST_CHECK_CLOSE(55, network.getLine("LINE_S1S2V1_1").getActivePowerLimits1().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(static_cast<bool>(network.getLine("LINE_S1S2V1_2").getActivePowerLimits1()));
    BOOST_CHECK_CLOSE(55, network.getLine("LINE_S1S2V1_2").getActivePowerLimits1().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(static_cast<bool>(network.getLine("LINE_S1S2V2").getActivePowerLimits1()));
    BOOST_CHECK_CLOSE(30, network.getLine("LINE_S1S2V2").getActivePowerLimits1().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(static_cast<bool>(network.getTwoWindingsTransformer("TWT").getActivePowerLimits1()));
    BOOST_CHECK_CLOSE(71, network.getTwoWindingsTransformer("TWT").getActivePowerLimits1().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(static_cast<bool>(network.getTwoWindingsTransformer("TWT2").getActivePowerLimits1()));
    BOOST_CHECK_CLOSE(55, network.getTwoWindingsTransformer("TWT2").getActivePowerLimits1().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace network

}  // namespace powsybl
