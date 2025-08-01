/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/Measurement.hpp>
#include <powsybl/iidm/extensions/iidm/MeasurementAdder.hpp>
#include <powsybl/iidm/extensions/iidm/Measurements.hpp>
#include <powsybl/iidm/extensions/iidm/MeasurementsAdder.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(MeasurementsTestSuite)

BOOST_AUTO_TEST_CASE(measurement) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Load& load = network.getLoad("LOAD");

    load.newExtension<MeasurementsAdder>().add();
    auto& measurements = load.getExtension<Measurements>();
    measurements.newMeasurement()
        .setId("m1")
        .setType(Measurement::Type::CURRENT)
        .setValid(false)
        .setStandardDeviation(1.1)
        .setValue(2.2)
        .putProperty("prop1_m1", "value1")
        .putProperty("prop2_m1", "value2")
        .add();

    measurements.newMeasurement()
        .setId("m2")
        .setType(Measurement::Type::CURRENT)
        .setValid(false)
        .setStandardDeviation(1.1)
        .setValue(2.2)
        .putProperty("prop1_m2", "value1")
        .putProperty("prop2_m2", "value2")
        .add();

    auto& m1 = measurements.getMeasurement("m1").get();
    BOOST_CHECK_EQUAL("m1", m1.getId());
    BOOST_CHECK(!m1.getSide());
    BOOST_CHECK_EQUAL(Measurement::Type::CURRENT, m1.getType());
    BOOST_CHECK(!m1.isValid());
    BOOST_CHECK_CLOSE(1.1, m1.getStandardDeviation(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.2, m1.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2, boost::size(m1.getPropertyNames()));
    BOOST_CHECK_EQUAL("value1", m1.getProperty("prop1_m1"));
    BOOST_CHECK_EQUAL("value2", m1.getProperty("prop2_m1"));
    BOOST_CHECK_EQUAL("", m1.getProperty("notExisting"));

    POWSYBL_ASSERT_THROW(m1.putProperty("", "val"), PowsyblException, "Unexpected empty property name");

    m1.putProperty("prop", "val");
    BOOST_CHECK_EQUAL("val", m1.getProperty("prop"));
    m1.removeProperty("prop");
    BOOST_CHECK_EQUAL("", m1.getProperty("prop"));
    m1.setValue(1.1);
    BOOST_CHECK_CLOSE(1.1, m1.getValue(), std::numeric_limits<double>::epsilon());
    m1.setStandardDeviation(2.2);
    BOOST_CHECK_CLOSE(2.2, m1.getStandardDeviation(), std::numeric_limits<double>::epsilon());
    m1.setValid(false);
    BOOST_CHECK(!m1.isValid());
    m1.remove();
    BOOST_CHECK_EQUAL(1, boost::size(measurements.getMeasurements(Measurement::Type::CURRENT)));

    auto& m2 = measurements.getMeasurement("m2").get();
    measurements.remove(m2);
    BOOST_CHECK_EQUAL(0, boost::size(measurements.getMeasurements(Measurement::Type::CURRENT)));
}

