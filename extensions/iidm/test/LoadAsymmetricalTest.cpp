/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/extensions/iidm/LoadAsymmetrical.hpp>
#include <powsybl/iidm/extensions/iidm/LoadAsymmetricalAdder.hpp>

#include <powsybl/network/EurostagFactory.hpp>

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(LoadAsymmetricalTestSuite)

Network createNetworkwithAsymmLoad() {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();

    Load& load = network.getLoad("LOAD");
    load.newExtension<LoadAsymmetricalAdder>()
                .withConnectionType(LoadConnectionType::DELTA)
                .withDeltaPa(-1)
                .withDeltaQa(1)
                .withDeltaPc(-2)
                .withDeltaQc(2)
                .add();

    return network;
}

BOOST_AUTO_TEST_CASE(LoadAsymmConstructorTest) {
    Network network = createNetworkwithAsymmLoad();

    LoadAsymmetrical& asym = network.getLoad("LOAD").getExtension<LoadAsymmetrical>();

    BOOST_CHECK_EQUAL(LoadConnectionType::DELTA, asym.getConnectionType());
    BOOST_CHECK_CLOSE(-1, asym.getDeltaPa(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1, asym.getDeltaQa(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0, asym.getDeltaPb(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0, asym.getDeltaQb(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-2, asym.getDeltaPc(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2, asym.getDeltaQc(), std::numeric_limits<double>::epsilon());

    asym.setConnectionType(LoadConnectionType::Y);
    asym.setDeltaPa(-1.5);
    asym.setDeltaQa(1.5);
    asym.setDeltaPb(-0.5);
    asym.setDeltaQb(0.5);
    asym.setDeltaPc(-2.5);
    asym.setDeltaQc(2.5);

    BOOST_CHECK_EQUAL(LoadConnectionType::Y, asym.getConnectionType());
    BOOST_CHECK_CLOSE(-1.5, asym.getDeltaPa(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.5, asym.getDeltaQa(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-0.5, asym.getDeltaPb(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.5, asym.getDeltaQb(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-2.5, asym.getDeltaPc(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.5, asym.getDeltaQc(), std::numeric_limits<double>::epsilon());
}

BOOST_FIXTURE_TEST_CASE(GeneratorFortescueXmlSerializerTest, test::ResourceFixture) {
    Network network = createNetworkwithAsymmLoad();
    network.setCaseDate(stdcxx::DateTime::parse("2016-12-07T11:18:52.881+01:00"));
    LoadAsymmetrical& asym = network.getLoad("LOAD").getExtension<LoadAsymmetrical>();

    const std::string& networkStr = ResourceFixture::getResource("/loadAsymmetricalRef.xml");

    Network network2 = test::converter::RoundTrip::runXml(network, networkStr);

    LoadAsymmetrical& asym2 = network2.getLoad("LOAD").getExtension<LoadAsymmetrical>();

    BOOST_CHECK_EQUAL(LoadConnectionType::DELTA, asym2.getConnectionType());
    BOOST_CHECK_CLOSE(asym.getDeltaPa(), asym2.getDeltaPa(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(asym.getDeltaQa(), asym2.getDeltaQa(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(asym.getDeltaPb(), asym2.getDeltaPb(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(asym.getDeltaQb(), asym2.getDeltaQb(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(asym.getDeltaPc(), asym2.getDeltaPc(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(asym.getDeltaQc(), asym2.getDeltaQc(), std::numeric_limits<double>::epsilon());

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

