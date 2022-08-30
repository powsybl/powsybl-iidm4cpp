/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/util/DanglingLineData.hpp>
#include <powsybl/stdcxx/DateTime.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

Network createDanglingDataNetwork() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T12:27:58.535+02:00"));
    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setNominalV(400)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl.getBusBreakerView().newBus()
        .setId("B")
        .add();
    vl.getBusBreakerView().getBus("B").get().setAngle(-8.60);
    vl.getBusBreakerView().getBus("B").get().setV(406.62);
    vl.newDanglingLine()
        .setId("DL")
        .setBus("B")
        .setR(0.05)
        .setX(0.2)
        .setG(0.0)
        .setB(0.000001)
        .setP0(-367.40)
        .setQ0(63.73)
        .add();

    return network;
}

bool dlCompareBoundaryBusVoltage(const DanglingLineData& dlData, double boundaryBusU, double boundaryBusAngle) {
    double tol = 0.00001;
    return !(std::abs(dlData.getBoundaryBusU() - boundaryBusU) > tol || std::abs(stdcxx::toDegrees * dlData.getBoundaryBusTheta() - boundaryBusAngle) > tol);
}

BOOST_AUTO_TEST_SUITE(DanglingLineDataTestSuite)

BOOST_AUTO_TEST_CASE(test) {
    Network network = createDanglingDataNetwork();
    const DanglingLine& danglingLine = network.getDanglingLine("DL");
    DanglingLineData dlData(danglingLine);

    BOOST_CHECK(dlCompareBoundaryBusVoltage(dlData, 406.63382758266334, -8.573434828294932));
}

BOOST_AUTO_TEST_CASE(testP0Q0zero) {
    Network network = createDanglingDataNetwork();
    DanglingLine& danglingLine = network.getDanglingLine("DL");
    danglingLine.setP0(0.0);
    danglingLine.setQ0(0.0);
    DanglingLineData dlData(danglingLine);

    BOOST_CHECK(dlCompareBoundaryBusVoltage(dlData, 406.6200406620039, -8.60000143239463));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
