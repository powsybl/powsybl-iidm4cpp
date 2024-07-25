/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurement.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurementAdder.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurements.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurementsAdder.hpp>
#include <powsybl/network/FourSubstationsNodeBreakerFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(DiscreteMeasurementsTestSuite)

BOOST_AUTO_TEST_CASE(discreteMeasurement) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    Switch& aSwitch = network.getSwitch("S1VL1_LD1_BREAKER");

    aSwitch.newExtension<DiscreteMeasurementsAdder>().add();
    auto& switchMeasurements = aSwitch.getExtension<DiscreteMeasurements>();

    auto& m1 = switchMeasurements.newDiscreteMeasurement()
        .setId("m1")
        .setType(DiscreteMeasurement::Type::SHUNT_COMPENSATOR_SECTION)
        .setValue("abcdef")
        .setValid(false)
        .putProperty("prop1_m1", "value1")
        .putProperty("prop2_m1", "value2")
        .add();

    auto& m2 = switchMeasurements.newDiscreteMeasurement()
        .setId("m2")
        .setType(DiscreteMeasurement::Type::SHUNT_COMPENSATOR_SECTION)
        .setValue(1)
        .setValid(true)
        .add();

    auto& m3 = switchMeasurements.newDiscreteMeasurement()
        .setId("m3")
        .setType(DiscreteMeasurement::Type::SHUNT_COMPENSATOR_SECTION)
        .setValue(false)
        .setValid(true)
        .add();

    BOOST_CHECK_EQUAL(3, boost::size(switchMeasurements.getDiscreteMeasurements(DiscreteMeasurement::Type::SHUNT_COMPENSATOR_SECTION)));

    POWSYBL_ASSERT_THROW(m2.getValueAsString(), PowsyblException, "Value type is not STRING but is: INT");
    POWSYBL_ASSERT_THROW(m2.getValueAsBoolean(), PowsyblException, "Value type is not BOOLEAN but is: INT");
    POWSYBL_ASSERT_THROW(m3.getValueAsString(), PowsyblException, "Value type is not STRING but is: BOOLEAN");
    POWSYBL_ASSERT_THROW(m3.getValueAsInt(), PowsyblException, "Value type is not INT but is: BOOLEAN");

    BOOST_CHECK(!m1.getTapChanger());
    BOOST_CHECK_EQUAL(DiscreteMeasurement::Type::SHUNT_COMPENSATOR_SECTION, m1.getType());
    BOOST_CHECK_EQUAL("abcdef", m1.getValueAsString());
    POWSYBL_ASSERT_THROW(m1.getValueAsBoolean(), PowsyblException, "Value type is not BOOLEAN but is: STRING");
    POWSYBL_ASSERT_THROW(m1.getValueAsInt(), PowsyblException, "Value type is not INT but is: STRING");
    BOOST_CHECK(!m1.isValid());
    BOOST_CHECK_EQUAL("value1", m1.getProperty("prop1_m1"));
    BOOST_CHECK_EQUAL("value2", m1.getProperty("prop2_m1"));
    BOOST_CHECK_EQUAL("", m1.getProperty("notExisting"));
    BOOST_CHECK_EQUAL(2, boost::size(m1.getPropertyNames()));
    m1.putProperty("prop", "val");

    POWSYBL_ASSERT_THROW(m1.putProperty("", "val"), PowsyblException, "Unexpected empty property name");

    BOOST_CHECK_EQUAL("val", m1.getProperty("prop"));
    m1.removeProperty("prop");
    BOOST_CHECK_EQUAL("", m1.getProperty("prop"));
    m1.setValid(true);
    BOOST_CHECK(m1.isValid());
    m1.setValue(12);
    BOOST_CHECK_EQUAL(12, m1.getValueAsInt());
    m1.setValue(true);
    BOOST_CHECK(m1.getValueAsBoolean());
    m1.setValue("str");
    BOOST_CHECK_EQUAL("str", m1.getValueAsString());
    m1.setValue(std::string("str"));
    BOOST_CHECK_EQUAL("str", m1.getValueAsString());
    m1.remove();
    BOOST_CHECK_EQUAL(2, boost::size(switchMeasurements.getDiscreteMeasurements(DiscreteMeasurement::Type::SHUNT_COMPENSATOR_SECTION)));

    switchMeasurements.remove(m2);
    BOOST_CHECK_EQUAL(1, boost::size(switchMeasurements.getDiscreteMeasurements(DiscreteMeasurement::Type::SHUNT_COMPENSATOR_SECTION)));
}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    Switch& aSwitch = network.getSwitch("S1VL1_LD1_BREAKER");

    aSwitch.newExtension<DiscreteMeasurementsAdder>().add();
    auto& switchMeasurements = aSwitch.getExtension<DiscreteMeasurements>();

    switchMeasurements.newDiscreteMeasurement()
        .setId("m1")
        .setType(DiscreteMeasurement::Type::SHUNT_COMPENSATOR_SECTION)
        .setValue("abcdef")
        .setValid(false)
        .putProperty("prop1_m1", "value1")
        .putProperty("prop2_m1", "value2")
        .add();

    switchMeasurements.newDiscreteMeasurement()
        .setId("m2")
        .setType(DiscreteMeasurement::Type::SHUNT_COMPENSATOR_SECTION)
        .setValue(1)
        .setValid(true)
        .add();

    switchMeasurements.newDiscreteMeasurement()
        .setId("m3")
        .setType(DiscreteMeasurement::Type::SHUNT_COMPENSATOR_SECTION)
        .setValue(true)
        .setValid(true)
        .add();

    switchMeasurements.newDiscreteMeasurement()
        .setId("m4")
        .setType(DiscreteMeasurement::Type::SWITCH_POSITION)
        .setValue(true)
        .setValid(true)
        .add();

    switchMeasurements.newDiscreteMeasurement()
        .setId("m5")
        .setType(DiscreteMeasurement::Type::SWITCH_POSITION)
        .setValue("str")
        .setValid(true)
        .add();

    switchMeasurements.newDiscreteMeasurement()
        .setId("m6")
        .setType(DiscreteMeasurement::Type::OTHER)
        .setValue("str")
        .setValid(true)
        .add();

    BOOST_CHECK_EQUAL(6, boost::size(switchMeasurements.getDiscreteMeasurements()));
    BOOST_CHECK_EQUAL(3, boost::size(switchMeasurements.getDiscreteMeasurements(DiscreteMeasurement::Type::SHUNT_COMPENSATOR_SECTION)));
    BOOST_CHECK_EQUAL(2, boost::size(switchMeasurements.getDiscreteMeasurements(DiscreteMeasurement::Type::SWITCH_POSITION)));
    BOOST_CHECK_EQUAL(1, boost::size(switchMeasurements.getDiscreteMeasurements(DiscreteMeasurement::Type::OTHER)));

    DiscreteMeasurementAdder swMeasurementAdder = switchMeasurements.newDiscreteMeasurement();

    POWSYBL_ASSERT_THROW(swMeasurementAdder.putProperty("", "value1"), PowsyblException, "Unexpected empty property name");

    swMeasurementAdder.setId("m1");
    POWSYBL_ASSERT_THROW(swMeasurementAdder.add(), PowsyblException, "There is already a discrete measurement with ID m1");

    swMeasurementAdder.setId("m7");
    swMeasurementAdder.setType(DiscreteMeasurement::Type::TAP_POSITION);
    POWSYBL_ASSERT_THROW(swMeasurementAdder.add(), PowsyblException, "TAP_POSITION discrete not linked to a transformer");

    swMeasurementAdder.setType(DiscreteMeasurement::Type::SWITCH_POSITION);
    swMeasurementAdder.setValid(true);
    POWSYBL_ASSERT_THROW(swMeasurementAdder.add(), PowsyblException, "A valid discrete measurement can not have an undefined value");

    swMeasurementAdder.setTapChanger(DiscreteMeasurement::TapChanger::PHASE_TAP_CHANGER);
    POWSYBL_ASSERT_THROW(swMeasurementAdder.add(), PowsyblException, "A tap changer is specified when the measured equipment is not a tap changer");

    
    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("TWT");
    twt.newExtension<DiscreteMeasurementsAdder>().add();
    auto& twtMeasurements = twt.getExtension<DiscreteMeasurements>();

    DiscreteMeasurementAdder twtMeasurementAdder = twtMeasurements.newDiscreteMeasurement();
    twtMeasurementAdder.setId("measure");

    twtMeasurementAdder.setType(DiscreteMeasurement::Type::SWITCH_POSITION);
    POWSYBL_ASSERT_THROW(twtMeasurementAdder.add(), PowsyblException, "SWITCH_POSITION discrete not linked to a switch");

    twtMeasurementAdder.setType(DiscreteMeasurement::Type::TAP_POSITION);
    POWSYBL_ASSERT_THROW(twtMeasurementAdder.add(), PowsyblException, "The measured tap changer must be specified");

    twtMeasurementAdder.setValue("test");
    POWSYBL_ASSERT_THROW(twtMeasurementAdder.add(), PowsyblException, "The measured tap changer must be specified");


    Switch& aSwitch2 = network.getSwitch("S1VL1_BBS_LD1_DISCONNECTOR");
    aSwitch2.newExtension<DiscreteMeasurementsAdder>().add();
    auto& aSwitch2Measurements = aSwitch2.getExtension<DiscreteMeasurements>();
    aSwitch2Measurements.cleanIfEmpty();
    POWSYBL_ASSERT_THROW(aSwitch2.getExtension<DiscreteMeasurements>(), PowsyblException, "Extension powsybl::iidm::extensions::iidm::DiscreteMeasurements not found");
}

