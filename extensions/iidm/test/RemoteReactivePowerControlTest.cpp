/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/extensions/iidm/RemoteReactivePowerControl.hpp>
#include <powsybl/iidm/extensions/iidm/RemoteReactivePowerControlAdder.hpp>
#include <powsybl/network/EurostagFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(RemoteReactivePowerControlTestSuite)

Network createNetworkTest() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T12:27:58.535+02:00"));

    Substation& s = network.newSubstation()
                .setId("S")
                .setCountry(Country::FR)
                .add();
    VoltageLevel& vl = s.newVoltageLevel()
                .setId("VL")
                .setNominalV(400)
                .setTopologyKind(TopologyKind::BUS_BREAKER)
                .add();
    vl.getBusBreakerView().newBus()
                .setId("B1")
                .add();
    vl.getBusBreakerView().newBus()
                .setId("B2")
                .add();
    Generator& gen = vl.newGenerator()
                .setId("G")
                .setBus("B1")
                .setConnectableBus("B1")
                .setTargetP(100)
                .setTargetQ(100)
                .setTargetV(400)
                .setMinP(0)
                .setMaxP(200)
                .setVoltageRegulatorOn(true)
                .add();
    Line& line = network.newLine()
                .setId("L12")
                .setVoltageLevel1("VL").setBus1("B1")
                .setVoltageLevel2("VL").setBus2("B2")
                .setR(0).setX(1).setB1(0).setB2(0).setG1(0).setG2(0)
                .add();

    gen.newExtension<RemoteReactivePowerControlAdder>()
                .withEnabled(true)
                .withTargetQ(123)
                .withRegulatingTerminal(line.getTerminal2())
                .add();
    return network;
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();

    Generator& generator = network.getGenerator("GEN");
    auto adder = generator.newExtension<RemoteReactivePowerControlAdder>().withTargetQ(1.0).withEnabled(true);
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "RemoteReactivePowerControl's regulating terminal must be set");

    Terminal& terminal = network.getLine("NHV1_NHV2_1").getTerminal1();
    generator.newExtension<RemoteReactivePowerControlAdder>().withTargetQ(2.0).withRegulatingTerminal(terminal).withEnabled(false).add();
    auto& ext = generator.getExtension<RemoteReactivePowerControl>();
    BOOST_CHECK_CLOSE(2.0, ext.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(terminal, ext.getRegulatingTerminal()));
    BOOST_CHECK(!ext.isEnabled());

    ext.setEnabled(true);
    BOOST_CHECK(ext.isEnabled());
    ext.setTargeQ(3.0);
    BOOST_CHECK_CLOSE(3.0, ext.getTargetQ(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(ext.setTargeQ(stdcxx::nan()), PowsyblException, "RemoteReactivePowerControl's reactive power target must be set");
    POWSYBL_ASSERT_THROW(ext.setRegulatingTerminal(stdcxx::Reference<Terminal>()), PowsyblException, "RemoteReactivePowerControl's regulating terminal must be set");
    Terminal& genTerminal = generator.getTerminal();
    ext.setRegulatingTerminal(stdcxx::Reference<Terminal>(genTerminal));
    BOOST_CHECK(!stdcxx::areSame(ext.getRegulatingTerminal(), terminal));
    BOOST_CHECK(stdcxx::areSame(ext.getRegulatingTerminal(), generator.getTerminal()));

}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();

    Generator& generator = network.getGenerator("GEN");
    auto adder = generator.newExtension<RemoteReactivePowerControlAdder>();
    
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "RemoteReactivePowerControl's reactive power target must be set");
    adder.withTargetQ(1.0);
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "RemoteReactivePowerControl's regulating terminal must be set");

    Terminal& terminal = network.getLine("NHV1_NHV2_1").getTerminal1();
    adder.withRegulatingTerminal(terminal).add();
    auto& ext = generator.getExtension<RemoteReactivePowerControl>();
    BOOST_CHECK_CLOSE(1.0, ext.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(terminal, ext.getRegulatingTerminal()));
    BOOST_CHECK(ext.isEnabled());

    generator.removeExtension<RemoteReactivePowerControl>();
    generator.newExtension<RemoteReactivePowerControlAdder>().withTargetQ(2.0).withRegulatingTerminal(terminal).withEnabled(false).add();

    BOOST_CHECK(!generator.getExtension<RemoteReactivePowerControl>().isEnabled());

}

BOOST_AUTO_TEST_CASE(removeTerminal) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();

    Generator& generator = network.getGenerator("GEN");
    Line& line = network.getLine("NHV1_NHV2_1");
    Terminal& terminal = line.getTerminal1();

    generator.newExtension<RemoteReactivePowerControlAdder>()
                    .withTargetQ(1.0)
                    .withEnabled(true)
                    .withRegulatingTerminal(terminal)
                    .add();

    stdcxx::Reference<RemoteReactivePowerControl> ext = generator.findExtension<RemoteReactivePowerControl>();
    POWSYBL_ASSERT_REF_TRUE(ext);
    BOOST_CHECK_EQUAL(1, line.getTerminal1().getReferrers().size());

    line.remove();
    //extension has been removed because regulating terminal is invalid
    ext = generator.findExtension<RemoteReactivePowerControl>();
    POWSYBL_ASSERT_REF_FALSE(ext);


}

BOOST_FIXTURE_TEST_CASE(rrpcXmlTest, test::ResourceFixture) {
    Network network = createNetworkTest();
    RemoteReactivePowerControl& rrpc = network.getGenerator("G").getExtension<RemoteReactivePowerControl>();

    const std::string& networkStrRef = ResourceFixture::getResource("remoteReactivePowerControlRef.xml");
    Network network2 = test::converter::RoundTrip::runXml(network, networkStrRef);


    Generator& gen2 = network2.getGenerator("G");
    Line& line = network.getLine("L12");
    Line& line2 = network2.getLine("L12");
    RemoteReactivePowerControl& rrpc2 = gen2.getExtension<RemoteReactivePowerControl>();

    BOOST_CHECK_EQUAL(rrpc.isEnabled(), rrpc2.isEnabled());
    BOOST_CHECK_CLOSE(rrpc.getTargetQ(), rrpc2.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(rrpc.getRegulatingTerminal().getConnectable().get().getId(), rrpc2.getRegulatingTerminal().getConnectable().get().getId());
    BOOST_CHECK_EQUAL(line.getSide(rrpc.getRegulatingTerminal()), line2.getSide(rrpc2.getRegulatingTerminal()));


    // check it fails for all versions < 1.5
    test::converter::RoundTrip::testForAllPreviousVersions(converter::xml::IidmXmlVersion::V1_5(), [&network](const converter::xml::IidmXmlVersion& version) {
        converter::ExportOptions options = converter::ExportOptions().setVersion(version.toString("."));
        const auto& writer = [&options](const Network& n, std::ostream& stream) {
            Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
        };
        try {
            test::converter::RoundTrip::writeXmlTest(network, writer, "notsupported");
        } catch (PowsyblException& e) {
            BOOST_CHECK_EQUAL("generatorRemoteReactivePowerControl. is not supported for IIDM-XML version " + version.toString(".") + ". IIDM-XML version should be >= 1.5", e.what());
        }
    });

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
