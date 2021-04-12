/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/LoadingLimits.hpp>
#include <powsybl/iidm/LoadingLimitsAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/network/ThreeWindingsTransformerNetworkFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(LoadingLimitsRoundTrip)

Network createDanglingLineNetwork() {
    Network network("dangling-line", "test");
    Substation& substation = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& voltageLevel = substation.newVoltageLevel()
        .setId("VL")
        .setNominalV(100.0)
        .setLowVoltageLimit(80.0)
        .setHighVoltageLimit(120.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    voltageLevel.getBusBreakerView().newBus()
        .setId("BUS")
        .add();
    voltageLevel.newGenerator()
        .setId("G")
        .setMinP(0.0)
        .setMaxP(100.0)
        .setVoltageRegulatorOn(true)
        .setTargetV(100.0)
        .setTargetP(50.0)
        .setTargetQ(30.0)
        .setBus("BUS")
        .add();

    DanglingLine& danglingLine = network.getVoltageLevel("VL").newDanglingLine()
        .setId("DL")
        .setBus("BUS")
        .setR(10.0)
        .setX(1.0)
        .setB(10e-6)
        .setG(10e-5)
        .setP0(50.0)
        .setQ0(30.0)
        .add();

    danglingLine.newCurrentLimits()
        .setPermanentLimit(100.0)
        .beginTemporaryLimit()
        .setName("20'")
        .setValue(120.0)
        .setAcceptableDuration(20 * 60)
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setName("10'")
        .setValue(140.0)
        .setAcceptableDuration(10 * 60)
        .endTemporaryLimit()
        .add();

    return network;
}

template <typename L, typename A>
void createLoadingLimits(LoadingLimitsAdder<L, A>& adder) {
    adder.setPermanentLimit(350)
        .beginTemporaryLimit()
        .setValue(370)
        .setAcceptableDuration(20 * 60)
        .setName("20'")
        .endTemporaryLimit()
        .beginTemporaryLimit()
        .setValue(380)
        .setAcceptableDuration(10 * 60)
        .setName("10'")
        .endTemporaryLimit()
        .add();
}

BOOST_FIXTURE_TEST_CASE(ThreeWindingsTransformerLoadingLimitsTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("t3w-loading-limits.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    // backward compatibility checks from version 1.5
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("t3w-loading-limits.xml", IidmXmlVersion::V1_5());

    Network network = powsybl::network::ThreeWindingsTransformerNetworkFactory::createWithCurrentLimits();
    ThreeWindingsTransformer& twt = network.getThreeWindingsTransformer("3WT");
    ActivePowerLimitsAdder l1AdderAct = twt.getLeg1().newActivePowerLimits();
    createLoadingLimits(l1AdderAct);
    ApparentPowerLimitsAdder l1AdderApp = twt.getLeg1().newApparentPowerLimits();
    createLoadingLimits(l1AdderApp);
    ActivePowerLimitsAdder l2AdderAct = twt.getLeg2().newActivePowerLimits();
    createLoadingLimits(l2AdderAct);
    ApparentPowerLimitsAdder l2AdderApp = twt.getLeg2().newApparentPowerLimits();
    createLoadingLimits(l2AdderApp);
    ActivePowerLimitsAdder l3AdderAct = twt.getLeg3().newActivePowerLimits();
    createLoadingLimits(l3AdderAct);
    ApparentPowerLimitsAdder l3AdderApp = twt.getLeg3().newApparentPowerLimits();
    createLoadingLimits(l3AdderApp);

    // check it fails for all versions < 1.5
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_5(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        std::stringstream ss;
        ExportOptions options = ExportOptions().setVersion(version.toString("."));
        const std::string& filename = stdcxx::format("%1%.xiidm", network.getId());
        POWSYBL_ASSERT_THROW(Network::writeXml(filename, ss, network, options), PowsyblException, stdcxx::format("threeWindingsTransformer.activePowerLimits1 is not null and not supported for IIDM-XML version %1%. IIDM-XML version should be >= 1.5", version.toString(".")).c_str());
    });

    // check it doesn't fail for all versions < 1.5 if IidmVersionIncompatibilityBehavior is to log error
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_5(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        ExportOptions options = ExportOptions().setVersion(version.toString(".")).setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
        const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
            iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
        };
        test::converter::RoundTrip::writeXmlTest(network, writer, test::converter::RoundTrip::getVersionedNetwork("t3w-loading-limits.xml", version));
    });
}

