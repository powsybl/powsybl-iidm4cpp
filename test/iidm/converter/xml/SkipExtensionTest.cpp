/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sstream>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/stdcxx/Properties.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(SkipExtensionTestSuite)

BOOST_FIXTURE_TEST_CASE(SkipExtensionTest, test::ResourceFixture) {
    Network network = Network::readXml(test::converter::RoundTrip::getVersionedNetwork("multiple-extensions.xml", IidmXmlVersion::V1_0()));
    const std::string& refNetwork = test::converter::RoundTrip::getVersionedNetwork("noExtension.xml", IidmXmlVersion::V1_0());

    stdcxx::Properties properties;
    std::stringstream ostream;

    properties.set(ExportOptions::EXTENSIONS_LIST, "");
    properties.set(ExportOptions::VERSION, "1.0");
    Network::writeXml(ostream, network, ExportOptions(properties));
    BOOST_CHECK_EQUAL(refNetwork, ostream.str());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
