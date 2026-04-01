/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>

#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/network/LoadFooExt.hpp>
#include <powsybl/network/MultipleExtensionsTestNetworkFactory.hpp>
#include <powsybl/network/VoltageLevelFooExt.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>


#include "extensions/LoadMockExt.hpp"
#include "extensions/LoadMockExtXmlSerializer.hpp"
#include "extensions/NetworkSourceXmlSerializer.hpp"


namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(ExtensionXmlSerializerTest)



BOOST_AUTO_TEST_CASE(UnversionedExtensionTest) {
    // Use the default extension version => also the default serialization name
    ExtensionXmlSerializer* serde = new extensions::NetworkSourceXmlSerializer();
    BOOST_CHECK(serde!=nullptr);

    BOOST_CHECK_EQUAL("1.0", serde->getVersion());
    BOOST_CHECK_EQUAL(serde->getExtensionName(), serde->getSerializationName(serde->getVersion()));
    std::set<std::string> serializationNames = serde->getSerializationNames();
    BOOST_CHECK_EQUAL(1, serializationNames.size());
    BOOST_CHECK(serializationNames.count("networkSource"));
}

BOOST_AUTO_TEST_CASE(getSerializationNameTest) {

    extensions::LoadMockExtXmlSerializer loadMockXml;
    BOOST_CHECK_EQUAL("loadMock", loadMockXml.getExtensionName());
    BOOST_CHECK_EQUAL("loadMock", loadMockXml.getSerializationName(loadMockXml.getVersion()));
    BOOST_CHECK_EQUAL("loadElementMock", loadMockXml.getSerializationName("0.1"));
    std::set<std::string> serializationNames = loadMockXml.getSerializationNames();
    BOOST_CHECK_EQUAL(3, serializationNames.size());
    BOOST_CHECK(serializationNames.count("loadMock"));
    BOOST_CHECK(serializationNames.count("loadElementMock"));
    BOOST_CHECK(serializationNames.count("loadEltMock"));
}

namespace {
    Network createTestNetwork() {
        Network network("test", "test");
        network.setCaseDate(stdcxx::DateTime::parse("2024-09-17T13:36:37.831Z"));
        Substation& s1 = network.newSubstation().setId("S1").add();
        VoltageLevel& vl1 = s1.newVoltageLevel().setId("VL1").setNominalV(450).setTopologyKind(TopologyKind::NODE_BREAKER).add();
        Load& load1 = vl1.newLoad().setId("Load1").setNode(0).setP0(10.).setQ0(20.).add();
        load1.addExtension(stdcxx::make_unique<extensions::LoadMockExt>(load1));
        return network;
    }


    class CustomExtXmlProvider : public AbstractExtensionXmlSerializer {
        public:
        CustomExtXmlProvider() : AbstractExtensionXmlSerializer("loadElementMock", "network", "", ""){
        }

        Extension& read(Extendable& extendable, NetworkXmlReaderContext& /*context*/) const override {
            if (!stdcxx::isInstanceOf<Load>(extendable)) {
                throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Load>()));
            }
            auto& load = dynamic_cast<Load&>(extendable);

            extendable.addExtension(stdcxx::make_unique<powsybl::network::LoadFooExt>(load));
            return extendable.getExtension<powsybl::network::LoadFooExt>();
        }

        void write(const Extension& /*extension*/, NetworkXmlWriterContext& /*context*/) const override {
            // Nothing to do
        }

    };

}


BOOST_FIXTURE_TEST_CASE(realExtensionNameRoundTrip,  test::ResourceFixture) {
    //default extension version with default serialization name
    Network network = createTestNetwork();

    std::string filename= "extensionName/extensionName_1_2.xml";
    const converter::xml::IidmXmlVersion& version = converter::xml::IidmXmlVersion::V1_1();

    const auto& writer = [&version, &filename](const Network& n, std::ostream& stream) {
        converter::ExportOptions options;
        // options.addExtensionVersion("loadMock", "0.1");
        options.setVersion(version.toString("."));
        Network::writeXml(filename, stream, n, options);
    };
    const auto& reader = [&filename](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return Network::readXml(filename, stream);
    };

    const std::string& expected = ResourceFixture::getResource(filename);
    Network networkRT = test::converter::RoundTrip::run(network, writer, reader, test::converter::RoundTrip::compareXml, expected);
    POWSYBL_ASSERT_REF_TRUE(networkRT.getLoad("Load1").findExtension<extensions::LoadMockExt>());

}

