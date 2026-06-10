/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>


#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/iidm/DroopCurveAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageSourceConverter.hpp>

#include <powsybl/network/DcDetailedNetworkFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace network {

BOOST_AUTO_TEST_SUITE(DroopCurveTestSuite)

BOOST_AUTO_TEST_CASE(testAdder) {
    iidm::Network network = DcDetailedNetworkFactory::createVscSymmetricalMonopole();
    iidm::VoltageSourceConverter& converter = network.getVoltageSourceConverter("VscFr");

    auto adder = converter.newDroopCurve()
            .addSegment(400.0, 10.0)
            .addSegment(100.0, 2.0)
            .addSegment(0.0, 1.0)
            .addSegment(400.0, 5.0) // will not be effective since segment at minV 400 already defined
            .addSegment(0.0, 5.0) // will not be effective since segment at minV 0 already defined
            .setMaxV(1000.0);

    iidm::DroopCurve& curve = adder.add();

    POWSYBL_ASSERT_REF_TRUE(converter.getDroopCurve());
    BOOST_CHECK(stdcxx::areSame(curve, converter.getDroopCurve().get()));

    BOOST_CHECK_CLOSE(1000.0, curve.getMaxV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(3, boost::size(curve.getSegments()));

    BOOST_CHECK_CLOSE(1.0, curve.getK(-4.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, curve.getK(0.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, curve.getK(100.0 - 1e-10), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, curve.getK(100.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, curve.getK(100.0 + 1e-10), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, curve.getK(400.0 - 1e-10), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10.0, curve.getK(400.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10.0, curve.getK(400.0 + 1e-10), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10.0, curve.getK(1000.0 - 1e-10), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10.0, curve.getK(1000.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(10.0, curve.getK(2000.0), std::numeric_limits<double>::epsilon());

    //segments are ordered by minV 
    BOOST_CHECK_CLOSE(0.0, curve.getSegments().front().getMinV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, curve.getSegments().front().getK(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(100.0, curve.getSegments().front().getMaxV(), std::numeric_limits<double>::epsilon());


    auto adderOneSegment = converter.newDroopCurve()
            .addSegment(0.0, 1.0)
            .setMaxV(1000.0);
    iidm::DroopCurve& curveOneSegment = adderOneSegment.add();

    BOOST_CHECK_EQUAL(1, boost::size(curveOneSegment.getSegments()));

    BOOST_CHECK_CLOSE(1.0, curveOneSegment.getK(-4.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, curveOneSegment.getK(0.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, curveOneSegment.getK(1000.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, curveOneSegment.getK(2000.0), std::numeric_limits<double>::epsilon());

    BOOST_CHECK_CLOSE(0.0, curveOneSegment.getSegments().front().getMinV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, curveOneSegment.getSegments().front().getK(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1000.0, curveOneSegment.getSegments().front().getMaxV(), std::numeric_limits<double>::epsilon());

    auto adderNoSegment = converter.newDroopCurve();
    iidm::DroopCurve& curveNoSegment = adderNoSegment.add();
    
    BOOST_CHECK_EQUAL(0, boost::size(curveNoSegment.getSegments()));
    BOOST_CHECK_CLOSE(0.0, curveNoSegment.getK(-4.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, curveNoSegment.getK(1.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, curveNoSegment.getK(1000.0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(std::isnan(curveNoSegment.getMaxV()));
}

BOOST_AUTO_TEST_CASE(invalidSegment) {
    iidm::Network network = DcDetailedNetworkFactory::createVscSymmetricalMonopole();
    iidm::VoltageSourceConverter& converter = network.getVoltageSourceConverter("VscFr");

    auto adder = converter.newDroopCurve();

    POWSYBL_ASSERT_THROW(adder.addSegment(100.0, stdcxx::nan()), iidm::ValidationException, "AC/DC Voltage Source Converter 'VscFr': Droop curve's segment : k is not set");
    POWSYBL_ASSERT_THROW(adder.addSegment(stdcxx::nan(), 10.0), iidm::ValidationException, "AC/DC Voltage Source Converter 'VscFr': Droop curve's segment : min V is not set");

}

BOOST_AUTO_TEST_CASE(invalidMaxV) {
    iidm::Network network = DcDetailedNetworkFactory::createVscSymmetricalMonopole();
    iidm::VoltageSourceConverter& converter = network.getVoltageSourceConverter("VscFr");

    auto adder = converter.newDroopCurve().addSegment(50.0, 1.0).addSegment(150.0, 4.0);

    POWSYBL_ASSERT_THROW(adder.add(), iidm::ValidationException, "AC/DC Voltage Source Converter 'VscFr': Droop curve : max V is not set");
    adder.setMaxV(40.);
    POWSYBL_ASSERT_THROW(adder.add(), iidm::ValidationException, "AC/DC Voltage Source Converter 'VscFr': Droop curve : max V (40) must be >= last segment's min V (150)");

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace network

}  // namespace powsybl
