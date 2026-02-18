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
#include <powsybl/iidm/Line.hpp>
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

    danglingLine.getOrCreateSelectedOperationalLimitsGroup().newCurrentLimits()
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
    ActivePowerLimitsAdder l1AdderAct = twt.getLeg1().getOrCreateSelectedOperationalLimitsGroup().newActivePowerLimits();
    createLoadingLimits(l1AdderAct);
    ApparentPowerLimitsAdder l1AdderApp = twt.getLeg1().getOrCreateSelectedOperationalLimitsGroup().newApparentPowerLimits();
    createLoadingLimits(l1AdderApp);
    ActivePowerLimitsAdder l2AdderAct = twt.getLeg2().getOrCreateSelectedOperationalLimitsGroup().newActivePowerLimits();
    createLoadingLimits(l2AdderAct);
    ApparentPowerLimitsAdder l2AdderApp = twt.getLeg2().getOrCreateSelectedOperationalLimitsGroup().newApparentPowerLimits();
    createLoadingLimits(l2AdderApp);
    ActivePowerLimitsAdder l3AdderAct = twt.getLeg3().getOrCreateSelectedOperationalLimitsGroup().newActivePowerLimits();
    createLoadingLimits(l3AdderAct);
    ApparentPowerLimitsAdder l3AdderApp = twt.getLeg3().getOrCreateSelectedOperationalLimitsGroup().newApparentPowerLimits();
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
    OperationalLimitsGroup& limitsGroups = danglingLine.getOrCreateSelectedOperationalLimitsGroup();
    ApparentPowerLimitsAdder activePowerLimitsAdder = limitsGroups.newApparentPowerLimits();
    createLoadingLimits(activePowerLimitsAdder);
    ActivePowerLimitsAdder apparentPowerLimitsAdder = limitsGroups.newActivePowerLimits();
    createLoadingLimits(apparentPowerLimitsAdder);
    CurrentLimitsAdder currentLimitsAdder = limitsGroups.newCurrentLimits();
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
    OperationalLimitsGroup& lineLimitsGroup1 = line.getOrCreateSelectedOperationalLimitsGroup1();
    OperationalLimitsGroup& lineLimitsGroup2 = line.getOrCreateSelectedOperationalLimitsGroup2();
    ApparentPowerLimitsAdder activePowerLimitsAdder1 = lineLimitsGroup1.newApparentPowerLimits();
    createLoadingLimits(activePowerLimitsAdder1);
    ActivePowerLimitsAdder ApparentPowerLimitsAdder1 = lineLimitsGroup1.newActivePowerLimits();
    createLoadingLimits(activePowerLimitsAdder1);
    CurrentLimitsAdder currentLimitsAdder1 = lineLimitsGroup1.newCurrentLimits();
    createLoadingLimits(currentLimitsAdder1);
    ApparentPowerLimitsAdder activePowerLimitsAdder2 = lineLimitsGroup2.newApparentPowerLimits();
    createLoadingLimits(activePowerLimitsAdder2);
    ActivePowerLimitsAdder ApparentPowerLimitsAdder2 = lineLimitsGroup2.newActivePowerLimits();
    createLoadingLimits(activePowerLimitsAdder2);
    CurrentLimitsAdder currentLimitsAdder2 = lineLimitsGroup2.newCurrentLimits();
    createLoadingLimits(currentLimitsAdder2);
    TwoWindingsTransformer& twt = network.getTwoWindingsTransformer("NGEN_NHV1");
    OperationalLimitsGroup& twtLimitsGroup1 = twt.getOrCreateSelectedOperationalLimitsGroup1();
    OperationalLimitsGroup& twtLimitsGroup2 = twt.getOrCreateSelectedOperationalLimitsGroup2();
    ActivePowerLimitsAdder adderAct1 = twtLimitsGroup1.newActivePowerLimits();
    createLoadingLimits(adderAct1);
    ApparentPowerLimitsAdder adderApp1 = twtLimitsGroup1.newApparentPowerLimits();
    createLoadingLimits(adderApp1);
    CurrentLimitsAdder adderCurr1 = twtLimitsGroup1.newCurrentLimits();
    createLoadingLimits(adderCurr1);
    ActivePowerLimitsAdder adderAct2 = twtLimitsGroup2.newActivePowerLimits();
    createLoadingLimits(adderAct2);
    ApparentPowerLimitsAdder adderApp2 = twtLimitsGroup2.newApparentPowerLimits();
    createLoadingLimits(adderApp2);
    CurrentLimitsAdder adderCurr2 = twtLimitsGroup2.newCurrentLimits();
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
    TieLine& tieLine = network.getTieLine("NHV1_NHV2_1");
    OperationalLimitsGroup& lineLimitsGroup1 = tieLine.getOrCreateSelectedOperationalLimitsGroup1();
    OperationalLimitsGroup& lineLimitsGroup2 = tieLine.getOrCreateSelectedOperationalLimitsGroup2();
    ActivePowerLimitsAdder activePowerLimitsAdder1 = lineLimitsGroup1.newActivePowerLimits();
    createLoadingLimits(activePowerLimitsAdder1);
    ApparentPowerLimitsAdder apparentPowerLimitsAdder1 = lineLimitsGroup1.newApparentPowerLimits();
    createLoadingLimits(apparentPowerLimitsAdder1);
    CurrentLimitsAdder currentLimitsAdder1 = lineLimitsGroup1.newCurrentLimits();
    createLoadingLimits(currentLimitsAdder1);
    ActivePowerLimitsAdder activePowerLimitsAdder2 = lineLimitsGroup2.newActivePowerLimits();
    createLoadingLimits(activePowerLimitsAdder2);
    ApparentPowerLimitsAdder apparentPowerLimitsAdder2 = lineLimitsGroup2.newApparentPowerLimits();
    createLoadingLimits(apparentPowerLimitsAdder2);
    CurrentLimitsAdder currentLimitsAdder2 = lineLimitsGroup2.newCurrentLimits();
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

