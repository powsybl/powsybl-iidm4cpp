/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/MinMaxReactiveLimits.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/network/EurostagFactory.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace network {

BOOST_AUTO_TEST_SUITE(EurostagFactoryTestSuite)

BOOST_AUTO_TEST_CASE(createTutorial1NetworkTest) {
    const iidm::Network& network = EurostagFactory::createTutorial1Network();
    BOOST_CHECK_EQUAL("eurostag1", network.getId());
    BOOST_CHECK_EQUAL("eurostag1", network.getName());
    BOOST_CHECK_EQUAL("test", network.getSourceFormat());
    BOOST_CHECK_EQUAL(0, network.getForecastDistance());

    // FIXME(mathbagu) BOOST_CHECK_EQUAL(16UL, network.getObjectCount<iidm::Identifiable>());

    // FIXME(mathbagu) BOOST_CHECK_EQUAL(6UL, network.getObjectCount<iidm::Container>());
    BOOST_CHECK_EQUAL(2UL, network.getSubstationCount());
    BOOST_CHECK_EQUAL(4UL, network.getVoltageLevelCount());

    // FIXME(mathbagu) BOOST_CHECK_EQUAL(6UL, network.getObjectCount<iidm::Connectable>());

    BOOST_CHECK_EQUAL(4UL, network.getBranchCount());
    BOOST_CHECK_EQUAL(2UL, network.getLineCount());
    BOOST_CHECK_EQUAL(2UL, network.getTwoWindingsTransformerCount());

    BOOST_CHECK_EQUAL(0UL, network.getThreeWindingsTransformerCount());

    // FIXME(mathbagu) BOOST_CHECK_EQUAL(2UL, network.getObjectCount<iidm::Injection>());
    BOOST_CHECK_EQUAL(0UL, network.getBatteryCount());
    BOOST_CHECK_EQUAL(0UL, network.getBusbarSectionCount());
    BOOST_CHECK_EQUAL(0UL, network.getDanglingLineCount());
    BOOST_CHECK_EQUAL(1UL, network.getGeneratorCount());
    BOOST_CHECK_EQUAL(0UL, network.getHvdcConverterStationCount());
    BOOST_CHECK_EQUAL(0UL, network.getLccConverterStationCount());
    BOOST_CHECK_EQUAL(0UL, network.getVscConverterStationCount());
    BOOST_CHECK_EQUAL(1UL, network.getLoadCount());
    BOOST_CHECK_EQUAL(0UL, network.getShuntCompensatorCount());
    BOOST_CHECK_EQUAL(0UL, network.getStaticVarCompensatorCount());

    BOOST_CHECK_EQUAL(0UL, network.getHvdcLineCount());
    BOOST_CHECK_EQUAL(0UL, network.getSwitchCount());

    BOOST_CHECK_EQUAL(1UL, network.getCountryCount());

    const auto& sub1 = network.getSubstation("P1");
    POWSYBL_ASSERT_ENUM_EQ(iidm::Country::FR, *sub1.getCountry());
    BOOST_CHECK_EQUAL("RTE", sub1.getTso());
    const std::set<std::string>& expected = {"A"};
    const std::set<std::string>& actual1 = sub1.getGeographicalTags();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual1.cbegin(), actual1.cend());

    const auto& sub2 = network.getSubstation("P2");
    POWSYBL_ASSERT_ENUM_EQ(iidm::Country::FR, *sub2.getCountry());
    BOOST_CHECK_EQUAL("RTE", sub2.getTso());
    const std::set<std::string>& actual2 = sub2.getGeographicalTags();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual2.cbegin(), actual2.cend());

    const auto& vlGen = network.getVoltageLevel("VLGEN");
    BOOST_CHECK_CLOSE(24.0, vlGen.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(std::isnan(vlGen.getLowVoltageLimit()));
    BOOST_TEST(std::isnan(vlGen.getHighVoltageLimit()));
    BOOST_CHECK_EQUAL(iidm::TopologyKind::BUS_BREAKER, vlGen.getTopologyKind());
    BOOST_CHECK_EQUAL(2UL, vlGen.getConnectableCount());

    const auto& vlHv1 = network.getVoltageLevel("VLHV1");
    BOOST_CHECK_CLOSE(380.0, vlHv1.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(std::isnan(vlHv1.getLowVoltageLimit()));
    BOOST_TEST(std::isnan(vlHv1.getHighVoltageLimit()));
    BOOST_CHECK_EQUAL(iidm::TopologyKind::BUS_BREAKER, vlHv1.getTopologyKind());
    BOOST_CHECK_EQUAL(3UL, vlHv1.getConnectableCount());

    const auto& vlHv2 = network.getVoltageLevel("VLHV2");
    BOOST_CHECK_CLOSE(380.0, vlHv2.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(std::isnan(vlHv2.getLowVoltageLimit()));
    BOOST_TEST(std::isnan(vlHv2.getHighVoltageLimit()));
    BOOST_CHECK_EQUAL(iidm::TopologyKind::BUS_BREAKER, vlHv2.getTopologyKind());
    BOOST_CHECK_EQUAL(3UL, vlHv2.getConnectableCount());

    const auto& vlLoad = network.getVoltageLevel("VLLOAD");
    BOOST_CHECK_CLOSE(150.0, vlLoad.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(std::isnan(vlLoad.getLowVoltageLimit()));
    BOOST_TEST(std::isnan(vlLoad.getHighVoltageLimit()));
    BOOST_CHECK_EQUAL(iidm::TopologyKind::BUS_BREAKER, vlLoad.getTopologyKind());
    BOOST_CHECK_EQUAL(2UL, vlLoad.getConnectableCount());

    const auto& ngen = vlGen.getBusBreakerView().getBus("NGEN").get();
    BOOST_CHECK_EQUAL(2UL, ngen.getConnectedTerminalCount());
    BOOST_TEST(std::isnan(ngen.getAngle()));
    BOOST_TEST(std::isnan(ngen.getV()));

    const auto& nhv1 = vlHv1.getBusBreakerView().getBus("NHV1").get();
    BOOST_CHECK_EQUAL(3UL, nhv1.getConnectedTerminalCount());
    BOOST_TEST(std::isnan(nhv1.getAngle()));
    BOOST_TEST(std::isnan(nhv1.getV()));

    const auto& nhv2 = vlHv2.getBusBreakerView().getBus("NHV2").get();
    BOOST_CHECK_EQUAL(3UL, nhv2.getConnectedTerminalCount());
    BOOST_TEST(std::isnan(nhv2.getAngle()));
    BOOST_TEST(std::isnan(nhv2.getV()));

    const auto& nload = vlLoad.getBusBreakerView().getBus("NLOAD").get();
    BOOST_CHECK_EQUAL(2UL, nload.getConnectedTerminalCount());
    BOOST_TEST(std::isnan(nload.getAngle()));
    BOOST_TEST(std::isnan(nload.getV()));

    const auto& line1 = network.getLine("NHV1_NHV2_1");
    BOOST_CHECK_EQUAL("", line1.getName());
    BOOST_TEST(!line1.isTieLine());
    BOOST_CHECK_CLOSE(3.0, line1.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(33.0, line1.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, line1.getG1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(386E-6 / 2.0, line1.getB1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, line1.getG2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(386E-6 / 2.0, line1.getB2(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_REF_FALSE(line1.getCurrentLimits1());
    POWSYBL_ASSERT_REF_FALSE(line1.getCurrentLimits2());
    POWSYBL_ASSERT_REF_FALSE(line1.checkTemporaryLimits1());
    POWSYBL_ASSERT_REF_FALSE(line1.checkTemporaryLimits2());
    BOOST_TEST(!line1.isOverloaded());

    const auto& line2 = network.getLine("NHV1_NHV2_2");
    BOOST_CHECK_EQUAL("", line2.getName());
    BOOST_TEST(!line2.isTieLine());
    BOOST_CHECK_CLOSE(3.0, line2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(33.0, line2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, line2.getG1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(386E-6 / 2.0, line2.getB1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, line2.getG2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(386E-6 / 2.0, line2.getB2(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_REF_FALSE(line2.getCurrentLimits1());
    POWSYBL_ASSERT_REF_FALSE(line2.getCurrentLimits2());
    POWSYBL_ASSERT_REF_FALSE(line2.checkTemporaryLimits1());
    POWSYBL_ASSERT_REF_FALSE(line2.checkTemporaryLimits2());
    BOOST_TEST(!line2.isOverloaded());

    const auto& transfo1 = network.getTwoWindingsTransformer("NGEN_NHV1");
    BOOST_CHECK_EQUAL("", transfo1.getName());
    BOOST_CHECK_CLOSE(24.0, transfo1.getRatedU1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(400.0, transfo1.getRatedU2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.24 / 1300.0 * (380.0 * 380.0 / 100.0), transfo1.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(std::sqrt(10.0 * 10.0 - 0.24 * 0.24) / 1300.0 * (380.0 * 380.0 / 100.0), transfo1.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, transfo1.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, transfo1.getB(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!transfo1.getRatioTapChanger());
    BOOST_TEST(!transfo1.getPhaseTapChanger());
    POWSYBL_ASSERT_REF_FALSE(transfo1.getCurrentLimits1());
    POWSYBL_ASSERT_REF_FALSE(transfo1.getCurrentLimits2());
    POWSYBL_ASSERT_REF_FALSE(transfo1.checkTemporaryLimits1());
    POWSYBL_ASSERT_REF_FALSE(transfo1.checkTemporaryLimits2());
    BOOST_TEST(!transfo1.isOverloaded());

    const auto& transfo2 = network.getTwoWindingsTransformer("NHV2_NLOAD");
    BOOST_CHECK_EQUAL("", transfo2.getName());
    BOOST_CHECK_CLOSE(400.0, transfo2.getRatedU1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(158.0, transfo2.getRatedU2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.21 / 1000 * (150.0 * 150.0 / 100.0), transfo2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(std::sqrt(18 * 18 - 0.21 * 0.21) / 1000 * (150.0 * 150.0 / 100.0), transfo2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, transfo2.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, transfo2.getB(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!transfo2.getPhaseTapChanger());
    POWSYBL_ASSERT_REF_FALSE(transfo2.getCurrentLimits1());
    POWSYBL_ASSERT_REF_FALSE(transfo2.getCurrentLimits2());
    POWSYBL_ASSERT_REF_FALSE(transfo2.checkTemporaryLimits1());
    POWSYBL_ASSERT_REF_FALSE(transfo2.checkTemporaryLimits2());
    BOOST_TEST(!transfo2.isOverloaded());

    const auto& ratioTapChanger = transfo2.getRatioTapChanger().get();
    BOOST_CHECK_EQUAL(3U, ratioTapChanger.getStepCount());
    BOOST_CHECK_EQUAL(0L, ratioTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(2L, ratioTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(1L, ratioTapChanger.getTapPosition());
    BOOST_TEST(ratioTapChanger.hasLoadTapChangingCapabilities());
    BOOST_TEST(ratioTapChanger.isRegulating());
    BOOST_CHECK_CLOSE(158.0, ratioTapChanger.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(transfo2.getTerminal2(), ratioTapChanger.getRegulationTerminal().get()));
    BOOST_TEST(stdcxx::areSame(ratioTapChanger.getCurrentStep(), ratioTapChanger.getStep(1)));
    auto step = ratioTapChanger.getStep(0);
    BOOST_CHECK_CLOSE(0.85 * ((158.0 / 150.0) / (400.0 / 380.0)), step.getRho(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getB(), std::numeric_limits<double>::epsilon());
    step = ratioTapChanger.getStep(1);
    BOOST_CHECK_CLOSE((158.0 / 150.0) / (400.0 / 380.0), step.getRho(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getB(), std::numeric_limits<double>::epsilon());
    step = ratioTapChanger.getStep(2);
    BOOST_CHECK_CLOSE(1.15 * ((158.0 / 150.0) / (400.0 / 380.0)), step.getRho(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getB(), std::numeric_limits<double>::epsilon());

    const auto& load = network.getLoad("LOAD");
    BOOST_CHECK_EQUAL("", load.getName());
    BOOST_CHECK_EQUAL(iidm::LoadType::UNDEFINED, load.getLoadType());
    BOOST_CHECK_CLOSE(600.0, load.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, load.getQ0(), std::numeric_limits<double>::epsilon());

    const auto& gen = network.getGenerator("GEN");
    BOOST_CHECK_EQUAL("", gen.getName());
    POWSYBL_ASSERT_ENUM_EQ(iidm::EnergySource::OTHER, gen.getEnergySource());
    BOOST_TEST(std::isnan(gen.getRatedS()));
    BOOST_TEST(!gen.getRegulatingTerminal());
    BOOST_CHECK_CLOSE(-9999.99, gen.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(9999.99, gen.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(gen.isVoltageRegulatorOn());
    BOOST_CHECK_CLOSE(24.5, gen.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(24.5, gen.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(607.0, gen.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(607, gen.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(301.0, gen.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(301.0, gen.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());

    const auto& limits = gen.getReactiveLimits<iidm::MinMaxReactiveLimits>();
    POWSYBL_ASSERT_ENUM_EQ(iidm::ReactiveLimitsKind::MIN_MAX, limits.getKind());
    BOOST_CHECK_CLOSE(-9999.99, limits.getMinQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-9999.99, limits.getMinQ(0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(9999.99, limits.getMaxQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(9999.99, limits.getMaxQ(0), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_CASE(createWithCurrentLimitsTest) {
    const iidm::Network& network = EurostagFactory::createWithCurrentLimits();

    BOOST_CHECK_EQUAL("eurostag1", network.getId());
    BOOST_CHECK_EQUAL("eurostag1", network.getName());
    BOOST_CHECK_EQUAL("test", network.getSourceFormat());
    BOOST_CHECK_EQUAL(0, network.getForecastDistance());
    BOOST_CHECK_EQUAL("2018-01-01T11:00:00+01:00", network.getCaseDate().toString());

    // FIXME(mathbagu) BOOST_CHECK_EQUAL(17UL, network.getObjectCount<iidm::Identifiable>());

    // FIXME(mathbagu) BOOST_CHECK_EQUAL(6UL, network.getObjectCount<iidm::Container>());
    BOOST_CHECK_EQUAL(2UL, network.getSubstationCount());
    BOOST_CHECK_EQUAL(4UL, network.getVoltageLevelCount());

    // FIXME(mathbagu) BOOST_CHECK_EQUAL(7UL, network.getObjectCount<iidm::Connectable>());

    BOOST_CHECK_EQUAL(4UL, network.getBranchCount());
    BOOST_CHECK_EQUAL(2UL, network.getLineCount());
    BOOST_CHECK_EQUAL(2UL, network.getTwoWindingsTransformerCount());

    BOOST_CHECK_EQUAL(0UL, network.getThreeWindingsTransformerCount());

    // FIXME(mathbagu) BOOST_CHECK_EQUAL(3UL, network.getObjectCount<iidm::Injection>());
    BOOST_CHECK_EQUAL(0UL, network.getBatteryCount());
    BOOST_CHECK_EQUAL(0UL, network.getBusbarSectionCount());
    BOOST_CHECK_EQUAL(0UL, network.getDanglingLineCount());
    BOOST_CHECK_EQUAL(2UL, network.getGeneratorCount());
    BOOST_CHECK_EQUAL(0UL, network.getHvdcConverterStationCount());
    BOOST_CHECK_EQUAL(0UL, network.getLccConverterStationCount());
    BOOST_CHECK_EQUAL(0UL, network.getVscConverterStationCount());
    BOOST_CHECK_EQUAL(1UL, network.getLoadCount());
    BOOST_CHECK_EQUAL(0UL, network.getShuntCompensatorCount());
    BOOST_CHECK_EQUAL(0UL, network.getStaticVarCompensatorCount());

    BOOST_CHECK_EQUAL(0UL, network.getHvdcLineCount());
    BOOST_CHECK_EQUAL(0UL, network.getSwitchCount());

    BOOST_CHECK_EQUAL(2UL, network.getCountryCount());

    const auto& sub1 = network.getSubstation("P1");
    POWSYBL_ASSERT_ENUM_EQ(iidm::Country::FR, *sub1.getCountry());
    BOOST_CHECK_EQUAL("RTE", sub1.getTso());
    const std::set<std::string>& expected = {"A"};
    const std::set<std::string>& actual1 = sub1.getGeographicalTags();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual1.cbegin(), actual1.cend());

    const auto& sub2 = network.getSubstation("P2");
    POWSYBL_ASSERT_ENUM_EQ(iidm::Country::BE, *sub2.getCountry());
    BOOST_CHECK_EQUAL("RTE", sub2.getTso());
    const std::set<std::string>& actual2 = sub2.getGeographicalTags();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), actual2.cbegin(), actual2.cend());

    const auto& vlGen = network.getVoltageLevel("VLGEN");
    BOOST_CHECK_CLOSE(24.0, vlGen.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(std::isnan(vlGen.getLowVoltageLimit()));
    BOOST_TEST(std::isnan(vlGen.getHighVoltageLimit()));
    BOOST_CHECK_EQUAL(iidm::TopologyKind::BUS_BREAKER, vlGen.getTopologyKind());
    BOOST_CHECK_EQUAL(3UL, vlGen.getConnectableCount());

    const auto& vlHv1 = network.getVoltageLevel("VLHV1");
    BOOST_CHECK_CLOSE(380.0, vlHv1.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(400.0, vlHv1.getLowVoltageLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(500.0, vlHv1.getHighVoltageLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(iidm::TopologyKind::BUS_BREAKER, vlHv1.getTopologyKind());
    BOOST_CHECK_EQUAL(3UL, vlHv1.getConnectableCount());

    const auto& vlHv2 = network.getVoltageLevel("VLHV2");
    BOOST_CHECK_CLOSE(380.0, vlHv2.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(300.0, vlHv2.getLowVoltageLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(500.0, vlHv2.getHighVoltageLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(iidm::TopologyKind::BUS_BREAKER, vlHv2.getTopologyKind());
    BOOST_CHECK_EQUAL(3UL, vlHv2.getConnectableCount());

    const auto& vlLoad = network.getVoltageLevel("VLLOAD");
    BOOST_CHECK_CLOSE(150.0, vlLoad.getNominalVoltage(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(std::isnan(vlLoad.getLowVoltageLimit()));
    BOOST_TEST(std::isnan(vlLoad.getHighVoltageLimit()));
    BOOST_CHECK_EQUAL(iidm::TopologyKind::BUS_BREAKER, vlLoad.getTopologyKind());
    BOOST_CHECK_EQUAL(2UL, vlLoad.getConnectableCount());

    const auto& ngen = vlGen.getBusBreakerView().getBus("NGEN").get();
    BOOST_CHECK_EQUAL(3UL, ngen.getConnectedTerminalCount());
    BOOST_TEST(std::isnan(ngen.getAngle()));
    BOOST_TEST(std::isnan(ngen.getV()));

    const auto& nhv1 = vlHv1.getBusBreakerView().getBus("NHV1").get();
    BOOST_CHECK_EQUAL(3UL, nhv1.getConnectedTerminalCount());
    BOOST_TEST(std::isnan(nhv1.getAngle()));
    BOOST_CHECK_CLOSE(380.0, nhv1.getV(), std::numeric_limits<double>::epsilon());

    const auto& nhv2 = vlHv2.getBusBreakerView().getBus("NHV2").get();
    BOOST_CHECK_EQUAL(3UL, nhv2.getConnectedTerminalCount());
    BOOST_TEST(std::isnan(nhv2.getAngle()));
    BOOST_CHECK_CLOSE(380.0, nhv2.getV(), std::numeric_limits<double>::epsilon());

    const auto& nload = vlLoad.getBusBreakerView().getBus("NLOAD").get();
    BOOST_CHECK_EQUAL(2UL, nload.getConnectedTerminalCount());
    BOOST_TEST(std::isnan(nload.getAngle()));
    BOOST_TEST(std::isnan(nload.getV()));

    const auto& line1 = network.getLine("NHV1_NHV2_1");
    BOOST_CHECK_EQUAL("", line1.getName());
    BOOST_TEST(!line1.isTieLine());
    BOOST_CHECK_CLOSE(3.0, line1.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(33.0, line1.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, line1.getG1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(386E-6 / 2.0, line1.getB1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, line1.getG2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(386E-6 / 2.0, line1.getB2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(560.0, line1.getTerminal1().getP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(550.0, line1.getTerminal1().getQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(560.0, line1.getTerminal2().getP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(550.0, line1.getTerminal2().getQ(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_REF_FALSE(line1.checkTemporaryLimits1());
    POWSYBL_ASSERT_REF_FALSE(line1.checkTemporaryLimits2());
    BOOST_TEST(!line1.isOverloaded());
    const auto& line1CurLimit1 = line1.getCurrentLimits1().get();
    BOOST_CHECK_CLOSE(500.0, line1CurLimit1.getPermanentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(0, line1CurLimit1.getTemporaryLimits().size());
    const auto& line1CurLimit2 = line1.getCurrentLimits2().get();
    BOOST_CHECK_CLOSE(1100.0, line1CurLimit2.getPermanentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(2, line1CurLimit2.getTemporaryLimits().size());
    auto tempLimit = line1CurLimit2.getTemporaryLimit(600UL);
    BOOST_CHECK_EQUAL("10'", tempLimit.getName());
    BOOST_CHECK_EQUAL(600UL, tempLimit.getAcceptableDuration());
    BOOST_CHECK_CLOSE(1200.0, tempLimit.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!tempLimit.isFictitious());
    tempLimit = line1CurLimit2.getTemporaryLimit(60UL);
    BOOST_CHECK_EQUAL("1'", tempLimit.getName());
    BOOST_CHECK_EQUAL(60UL, tempLimit.getAcceptableDuration());
    BOOST_CHECK_CLOSE(1500.0, tempLimit.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!tempLimit.isFictitious());

    const auto& line2 = network.getLine("NHV1_NHV2_2");
    BOOST_CHECK_EQUAL("", line2.getName());
    BOOST_TEST(!line2.isTieLine());
    BOOST_CHECK_CLOSE(3.0, line2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(33.0, line2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, line2.getG1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(386E-6 / 2.0, line2.getB1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, line2.getG2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(386E-6 / 2.0, line2.getB2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(560.0, line2.getTerminal1().getP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(550.0, line2.getTerminal1().getQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(560.0, line2.getTerminal2().getP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(550.0, line2.getTerminal2().getQ(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_REF_FALSE(line2.checkTemporaryLimits1());
    POWSYBL_ASSERT_REF_FALSE(line2.checkTemporaryLimits2());
    BOOST_TEST(!line2.isOverloaded());
    const auto& line2CurLimit1 = line2.getCurrentLimits1().get();
    BOOST_CHECK_CLOSE(1100.0, line2CurLimit1.getPermanentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(1, line2CurLimit1.getTemporaryLimits().size());
    tempLimit = line2CurLimit1.getTemporaryLimit(1200UL);
    BOOST_CHECK_EQUAL("20'", tempLimit.getName());
    BOOST_CHECK_EQUAL(1200UL, tempLimit.getAcceptableDuration());
    BOOST_CHECK_CLOSE(1200.0, tempLimit.getValue(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!tempLimit.isFictitious());
    const auto& line2CurLimit2 = line2.getCurrentLimits2().get();
    BOOST_CHECK_CLOSE(500.0, line2CurLimit2.getPermanentLimit(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_EQUAL(0, line2CurLimit2.getTemporaryLimits().size());

    const auto& transfo1 = network.getTwoWindingsTransformer("NGEN_NHV1");
    BOOST_CHECK_EQUAL("", transfo1.getName());
    BOOST_CHECK_CLOSE(24.0, transfo1.getRatedU1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(400.0, transfo1.getRatedU2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.24 / 1300.0 * (380.0 * 380.0 / 100.0), transfo1.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(std::sqrt(10.0 * 10.0 - 0.24 * 0.24) / 1300.0 * (380.0 * 380.0 / 100.0), transfo1.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, transfo1.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, transfo1.getB(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!transfo1.getRatioTapChanger());
    BOOST_TEST(!transfo1.getPhaseTapChanger());
    POWSYBL_ASSERT_REF_FALSE(transfo1.getCurrentLimits1());
    POWSYBL_ASSERT_REF_FALSE(transfo1.getCurrentLimits2());
    POWSYBL_ASSERT_REF_FALSE(transfo1.checkTemporaryLimits1());
    POWSYBL_ASSERT_REF_FALSE(transfo1.checkTemporaryLimits2());
    BOOST_TEST(!transfo1.isOverloaded());

    const auto& transfo2 = network.getTwoWindingsTransformer("NHV2_NLOAD");
    BOOST_CHECK_EQUAL("", transfo2.getName());
    BOOST_CHECK_CLOSE(400.0, transfo2.getRatedU1(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(158.0, transfo2.getRatedU2(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.21 / 1000 * (150.0 * 150.0 / 100.0), transfo2.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(std::sqrt(18 * 18 - 0.21 * 0.21) / 1000 * (150.0 * 150.0 / 100.0), transfo2.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, transfo2.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, transfo2.getB(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(!transfo2.getPhaseTapChanger());
    POWSYBL_ASSERT_REF_FALSE(transfo2.getCurrentLimits1());
    POWSYBL_ASSERT_REF_FALSE(transfo2.getCurrentLimits2());
    POWSYBL_ASSERT_REF_FALSE(transfo2.checkTemporaryLimits1());
    POWSYBL_ASSERT_REF_FALSE(transfo2.checkTemporaryLimits2());
    BOOST_TEST(!transfo2.isOverloaded());

    const auto& ratioTapChanger = transfo2.getRatioTapChanger().get();
    BOOST_CHECK_EQUAL(3U, ratioTapChanger.getStepCount());
    BOOST_CHECK_EQUAL(0L, ratioTapChanger.getLowTapPosition());
    BOOST_CHECK_EQUAL(2L, ratioTapChanger.getHighTapPosition());
    BOOST_CHECK_EQUAL(1L, ratioTapChanger.getTapPosition());
    BOOST_TEST(ratioTapChanger.hasLoadTapChangingCapabilities());
    BOOST_TEST(ratioTapChanger.isRegulating());
    BOOST_CHECK_CLOSE(158.0, ratioTapChanger.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(stdcxx::areSame(transfo2.getTerminal2(), ratioTapChanger.getRegulationTerminal().get()));
    BOOST_TEST(stdcxx::areSame(ratioTapChanger.getCurrentStep(), ratioTapChanger.getStep(1)));
    auto step = ratioTapChanger.getStep(0);
    BOOST_CHECK_CLOSE(0.85 * ((158.0 / 150.0) / (400.0 / 380.0)), step.getRho(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getB(), std::numeric_limits<double>::epsilon());
    step = ratioTapChanger.getStep(1);
    BOOST_CHECK_CLOSE((158.0 / 150.0) / (400.0 / 380.0), step.getRho(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getB(), std::numeric_limits<double>::epsilon());
    step = ratioTapChanger.getStep(2);
    BOOST_CHECK_CLOSE(1.15 * ((158.0 / 150.0) / (400.0 / 380.0)), step.getRho(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getR(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getX(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getG(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, step.getB(), std::numeric_limits<double>::epsilon());

    const auto& load = network.getLoad("LOAD");
    BOOST_CHECK_EQUAL("", load.getName());
    BOOST_CHECK_EQUAL(iidm::LoadType::UNDEFINED, load.getLoadType());
    BOOST_CHECK_CLOSE(600.0, load.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(200.0, load.getQ0(), std::numeric_limits<double>::epsilon());

    const auto& gen = network.getGenerator("GEN");
    BOOST_CHECK_EQUAL("", gen.getName());
    POWSYBL_ASSERT_ENUM_EQ(iidm::EnergySource::OTHER, gen.getEnergySource());
    BOOST_TEST(std::isnan(gen.getRatedS()));
    BOOST_TEST(!gen.getRegulatingTerminal());
    BOOST_CHECK_CLOSE(-9999.99, gen.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(9999.99, gen.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(gen.isVoltageRegulatorOn());
    BOOST_CHECK_CLOSE(24.5, gen.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(24.5, gen.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(607.0, gen.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(607, gen.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(301.0, gen.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(301.0, gen.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());

    const auto& limits = gen.getReactiveLimits<iidm::MinMaxReactiveLimits>();
    POWSYBL_ASSERT_ENUM_EQ(iidm::ReactiveLimitsKind::MIN_MAX, limits.getKind());
    BOOST_CHECK_CLOSE(-9999.99, limits.getMinQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(-9999.99, limits.getMinQ(0), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(9999.99, limits.getMaxQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(9999.99, limits.getMaxQ(0), std::numeric_limits<double>::epsilon());

    const auto& gen2 = network.getGenerator("GEN2");
    BOOST_CHECK_EQUAL("", gen2.getName());
    POWSYBL_ASSERT_ENUM_EQ(iidm::EnergySource::OTHER, gen2.getEnergySource());
    BOOST_TEST(std::isnan(gen2.getRatedS()));
    BOOST_TEST(!gen2.getRegulatingTerminal());
    BOOST_CHECK_CLOSE(-9999.99, gen2.getMinP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(9999.99, gen2.getMaxP(), std::numeric_limits<double>::epsilon());
    BOOST_TEST(gen2.isVoltageRegulatorOn());
    BOOST_CHECK_CLOSE(24.5, gen2.getTargetV(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(24.5, gen2.getVoltageSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(607.0, gen2.getTargetP(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(607, gen2.getActivePowerSetpoint(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(301.0, gen2.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(301.0, gen2.getReactivePowerSetpoint(), std::numeric_limits<double>::epsilon());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace network

}  // namespace powsybl