BOOST_FIXTURE_TEST_CASE(DiscreteMeasurementsXmlSerializerTest, test::ResourceFixture) {
    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T12:27:58.535+02:00"));

    Switch& sw = network.getSwitch("S1VL1_BBS_LD1_DISCONNECTOR");
    sw.newExtension<DiscreteMeasurementsAdder>().add();
    sw.getExtension<DiscreteMeasurements>()
        .newDiscreteMeasurement()
        .setId("DIS_MEAS_SW_POS")
        .setType(DiscreteMeasurement::Type::SWITCH_POSITION)
        .setValue("CLOSED")
        .setValid(false)
        .putProperty("source", "test")
        .add();

    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("TWT");
    twt.newExtension<DiscreteMeasurementsAdder>().add();
    twt.getExtension<DiscreteMeasurements>()
        .newDiscreteMeasurement()
        .setId("DIS_MEAS_TAP_POS")
        .setType(DiscreteMeasurement::Type::TAP_POSITION)
        .setTapChanger(DiscreteMeasurement::TapChanger::PHASE_TAP_CHANGER)
        .setValue(15)
        .setValid(true)
        .putProperty("source", "test2")
        .add();

    Switch& sw2 = network.getSwitch("S1VL1_BBS_TWT_DISCONNECTOR");
    sw2.newExtension<DiscreteMeasurementsAdder>().add();
    sw2.getExtension<DiscreteMeasurements>()
        .newDiscreteMeasurement()
        .setType(DiscreteMeasurement::Type::SWITCH_POSITION)
        .setValue("CLOSED")
        .setValid(false)
        .putProperty("source", "test")
        .add();
    sw2.getExtension<DiscreteMeasurements>()
        .newDiscreteMeasurement()
        .setType(DiscreteMeasurement::Type::OTHER)
        .setValue("CLOSED")
        .setValid(false)
        .putProperty("source", "test")
        .add();

    const std::string& networkStrRef = ResourceFixture::getResource("/disMeasRef.xml");

    test::converter::RoundTrip::runXml(network, networkStrRef);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