BOOST_FIXTURE_TEST_CASE(oldExtensionNameRoundTrip,  test::ResourceFixture) {
    // Use an extension version which serialization name is not the default
    Network network = createTestNetwork();

    std::string filename= "extensionName/extensionName_0_1.xml";
    const converter::xml::IidmXmlVersion& version = converter::xml::IidmXmlVersion::V1_0();

    const auto& writer = [&version, &filename](const Network& n, std::ostream& stream) {
        converter::ExportOptions options;
        options.addExtensionVersion("loadMock", "0.1");
        options.setVersion(version.toString("."));
        Network::writeXml(filename, stream, n, options);
    };
    const auto& reader = [&filename](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return Network::readXml(filename, stream);
    };

    const std::string& expected = ResourceFixture::getResource(filename);
    Network networkRT = test::converter::RoundTrip::run(network, writer, reader, test::converter::RoundTrip::compareXml, expected);
    POWSYBL_ASSERT_REF_TRUE(networkRT.getLoad("Load1").findExtension<extensions::LoadMockExt>());

}

BOOST_FIXTURE_TEST_CASE(importExtensionWithAnotherPrefixTest,  test::ResourceFixture) {

    std::string filename= "extensionName/extensionName_0_1_otherPrefix.xml";
    Network network = Network::readXml(ResourceFixture::getResourcePath(filename));
    POWSYBL_ASSERT_REF_TRUE(network.getLoad("Load1").findExtension<extensions::LoadMockExt>());

}

BOOST_FIXTURE_TEST_CASE(indicateExtensionAtImportTest,  test::ResourceFixture) {

    std::string filename= "extensionName/extensionName_0_1_otherPrefix.xml";
    ImportOptions options1;
    options1.addExtension("loadElementMock"); //Using serialization name
    Network network1 = Network::readXml(ResourceFixture::getResourcePath(filename), options1);
    POWSYBL_ASSERT_REF_TRUE(network1.getLoad("Load1").findExtension<extensions::LoadMockExt>());

    ImportOptions options2;
    options2.addExtension("loadMock"); //Using "real" extension name
    Network network2 = Network::readXml(ResourceFixture::getResourcePath(filename), options2);
    POWSYBL_ASSERT_REF_TRUE(network2.getLoad("Load1").findExtension<extensions::LoadMockExt>());

}

BOOST_FIXTURE_TEST_CASE(alternativeSerializationNameIgnoredIfUsed,  test::ResourceFixture) {

    std::string filename= "extensionName/extensionName_0_1_otherPrefix.xml";

    //Add custom provider:
    auto& extProviders = ExtensionProviders<converter::xml::ExtensionXmlSerializer>::getInstance();
    std::unique_ptr<ExtensionXmlSerializer> customserializer = stdcxx::make_unique<CustomExtXmlProvider>();
    extProviders.registerExtension(std::move(customserializer), "");
    //"Real" name has priority over "alternative" names so it's the custom extension provider that is used instead of LoadMockExtXmlSerializer
    Network networkCustom = Network::readXml(ResourceFixture::getResourcePath(filename));
    Load& load1Custom = networkCustom.getLoad("Load1");
    POWSYBL_ASSERT_REF_FALSE(load1Custom.findExtension<extensions::LoadMockExt>());
    POWSYBL_ASSERT_REF_TRUE(load1Custom.findExtension<powsybl::network::LoadFooExt>());


    //remove custom provider
    extProviders.unregisterExtension("loadElementMock");
    BOOST_CHECK_EQUAL(10, boost::size(extProviders.getProviders()));


    //Default xmlSerializer used, "loadElementMock" has been re-registered as an alternative name
    Network networkDefault = Network::readXml(ResourceFixture::getResourcePath(filename));
    Load& load1Default = networkDefault.getLoad("Load1");
    POWSYBL_ASSERT_REF_TRUE(load1Default.findExtension<extensions::LoadMockExt>());
    POWSYBL_ASSERT_REF_FALSE(load1Default.findExtension<powsybl::network::LoadFooExt>());

}

BOOST_FIXTURE_TEST_CASE(exportWithAnotherPrefixTest,  test::ResourceFixture) {

    Network network = createTestNetwork();

    ExportOptions options;
    options.setVersion(IidmXmlVersion::V1_0().toString("."));
    options.addExtensionVersion("loadMock", "0.2");

    const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
        iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
    };

    std::string filename= "extensionName/extensionName_0_2.xml";
    test::converter::RoundTrip::writeXmlTest(network, writer, ResourceFixture::getResource(filename));

}

BOOST_FIXTURE_TEST_CASE(exportWithNamespaceCollisionTest,  test::ResourceFixture) {

    Network network = powsybl::network::MultipleExtensionsTestNetworkFactory::create();
    VoltageLevel& vl = network.getVoltageLevel("VL");
    vl.addExtension(stdcxx::make_unique<powsybl::network::VoltageLevelFooExt>(vl));

    std::string filename= "extensionName/namespace-prefix-collision.xml";
    test::converter::RoundTrip::runXml(network, ResourceFixture::getResource(filename));

}




BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
