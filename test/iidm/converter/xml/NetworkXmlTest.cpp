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

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
