/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/cvg/HvdcAngleDroopActivePowerControl.hpp>
#include <powsybl/iidm/extensions/cvg/HvdcAngleDroopActivePowerControlXmlSerializer.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

#include "iidm/converter/ResourceFixture.hpp"

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

BOOST_AUTO_TEST_SUITE(HvdcAngleDroopActivePowerControlTestSuite)

BOOST_FIXTURE_TEST_CASE(HvdcAngleDroopActivePowerControlConstructor, ResourceFixture) {
    const std::string& networkStr = ResourceFixture::getResource("/VscRoundTripRef.xml");
    Network network = Network::readXml(networkStr);
    HvdcLine& line = network.getHvdcLine("L");

    HvdcAngleDroopActivePowerControl extension(line, 1.0, 2.0, true);
    BOOST_CHECK_EQUAL("hvdcAngleDroopActivePowerControl", extension.getName());
    BOOST_CHECK(stdcxx::areSame(line, extension.getExtendable().get()));
    BOOST_CHECK_CLOSE(1.0, extension.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, extension.getDroop(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(extension.isEnabled());

    extension.setDroop(11.0);
    BOOST_CHECK_CLOSE(11.0, extension.getDroop(), std::numeric_limits<double>::epsilon());

    extension.setP0(22.0);
    BOOST_CHECK_CLOSE(22.0, extension.getP0(), std::numeric_limits<double>::epsilon());

    extension.setEnabled(false);
    BOOST_CHECK(!extension.isEnabled());

    POWSYBL_ASSERT_THROW(HvdcAngleDroopActivePowerControl hadapc(line, stdcxx::nan(), 2.0, true), PowsyblException, "p0 is not set");
    POWSYBL_ASSERT_THROW(extension.setP0(stdcxx::nan()), PowsyblException, "p0 is not set");
    POWSYBL_ASSERT_THROW(extension.setDroop(stdcxx::nan()), PowsyblException, "droop is not set");
}

BOOST_FIXTURE_TEST_CASE(HvdcAngleDroopActivePowerControlSerializerTest, ResourceFixture) {
    const std::string& networkStr = ResourceFixture::getResource("/VscRoundTripRef.xml");
    Network network = Network::readXml(networkStr);
    HvdcLine& line = network.getHvdcLine("L");
    line.addExtension(Extension::create<HvdcAngleDroopActivePowerControl>(line, 11.1, 22.2, true));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("hvdcAngleDroopActivePowerControl", stdcxx::make_unique<HvdcAngleDroopActivePowerControlXmlSerializer>());

    const std::string& networkStrRef = ResourceFixture::getResource("/extensions/cvg/hvdcAngleDroopActivePowerControl.xml");

    converter::RoundTrip::runXml(network, networkStrRef);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

