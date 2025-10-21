/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <boost/algorithm/string/join.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/DanglingLineGeneration.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/LoadZipModel.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/StaticVarCompensatorAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/network/LoadBarExt.hpp>
#include <powsybl/network/LoadFooExt.hpp>
#include <powsybl/network/MultipleExtensionsTestNetworkFactory.hpp>
#include <powsybl/network/ScadaNetworkFactory.hpp>
#include <powsybl/stdcxx/Properties.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

#include "extensions/NetworkSourceExt.hpp"
#include "extensions/TerminalMockExt.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(NetworkXmlTest)

void assertNetwork(const Network& network) {
    BOOST_CHECK_EQUAL("terminalRef", network.getId());
    BOOST_CHECK_EQUAL("2000-11-08T19:00:00+01:00", network.getCaseDate().toString());
    BOOST_CHECK_EQUAL(0, network.getForecastDistance());
    BOOST_CHECK_EQUAL("test", network.getSourceFormat());

}

Network& createSubnetwork(Network& rootnetwork, int num) {
    std::string dlId = "dl" + std::to_string(num);
    std::string voltageLevelId = "vl" + std::to_string(num);
    std::string busId = "b" + std::to_string(num);

    Network& network = rootnetwork.newSubnetwork("Network-" + std::to_string(num), "format");
    Substation& s1 = network.newSubstation()
                        .setId("s" + std::to_string(num))
                        .setCountry(Country::FR)
                        .add();
    VoltageLevel& vl1 = s1.newVoltageLevel()
                           .setId(voltageLevelId)
                           .setNominalV(380)
                           .setTopologyKind(TopologyKind::BUS_BREAKER)
                           .add();
    vl1.getBusBreakerView().newBus().setId(busId).add();
    network.getVoltageLevel(voltageLevelId).newDanglingLine().setId(dlId).setName(dlId + "_name").setConnectableBus(busId).setBus(busId).setP0(0.0).setQ0(0.0).setR(1.0).setX(2.0).setG(4.0).setB(5.0).setPairingKey("code").add();

    // Add an extension on the network and on an inner element
    network.addExtension(stdcxx::make_unique<extensions::NetworkSourceExt>(network, "Source_" + std::to_string(num)));

    if (num == 1) {
        Generator& generator = vl1.newGenerator()
                                  .setId("GEN")
                                  .setBus(busId)
                                  .setConnectableBus(busId)
                                  .setMinP(-9999.99)
                                  .setMaxP(9999.99)
                                  .setVoltageRegulatorOn(true)
                                  .setTargetV(24.5)
                                  .setTargetP(607.0)
                                  .setTargetQ(301.0)
                                  .add();
        generator.newMinMaxReactiveLimits()
            .setMinQ(-9999.99)
            .setMaxQ(9999.99)
            .add();
    } else if (num == 2) {
        vl1.newLoad()
            .setId("LOAD")
            .setBus(busId)
            .setConnectableBus(busId)
            .setP0(600.0)
            .setQ0(200.0)
            .add();

        // Add an extension on an inner element
        Load& load = network.getLoad("LOAD");
        load.addExtension(stdcxx::make_unique<extensions::TerminalMockExt>(load));
    }
    return network;
}

BOOST_AUTO_TEST_CASE(StartByComments) {

    const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                    "<!-- test --> "
                                    "<iidm:network xmlns:iidm=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                                    "              id=\"terminalRef\" "
                                    "              caseDate=\"2000-11-08T19:00:00.000+01:00\" "
                                    "              forecastDistance=\"0\" "
                                    "              sourceFormat=\"test\">"
                                    "    <!-- test comment --> "
                                    "</iidm:network>";

    std::istringstream stream(networkStr);
    const Network& network = Network::readXml("network.xiidm", stream);
    assertNetwork(network);
}

BOOST_AUTO_TEST_CASE(Basic) {

    const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                    "<iidm:network xmlns:iidm=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                                    "              id=\"terminalRef\" "
                                    "              caseDate=\"2000-11-08T19:00:00.000+01:00\" "
                                    "              forecastDistance=\"0\" "
                                    "              sourceFormat=\"test\">"
                                    "</iidm:network>";

    std::istringstream stream(networkStr);
    const Network& network = Network::readXml("network.xiidm", stream);
    assertNetwork(network);
}

