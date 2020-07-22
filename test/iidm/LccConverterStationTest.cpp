/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/stdcxx/math.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(LccConverterStationTestSuite)

BOOST_AUTO_TEST_CASE(adder) {
    Network network = createHvdcConverterStationTestNetwork();

    unsigned long lccCount = network.getLccConverterStationCount();
    unsigned long hvdcCount = network.getHvdcConverterStationCount();
    BOOST_CHECK_EQUAL(lccCount + network.getVscConverterStationCount(), hvdcCount);

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    LccConverterStationAdder adder = vl1.newLccConverterStation()
        .setId("LCC1")
        .setBus("VL1_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "lccConverterStation 'LCC1': loss factor is invalid");
    adder.setLossFactor(-10.0);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "lccConverterStation 'LCC1': loss factor must be >= 0");
    adder.setLossFactor(10.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "lccConverterStation 'LCC1': power factor is invalid");
    adder.setPowerFactor(50.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "lccConverterStation 'LCC1': power factor is invalid, it should be between -1 and 1");
    adder.setPowerFactor(0.1);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'LccConverterStation' with the id 'LCC1'");
    adder.setEnsureIdUnicity(true);

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL(lccCount + 1, network.getLccConverterStationCount());
    BOOST_CHECK_EQUAL(hvdcCount + 1, network.getHvdcConverterStationCount());
}

BOOST_AUTO_TEST_CASE(constructor) {
    const Network& network = createHvdcConverterStationTestNetwork();

    const LccConverterStation& lcc = network.getLccConverterStation("LCC1");
    const HvdcConverterStation& hvdc = network.getHvdcConverterStation("LCC1");
    BOOST_TEST(stdcxx::areSame(lcc, hvdc));
    BOOST_CHECK_EQUAL("LCC1", lcc.getId());
    BOOST_CHECK_EQUAL(lcc.getId(), hvdc.getId());
    BOOST_CHECK_EQUAL("LCC1_NAME", lcc.getOptionalName());
    BOOST_CHECK_EQUAL(ConnectableType::HVDC_CONVERTER_STATION, lcc.getType());
    std::ostringstream oss;
    oss << lcc.getType();
    BOOST_CHECK_EQUAL("HVDC_CONVERTER_STATION", oss.str());
    BOOST_CHECK_EQUAL(hvdc.getType(), lcc.getType());
    POWSYBL_ASSERT_ENUM_EQ(HvdcConverterStation::HvdcType::LCC, lcc.getHvdcType());
    POWSYBL_ASSERT_ENUM_EQ(lcc.getHvdcType(), hvdc.getHvdcType());
    BOOST_CHECK_CLOSE(1.0, lcc.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(lcc.getLossFactor(), hvdc.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(1.0, lcc.getPowerFactor(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createHvdcConverterStationTestNetwork();

    LccConverterStation& lcc = network.getLccConverterStation("LCC1");
    HvdcConverterStation& hvdc = network.getHvdcConverterStation("LCC1");
    BOOST_TEST(stdcxx::areSame(lcc, hvdc));

    BOOST_TEST(stdcxx::areSame(lcc, lcc.setLossFactor(100.0)));
    BOOST_CHECK_CLOSE(100.0, lcc.getLossFactor(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(lcc.getLossFactor(), hvdc.getLossFactor(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(lcc.setLossFactor(stdcxx::nan()), ValidationException, "lccConverterStation 'LCC1': loss factor is invalid");
    POWSYBL_ASSERT_THROW(lcc.setLossFactor(-100.0), ValidationException, "lccConverterStation 'LCC1': loss factor must be >= 0");

    BOOST_TEST(stdcxx::areSame(lcc, lcc.setPowerFactor(0.3)));
    BOOST_TEST(stdcxx::areSame(hvdc, lcc.setPowerFactor(0.4)));
    BOOST_CHECK_CLOSE(0.4, lcc.getPowerFactor(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(lcc.setPowerFactor(stdcxx::nan()), ValidationException, "lccConverterStation 'LCC1': power factor is invalid");

    lcc.remove();
    POWSYBL_ASSERT_THROW(network.getLccConverterStation("LCC1"), PowsyblException, "Unable to find to the identifiable 'LCC1'");
}

BOOST_AUTO_TEST_CASE(loop) {
    Network network = createHvdcConverterStationTestNetwork();

    unsigned long lccCount = network.getLccConverterStationCount();
    unsigned long hvdcCount = network.getHvdcConverterStationCount();
    BOOST_CHECK_EQUAL(lccCount + network.getVscConverterStationCount(), hvdcCount);

    unsigned long lccLoopCount = boost::size(network.getLccConverterStations());
    unsigned long hvdcLoopCount = boost::size(network.getHvdcConverterStations());
    BOOST_CHECK_EQUAL(lccLoopCount, lccCount);
    BOOST_CHECK_EQUAL(hvdcLoopCount, hvdcCount);

    const Network& cNetwork = network;
    BOOST_CHECK_EQUAL(boost::size(network.getLccConverterStations()), boost::size(cNetwork.getLccConverterStations()));
    BOOST_CHECK_EQUAL(boost::size(network.getHvdcConverterStations()), boost::size(cNetwork.getHvdcConverterStations()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
