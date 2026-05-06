/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(EncodingTestSuite)

BOOST_FIXTURE_TEST_CASE(EncodingISO88591ToISO88591, test::ResourceFixture) {

    Network network = Network::readXml(ResourceFixture::getResourcePath("encoding/networkISO-8859-1.xml"));

    std::stringstream ostream;
    const std::string& filename = stdcxx::format("%1%.xiidm", network.getId());
    ExportOptions options;
    options.setXmlEncoding("ISO-8859-1");
    Network::writeXml(filename, ostream, network, options);

    test::converter::RoundTrip::compareXml(ResourceFixture::getResource("encoding/networkISO-8859-1.xml"),ostream.str());

}

BOOST_FIXTURE_TEST_CASE(EncodingISO88591ToUTF8, test::ResourceFixture) {

    Network network = Network::readXml(ResourceFixture::getResourcePath("encoding/networkISO-8859-1.xml"));

    std::stringstream ostream;
    const std::string& filename = stdcxx::format("%1%.xiidm", network.getId());
    ExportOptions options;
    options.setXmlEncoding("UTF-8");
    Network::writeXml(filename, ostream, network, options);

    test::converter::RoundTrip::compareXml(ResourceFixture::getResource("encoding/networkUTF-8.xml"),ostream.str());

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

