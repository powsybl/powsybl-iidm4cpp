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

#include <powsybl/network/FictitiousSwitchFactory.hpp> 

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(NodeBreakerPropertiesOnBusTestSuite)

BOOST_FIXTURE_TEST_CASE(NodeBreakerPropertiesOnBusTest, test::ResourceFixture) {

    Network network = powsybl::network::FictitiousSwitchFactory::create();
    network.getVoltageLevel("C").getBusView().getBus("C_0").get().setProperty("key_test1","value_test1");
    network.getVoltageLevel("C").getBusView().getBus("C_0").get().setProperty("key_test2","value_test2");

    const std::string& networkStrRef = test::converter::RoundTrip::getVersionedNetwork("nodebreaker-busproperties.xml",IidmXmlVersion::CURRENT_IIDM_XML_VERSION());
    test::converter::RoundTrip::runXml(network, networkStrRef);

    // backward compatibility
    test::converter::RoundTrip::roundTripVersionedXmlTest("nodebreaker-busproperties.xml", IidmXmlVersion::all());

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

