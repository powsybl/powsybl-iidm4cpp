/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/shortcircuits/VoltageLevelShortCircuits.hpp>
#include <powsybl/iidm/extensions/shortcircuits/VoltageLevelShortCircuitsXmlSerializer.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace shortcircuits {

BOOST_AUTO_TEST_SUITE(VoltageLevelShortCircuitsTestSuite)

BOOST_AUTO_TEST_CASE(VoltageLevelShortCircuitsConstructor) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    VoltageLevel& voltageLevel = network.getVoltageLevel("VLGEN");

    VoltageLevelShortCircuits extension(voltageLevel, 0.1, 0.2);
    BOOST_CHECK_EQUAL("voltageLevelShortCircuits", extension.getName());
    BOOST_CHECK(stdcxx::areSame(voltageLevel, extension.getExtendable().get()));
    BOOST_CHECK_CLOSE(0.1, extension.getMinShortCircuitsCurrent(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, extension.getMaxShortCircuitsCurrent(), std::numeric_limits<double>::epsilon());

    extension.setMaxShortCircuitsCurrent(0.4);
    extension.setMinShortCircuitsCurrent(0.3);
    BOOST_CHECK_CLOSE(0.3, extension.getMinShortCircuitsCurrent(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.4, extension.getMaxShortCircuitsCurrent(), std::numeric_limits<double>::epsilon());

    // nan checks
    POWSYBL_ASSERT_THROW(VoltageLevelShortCircuits(voltageLevel, stdcxx::nan(), 0.1), PowsyblException, "Short circuit current limit is not defined.");
    POWSYBL_ASSERT_THROW(VoltageLevelShortCircuits(voltageLevel, 0.2, stdcxx::nan()), PowsyblException, "Short circuit current limit is not defined.");
    POWSYBL_ASSERT_THROW(extension.setMinShortCircuitsCurrent(stdcxx::nan()), PowsyblException, "Short circuit current limit is not defined.");
    POWSYBL_ASSERT_THROW(extension.setMaxShortCircuitsCurrent(stdcxx::nan()), PowsyblException, "Short circuit current limit is not defined.");

    // >=0 checks
    POWSYBL_ASSERT_THROW(VoltageLevelShortCircuits(voltageLevel, -1.0, 0.2), PowsyblException, "Short circuit current limit may not be negative.");
    POWSYBL_ASSERT_THROW(VoltageLevelShortCircuits(voltageLevel, 0.1, -1.0), PowsyblException, "Short circuit current limit may not be negative.");
    POWSYBL_ASSERT_THROW(extension.setMinShortCircuitsCurrent(-1.0), PowsyblException, "Short circuit current limit may not be negative.");
    POWSYBL_ASSERT_THROW(extension.setMaxShortCircuitsCurrent(-1.0), PowsyblException, "Short circuit current limit may not be negative.");

    // min <= max check
    POWSYBL_ASSERT_THROW(VoltageLevelShortCircuits(voltageLevel, 0.2, 0.1), PowsyblException, "Min short circuit current > Max short circuit current.");
}

BOOST_FIXTURE_TEST_CASE(VoltageLevelShortCircuitsXmlSerializerTest, ResourceFixture) {
    Network network = ::powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2017-06-25T17:43:00.000+01:00"));
    VoltageLevel& voltageLevel = network.getVoltageLevel("VLGEN");
    voltageLevel.addExtension(Extension::create<VoltageLevelShortCircuits>(voltageLevel, 1.23, 2.34));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("voltageLevelShortCircuits", stdcxx::make_unique<VoltageLevelShortCircuitsXmlSerializer>());

    const std::string& networkStr = ResourceFixture::getResource("/extensions/shortcircuits/voltageLevelShortCircuits.xml");

    converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace shortcircuits

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

