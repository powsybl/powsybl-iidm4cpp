/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/entsoe/Xnode.hpp>
#include <powsybl/iidm/extensions/entsoe/XnodeXmlSerializer.hpp>
#include <powsybl/network/EurostagFactory.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/extensions/ExtensionRoundTrip.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

BOOST_AUTO_TEST_SUITE(XnodeTestSuite)

Network createNetwork() {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    VoltageLevel& vl = network.getVoltageLevel("VLGEN");
    vl.newDanglingLine()
        .setId("DL1")
        .setBus("NGEN")
        .setP0(0.0)
        .setQ0(0.0)
        .setR(1.0)
        .setX(0.1)
        .setG(0.00005)
        .setB(0.00005)
        .add();

    return network;
}

BOOST_AUTO_TEST_CASE(XnodeTest) {
    Network network = createNetwork();
    DanglingLine& dl = network.getDanglingLine("DL1");

    Xnode extension(dl, "countryCode");
    BOOST_CHECK_EQUAL("xnode", extension.getName());
    BOOST_CHECK(stdcxx::areSame(dl, extension.getExtendable().get()));
    BOOST_CHECK_EQUAL("countryCode", extension.getCode());
}

BOOST_FIXTURE_TEST_CASE(XnodeXmlSerializerTest, ResourceFixture) {
    Network network = createNetwork();
    DanglingLine& dl = network.getDanglingLine("DL1");

    Xnode ext(dl, "countryCode");
    XnodeXmlSerializer serializer;

    const std::string& extensionStr = ResourceFixture::getResource("/extensions/entsoe/xnode.xml");

    ExtensionRoundTrip::runXml(dl, ext, serializer, extensionStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

