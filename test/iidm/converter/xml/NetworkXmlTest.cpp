/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <boost/algorithm/string/join.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/DanglingLineGeneration.hpp>
#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/StaticVarCompensatorAdder.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/network/MultipleExtensionsTestNetworkFactory.hpp>
#include <powsybl/stdcxx/Properties.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(NetworkXmlTest)

Network createScadaNetwork() {
    Network network("scada", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2017-06-25T17:43:00.000+01:00"));
    network.setMinimumValidationLevel("EQUIPMENT");

    Substation &sub = network.newSubstation()
                          .setId("sub")
                          .add();

    VoltageLevel &vl = sub.newVoltageLevel()
                           .setNominalV(440.0)
                           .setId("vl")
                           .setTopologyKind(TopologyKind::BUS_BREAKER)
                           .add();
    std::string busId = "bus";
    vl.getBusBreakerView()
        .newBus()
        .setId(busId)
        .add();
    vl.newBattery()
        .setId("battery")
        .setMinP(0.0)
        .setMaxP(100.0)
        .setBus(busId)
        .add();
    DanglingLine &dl = vl.newDanglingLine()
                           .setId("dl")
                           .setBus(busId)
                           .setR(1.0)
                           .setX(1.0)
                           .setG(0.0)
                           .setB(0.0)
                           .newGeneration()
                           .setMinP(100.0)
                           .setMaxP(200.0)
                           .add()
                           .add();
    dl.getGeneration().get().newMinMaxReactiveLimits().setMinQ(100.0).setMaxQ(200.0).add();
    Generator &generator = vl.newGenerator()
                               .setVoltageRegulatorOn(true)
                               .setId("generator")
                               .setBus(busId)
                               .setMinP(0.0)
                               .setMaxP(100.0)
                               .add();
    generator.newReactiveCapabilityCurve()
        .beginPoint()
        .setP(0.0)
        .setMinQ(0.0)
        .setMaxQ(0.0)
        .endPoint()
        .beginPoint()
        .setP(10.0)
        .setMinQ(5.0)
        .setMaxQ(15.0)
        .endPoint()
        .add();
    LccConverterStation& lcs = vl.newLccConverterStation()
                                  .setId("lcs")
                                  .setLossFactor(80.0)
                                  .setPowerFactor(0.2f)
                                  .setBus(busId)
                                  .add();

    VoltageLevel& vl2 = sub.newVoltageLevel()
                           .setId("vl2")
                           .setNominalV(220.0)
                           .setTopologyKind(TopologyKind::BUS_BREAKER)
                           .add();
    std::string bus2Id = "bus2";
    vl2.getBusBreakerView()
        .newBus()
        .setId(bus2Id)
        .add();

    network.newLine()
        .setId("line")
        .setR(1.0)
        .setX(1.0)
        .setG1(0.0)
        .setB1(0.0)
        .setG2(0.0)
        .setB2(0.0)
        .setBus1(busId)
        .setBus2(bus2Id)
        .setVoltageLevel1(vl.getId())
        .setVoltageLevel2(vl2.getId())
        .add();

    vl2.newLoad()
        .setId("load")
        .setBus(bus2Id)
        .add();
    vl2.newShuntCompensator()
        .setId("shunt")
        .setBus(bus2Id)
        .setVoltageRegulatorOn(true)
        .newLinearModel()
        .setBPerSection(10.0)
        .setMaximumSectionCount(1)
        .add()
        .add();
    vl2.newStaticVarCompensator()
        .setId("svc")
        .setBmin(1.1)
        .setBmax(3.2)
        .setBus(bus2Id)
        .add();

    VoltageLevel& vl3 = sub.newVoltageLevel()
                           .setId("vl3")
                           .setNominalV(420.0)
                           .setTopologyKind(TopologyKind::NODE_BREAKER)
                           .add();
    vl3.getNodeBreakerView().newSwitch().setId("switch").setNode1(0).setNode2(1).setKind(SwitchKind::BREAKER).add();

    ThreeWindingsTransformer& t3wt = sub.newThreeWindingsTransformer()
                                        .setId("t3wt")
                                        .newLeg1()
                                        .setRatedU(400)
                                        .setR(1.0)
                                        .setX(1.0)
                                        .setG(0.0)
                                        .setB(0.0)
                                        .setBus(busId)
                                        .setVoltageLevel(vl.getId())
                                        .add()
                                        .newLeg2()
                                        .setRatedU(220)
                                        .setR(1.0)
                                        .setX(1.0)
                                        .setG(0.0)
                                        .setB(0.0)
                                        .setBus(bus2Id)
                                        .setVoltageLevel(vl2.getId())
                                        .add()
                                        .newLeg3()
                                        .setRatedU(190)
                                        .setR(1.0)
                                        .setX(1.0)
                                        .setG(0.0)
                                        .setB(0.0)
                                        .setNode(0)
                                        .setVoltageLevel(vl3.getId())
                                        .add()
                                        .add();
    t3wt.getLeg1().newRatioTapChanger().setRegulating(true).beginStep().setRho(1.0).setR(1.0).setX(1.0).setG(0.0).setB(0.0).endStep().add();
    t3wt.getLeg2().newPhaseTapChanger().beginStep().setAlpha(1.0).setRho(1.0).setR(1.0).setX(1.0).setG(0.0).setB(0.0).endStep().add();
    TwoWindingsTransformer& t2wt = sub.newTwoWindingsTransformer()
                                      .setId("tw2t")
                                      .setR(1.0)
                                      .setX(1.0)
                                      .setG(0.0)
                                      .setB(0.0)
                                      .setRatedU1(440.0)
                                      .setRatedU2(220.0)
                                      .setBus1(busId)
                                      .setVoltageLevel1(vl.getId())
                                      .setBus2(bus2Id)
                                      .setVoltageLevel2(vl2.getId())
                                      .add();
    t2wt.newRatioTapChanger()
        .setRegulating(true)
        .beginStep()
        .setRho(1.0)
        .setR(1.0)
        .setX(1.0)
        .setG(0.0)
        .setB(0.0)
        .endStep()
        .add();
    t2wt.newPhaseTapChanger()
        .beginStep()
        .setAlpha(1.0)
        .setRho(1.0)
        .setR(1.0)
        .setX(1.0)
        .setG(0.0)
        .setB(0.0)
        .endStep()
        .add();
    VscConverterStation& vcs = vl2.newVscConverterStation()
                                  .setId("vcs")
                                  .setBus(bus2Id)
                                  .setLossFactor(80.0)
                                  .setVoltageRegulatorOn(true)
                                  .add();

    network.newHvdcLine()
        .setNominalV(400.0)
        .setR(1.0)
        .setMaxP(20.0)
        .setConverterStationId1(lcs.getId())
        .setConverterStationId2(vcs.getId())
        .setConvertersMode(HvdcLine::ConvertersMode::SIDE_1_INVERTER_SIDE_2_RECTIFIER)
        .setId("hvdcline")
        .add();

    return network;
}

void assertNetwork(const Network& network) {
    BOOST_CHECK_EQUAL("terminalRef", network.getId());
    BOOST_CHECK_EQUAL("2000-11-08T19:00:00+01:00", network.getCaseDate().toString());
    BOOST_CHECK_EQUAL(0, network.getForecastDistance());
    BOOST_CHECK_EQUAL("test", network.getSourceFormat());

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

BOOST_FIXTURE_TEST_CASE(testOptionalSubstation, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("eurostag-tutorial-example1-opt-sub.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    // backward compatibility checks from version 1.6
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("eurostag-tutorial-example1-opt-sub.xml", IidmXmlVersion::V1_6());
}

BOOST_FIXTURE_TEST_CASE(testScada, test::ResourceFixture) {
    //Network network = createScadaNetwork();

    const std::string& expected = test::converter::RoundTrip::getVersionedNetwork("scadaNetwork.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());
    std::istringstream stream(expected);
    Network network = Network::readXml("scadaNetwork.xml", stream);

    test::converter::RoundTrip::roundTripVersionedXmlTest("scadaNetwork.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    // backward compatibility
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("scadaNetwork.xml", IidmXmlVersion::V1_7());

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
