/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

#include <powsybl/network/EurostagFactory.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(EurostagRoundTrip)

BOOST_FIXTURE_TEST_CASE(EurostagRoundTripTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("eurostag-tutorial-example1.xml", IidmXmlVersion::all());
}

BOOST_FIXTURE_TEST_CASE(EurostagLfRoundTripTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("eurostag-tutorial1-lf.xml", IidmXmlVersion::all());
}

BOOST_FIXTURE_TEST_CASE(EurostagPropertiesRoundTripTest, test::ResourceFixture) {
    test::converter::RoundTrip::roundTripVersionedXmlTest("eurostag-tutorial-example1-properties.xml", IidmXmlVersion::all());
}

BOOST_FIXTURE_TEST_CASE(EurostagWithMultipleSelectedOperationalLimitsGroup, test::ResourceFixture) {
    Network network =  ::powsybl::network::EurostagFactory::createWithMultipleSelectedFixedCurrentLimits();

    test::converter::RoundTrip::runXml(network, test::converter::RoundTrip::getVersionedNetwork("eurostag-tutorial-multiple-selected-op-lim-group.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION()));

    //Backward compatibility checks from version 1.12 (Operational Limtis Group did not exist before that)
    // Between 1.12 and 1.15, if multiple selected Limits Group is exported on previous versions, only the "last" selected group is kept
    test::converter::RoundTrip::testForAllVersionsBetween(IidmXmlVersion::V1_12(), IidmXmlVersion::V1_15(), [&network](const iidm::converter::xml::IidmXmlVersion& version){
        ExportOptions options = ExportOptions().setVersion(version.toString("."));
        const auto& writer = [&options](const iidm::Network& n, std::ostream& stream) {
            iidm::Network::writeXml(stdcxx::format("%1%.xiidm", n.getId()), stream, n, options);
        };

        test::converter::RoundTrip::writeXmlTest(network, writer, test::converter::RoundTrip::getVersionedNetwork("eurostag-tutorial-multiple-selected-op-lim-group.xml", version));
    });


    // backward compatibility round trip
    test::converter::RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest("eurostag-tutorial-multiple-selected-op-lim-group.xml", IidmXmlVersion::V1_12());

}



BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

