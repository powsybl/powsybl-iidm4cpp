/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/network/FictitiousSwitchFactory.hpp> 

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(FictitiousInjectionsXml)

BOOST_FIXTURE_TEST_CASE(FictitiousInjectionsBb, test::ResourceFixture) {

    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));

    network.getBusBreakerView().getBus("NGEN").get().setFictitiousP0(1.0).setFictitiousQ0(2.0);
    network.getBusBreakerView().getBus("NLOAD").get().setFictitiousP0(3.0);
    network.getBusBreakerView().getBus("NHV1").get().setFictitiousQ0(4.0);

    const std::string& networkStrRef = test::converter::RoundTrip::getVersionedNetwork("eurostag-fict-inj.xml",IidmXmlVersion::CURRENT_IIDM_XML_VERSION());
    test::converter::RoundTrip::runXml(network, networkStrRef);

    // backward compatibility
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("eurostag-fict-inj.xml", IidmXmlVersion::V1_8());

}

BOOST_FIXTURE_TEST_CASE(FictitiousInjectionsNb, test::ResourceFixture) {

    Network network = powsybl::network::FictitiousSwitchFactory::create();
    network.getVoltageLevel("C").getNodeBreakerView()
                .setFictitiousP0(0, 1.0)
                .setFictitiousQ0(1, 2.0)
                .setFictitiousP0(2, 3.0)
                .setFictitiousQ0(2, 4.0);

    const std::string& networkStrRef = test::converter::RoundTrip::getVersionedNetwork("fictitiousSwitchRef-fict-inj.xml",IidmXmlVersion::CURRENT_IIDM_XML_VERSION());
    test::converter::RoundTrip::runXml(network, networkStrRef);

    // backward compatibility
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("fictitiousSwitchRef-fict-inj.xml", IidmXmlVersion::V1_8());

}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

