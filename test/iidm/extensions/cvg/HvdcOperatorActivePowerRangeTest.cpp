/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/cvg/HvdcOperatorActivePowerRange.hpp>
#include <powsybl/iidm/extensions/cvg/HvdcOperatorActivePowerRangeXmlSerializer.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

BOOST_AUTO_TEST_SUITE(HvdcOperatorActivePowerRangeTestSuite)

BOOST_FIXTURE_TEST_CASE(HvdcOperatorActivePowerRangeConstructor, ResourceFixture) {
    const std::string& networkStr = ResourceFixture::getResource("/LccRoundTripRef.xml");
    Network network = Network::readXml(networkStr);
    HvdcLine& hvdcLine = network.getHvdcLine("L");

    HvdcOperatorActivePowerRange extension(hvdcLine, 1.1, 2.2);
    BOOST_CHECK_EQUAL("hvdcOperatorActivePowerRange", extension.getName());
    BOOST_CHECK_CLOSE(1.1, extension.getOprFromCS1toCS2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.2, extension.getOprFromCS2toCS1(), std::numeric_limits<double>::epsilon());

    extension.setOprFromCS1toCS2(11.1);
    BOOST_CHECK_CLOSE(11.1, extension.getOprFromCS1toCS2(), std::numeric_limits<double>::epsilon());

    extension.setOprFromCS2toCS1(22.2);
    BOOST_CHECK_CLOSE(22.2, extension.getOprFromCS2toCS1(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(extension.setOprFromCS2toCS1(-1.0), PowsyblException, "OPR from C2 to C1 must be greater than 0.");
}

BOOST_FIXTURE_TEST_CASE(HvdcOperatorActivePowerRangeXmlSerializerTest, ResourceFixture) {
    const std::string& networkStr = ResourceFixture::getResource("/LccRoundTripRef.xml");
    Network network = Network::readXml(networkStr);
    HvdcLine& hvdcLine = network.getHvdcLine("L");
    hvdcLine.addExtension(Extension::create<HvdcOperatorActivePowerRange>(hvdcLine, 0.1, 0.2));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("hvdcOperatorActivePowerRange", stdcxx::make_unique<HvdcOperatorActivePowerRangeXmlSerializer>());

    const std::string& networkStrRef = ResourceFixture::getResource("/extensions/cvg/hvdcOperatorActivePowerRange.xml");

    converter::RoundTrip::runXml(network, networkStrRef);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

