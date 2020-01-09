/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/extensions/cvg/StandbyAutomaton.hpp>
#include <powsybl/iidm/extensions/cvg/StandbyAutomatonXmlSerializer.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

BOOST_AUTO_TEST_SUITE(StandbyAutomatonTestSuite)

BOOST_FIXTURE_TEST_CASE(StandbyAutomatonConstructor, ResourceFixture) {
    const std::string& networkStr = ResourceFixture::getResource("/staticVarCompensatorRoundTripRef.xml");
    Network network = Network::readXml(networkStr);
    StaticVarCompensator& svc = network.getStaticVarCompensator("SVC2");

    StandbyAutomaton extension(svc, 1.1, true, 2.2, 3.3, 4.4, 5.5);
    BOOST_CHECK_EQUAL("standbyAutomaton", extension.getName());
    BOOST_CHECK(stdcxx::areSame(svc, extension.getExtendable().get()));
    BOOST_CHECK_CLOSE(1.1, extension.getB0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(extension.isStandby());
    BOOST_CHECK_CLOSE(2.2, extension.getLowVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.3, extension.getHighVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.4, extension.getLowVoltageThreshold(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.5, extension.getHighVoltageThreshold(), std::numeric_limits<double>::epsilon());

    extension.setB0(1.2);
    BOOST_CHECK_CLOSE(1.2, extension.getB0(), std::numeric_limits<double>::epsilon());
    extension.setStandby(false);
    BOOST_CHECK(!extension.isStandby());
    extension.setLowVoltageSetpoint(2.3);
    BOOST_CHECK_CLOSE(2.3, extension.getLowVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    extension.setHighVoltageSetpoint(3.4);
    BOOST_CHECK_CLOSE(3.4, extension.getHighVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    extension.setLowVoltageThreshold(4.5);
    BOOST_CHECK_CLOSE(4.5, extension.getLowVoltageThreshold(), std::numeric_limits<double>::epsilon());
    extension.setHighVoltageThreshold(5.6);
    BOOST_CHECK_CLOSE(5.6, extension.getHighVoltageThreshold(), std::numeric_limits<double>::epsilon());

    extension.setHighVoltageSetpoint(5.7);

    // error cases
    POWSYBL_ASSERT_THROW(extension.setB0(stdcxx::nan()), PowsyblException, "b0 is invalid");
    POWSYBL_ASSERT_THROW(extension.setLowVoltageSetpoint(stdcxx::nan()), PowsyblException, "lowVoltageSetpoint is invalid");
    POWSYBL_ASSERT_THROW(extension.setHighVoltageSetpoint(stdcxx::nan()), PowsyblException, "highVoltageSetpoint is invalid");
    POWSYBL_ASSERT_THROW(extension.setLowVoltageThreshold(stdcxx::nan()), PowsyblException, "lowVoltageThreshold is invalid");
    POWSYBL_ASSERT_THROW(extension.setHighVoltageThreshold(stdcxx::nan()), PowsyblException, "highVoltageThreshold is invalid");
    POWSYBL_ASSERT_THROW(extension.setLowVoltageThreshold(5.7), PowsyblException, "Inconsistent low (5.7) and high (5.6) voltage thresholds");

    POWSYBL_ASSERT_THROW(StandbyAutomaton(svc, stdcxx::nan(), true, 2.2, 3.3, 4.4, 5.5), PowsyblException, "b0 is invalid");
    POWSYBL_ASSERT_THROW(StandbyAutomaton(svc, 1.1, true, stdcxx::nan(), 3.3, 4.4, 5.5), PowsyblException, "lowVoltageSetpoint is invalid");
    POWSYBL_ASSERT_THROW(StandbyAutomaton(svc, 1.1, true, 2.2, stdcxx::nan(), 4.4, 5.5), PowsyblException, "highVoltageSetpoint is invalid");
    POWSYBL_ASSERT_THROW(StandbyAutomaton(svc, 1.1, true, 2.2, 3.3, stdcxx::nan(), 5.5), PowsyblException, "lowVoltageThreshold is invalid");
    POWSYBL_ASSERT_THROW(StandbyAutomaton(svc, 1.1, true, 2.2, 3.3, 4.4, stdcxx::nan()), PowsyblException, "highVoltageThreshold is invalid");
    POWSYBL_ASSERT_THROW(StandbyAutomaton(svc, 1.1, true, 2.2, 3.3, 4.4, 4.3), PowsyblException, "Inconsistent low (4.4) and high (4.3) voltage thresholds");
}

BOOST_FIXTURE_TEST_CASE(StandbyAutomatonXmlSerializerTest, ResourceFixture) {
    const std::string& networkStr = ResourceFixture::getResource("/staticVarCompensatorRoundTripRef.xml");
    Network network = Network::readXml(networkStr);
    StaticVarCompensator& svc = network.getStaticVarCompensator("SVC2");
    svc.addExtension(Extension::create<StandbyAutomaton>(svc, 1.1, true, 2.2, 3.3, 4.4, 5.5));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("standbyAutomaton", stdcxx::make_unique<StandbyAutomatonXmlSerializer>());

    const std::string& networkStrRef = ResourceFixture::getResource("/extensions/cvg/standbyAutomaton.xml");

    converter::RoundTrip::runXml(network, networkStrRef);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