BOOST_FIXTURE_TEST_CASE(DanglingLineLoadingLimitsTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("dl-loading-limits.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    // backward compatibility checks from version 1.5
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("dl-loading-limits.xml", IidmXmlVersion::V1_5());

    Network network = createDanglingLineNetwork();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    DanglingLine& danglingLine = network.getDanglingLine("DL");
    ApparentPowerLimitsAdder activePowerLimitsAdder = danglingLine.newApparentPowerLimits();
    createLoadingLimits(activePowerLimitsAdder);
    ActivePowerLimitsAdder ApparentPowerLimitsAdder = danglingLine.newActivePowerLimits();
    createLoadingLimits(ApparentPowerLimitsAdder);
    CurrentLimitsAdder currentLimitsAdder = danglingLine.newCurrentLimits();
    createLoadingLimits(currentLimitsAdder);

    // check it fails for all versions < 1.5
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_5(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        std::stringstream ss;
        ExportOptions options = ExportOptions().setVersion(version.toString("."));
        const std::string& filename = stdcxx::format("%1%.xiidm", network.getId());
        POWSYBL_ASSERT_THROW(Network::writeXml(filename, ss, network, options), PowsyblException, stdcxx::format("danglingLine.activePowerLimits is not null and not supported for IIDM-XML version %1%. IIDM-XML version should be >= 1.5", version.toString(".")).c_str());
    });

    // check it doesn't fail for all versions < 1.5 if IidmVersionIncompatibilityBehavior is to log error
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_5(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        ExportOptions options = ExportOptions().setVersion(version.toString(".")).setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
        const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
            iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
        };
        test::converter::RoundTrip::writeXmlTest(network, writer, test::converter::RoundTrip::getVersionedNetwork("dl-loading-limits.xml", version));
    });
}

BOOST_FIXTURE_TEST_CASE(EurostagLoadingLimitsTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("eurostag-loading-limits.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    // backward compatibility checks from version 1.5
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("eurostag-loading-limits.xml", IidmXmlVersion::V1_5());

    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    Line& line = network.getLine("NHV1_NHV2_2");
    ApparentPowerLimitsAdder activePowerLimitsAdder1 = line.newApparentPowerLimits1();
    createLoadingLimits(activePowerLimitsAdder1);
    ActivePowerLimitsAdder ApparentPowerLimitsAdder1 = line.newActivePowerLimits1();
    createLoadingLimits(activePowerLimitsAdder1);
    CurrentLimitsAdder currentLimitsAdder1 = line.newCurrentLimits1();
    createLoadingLimits(currentLimitsAdder1);
    ApparentPowerLimitsAdder activePowerLimitsAdder2 = line.newApparentPowerLimits2();
    createLoadingLimits(activePowerLimitsAdder2);
    ActivePowerLimitsAdder ApparentPowerLimitsAdder2 = line.newActivePowerLimits2();
    createLoadingLimits(activePowerLimitsAdder2);
    CurrentLimitsAdder currentLimitsAdder2 = line.newCurrentLimits2();
    createLoadingLimits(currentLimitsAdder2);
    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("NGEN_NHV1");
    ActivePowerLimitsAdder adderAct1 = twt.newActivePowerLimits1();
    createLoadingLimits(adderAct1);
    ApparentPowerLimitsAdder adderApp1 = twt.newApparentPowerLimits1();
    createLoadingLimits(adderApp1);
    CurrentLimitsAdder adderCurr1 = twt.newCurrentLimits1();
    createLoadingLimits(adderCurr1);
    ActivePowerLimitsAdder adderAct2 = twt.newActivePowerLimits2();
    createLoadingLimits(adderAct2);
    ApparentPowerLimitsAdder adderApp2 = twt.newApparentPowerLimits2();
    createLoadingLimits(adderApp2);
    CurrentLimitsAdder adderCurr2 = twt.newCurrentLimits2();
    createLoadingLimits(adderCurr2);

    // check it fails for all versions < 1.5
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_5(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        std::stringstream ss;
        ExportOptions options = ExportOptions().setVersion(version.toString("."));
        const std::string& filename = stdcxx::format("%1%.xiidm", network.getId());
        POWSYBL_ASSERT_THROW(Network::writeXml(filename, ss, network, options), PowsyblException, stdcxx::format("twoWindingsTransformer.activePowerLimits1 is not null and not supported for IIDM-XML version %1%. IIDM-XML version should be >= 1.5", version.toString(".")).c_str());
    });

    // check it doesn't fail for all versions < 1.5 if IidmVersionIncompatibilityBehavior is to log error
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_5(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        ExportOptions options = ExportOptions().setVersion(version.toString(".")).setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
        const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
            iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
        };
        test::converter::RoundTrip::writeXmlTest(network, writer, test::converter::RoundTrip::getVersionedNetwork("eurostag-loading-limits.xml", version));
    });
}

