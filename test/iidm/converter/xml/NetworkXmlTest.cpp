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
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/iidm/converter/Parameter.hpp>
#include <powsybl/iidm/converter/xml/Constants.hpp>
#include <powsybl/network/MultipleExtensionsTestNetworkFactory.hpp>
#include <powsybl/stdcxx/Properties.hpp>
#include <powsybl/test/AssertionUtils.hpp>

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

    const Network& network = Network::readXml(networkStr);
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

    const Network& network = Network::readXml(networkStr);
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

    const Network& network = Network::readXml(networkStr);
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

    const Network& network = Network::readXml(networkStr);
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
    properties.set(xml::THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND, "true");
    properties.set(xml::TOPOLOGY_LEVEL, "BUS_BREAKER");

    FakeAnonymizer anonymizer;
    const Network& network = Network::readXml(stream, ImportOptions(properties), anonymizer);

    std::stringstream ostream;
    Network::writeXml(ostream, network, properties);

    properties.set(xml::TOPOLOGY_LEVEL, "true");
    POWSYBL_ASSERT_THROW(Network::writeXml(ostream, network, ExportOptions(properties)), AssertionError, "Unexpected TopologyLevel name: true");
    properties.remove(xml::TOPOLOGY_LEVEL);

    std::set<std::string> extensions;
    extensions.insert("extension1");
    extensions.insert("extension2");
    properties.set(xml::EXTENSIONS_LIST, boost::algorithm::join(extensions, ","));
    Network::writeXml(ostream, network, properties);
}

BOOST_AUTO_TEST_CASE(WriteFromParametersCheckExtensions) {
    Network network = powsybl::network::MultipleExtensionsTestNetworkFactory::MultipleExtensionsTestNetworkFactory::create();

    stdcxx::Properties properties;
    std::stringstream ostream;

    properties.set(xml::EXTENSIONS_LIST, "loadBar");
    Network::writeXml(ostream, network, ExportOptions(properties));
    const std::string& loadBarOnly = ostream.str();
    BOOST_TEST(loadBarOnly.find("loadBar") != std::string::npos);
    BOOST_TEST(loadBarOnly.find("loadFoo") == std::string::npos);

    ostream.str("");
    ostream.clear();
    properties.set(xml::EXTENSIONS_LIST, "loadFoo");
    Network::writeXml(ostream, network, ExportOptions(properties));
    const std::string& loadFooOnly = ostream.str();
    BOOST_TEST(loadFooOnly.find("loadBar") == std::string::npos);
    BOOST_TEST(loadFooOnly.find("loadFoo") != std::string::npos);

    ostream.str("");
    ostream.clear();
    properties.remove(xml::EXTENSIONS_LIST);
    Network::writeXml(ostream, network, ExportOptions(properties));
    const std::string& loadAllExtsOutput = ostream.str();
    BOOST_TEST(loadAllExtsOutput.find("loadBar") != std::string::npos);
    BOOST_TEST(loadAllExtsOutput.find("loadFoo") != std::string::npos);

    std::stringstream referenceStream;
    Network::writeXml(referenceStream, network);
    const std::string& refOutput = referenceStream.str();
    BOOST_TEST(refOutput.find("loadBar") != std::string::npos);
    BOOST_TEST(refOutput.find("loadFoo") != std::string::npos);

    BOOST_CHECK_EQUAL(refOutput, loadAllExtsOutput);
}

BOOST_AUTO_TEST_CASE(ReadFromParametersCheckExtensions) {
    stdcxx::Properties properties;
    FakeAnonymizer anonymizer;
    std::stringstream inputStream;

    Network network = powsybl::network::MultipleExtensionsTestNetworkFactory::MultipleExtensionsTestNetworkFactory::create();
    Network::writeXml(inputStream, network);

    std::string refString = inputStream.str();

    properties.set(xml::EXTENSIONS_LIST, "loadFoo");
    inputStream.str(refString);
    inputStream.clear();
    Network fooNetwork = Network::readXml(inputStream, ImportOptions(properties), anonymizer);
    BOOST_CHECK_EQUAL(1UL, boost::size(fooNetwork.getLoad("LOAD").getExtensions()));
    BOOST_CHECK_EQUAL(1UL, boost::size(fooNetwork.getLoad("LOAD2").getExtensions()));

    properties.set(xml::EXTENSIONS_LIST, "loadBar");
    inputStream.str(refString);
    inputStream.clear();
    Network barNetwork = Network::readXml(inputStream, ImportOptions(properties), anonymizer);
    BOOST_CHECK_EQUAL(1UL, boost::size(barNetwork.getLoad("LOAD").getExtensions()));
    BOOST_CHECK_EQUAL(0UL, boost::size(barNetwork.getLoad("LOAD2").getExtensions()));

    properties.remove(xml::EXTENSIONS_LIST);
    inputStream.str(refString);
    inputStream.clear();
    Network allExtNetwork = Network::readXml(inputStream, ImportOptions(properties), anonymizer);
    BOOST_CHECK_EQUAL(2UL, boost::size(allExtNetwork.getLoad("LOAD").getExtensions()));
    BOOST_CHECK_EQUAL(1UL, boost::size(allExtNetwork.getLoad("LOAD2").getExtensions()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
