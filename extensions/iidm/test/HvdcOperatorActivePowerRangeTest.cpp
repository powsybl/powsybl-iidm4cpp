/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/iidm/extensions/iidm/HvdcOperatorActivePowerRange.hpp>
#include <powsybl/iidm/extensions/iidm/HvdcOperatorActivePowerRangeAdder.hpp>
#include <powsybl/stdcxx/memory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

Network createVsc() {
    Network network("hvdctest", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T16:34:55.930+02:00"));
    Substation& s1 = network.newSubstation()
        .setId("S1")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
        .setId("VL1")
        .setNominalV(400.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl1.getBusBreakerView().newBus()
        .setId("B1")
        .add();
    Substation& s2 = network.newSubstation()
        .setId("S2")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl2 = s2.newVoltageLevel()
        .setId("VL2")
        .setNominalV(400.0)
        .setTopologyKind(TopologyKind::NODE_BREAKER)
        .add();
    vl2.getNodeBreakerView().newBusbarSection()
        .setId("BBS1")
        .setName("BusbarSection")
        .setNode(0)
        .add();
    vl2.getNodeBreakerView().newDisconnector()
        .setId("DISC_BBS1_BK1")
        .setName("Disconnector")
        .setNode1(0)
        .setNode2(1)
        .setOpen(false)
        .setRetained(false)
        .add();
    vl2.getNodeBreakerView().newBreaker()
        .setId("BK1")
        .setName("Breaker")
        .setNode1(1)
        .setNode2(2)
        .setOpen(false)
        .setRetained(true)
        .add();
    VscConverterStation& cs1 = vl1.newVscConverterStation()
        .setId("C1")
        .setName("Converter1")
        .setConnectableBus("B1")
        .setBus("B1")
        .setLossFactor(1.1)
        .setVoltageSetpoint(405.0)
        .setVoltageRegulatorOn(true)
        .add();
    cs1.getTerminal()
        .setP(100.0)
        .setQ(50.0);
    cs1.newReactiveCapabilityCurve()
        .beginPoint()
        .setP(5.0)
        .setMinQ(0.0)
        .setMaxQ(10.0)
        .endPoint()
        .beginPoint()
        .setP(10.0)
        .setMinQ(0.0)
        .setMaxQ(10.0)
        .endPoint()
        .add();
    VscConverterStation& cs2 = vl2.newVscConverterStation()
        .setId("C2")
        .setName("Converter2")
        .setNode(2)
        .setLossFactor(1.1F)
        .setReactivePowerSetpoint(123.0)
        .setVoltageRegulatorOn(false)
        .add();
    cs2.newMinMaxReactiveLimits()
        .setMinQ(0.0)
        .setMaxQ(10.0)
        .add();
    network.newHvdcLine()
        .setId("L")
        .setName("HVDC")
        .setConverterStationId1("C1")
        .setConverterStationId2("C2")
        .setR(1.0)
        .setNominalV(400.0)
        .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER)
        .setMaxP(300.0)
        .setActivePowerSetpoint(280.0)
        .add();
    return network;
}

BOOST_AUTO_TEST_SUITE(HvdcOperatorActivePowerRangeTestSuite)

BOOST_FIXTURE_TEST_CASE(HvdcOperatorActivePowerRangeConstructor, test::ResourceFixture) {
    Network network = Network::readXml(ResourceFixture::getResourcePath("LccRoundTripRef.xml"));
    HvdcLine& hvdcLine = network.getHvdcLine("L");

    hvdcLine.newExtension<HvdcOperatorActivePowerRangeAdder>().withOprFromCS1toCS2(1.1).withOprFromCS2toCS1(2.2).add();
    auto& extension = hvdcLine.getExtension<HvdcOperatorActivePowerRange>();
    BOOST_CHECK_EQUAL("hvdcOperatorActivePowerRange", extension.getName());
    BOOST_CHECK_CLOSE(1.1, extension.getOprFromCS1toCS2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.2, extension.getOprFromCS2toCS1(), std::numeric_limits<double>::epsilon());

    extension.setOprFromCS1toCS2(11.1);
    BOOST_CHECK_CLOSE(11.1, extension.getOprFromCS1toCS2(), std::numeric_limits<double>::epsilon());

    extension.setOprFromCS2toCS1(22.2);
    BOOST_CHECK_CLOSE(22.2, extension.getOprFromCS2toCS1(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(extension.setOprFromCS1toCS2(-1.0), PowsyblException, "OPR from C1 to C2 must be greater than 0 (current value -1).");
    POWSYBL_ASSERT_THROW(extension.setOprFromCS2toCS1(-1.0), PowsyblException, "OPR from C2 to C1 must be greater than 0 (current value -1).");
}

BOOST_FIXTURE_TEST_CASE(HvdcOperatorActivePowerRangeXmlSerializerTest, test::ResourceFixture) {
    Network network = createVsc();
    HvdcLine& hvdcLine = network.getHvdcLine("L");
    hvdcLine.newExtension<HvdcOperatorActivePowerRangeAdder>().withOprFromCS1toCS2(500).withOprFromCS2toCS1(700).add();

    const std::string& networkStrRef = ResourceFixture::getResource("/hvdcOperatorActivePowerRange.xml");

    test::converter::RoundTrip::runXml(network, networkStrRef);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