BOOST_AUTO_TEST_CASE(NoPrefix) {

    const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                    "<network xmlns=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                                    "              id=\"terminalRef\" "
                                    "              caseDate=\"2000-11-08T19:00:00.000+01:00\" "
                                    "              forecastDistance=\"0\" "
                                    "              sourceFormat=\"test\">"
                                    "</network>";

    std::istringstream stream(networkStr);
    const Network& network = Network::readXml("network.xiidm", stream);
    assertNetwork(network);
}

BOOST_AUTO_TEST_CASE(SpecialChars) {

    const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                    "<network xmlns:iidm=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                                    "              id=\"ø/Ø - ö/Ö - æ/Æ - ä/Ä - å/Å (aa/Aa)\" "
                                    "              caseDate=\"2014-11-08T19:00:00.000+01:00\" "
                                    "              forecastDistance=\"0\" "
                                    "              sourceFormat=\"test\">"
                                    "</network>";

    std::istringstream stream(networkStr);
    const Network& network = Network::readXml("network.xiidm", stream);
    BOOST_CHECK_EQUAL("ø/Ø - ö/Ö - æ/Æ - ä/Ä - å/Å (aa/Aa)", network.getId());
}

BOOST_AUTO_TEST_CASE(FromParameters) {

    const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                    "<network xmlns:iidm=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                                    "              id=\"ø/Ø - ö/Ö - æ/Æ - ä/Ä - å/Å (aa/Aa)\" "
                                    "              caseDate=\"2014-11-08T19:00:00.000+01:00\" "
                                    "              forecastDistance=\"0\" "
                                    "              sourceFormat=\"test\">"
                                    "</network>";
    std::stringstream stream;
    stream << networkStr;

    stdcxx::Properties properties;
    properties.set(ImportOptions::THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND, "true");

    const Network& network = Network::readXml("network.xiidm", stream, ImportOptions(properties));

    std::stringstream ostream;
    const std::string& filename = stdcxx::format("%1%.xiidm", network.getId());
    Network::writeXml(filename, ostream, network, ExportOptions(properties));

    properties.set(ExportOptions::TOPOLOGY_LEVEL, "true");
    POWSYBL_ASSERT_THROW(Network::writeXml(filename, ostream, network, ExportOptions(properties)), AssertionError, "Unexpected TopologyLevel name: true");
    properties.remove(ExportOptions::TOPOLOGY_LEVEL);

    std::set<std::string> extensions;
    extensions.insert("extension1");
    extensions.insert("extension2");
    properties.set(ExportOptions::EXTENSIONS_LIST, boost::algorithm::join(extensions, ","));
    Network::writeXml(filename, ostream, network, ExportOptions(properties));
}

BOOST_AUTO_TEST_CASE(WriteFromParametersCheckExtensions) {
    Network network = powsybl::network::MultipleExtensionsTestNetworkFactory::MultipleExtensionsTestNetworkFactory::create();

    stdcxx::Properties properties;
    std::stringstream ostream;
    const std::string& filename = stdcxx::format("%1%.xiidm", network.getId());

    properties.set(ExportOptions::EXTENSIONS_LIST, "loadBar");
    Network::writeXml(filename, ostream, network, ExportOptions(properties));
    const std::string& loadBarOnly = ostream.str();
    BOOST_TEST(loadBarOnly.find("loadBar") != std::string::npos);
    BOOST_TEST(loadBarOnly.find("loadFoo") == std::string::npos);

    ostream.str("");
    ostream.clear();
    properties.set(ExportOptions::EXTENSIONS_LIST, "loadFoo");
    Network::writeXml(filename, ostream, network, ExportOptions(properties));
    const std::string& loadFooOnly = ostream.str();
    BOOST_TEST(loadFooOnly.find("loadBar") == std::string::npos);
    BOOST_TEST(loadFooOnly.find("loadFoo") != std::string::npos);

    ostream.str("");
    ostream.clear();
    properties.remove(ExportOptions::EXTENSIONS_LIST);
    Network::writeXml(filename, ostream, network, ExportOptions(properties));
    const std::string& loadAllExtsOutput = ostream.str();
    BOOST_TEST(loadAllExtsOutput.find("loadBar") != std::string::npos);
    BOOST_TEST(loadAllExtsOutput.find("loadFoo") != std::string::npos);

    std::stringstream referenceStream;
    Network::writeXml(filename, referenceStream, network);
    const std::string& refOutput = referenceStream.str();
    BOOST_TEST(refOutput.find("loadBar") != std::string::npos);
    BOOST_TEST(refOutput.find("loadFoo") != std::string::npos);

    BOOST_CHECK_EQUAL(refOutput, loadAllExtsOutput);
}