BOOST_AUTO_TEST_CASE(adder) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Load& load = network.getLoad("LOAD");

    load.newExtension<MeasurementsAdder>().add();
    auto& measurements = load.getExtension<Measurements>();
    measurements.newMeasurement()
        .setId("m1")
        .setType(Measurement::Type::CURRENT)
        .setValid(false)
        .setStandardDeviation(1.1)
        .setValue(2.2)
        .putProperty("prop1_m1", "value1")
        .putProperty("prop2_m1", "value2")
        .add();

    POWSYBL_ASSERT_THROW(measurements.newMeasurement().putProperty("", "value1"), PowsyblException, "Unexpected empty property name");

    MeasurementAdder adder = measurements.newMeasurement();
    adder.setId("m1");
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "There is already a measurement with ID m1");

    adder.setId("m2");
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Measurement type can not be null");

    adder.setType(Measurement::Type::CURRENT);
    adder.setValid(true);
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Valid measurement can not have an undefined value");

    adder.setValue(3.3);
    adder.setSide(Measurement::Side::ONE);
    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "Inconsistent side for measurement of injection");

    MeasurementAdder adderUnicity = measurements.newMeasurement();
    adderUnicity.setId("m1")
        .setType(Measurement::Type::ANGLE)
        .setValid(true)
        .setStandardDeviation(8.8)
        .setValue(5.5);
    POWSYBL_ASSERT_THROW(adderUnicity.add(), PowsyblException, "There is already a measurement with ID m1");
    
    Measurement& measurementUniqueId = adderUnicity.setEnsureIdUnicity(true).add();
    BOOST_CHECK_EQUAL("m1#0", measurementUniqueId.getId());

    measurements.newMeasurement()
        .setId("m2")
        .setType(Measurement::Type::CURRENT)
        .setValid(false)
        .setStandardDeviation(3.3)
        .setValue(4.4)
        .putProperty("prop1_m2", "value1")
        .putProperty("prop2_m2", "value2")
        .add();

    measurements.newMeasurement()
        .setId("angle1")
        .setType(Measurement::Type::ANGLE)
        .setValid(true)
        .setStandardDeviation(3.3)
        .setValue(4.4)
        .add();

    measurements.newMeasurement()
        .setId("angle2")
        .setType(Measurement::Type::ANGLE)
        .setValid(true)
        .setStandardDeviation(5.5)
        .setValue(6.6)
        .add();

    measurements.newMeasurement()
        .setId("active_power1")
        .setType(Measurement::Type::ACTIVE_POWER)
        .setValid(true)
        .setStandardDeviation(7.7)
        .setValue(8.8)
        .add();

    BOOST_CHECK_EQUAL(6, boost::size(measurements.getMeasurements()));
    BOOST_CHECK_EQUAL(2, boost::size(measurements.getMeasurements(Measurement::Type::CURRENT)));
    BOOST_CHECK_EQUAL(3, boost::size(measurements.getMeasurements(Measurement::Type::ANGLE)));
    BOOST_CHECK_EQUAL(1, boost::size(measurements.getMeasurements(Measurement::Type::ACTIVE_POWER)));

    Line& line = network.getLine("NHV1_NHV2_1");
    line.newExtension<MeasurementsAdder>().add();
    auto& lineMeasurements = line.getExtension<Measurements>();
    MeasurementAdder lineMeasurementAdder = lineMeasurements.newMeasurement();
    lineMeasurementAdder.setId("m1")
        .setType(Measurement::Type::CURRENT)
        .setValid(true)
        .setStandardDeviation(1.1)
        .setValue(2.2);
    POWSYBL_ASSERT_THROW(lineMeasurementAdder.add(), PowsyblException, "Inconsistent null side for measurement of branch or three windings transformer");

    lineMeasurements.cleanIfEmpty();
    POWSYBL_ASSERT_THROW(line.getExtension<Measurements>(), PowsyblException, "Extension powsybl::iidm::extensions::iidm::Measurements not found");
}

BOOST_FIXTURE_TEST_CASE(MeasurementsXmlSerializerTest, test::ResourceFixture) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T12:27:58.535+02:00"));

    Load& load = network.getLoad("LOAD");
    load.newExtension<MeasurementsAdder>().add();
    load.getExtension<Measurements>()
        .newMeasurement()
        .setId("MEAS_LOAD_P")
        .setType(Measurement::Type::ACTIVE_POWER)
        .setValue(580.0)
        .setStandardDeviation(5.0)
        .setValid(false)
        .putProperty("source", "test")
        .add();

    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("NGEN_NHV1");
    twt.newExtension<MeasurementsAdder>().add();
    twt.getExtension<Measurements>()
        .newMeasurement()
        .setId("MEAS_TWT_Q_2")
        .setType(Measurement::Type::REACTIVE_POWER)
        .setSide(Measurement::Side::TWO)
        .setValue(-600.07)
        .setStandardDeviation(10.2)
        .setValid(true)
        .add();
    twt.getExtension<Measurements>()
        .newMeasurement()
        .setId("MEAS_TWT_Q_1")
        .setType(Measurement::Type::REACTIVE_POWER)
        .setSide(Measurement::Side::ONE)
        .setValue(605.2)
        .setStandardDeviation(9.7)
        .setValid(true)
        .putProperty("source", "test2")
        .add();
    twt.getExtension<Measurements>()
        .newMeasurement()
        .setType(Measurement::Type::ACTIVE_POWER)
        .setSide(Measurement::Side::TWO)
        .setValue(-300.0)
        .setStandardDeviation(10.2)
        .setValid(true)
        .add();
    twt.getExtension<Measurements>()
        .newMeasurement()
        .setType(Measurement::Type::ACTIVE_POWER)
        .setSide(Measurement::Side::ONE)
        .setValue(301.2)
        .setStandardDeviation(9.7)
        .setValid(true)
        .putProperty("source", "test2")
        .add();

    const std::string& networkStrRef = ResourceFixture::getResource("/measRef.xml");

    test::converter::RoundTrip::runXml(network, networkStrRef);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