BOOST_FIXTURE_TEST_CASE(TieLineLoadingLimitsTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("tl-loading-limits.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    // backward compatibility checks from version 1.5
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("tl-loading-limits.xml", IidmXmlVersion::V1_5());

    std::stringstream stream(test::converter::RoundTrip::getVersionedNetwork("tieline.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION()));
    Network network = Network::readXml("network.xiidm", stream);
    TieLine& tieLine = dynamic_cast<TieLine&>(network.getLine("NHV1_NHV2_1"));
    ActivePowerLimitsAdder activePowerLimitsAdder1 = tieLine.newActivePowerLimits1();
    createLoadingLimits(activePowerLimitsAdder1);
    ApparentPowerLimitsAdder apparentPowerLimitsAdder1 = tieLine.newApparentPowerLimits1();
    createLoadingLimits(apparentPowerLimitsAdder1);
    CurrentLimitsAdder currentLimitsAdder1 = tieLine.newCurrentLimits1();
    createLoadingLimits(currentLimitsAdder1);
    ActivePowerLimitsAdder activePowerLimitsAdder2 = tieLine.newActivePowerLimits2();
    createLoadingLimits(activePowerLimitsAdder2);
    ApparentPowerLimitsAdder apparentPowerLimitsAdder2 = tieLine.newApparentPowerLimits2();
    createLoadingLimits(apparentPowerLimitsAdder2);
    CurrentLimitsAdder currentLimitsAdder2 = tieLine.newCurrentLimits2();
    createLoadingLimits(currentLimitsAdder2);

    // check it fails for all versions < 1.5
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_5(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        std::stringstream ss;
        ExportOptions options = ExportOptions().setVersion(version.toString("."));
        const std::string& filename = stdcxx::format("%1%.xiidm", network.getId());
        POWSYBL_ASSERT_THROW(Network::writeXml(filename, ss, network, options), PowsyblException, stdcxx::format("tieLine.activePowerLimits1 is not null and not supported for IIDM-XML version %1%. IIDM-XML version should be >= 1.5", version.toString(".")).c_str());
    });

    // check it doesn't fail for all versions < 1.5 if IidmVersionIncompatibilityBehavior is to log error
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_5(), [&network](const iidm::converter::xml::IidmXmlVersion& version) {
        ExportOptions options = ExportOptions().setVersion(version.toString(".")).setIidmVersionIncompatibilityBehavior(ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR);
        const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
            iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
        };
        test::converter::RoundTrip::writeXmlTest(network, writer, test::converter::RoundTrip::getVersionedNetwork("tl-loading-limits.xml", version));
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
