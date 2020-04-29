/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

#include "extensions/TerminalMockExt.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(IdentifiableExtensionXml)

BOOST_AUTO_TEST_CASE(ExtensionXmlSerializerTest) {
    auto& providers = ExtensionProviders<ExtensionXmlSerializer>::getInstance();
    const auto& cProviders = providers;
    BOOST_CHECK_EQUAL(3, boost::size(providers.getProviders()));
    BOOST_CHECK_EQUAL(3, boost::size(cProviders.getProviders()));

    const auto& serializer = providers.findProvider("loadMock");
    BOOST_CHECK_EQUAL(true, static_cast<bool>(serializer));

    BOOST_CHECK_EQUAL("loadMock", serializer.get().getName());
    BOOST_CHECK_EQUAL("loadMock", serializer.get().getExtensionName());
    BOOST_CHECK_EQUAL("network", serializer.get().getCategoryName());
    BOOST_CHECK_EQUAL("1.2", serializer.get().getVersion());
    BOOST_CHECK_EQUAL("http://www.powsybl.org/schema/iidm/ext/load_mock/1_2", serializer.get().getNamespaceUri());
    BOOST_CHECK_EQUAL("http://www.powsybl.org/schema/iidm/ext/load_mock/1_0", serializer.get().getNamespaceUri("1.0"));
    BOOST_CHECK_EQUAL("http://www.powsybl.org/schema/iidm/ext/load_mock/1_1", serializer.get().getNamespaceUri("1.1"));
    BOOST_CHECK_EQUAL("http://www.powsybl.org/schema/iidm/ext/load_mock/1_2", serializer.get().getNamespaceUri("1.2"));
}

BOOST_AUTO_TEST_CASE(TerminalExtension) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Load& load = network.getLoad("LOAD");
    load.addExtension(stdcxx::make_unique<extensions::TerminalMockExt>(load));

    Network network2 = Network::readXml(test::converter::RoundTrip::getVersionedNetwork("eurostag-tutorial-example1-with-terminalMock-ext.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION()));
    Load& load2 = network2.getLoad("LOAD");
    const auto& terminalMockExt = load2.getExtension<extensions::TerminalMockExt>();
    BOOST_TEST(stdcxx::areSame(load2.getTerminal(), terminalMockExt.getTerminal()));

    // FIXME(mathbagu): it requires to be able to write an extension in a previous version
    // test::converter::RoundTrip::roundTripAllPreviousVersionedXmlTest("eurostag-tutorial-example1-with-terminalMock-ext.xml");
}

BOOST_AUTO_TEST_CASE(IncompatibleExtensionVersion) {
    const std::string& strNetwork = test::converter::RoundTrip::getVersionedNetwork("eurostag-tutorial-example1-with-bad-loadMockExt.xml", IidmXmlVersion::V1_1());

    POWSYBL_ASSERT_THROW(Network::readXml(strNetwork), PowsyblException, "IIDM-XML version of network (1.1) is not compatible with the loadMock extension's namespace URI");
}

BOOST_AUTO_TEST_CASE(UnsupportedExtensionVersion) {
    const std::string& strNetwork = test::converter::RoundTrip::getVersionedNetwork("eurostag-tutorial-example1-with-bad-loadQuxExt.xml", IidmXmlVersion::V1_1());

    POWSYBL_ASSERT_THROW(Network::readXml(strNetwork), PowsyblException, "IIDM-XML version of network (1.1) is not supported by the loadQux extension's XML serializer");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