BOOST_AUTO_TEST_CASE(ReadFromParametersCheckExtensions) {
    stdcxx::Properties properties;
    std::stringstream inputStream;

    Network network = powsybl::network::MultipleExtensionsTestNetworkFactory::MultipleExtensionsTestNetworkFactory::create();
    const std::string& filename = stdcxx::format("%1%.xiidm", network.getId());
    Network::writeXml(filename, inputStream, network);

    std::string refString = inputStream.str();

    properties.set(ImportOptions::EXTENSIONS_LIST, "loadFoo");
    inputStream.str(refString);
    inputStream.clear();
    Network fooNetwork = Network::readXml(filename, inputStream, ImportOptions(properties));
    BOOST_CHECK_EQUAL(1UL, boost::size(fooNetwork.getLoad("LOAD").getExtensions()));
    BOOST_CHECK_EQUAL(1UL, boost::size(fooNetwork.getLoad("LOAD2").getExtensions()));

    properties.set(ImportOptions::EXTENSIONS_LIST, "loadBar");
    inputStream.str(refString);
    inputStream.clear();
    Network barNetwork = Network::readXml(filename, inputStream, ImportOptions(properties));
    BOOST_CHECK_EQUAL(1UL, boost::size(barNetwork.getLoad("LOAD").getExtensions()));
    BOOST_CHECK_EQUAL(0UL, boost::size(barNetwork.getLoad("LOAD2").getExtensions()));

    properties.remove(ImportOptions::EXTENSIONS_LIST);
    inputStream.str(refString);
    inputStream.clear();
    Network allExtNetwork = Network::readXml(filename, inputStream, ImportOptions(properties));
    BOOST_CHECK_EQUAL(2UL, boost::size(allExtNetwork.getLoad("LOAD").getExtensions()));
    BOOST_CHECK_EQUAL(1UL, boost::size(allExtNetwork.getLoad("LOAD2").getExtensions()));
}

BOOST_FIXTURE_TEST_CASE(testScada, test::ResourceFixture) {

    Network network = powsybl::network::ScadaNetworkFactory::create();
    BOOST_CHECK_EQUAL(ValidationLevel::EQUIPMENT, network.getMinimumValidationLevel());
    BOOST_CHECK_EQUAL(ValidationLevel::EQUIPMENT, network.runValidationChecks(ValidationLevel::EQUIPMENT));

    const auto& scadaRefXmlPath = test::converter::RoundTrip::getVersionedNetworkPath("scadaNetwork.xml",IidmXmlVersion::CURRENT_IIDM_XML_VERSION());
    Network networkRef = Network::readXml(scadaRefXmlPath);
    BOOST_CHECK_EQUAL(ValidationLevel::EQUIPMENT, networkRef.getMinimumValidationLevel());
    BOOST_CHECK_EQUAL(ValidationLevel::EQUIPMENT, networkRef.runValidationChecks(ValidationLevel::EQUIPMENT));
    POWSYBL_ASSERT_THROW(networkRef.runValidationChecks(ValidationLevel::STEADY_STATE_HYPOTHESIS), ValidationException, "Battery 'battery': p0 is invalid");
    

    const std::string& filename = stdcxx::format("%1%.xiidm", network.getId());
    const std::string& filenameRef = stdcxx::format("%1%.xiidm", networkRef.getId());
    std::stringstream buffer;
    iidm::Network::writeXml(filename, buffer, network);
    std::stringstream bufferRef;
    iidm::Network::writeXml(filenameRef, bufferRef, networkRef);

    test::converter::RoundTrip::compareXml(bufferRef.str(), buffer.str());
    test::converter::RoundTrip::compareXml(test::converter::RoundTrip::getVersionedNetwork("scadaNetworkRoundTrip.xml",IidmXmlVersion::CURRENT_IIDM_XML_VERSION()),buffer.str());
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("scadaNetworkRoundTrip.xml", IidmXmlVersion::V1_7());

}

