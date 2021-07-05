/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/iidm/HvdcAngleDroopActivePowerControl.hpp>
#include <powsybl/iidm/extensions/iidm/HvdcAngleDroopActivePowerControlAdder.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(HvdcAngleDroopActivePowerControlTestSuite)

BOOST_FIXTURE_TEST_CASE(HvdcAngleDroopActivePowerControlConstructor, test::ResourceFixture) {
    Network network = Network::readXml(ResourceFixture::getResourcePath("VscRoundTripRef.xml"));
    HvdcLine& line = network.getHvdcLine("L");

    POWSYBL_ASSERT_THROW(HvdcAngleDroopActivePowerControl(line, stdcxx::nan(), 2.0, true), PowsyblException, "p0 is not set");
    POWSYBL_ASSERT_THROW(HvdcAngleDroopActivePowerControl(line, 1.0, stdcxx::nan(), true), PowsyblException, "droop is not set");

    HvdcAngleDroopActivePowerControl extension(line, 1.0, 2.0, true);
    BOOST_CHECK_EQUAL("hvdcAngleDroopActivePowerControl", extension.getName());
    BOOST_CHECK(stdcxx::areSame(line, extension.getExtendable().get()));
    BOOST_CHECK_CLOSE(1.0, extension.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, extension.getDroop(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(extension.isEnabled());

    POWSYBL_ASSERT_THROW(extension.setDroop(stdcxx::nan()), PowsyblException, "droop is not set");
    extension.setDroop(11.0);
    BOOST_CHECK_CLOSE(11.0, extension.getDroop(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(extension.setP0(stdcxx::nan()), PowsyblException, "p0 is not set");
    extension.setP0(22.0);
    BOOST_CHECK_CLOSE(22.0, extension.getP0(), std::numeric_limits<double>::epsilon());

    extension.setEnabled(false);
    BOOST_CHECK(!extension.isEnabled());
}

BOOST_FIXTURE_TEST_CASE(HvdcAngleDroopActivePowerControlSerializerTest, test::ResourceFixture) {
    Network network = Network::readXml(ResourceFixture::getResourcePath("VscRoundTripRef.xml"));
    HvdcLine& line = network.getHvdcLine("L");
    line.newExtension<HvdcAngleDroopActivePowerControlAdder>().withP0(11.1).withDroop(22.2).withEnabled(true).add();

    const std::string& networkStrRef = ResourceFixture::getResource("/hvdcAngleDroopActivePowerControl.xml");

    test::converter::RoundTrip::runXml(network, networkStrRef);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
