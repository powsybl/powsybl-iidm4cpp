/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/extensions/entsoe/MergedXnode.hpp>
#include <powsybl/iidm/extensions/entsoe/MergedXnodeXmlSerializer.hpp>
#include <powsybl/network/EurostagFactory.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/extensions/ExtensionRoundTrip.hpp"

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

BOOST_AUTO_TEST_SUITE(MergedXnodeTestSuite)

BOOST_AUTO_TEST_CASE(MergedXnodeTest) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Line& line = network.getLine("NHV1_NHV2_1");

    MergedXnode extension(line, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, "line1", "line2", "countryCode");
    BOOST_CHECK_EQUAL("mergedXnode", extension.getName());
    BOOST_CHECK(stdcxx::areSame(line, extension.getExtendable().get()));
    BOOST_CHECK_EQUAL("countryCode", extension.getCode());
    BOOST_CHECK_EQUAL("line1", extension.getLine1Name());
    BOOST_CHECK_EQUAL("line2", extension.getLine2Name());
    BOOST_CHECK_CLOSE(0.1, extension.getRdp(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, extension.getXdp(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.3, extension.getXnodeP1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.4, extension.getXnodeQ1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.5, extension.getXnodeP2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.6, extension.getXnodeQ2(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(MergedXnode(line, 10, 0.2, 0.3, 0.4, 0.5, 0.6, "", "", ""), PowsyblException, "Invalid divider position: 10");
    POWSYBL_ASSERT_THROW(extension.setRdp(10), PowsyblException, "Invalid divider position: 10");

    POWSYBL_ASSERT_THROW(MergedXnode(line, 0.1, 10, 0.3, 0.4, 0.5, 0.6, "", "", ""), PowsyblException, "Invalid divider position: 10");
    POWSYBL_ASSERT_THROW(extension.setXdp(10), PowsyblException, "Invalid divider position: 10");

    POWSYBL_ASSERT_THROW(MergedXnode(line, 0.1, 0.2, stdcxx::nan(), 0.4, 0.5, 0.6, "", "", ""), PowsyblException, "Power flow is invalid");
    POWSYBL_ASSERT_THROW(extension.setXnodeP1(stdcxx::nan()), PowsyblException, "Power flow is invalid");

    POWSYBL_ASSERT_THROW(MergedXnode(line, 0.1, 0.2, 0.3, stdcxx::nan(), 0.5, 0.6, "", "", ""), PowsyblException, "Power flow is invalid");
    POWSYBL_ASSERT_THROW(extension.setXnodeQ1(stdcxx::nan()), PowsyblException, "Power flow is invalid");

    POWSYBL_ASSERT_THROW(MergedXnode(line, 0.1, 0.2, 0.3, 0.4, stdcxx::nan(), 0.6, "", "", ""), PowsyblException, "Power flow is invalid");
    POWSYBL_ASSERT_THROW(extension.setXnodeP2(stdcxx::nan()), PowsyblException, "Power flow is invalid");

    POWSYBL_ASSERT_THROW(MergedXnode(line, 0.1, 0.2, 0.3, 0.4, 0.5, stdcxx::nan(), "", "", ""), PowsyblException, "Power flow is invalid");
    POWSYBL_ASSERT_THROW(extension.setXnodeQ2(stdcxx::nan()), PowsyblException, "Power flow is invalid");
}

BOOST_FIXTURE_TEST_CASE(XnodeXmlSerializerTest, ResourceFixture) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Line& line = network.getLine("NHV1_NHV2_1");

    MergedXnode ext(line, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, "line1", "line2", "countryCode");
    MergedXnodeXmlSerializer serializer;

    const std::string& extensionStr = ResourceFixture::getResource("/extensions/entsoe/mergedXnode.xml");

    ExtensionRoundTrip::runXml(line, ext, serializer, extensionStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

