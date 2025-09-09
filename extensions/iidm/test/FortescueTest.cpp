/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>

#include <powsybl/iidm/extensions/iidm/GeneratorFortescue.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorFortescueAdder.hpp>
#include <powsybl/iidm/extensions/iidm/LineFortescue.hpp>
#include <powsybl/iidm/extensions/iidm/LineFortescueAdder.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerFortescue.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerFortescueAdder.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerFortescue.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerFortescueAdder.hpp>

#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/network/ThreeWindingsTransformerNetworkFactory.hpp>

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(FortescueTestSuite)

Network createGeneratorFortescueNetwork() {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();

    Generator& gen = network.getGenerator("GEN");
    gen.newExtension<GeneratorFortescueAdder>()
                .withRz(0.1)
                .withXz(2.0)
                .withRn(0.2)
                .withXn(2.4)
                .withGrounded(true)
                .withGroundingR(0.02)
                .withGroundingX(0.3)
                .add();

    return network;
}

Network createLineFortescueNetwork() {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();

    Line& l = network.getLine("NHV1_NHV2_1");
    l.newExtension<LineFortescueAdder>()
                .withRz(0.1)
                .withXz(2.0)
                .add();

    return network;
}

Network create2WTFortescueNetwork() {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();

    TwoWindingsTransformer& twowt = network.getTwoWindingsTransformer("NGEN_NHV1");
    twowt.newExtension<TwoWindingsTransformerFortescueAdder>()
                .withRz(0.1)
                .withXz(2.0)
                .withFreeFluxes(true)
                .withConnectionType1(WindingConnectionType::Y_GROUNDED)
                .withConnectionType2(WindingConnectionType::DELTA)
                .withGroundingR1(0.02)
                .withGroundingX1(0.3)
                .withGroundingR2(0.04)
                .withGroundingX2(0.95)
                .add();

    return network;
}

Network create3WTFortescueNetwork() {
    Network network = ::powsybl::network::ThreeWindingsTransformerNetworkFactory::create();

    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT");
    twt.newExtension<ThreeWindingsTransformerFortescueAdder>()
                .newLeg1()
                    .withRz(0.1)
                    .withXz(2.0)
                    .withFreeFluxes(true)
                    .withConnectionType(WindingConnectionType::Y_GROUNDED)
                    .withGroundingR(0.02)
                    .withGroundingX(0.3)
                    .add()
                .newLeg2()
                    .withRz(0.2)
                    .withXz(2.1)
                    .withFreeFluxes(false)
                    .withConnectionType(WindingConnectionType::Y)
                    .withGroundingR(0.12)
                    .withGroundingX(0.4)
                    .add()
                .newLeg3()
                    .withRz(0.3)
                    .withXz(2.2)
                    .withFreeFluxes(true)
                    .withConnectionType(WindingConnectionType::DELTA)
                    .withGroundingR(0.22)
                    .withGroundingX(0.5)
                    .add()
                .add();

    return network;
}