BOOST_FIXTURE_TEST_CASE(subnetworksRoundTrip, test::ResourceFixture) {

    Network n0 = Network("Network-0", "format");
    n0.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:40:00+01:00"));

    // add an extension at root network level
    n0.addExtension(stdcxx::make_unique<extensions::NetworkSourceExt>(n0, "Source_0"));

    Network& n1 = createSubnetwork(n0, 1);
    Network& n2 = createSubnetwork(n0, 2);
    n1.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:41:00+01:00"));
    n2.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:42:00+01:00"));

    n0.newTieLine().setId("dl1 + dl2")
                    .setName("dl1_name + dl2_name")
                    .setDanglingLine1("dl1")
                    .setDanglingLine2("dl2")
                    .add();

    std::string filename = "subnetworks.xml";
    for (const auto& version : iidm::converter::xml::IidmXmlVersion::all()) {
        if(version.get() >= IidmXmlVersion::V1_5()) {
            const auto& writer = [&version, &filename](const Network& n, std::ostream& stream) {
                converter::ExportOptions options;
                options.setVersion(version.get().toString("."));
                Network::writeXml(filename, stream, n, options);
            };
            const auto& reader = [&filename](const std::string& xmlBytes) {
                std::istringstream stream(xmlBytes);
                return Network::readXml(filename, stream);
            };
            const auto& ref = test::converter::RoundTrip::getVersionedNetwork(filename, version);
            test::converter::RoundTrip::run(n0, writer, reader, test::converter::RoundTrip::compareXml, ref.c_str());
        }
    }

}

BOOST_FIXTURE_TEST_CASE(failImportSeveralSubnetworkLevels, test::ResourceFixture) {
    const auto& refXmlPath = test::converter::RoundTrip::getVersionedNetworkPath("multiple-subnetwork-levels.xml",IidmXmlVersion::CURRENT_IIDM_XML_VERSION());
    POWSYBL_ASSERT_THROW(Network::readXml(refXmlPath), PowsyblException, "Only one level of subnetwork is currently supported.");
}

BOOST_FIXTURE_TEST_CASE(skipExtensionsTest, test::ResourceFixture) {
    const auto& refXmlPath = test::converter::RoundTrip::getVersionedNetworkPath("skippedExtensions.xml",IidmXmlVersion::CURRENT_IIDM_XML_VERSION());
    Network networkReadExtensions = Network::readXml(refXmlPath);
    Load& load1 = networkReadExtensions.getLoad("LOAD");

    POWSYBL_ASSERT_REF_TRUE(load1.findExtension<powsybl::network::LoadBarExt>());
    POWSYBL_ASSERT_REF_TRUE(load1.findExtension<powsybl::network::LoadFooExt>());
    BOOST_CHECK(load1.getModelType() == LoadModelType::ZIP);

    // Read file with only terminalMockNoSerialize
    converter::ImportOptions importSkipOptions = ImportOptions();
    importSkipOptions.addExtension("terminalMockNoSerialize").addExtension("loadFoo");
    Network networkReadSkipExtensions = Network::readXml(refXmlPath, importSkipOptions);
    Load& load2 = networkReadSkipExtensions.getLoad("LOAD");
    POWSYBL_ASSERT_REF_FALSE(load2.findExtension<powsybl::network::LoadBarExt>());
    POWSYBL_ASSERT_REF_TRUE(load2.findExtension<powsybl::network::LoadFooExt>());
    BOOST_CHECK(load2.getModelType() == LoadModelType::ZIP);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
