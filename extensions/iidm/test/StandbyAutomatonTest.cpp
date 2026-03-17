/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/extensions/iidm/StandbyAutomaton.hpp>
#include <powsybl/iidm/extensions/iidm/StandbyAutomatonAdder.hpp>

#include <powsybl/stdcxx/math.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(StandbyAutomatonTestSuite)

BOOST_FIXTURE_TEST_CASE(StandbyAutomatonConstructor, test::ResourceFixture) {
    Network network = Network::readXml(ResourceFixture::getResourcePath("staticVarCompensatorRef.xml"));
    StaticVarCompensator& svc = network.getStaticVarCompensator("SVC2");

    svc.newExtension<StandbyAutomatonAdder>()
        .withB0(1.1)
        .withStandby(true)
        .withLowVoltageSetpoint(2.2)
        .withHighVoltageSetpoint(3.3)
        .withLowVoltageThreshold(4.4)
        .withHighVoltageThreshold(5.5)
        .add();
    auto& extension = svc.getExtension<StandbyAutomaton>();
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
    POWSYBL_ASSERT_THROW(extension.setB0(stdcxx::nan()), ValidationException, "staticVarCompensator 'SVC2': b0 (nan) is invalid");
    POWSYBL_ASSERT_THROW(extension.setLowVoltageSetpoint(stdcxx::nan()), ValidationException, "staticVarCompensator 'SVC2': lowVoltageSetpoint (nan) is invalid");
    POWSYBL_ASSERT_THROW(extension.setHighVoltageSetpoint(stdcxx::nan()), ValidationException, "staticVarCompensator 'SVC2': highVoltageSetpoint (nan) is invalid");
    POWSYBL_ASSERT_THROW(extension.setLowVoltageThreshold(stdcxx::nan()), ValidationException, "staticVarCompensator 'SVC2': lowVoltageThreshold (nan) is invalid");
    POWSYBL_ASSERT_THROW(extension.setHighVoltageThreshold(stdcxx::nan()), ValidationException, "staticVarCompensator 'SVC2': highVoltageThreshold (nan) is invalid");
    POWSYBL_ASSERT_THROW(extension.setLowVoltageThreshold(5.7), ValidationException, "staticVarCompensator 'SVC2': Inconsistent low (5.7) and high (5.6) voltage thresholds");

    //When standby is false : does not throw if inconsistent low and high voltage thresholds :
    extension.setStandby(false);
    BOOST_CHECK(!extension.isStandby());
    extension.setLowVoltageThreshold(5.7);
    BOOST_CHECK_CLOSE(5.7, extension.getLowVoltageThreshold(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.6, extension.getHighVoltageThreshold(), std::numeric_limits<double>::epsilon());
    //But throws when standby set to true afterwards, while still inconsistent:
    POWSYBL_ASSERT_THROW(extension.setStandby(true), ValidationException, "staticVarCompensator 'SVC2': Inconsistent low (5.7) and high (5.6) voltage thresholds");


    auto adder = svc.newExtension<StandbyAutomatonAdder>();
    
    POWSYBL_ASSERT_THROW(adder.withB0(stdcxx::nan()).add(), ValidationException, "staticVarCompensator 'SVC2': b0 (nan) is invalid");
    POWSYBL_ASSERT_THROW(adder.withB0(1.1).withStandby(true).withLowVoltageSetpoint(stdcxx::nan()).add(), ValidationException, "staticVarCompensator 'SVC2': lowVoltageSetpoint (nan) is invalid");
    POWSYBL_ASSERT_THROW(adder.withLowVoltageSetpoint(2.2).withHighVoltageSetpoint(stdcxx::nan()).add(), ValidationException, "staticVarCompensator 'SVC2': highVoltageSetpoint (nan) is invalid");
    POWSYBL_ASSERT_THROW(adder.withHighVoltageSetpoint(3.3).withLowVoltageThreshold(stdcxx::nan()).add(), ValidationException, "staticVarCompensator 'SVC2': lowVoltageThreshold (nan) is invalid");
    POWSYBL_ASSERT_THROW(adder.withLowVoltageThreshold(4.4).withHighVoltageThreshold(stdcxx::nan()).add(), ValidationException, "staticVarCompensator 'SVC2': highVoltageThreshold (nan) is invalid");
    POWSYBL_ASSERT_THROW(adder.withLowVoltageThreshold(4.4).withHighVoltageThreshold(4.3).add(), ValidationException, "staticVarCompensator 'SVC2': Inconsistent low (4.4) and high (4.3) voltage thresholds");
}

BOOST_FIXTURE_TEST_CASE(StandbyAutomatonXmlSerializerTest, test::ResourceFixture) {
    Network network = Network::readXml(ResourceFixture::getResourcePath("staticVarCompensatorRef.xml"));
    StaticVarCompensator& svc = network.getStaticVarCompensator("SVC2");
    svc.newExtension<StandbyAutomatonAdder>()
        .withB0(1.1)
        .withStandby(true)
        .withLowVoltageSetpoint(2.2)
        .withHighVoltageSetpoint(3.3)
        .withLowVoltageThreshold(4.4)
        .withHighVoltageThreshold(5.5)
        .add();

    const std::string& networkStrRef = ResourceFixture::getResource("/standbyAutomaton.xml");

    test::converter::RoundTrip::runXml(network, networkStrRef);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