BOOST_FIXTURE_TEST_CASE(importWithoutPermanentLimits, test::ResourceFixture) {
    ImportOptions options;
    options.setMissingPermanentLimitPercentage(90.);

    //Check that import succeeds for versions prior to 1.12
    //(the missing permanent limit is computed)
    test::converter::RoundTrip::testForAllPreviousVersions(IidmXmlVersion::V1_12(), [&options](const iidm::converter::xml::IidmXmlVersion& version){
        iidm::Network n = Network::readXml(test::converter::RoundTrip::getVersionedNetworkPath("withoutPermanentLimit.xml", version), options);
        Line& line = n.getLine("NHV1_NHV2_1");
        BOOST_CHECK_CLOSE(900., line.getCurrentLimits1().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());
        BOOST_CHECK_CLOSE(300., line.getCurrentLimits2().get().getPermanentLimit(), std::numeric_limits<double>::epsilon());
    });

    //From 1.12, import should fail when permanent limit is missing
    test::converter::RoundTrip::testForAllVersionsSince(IidmXmlVersion::V1_12(), [&options](const iidm::converter::xml::IidmXmlVersion& version){
        POWSYBL_ASSERT_THROW(Network::readXml(test::converter::RoundTrip::getVersionedNetworkPath("withoutPermanentLimit.xml", version), options), PowsyblException,
            stdcxx::format("permanentLimit is absent in 'currentLimits1'").c_str());
    });
}

BOOST_FIXTURE_TEST_CASE(importWithoutPermanentLimitsOption, test::ResourceFixture) {
    ImportOptions options;
    options.setMinimalValidationLevel(Enum::toString(ValidationLevel::EQUIPMENT));

    //Check that import succeeds for all versions
    //(the missing permanent limit is set to nan)
    test::converter::RoundTrip::testForAllVersionsSince(IidmXmlVersion::V1_0(), [&options](const iidm::converter::xml::IidmXmlVersion& version){
        iidm::Network n = Network::readXml(test::converter::RoundTrip::getVersionedNetworkPath("withoutPermanentLimit.xml", version), options);
        Line& line = n.getLine("NHV1_NHV2_1");
        BOOST_CHECK(std::isnan(line.getCurrentLimits1().get().getPermanentLimit()));
        BOOST_CHECK(std::isnan(line.getCurrentLimits2().get().getPermanentLimit()));
    });

}

BOOST_AUTO_TEST_CASE(testWrongParametersValue) {
    ImportOptions options;

    POWSYBL_ASSERT_THROW(options.setMinimalValidationLevel("Unknown value"), AssertionError, "Unexpected ValidationLevel name: Unknown value");

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
