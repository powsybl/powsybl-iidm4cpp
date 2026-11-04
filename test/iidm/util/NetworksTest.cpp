/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/DcBus.hpp>
#include <powsybl/iidm/DcLine.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/LineCommutatedConverter.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>

#include <powsybl/iidm/util/Networks.hpp>

#include <powsybl/network/BatteryNetworkFactory.hpp>
#include <powsybl/network/BoundaryLineNetworkFactory.hpp>
#include <powsybl/network/DcDetailedNetworkFactory.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/network/FourSubstationsNodeBreakerFactory.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(NetworksUtilTestSuite)

BOOST_AUTO_TEST_CASE(applySolvedValuesTest) {

    Network network = powsybl::network::FourSubstationsNodeBreakerFactory::create();
    ShuntCompensator& shuntCompensator = network.getShuntCompensator("SHUNT");
    shuntCompensator.setSolvedSectionCount(0);
    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("TWT");
    twt.getPhaseTapChanger().setSolvedTapPosition(13);
    twt.getRatioTapChanger().setSolvedTapPosition(2);
    // Modify p and q of load so that it is different to P0 and Q0
    Load& load = network.getLoad("LD1");
    load.getTerminal().setP(81).setQ(11);
    // Modify p, q and v of generator so that it is different to the targets
    Generator& generator = network.getGenerator("GH1");
    generator.getTerminal().setP(-86).setQ(-512);
    generator.getTerminal().getBusView().getBus().get().setV(401);

    BOOST_CHECK_NE(shuntCompensator.getSolvedSectionCount().get(), shuntCompensator.getSectionCount());
    BOOST_CHECK_NE(twt.getPhaseTapChanger().getSolvedTapPosition().get(), twt.getPhaseTapChanger().getTapPosition());
    BOOST_CHECK_NE(twt.getRatioTapChanger().getSolvedTapPosition().get(), twt.getRatioTapChanger().getTapPosition());
    BOOST_CHECK_NE(load.getTerminal().getP(), load.getP0());
    BOOST_CHECK_NE(load.getTerminal().getQ(), load.getQ0());
    BOOST_CHECK_NE(-generator.getTerminal().getP(), generator.getTargetP());
    BOOST_CHECK_NE(-generator.getTerminal().getQ(), generator.getTargetQ());
    BOOST_CHECK_NE(generator.getTerminal().getBusBreakerView().getBus().get().getV(), generator.getTargetV());

    Networks::applySolvedValues(network);
    BOOST_CHECK_EQUAL(shuntCompensator.getSolvedSectionCount().get(), shuntCompensator.getSectionCount());
    BOOST_CHECK_EQUAL(twt.getPhaseTapChanger().getSolvedTapPosition().get(), twt.getPhaseTapChanger().getTapPosition());
    BOOST_CHECK_EQUAL(twt.getRatioTapChanger().getSolvedTapPosition().get(), twt.getRatioTapChanger().getTapPosition());
    BOOST_CHECK_CLOSE(load.getTerminal().getP(), load.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(load.getTerminal().getQ(), load.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-generator.getTerminal().getP(), generator.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-generator.getTerminal().getQ(), generator.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(generator.getTerminal().getBusBreakerView().getBus().get().getV(), generator.getTargetV(), std::numeric_limits<double>::epsilon());

}

BOOST_AUTO_TEST_CASE(applySolvedValuesBatteryTest) {
    Network network = powsybl::network::BatteryNetworkFactory::create();
    Battery& battery = network.getBattery("BAT");
    BOOST_CHECK_NE(battery.getTerminal().getP(), battery.getTargetP());
    BOOST_CHECK_NE(battery.getTerminal().getQ(), battery.getTargetQ());

    Networks::applySolvedValues(network);
    BOOST_CHECK_CLOSE(-battery.getTerminal().getP(), battery.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-battery.getTerminal().getQ(), battery.getTargetQ(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(applySolvedValuesBoundaryLineTest) {
    Network network = powsybl::network::BoundaryLineNetworkFactory::createWithGeneration();
    BoundaryLine& bl = network.getBoundaryLine("BL");
    bl.getTerminal().setP(441).setQ(30);
    bl.getTerminal().getBusView().getBus().get().setV(100);
    BOOST_CHECK_NE(-bl.getTerminal().getP(), bl.getGeneration().get().getTargetP());
    BOOST_CHECK_NE(bl.getTerminal().getBusView().getBus().get().getV(), bl.getGeneration().get().getTargetV());

    Networks::applySolvedValues(network);
    BOOST_CHECK_CLOSE(-bl.getTerminal().getP(), bl.getGeneration().get().getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(bl.getTerminal().getBusView().getBus().get().getV(), bl.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());

    bl.getGeneration().get().setVoltageRegulationOn(false).setTargetV(stdcxx::nan()).setTargetQ(35);
    BOOST_CHECK_NE(-bl.getTerminal().getQ(), bl.getGeneration().get().getTargetQ());
    BOOST_CHECK(std::isnan(bl.getGeneration().get().getTargetV()));

    Networks::applySolvedValues(network);
    BOOST_CHECK_CLOSE(-bl.getTerminal().getQ(), bl.getGeneration().get().getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(bl.getTerminal().getBusView().getBus().get().getV(), bl.getGeneration().get().getTargetV(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(unsetSolvedValuesTest) {
    Network network = powsybl::network::EurostagFactory::createWithTieLinesAndAreas();
    TieLine& tl = network.getTieLine("NHV1_NHV2_1");
    Bus& bus = network.getBusView().getBus("VLHV1_0").get();
    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("NHV2_NLOAD");
    twt.getRatioTapChanger().setSolvedTapPosition(2);

    BOOST_CHECK(!std::isnan(tl.getTerminal1().getP()));
    BOOST_CHECK(!std::isnan(tl.getTerminal1().getQ()));
    BOOST_CHECK(!std::isnan(tl.getTerminal2().getP()));
    BOOST_CHECK(!std::isnan(tl.getTerminal2().getQ()));
    BOOST_CHECK(!std::isnan(bus.getV()));
    BOOST_CHECK(!std::isnan(bus.getAngle()));
    BOOST_CHECK(twt.getRatioTapChanger().getSolvedTapPosition().has_value());
    BOOST_CHECK_EQUAL(2, twt.getRatioTapChanger().getSolvedTapPosition().get());

    Networks::unsetSolvedValues(network);

    BOOST_CHECK(std::isnan(tl.getTerminal1().getP()));
    BOOST_CHECK(std::isnan(tl.getTerminal1().getQ()));
    BOOST_CHECK(std::isnan(tl.getTerminal2().getP()));
    BOOST_CHECK(std::isnan(tl.getTerminal2().getQ()));
    BOOST_CHECK(std::isnan(bus.getV()));
    BOOST_CHECK(std::isnan(bus.getAngle()));
    BOOST_CHECK(!twt.getRatioTapChanger().getSolvedTapPosition().has_value());
}

BOOST_AUTO_TEST_CASE(unsetDcSolvedValuesTest) {
    Network network = powsybl::network::DcDetailedNetworkFactory::createLccMonopoleGroundReturn();
    DcLine& line = network.getDcLine(powsybl::network::DcDetailedNetworkFactory::DC_LINE1);
    LineCommutatedConverter& lcc = network.getLineCommutatedConverter("LccFr");
    DcBus& dcBus = network.getDcBus("dcNodeFrPos_dcBus").get();

    line.getDcTerminal1().setP(10.0);
    line.getDcTerminal1().setI(20.0);

    lcc.getDcTerminal1().setP(30.0);
    lcc.getDcTerminal1().setI(40.0);
    lcc.getTerminal1().setP(50.0);
    lcc.getTerminal1().setQ(60.0);

    dcBus.setV(100.0);

    BOOST_CHECK(!std::isnan(line.getDcTerminal1().getP()));
    BOOST_CHECK(!std::isnan(line.getDcTerminal1().getI()));
    BOOST_CHECK(!std::isnan(lcc.getDcTerminal1().getP()));
    BOOST_CHECK(!std::isnan(lcc.getDcTerminal1().getI()));
    BOOST_CHECK(!std::isnan(lcc.getTerminal1().getP()));
    BOOST_CHECK(!std::isnan(lcc.getTerminal1().getQ()));
    BOOST_CHECK(!std::isnan(dcBus.getV()));

    Networks::unsetSolvedValues(network);

    BOOST_CHECK(std::isnan(line.getDcTerminal1().getP()));
    BOOST_CHECK(std::isnan(line.getDcTerminal1().getI()));
    BOOST_CHECK(std::isnan(lcc.getDcTerminal1().getP()));
    BOOST_CHECK(std::isnan(lcc.getDcTerminal1().getI()));
    BOOST_CHECK(std::isnan(lcc.getTerminal1().getP()));
    BOOST_CHECK(std::isnan(lcc.getTerminal1().getQ()));
    BOOST_CHECK(std::isnan(dcBus.getV()));
}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
