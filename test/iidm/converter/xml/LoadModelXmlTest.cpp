/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/LoadZipModelAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/network/EurostagFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>



namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(LoadModelTestSuite)

BOOST_FIXTURE_TEST_CASE(LoadZipModel, test::ResourceFixture) {

    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2020-07-16T10:08:48.321+02:00"));
    
    network.getVoltageLevel("VLLOAD").newLoad()
                .setId("LOAD2")
                .setBus("NLOAD")
                .setP0(10)
                .setQ0(5)
                .newZipModel()
                    .setC0p(0.3)
                    .setC1p(0.5)
                    .setC2p(0.2)
                    .setC0q(0.1)
                    .setC1q(0.2)
                    .setC2q(0.7)
                    .add()
                .add();

    const std::string& networkStr = test::converter::RoundTrip::getVersionedNetwork("eurostag-tutorial-example1-zip-load-model.xml", converter::xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    test::converter::RoundTrip::runXml(network, networkStr);

    // backward compatibility, load model is skipped
    test::converter::RoundTrip::roundTripAllPreviousVersionedXmlTest("eurostag-tutorial-example1.xml");

}

BOOST_FIXTURE_TEST_CASE(LoadExpModel, test::ResourceFixture) {

    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    network.setCaseDate(stdcxx::DateTime::parse("2020-07-16T10:08:48.321+02:00"));
    
    network.getVoltageLevel("VLLOAD").newLoad()
                .setId("LOAD2")
                .setBus("NLOAD")
                .setP0(10)
                .setQ0(5)
                .newExponentialModel()
                    .setNp(0.6)
                    .setNq(0.5)
                    .add()
                .add();

    const std::string& networkStr = test::converter::RoundTrip::getVersionedNetwork("eurostag-tutorial-example1-expo-load-model.xml", converter::xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    test::converter::RoundTrip::runXml(network, networkStr);

    // backward compatibility, load model is skipped
    test::converter::RoundTrip::roundTripAllPreviousVersionedXmlTest("eurostag-tutorial-example1.xml");

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

