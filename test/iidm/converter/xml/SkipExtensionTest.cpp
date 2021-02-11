/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sstream>

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/network/LoadBarExt.hpp>
#include <powsybl/network/LoadFooExt.hpp>
#include <powsybl/stdcxx/Properties.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(SkipExtensionTestSuite)

BOOST_FIXTURE_TEST_CASE(SkipExtensionTest, test::ResourceFixture) {
    std::istringstream stream(test::converter::RoundTrip::getVersionedNetwork("multiple-extensions.xml", IidmXmlVersion::V1_0()));
    Network network = Network::readXml("network.xiidm", stream);
    const std::string& refNetwork = test::converter::RoundTrip::getVersionedNetwork("noExtension.xml", IidmXmlVersion::V1_0());

    stdcxx::Properties properties;
    std::stringstream ostream;

    properties.set(ExportOptions::EXTENSIONS_LIST, "");
    properties.set(ExportOptions::VERSION, "1.0");
    Network::writeXml(stdcxx::format("%1%.xiidm", network.getId()), ostream, network, ExportOptions(properties));
    BOOST_CHECK_EQUAL(refNetwork, ostream.str());
}

BOOST_FIXTURE_TEST_CASE(checkSomeFiltered, test::ResourceFixture) {
    std::stringstream stream;
    stream << test::converter::RoundTrip::getVersionedNetwork("multiple-extensions.xml", IidmXmlVersion::V1_0());

    stdcxx::Properties properties;
    properties.set(ImportOptions::EXTENSIONS_LIST, "loadFoo");

    Network network = Network::readXml("network.xiidm", stream, ImportOptions(properties));
    network.getLoad("LOAD").getExtension<powsybl::network::LoadFooExt>();
    POWSYBL_ASSERT_THROW(network.getLoad("LOAD").getExtension<powsybl::network::LoadBarExt>(), PowsyblException, "Extension powsybl::network::LoadBarExt not found");
    network.getLoad("LOAD2").getExtension<powsybl::network::LoadFooExt>();
}

BOOST_FIXTURE_TEST_CASE(checkReadNoExtension, test::ResourceFixture) {
    std::stringstream stream;
    stream << test::converter::RoundTrip::getVersionedNetwork("multiple-extensions.xml", IidmXmlVersion::V1_0());

    stdcxx::Properties properties;
    properties.set(ImportOptions::EXTENSIONS_LIST, "");

    Network network = Network::readXml("network.xiidm", stream, ImportOptions(properties));
    POWSYBL_ASSERT_THROW(network.getLoad("LOAD").getExtension<powsybl::network::LoadFooExt>(), PowsyblException, "Extension powsybl::network::LoadFooExt not found");
    POWSYBL_ASSERT_THROW(network.getLoad("LOAD").getExtension<powsybl::network::LoadBarExt>(), PowsyblException, "Extension powsybl::network::LoadBarExt not found");
    POWSYBL_ASSERT_THROW(network.getLoad("LOAD2").getExtension<powsybl::network::LoadFooExt>(), PowsyblException, "Extension powsybl::network::LoadFooExt not found");
}

BOOST_FIXTURE_TEST_CASE(checkReadAllExtensions, test::ResourceFixture) {
    std::stringstream stream;
    stream << test::converter::RoundTrip::getVersionedNetwork("multiple-extensions.xml", IidmXmlVersion::V1_0());

    stdcxx::Properties properties;
    properties.set(ImportOptions::EXTENSIONS_LIST, "loadFoo,loadBar");

    Network network = Network::readXml("network.xiidm", stream, ImportOptions(properties));
    network.getLoad("LOAD").getExtension<powsybl::network::LoadFooExt>();
    network.getLoad("LOAD").getExtension<powsybl::network::LoadBarExt>();
    network.getLoad("LOAD2").getExtension<powsybl::network::LoadFooExt>();
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
