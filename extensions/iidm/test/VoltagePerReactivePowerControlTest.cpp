/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensatorAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/extensions/iidm/VoltagePerReactivePowerControl.hpp>
#include <powsybl/iidm/extensions/iidm/VoltagePerReactivePowerControlAdder.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

Network createStaticVarCompensatorTestNetwork() {
    Network network("svcTestCase", "code");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-29T14:54:03.427+02:00"));
    Substation& s1 = network.newSubstation()
        .setId("S1")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
        .setId("VL1")
        .setNominalVoltage(380)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl1.getBusBreakerView().newBus()
        .setId("B1")
        .add();
    vl1.newGenerator()
        .setId("G1")
        .setConnectableBus("B1")
        .setBus("B1")
        .setVoltageRegulatorOn(true)
        .setTargetP(100.0)
        .setTargetV(400.0)
        .setMinP(50.0)
        .setMaxP(150.0)
        .add();
    Substation& s2 = network.newSubstation()
        .setId("S2")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl2 = s2.newVoltageLevel()
        .setId("VL2")
        .setNominalVoltage(380)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl2.getBusBreakerView().newBus()
        .setId("B2")
        .add();
    vl2.newLoad()
        .setId("L2")
        .setConnectableBus("B2")
        .setBus("B2")
        .setP0(100.0)
        .setQ0(50.0)
        .add();
    vl2.newStaticVarCompensator()
        .setId("SVC2")
        .setConnectableBus("B2")
        .setBus("B2")
        .setBmin(0.0002)
        .setBmax(0.0008)
        .setRegulationMode(StaticVarCompensator::RegulationMode::VOLTAGE)
        .setVoltageSetpoint(390)
        .add();
    network.newLine()
        .setId("L1")
        .setVoltageLevel1("VL1")
        .setConnectableBus1("B1")
        .setBus1("B1")
        .setVoltageLevel2("VL2")
        .setConnectableBus2("B2")
        .setBus2("B2")
        .setR(4.0)
        .setX(200.0)
        .setG1(0.0)
        .setB1(0.0)
        .setG2(0.0)
        .setB2(0.0)
        .add();
    return network;
}

BOOST_AUTO_TEST_SUITE(VoltagePerReactivePowerControlTestSuite)

BOOST_AUTO_TEST_CASE(VoltagePerReactivePowerControlTest) {
    Network network = createStaticVarCompensatorTestNetwork();
    StaticVarCompensator& svc = network.getStaticVarCompensator("SVC2");
    svc.newExtension<VoltagePerReactivePowerControlAdder>().withSlope(10.0).add();
    auto& vprpc = svc.getExtension<VoltagePerReactivePowerControl>();

    BOOST_CHECK_EQUAL("voltagePerReactivePowerControl", vprpc.getName());
    BOOST_CHECK_CLOSE(10.0, vprpc.getSlope(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(vprpc, vprpc.setSlope(11.0)));
    BOOST_CHECK_CLOSE(11.0, vprpc.getSlope(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(vprpc.setSlope(stdcxx::nan()), PowsyblException, "Undefined value for slope");
    POWSYBL_ASSERT_THROW(VoltagePerReactivePowerControl(svc, stdcxx::nan()), PowsyblException, "Undefined value for slope");
    POWSYBL_ASSERT_THROW(svc.newExtension<VoltagePerReactivePowerControlAdder>().add(), PowsyblException, "Undefined value for slope");

    POWSYBL_ASSERT_THROW(vprpc.setSlope(-1.0), PowsyblException, "Slope value of SVC SVC2 must be positive: -1");
    POWSYBL_ASSERT_THROW(VoltagePerReactivePowerControl(svc, -1.0), PowsyblException, "Slope value of SVC SVC2 must be positive: -1");
    POWSYBL_ASSERT_THROW(svc.newExtension<VoltagePerReactivePowerControlAdder>().withSlope(-1.0).add(), PowsyblException, "Slope value of SVC SVC2 must be positive: -1");

    BOOST_CHECK(stdcxx::areSame(svc, vprpc.getExtendable<StaticVarCompensator>().get()));
    BOOST_CHECK(stdcxx::areSame(vprpc, svc.getExtension<VoltagePerReactivePowerControl>()));
}

BOOST_FIXTURE_TEST_CASE(VoltagePerReactivePowerControlXmlSerializerTest, test::ResourceFixture) {
    Network network = createStaticVarCompensatorTestNetwork();
    network.setCaseDate(stdcxx::DateTime::parse("2019-05-27T12:17:02.504+02:00"));

    StaticVarCompensator& svc = network.getStaticVarCompensator("SVC2");
    svc.newExtension<VoltagePerReactivePowerControlAdder>().withSlope(0.5).add();

    const std::string& networkStr = ResourceFixture::getResource("voltagePerReactivePowerControl.xml");

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