BOOST_AUTO_TEST_CASE(FortescueConstructorTest) {
    Network networkGenF = createGeneratorFortescueNetwork();
    Network networkLineF = createLineFortescueNetwork();
    Network network2WTF = create2WTFortescueNetwork();
    Network network3WTF = create3WTFortescueNetwork();

    GeneratorFortescue& fGen = networkGenF.getGenerator("GEN").getExtension<GeneratorFortescue>();
    LineFortescue& fLine = networkLineF.getLine("NHV1_NHV2_1").getExtension<LineFortescue>();
    TwoWindingsTransformerFortescue& fTwoWT = network2WTF.getTwoWindingsTransformer("NGEN_NHV1").getExtension<TwoWindingsTransformerFortescue>();
    ThreeWindingsTransformerFortescue& fThreeWT = network3WTF.getThreeWindingsTransformer("3WT").getExtension<ThreeWindingsTransformerFortescue>();

    BOOST_CHECK_CLOSE(0.1, fGen.getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, fGen.getXz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, fGen.getRn(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.4, fGen.getXn(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(fGen.isGrounded());
    BOOST_CHECK_CLOSE(0.02, fGen.getGroundingR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.3, fGen.getGroundingX(), std::numeric_limits<double>::epsilon());
    fGen.setRz(0.11);
    fGen.setXz(2.03);
    fGen.setRn(0.1);
    fGen.setXn(2.3);
    fGen.setGrounded(false);
    fGen.setGroundingR(0.0001);
    fGen.setGroundingX(0.35);
    BOOST_CHECK_CLOSE(0.11, fGen.getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.03, fGen.getXz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.1, fGen.getRn(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.3, fGen.getXn(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!fGen.isGrounded());
    BOOST_CHECK_CLOSE(0.0001, fGen.getGroundingR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.35, fGen.getGroundingX(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(0.1, fLine.getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, fLine.getXz(), std::numeric_limits<double>::epsilon());
    fLine.setRz(0.11);
    fLine.setXz(2.03);
    BOOST_CHECK_CLOSE(0.11, fLine.getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.03, fLine.getXz(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(0.1, fTwoWT.getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, fTwoWT.getXz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(fTwoWT.isFreeFluxes());
    BOOST_CHECK_EQUAL(WindingConnectionType::Y_GROUNDED, fTwoWT.getConnectionType1());
    BOOST_CHECK_EQUAL(WindingConnectionType::DELTA, fTwoWT.getConnectionType2());
    BOOST_CHECK_CLOSE(0.02, fTwoWT.getGroundingR1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.3, fTwoWT.getGroundingX1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.04, fTwoWT.getGroundingR2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.95, fTwoWT.getGroundingX2(), std::numeric_limits<double>::epsilon());
    fTwoWT.setRz(0.11);
    fTwoWT.setXz(2.03);
    fTwoWT.setFreeFluxes(false);
    fTwoWT.setConnectionType1(WindingConnectionType::Y);
    fTwoWT.setConnectionType2(WindingConnectionType::Y_GROUNDED);
    fTwoWT.setGroundingR1(0.03);
    fTwoWT.setGroundingX1(0.33);
    fTwoWT.setGroundingR2(0.045);
    fTwoWT.setGroundingX2(0.0001);
    BOOST_CHECK_CLOSE(0.11, fTwoWT.getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.03, fTwoWT.getXz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!fTwoWT.isFreeFluxes());
    BOOST_CHECK_EQUAL(WindingConnectionType::Y, fTwoWT.getConnectionType1());
    BOOST_CHECK_EQUAL(WindingConnectionType::Y_GROUNDED, fTwoWT.getConnectionType2());
    BOOST_CHECK_CLOSE(0.03, fTwoWT.getGroundingR1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.33, fTwoWT.getGroundingX1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.045, fTwoWT.getGroundingR2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0001, fTwoWT.getGroundingX2(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(0.1, fThreeWT.getLeg1().getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, fThreeWT.getLeg1().getXz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(fThreeWT.getLeg1().isFreeFluxes());
    BOOST_CHECK_EQUAL(WindingConnectionType::Y_GROUNDED, fThreeWT.getLeg1().getConnectionType());
    BOOST_CHECK_CLOSE(0.02, fThreeWT.getLeg1().getGroundingR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.3, fThreeWT.getLeg1().getGroundingX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, fThreeWT.getLeg2().getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.1, fThreeWT.getLeg2().getXz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!fThreeWT.getLeg2().isFreeFluxes());
    BOOST_CHECK_EQUAL(WindingConnectionType::Y, fThreeWT.getLeg2().getConnectionType());
    BOOST_CHECK_CLOSE(0.12, fThreeWT.getLeg2().getGroundingR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.4, fThreeWT.getLeg2().getGroundingX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.3, fThreeWT.getLeg3().getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.2, fThreeWT.getLeg3().getXz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(fThreeWT.getLeg3().isFreeFluxes());
    BOOST_CHECK_EQUAL(WindingConnectionType::DELTA, fThreeWT.getLeg3().getConnectionType());
    BOOST_CHECK_CLOSE(0.22, fThreeWT.getLeg3().getGroundingR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.5, fThreeWT.getLeg3().getGroundingX(), std::numeric_limits<double>::epsilon());
    fThreeWT.getLeg1().setRz(1.1);
    fThreeWT.getLeg1().setXz(3.0);
    fThreeWT.getLeg1().setFreeFluxes(false);
    fThreeWT.getLeg1().setConnectionType(WindingConnectionType::DELTA);
    fThreeWT.getLeg1().setGroundingR(1.2);
    fThreeWT.getLeg1().setGroundingX(3.1);
    BOOST_CHECK_CLOSE(1.1, fThreeWT.getLeg1().getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.0, fThreeWT.getLeg1().getXz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!fThreeWT.getLeg1().isFreeFluxes());
    BOOST_CHECK_EQUAL(WindingConnectionType::DELTA, fThreeWT.getLeg1().getConnectionType());
    BOOST_CHECK_CLOSE(1.2, fThreeWT.getLeg1().getGroundingR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.1, fThreeWT.getLeg1().getGroundingX(), std::numeric_limits<double>::epsilon());
}

BOOST_FIXTURE_TEST_CASE(GeneratorFortescueXmlSerializerTest, test::ResourceFixture) {
    Network network = createGeneratorFortescueNetwork();
    network.setCaseDate(stdcxx::DateTime::parse("2016-12-07T11:18:52.881+01:00"));
    GeneratorFortescue& fGen = network.getGenerator("GEN").getExtension<GeneratorFortescue>();

    const std::string& networkStr = ResourceFixture::getResource("/fortescue/generatorFortescueRef.xml");

    Network network2 = test::converter::RoundTrip::runXml(network, networkStr);

    GeneratorFortescue& fGen2 = network2.getGenerator("GEN").getExtension<GeneratorFortescue>();
    BOOST_CHECK_CLOSE(fGen.getRz(), fGen2.getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(fGen.getRn(), fGen2.getRn(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(fGen.getXz(), fGen2.getXz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(fGen.getXn(), fGen2.getXn(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(fGen.isGrounded(), fGen2.isGrounded());
    BOOST_CHECK_CLOSE(fGen.getGroundingR(), fGen2.getGroundingR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(fGen.getGroundingX(), fGen2.getGroundingX(), std::numeric_limits<double>::epsilon());
}

BOOST_FIXTURE_TEST_CASE(LineFortescueXmlSerializerTest, test::ResourceFixture) {
    Network network = createLineFortescueNetwork();
    network.setCaseDate(stdcxx::DateTime::parse("2016-12-07T11:18:52.881+01:00"));
    LineFortescue& fLine = network.getLine("NHV1_NHV2_1").getExtension<LineFortescue>();

    const std::string& networkStr = ResourceFixture::getResource("/fortescue/lineFortescueRef.xml");

    Network network2 = test::converter::RoundTrip::runXml(network, networkStr);

    LineFortescue& fLine2 = network2.getLine("NHV1_NHV2_1").getExtension<LineFortescue>();

    BOOST_CHECK_CLOSE(fLine.getRz(), fLine2.getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(fLine.getXz(), fLine2.getXz(), std::numeric_limits<double>::epsilon());
}

BOOST_FIXTURE_TEST_CASE(TwoWTFortescueXmlSerializerTest, test::ResourceFixture) {
    Network network = create2WTFortescueNetwork();
    network.setCaseDate(stdcxx::DateTime::parse("2016-12-07T11:18:52.881+01:00"));
    TwoWindingsTransformerFortescue& f2WT = network.getTwoWindingsTransformer("NGEN_NHV1").getExtension<TwoWindingsTransformerFortescue>();

    const std::string& networkStr = ResourceFixture::getResource("/fortescue/twoWindingsTransformerFortescueRef.xml");

    Network network2 = test::converter::RoundTrip::runXml(network, networkStr);

    TwoWindingsTransformerFortescue& f2WT2 = network2.getTwoWindingsTransformer("NGEN_NHV1").getExtension<TwoWindingsTransformerFortescue>();

    BOOST_CHECK_CLOSE(f2WT.getRz(), f2WT2.getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(f2WT.getXz(), f2WT2.getXz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(f2WT.isFreeFluxes(), f2WT2.isFreeFluxes());
    BOOST_CHECK_EQUAL(f2WT.getConnectionType1(), f2WT2.getConnectionType1());
    BOOST_CHECK_EQUAL(f2WT.getConnectionType2(), f2WT2.getConnectionType2());
    BOOST_CHECK_CLOSE(f2WT.getGroundingR1(), f2WT2.getGroundingR1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(f2WT.getGroundingX1(), f2WT2.getGroundingX1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(f2WT.getGroundingR2(), f2WT2.getGroundingR2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(f2WT.getGroundingX2(), f2WT2.getGroundingX2(), std::numeric_limits<double>::epsilon());
}

BOOST_FIXTURE_TEST_CASE(ThreeWTFortescueXmlSerializerTest, test::ResourceFixture) {
    Network network = create3WTFortescueNetwork();
    network.setCaseDate(stdcxx::DateTime::parse("2016-12-07T11:18:52.881+01:00"));
    ThreeWindingsTransformerFortescue& f3WT = network.getThreeWindingsTransformer("3WT").getExtension<ThreeWindingsTransformerFortescue>();

    const std::string& networkStr = ResourceFixture::getResource("/fortescue/threeWindingsTransformerFortescueRef.xml");

    Network network2 = test::converter::RoundTrip::runXml(network, networkStr);

    ThreeWindingsTransformerFortescue& f3WT2 = network2.getThreeWindingsTransformer("3WT").getExtension<ThreeWindingsTransformerFortescue>();

    BOOST_CHECK_CLOSE(f3WT.getLeg1().getRz(), f3WT2.getLeg1().getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(f3WT.getLeg1().getXz(), f3WT2.getLeg1().getXz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(f3WT.getLeg1().isFreeFluxes(), f3WT2.getLeg1().isFreeFluxes());
    BOOST_CHECK_EQUAL(f3WT.getLeg1().getConnectionType(), f3WT2.getLeg1().getConnectionType());
    BOOST_CHECK_CLOSE(f3WT.getLeg1().getGroundingR(), f3WT2.getLeg1().getGroundingR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(f3WT.getLeg1().getGroundingX(), f3WT2.getLeg1().getGroundingX(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(f3WT.getLeg2().getRz(), f3WT2.getLeg2().getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(f3WT.getLeg2().getXz(), f3WT2.getLeg2().getXz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(f3WT.getLeg2().isFreeFluxes(), f3WT2.getLeg2().isFreeFluxes());
    BOOST_CHECK_EQUAL(f3WT.getLeg2().getConnectionType(), f3WT2.getLeg2().getConnectionType());
    BOOST_CHECK_CLOSE(f3WT.getLeg2().getGroundingR(), f3WT2.getLeg2().getGroundingR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(f3WT.getLeg2().getGroundingX(), f3WT2.getLeg2().getGroundingX(), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(f3WT.getLeg3().getRz(), f3WT2.getLeg3().getRz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(f3WT.getLeg3().getXz(), f3WT2.getLeg3().getXz(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(f3WT.getLeg3().isFreeFluxes(), f3WT2.getLeg3().isFreeFluxes());
    BOOST_CHECK_EQUAL(f3WT.getLeg3().getConnectionType(), f3WT2.getLeg3().getConnectionType());
    BOOST_CHECK_CLOSE(f3WT.getLeg3().getGroundingR(), f3WT2.getLeg3().getGroundingR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(f3WT.getLeg3().getGroundingX(), f3WT2.getLeg3().getGroundingX(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

