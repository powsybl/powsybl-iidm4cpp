/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sstream>

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/iidm/converter/SimpleAnonymizer.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/stdcxx/Properties.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

BOOST_AUTO_TEST_SUITE(SimpleAnonymizerTestSuite)

BOOST_AUTO_TEST_CASE(SimpleAnonymizationTest) {
    SimpleAnonymizer anonymizer;

    BOOST_CHECK_EQUAL(Country::FR, anonymizer.anonymizeCountry(Country::FR));
    BOOST_CHECK_EQUAL(Country::BE, anonymizer.anonymizeCountry(Country::BE));
    BOOST_CHECK_EQUAL(Country::FR, anonymizer.deanonymizeCountry(Country::FR));
    BOOST_CHECK_EQUAL(Country::BE, anonymizer.deanonymizeCountry(Country::BE));

    BOOST_CHECK_EQUAL("A", anonymizer.anonymizeString("test"));
    BOOST_CHECK_EQUAL("B", anonymizer.anonymizeString("a"));
    BOOST_CHECK_EQUAL("C", anonymizer.anonymizeString("b"));
    BOOST_CHECK_EQUAL("", anonymizer.anonymizeString(""));

    BOOST_CHECK_EQUAL("test", anonymizer.deanonymizeString("A"));
    BOOST_CHECK_EQUAL("a", anonymizer.deanonymizeString("B"));
    BOOST_CHECK_EQUAL("b", anonymizer.deanonymizeString("C"));
    BOOST_CHECK_EQUAL("", anonymizer.deanonymizeString(""));

    POWSYBL_ASSERT_THROW(anonymizer.deanonymizeString("FAKE"), PowsyblException, "Mapping not found for anonymized string 'FAKE'");
}

BOOST_AUTO_TEST_CASE(anonymizedRoadTrip) {
    const std::string& ref = test::converter::RoundTrip::getVersionedNetwork("eurostag-tutorial-example1.xml", xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION());
    Network network = Network::readXml(ref);

    // write anonymously
    stdcxx::Properties properties;
    properties.set(ExportOptions::ANONYMISED, "true");
    std::stringstream anonymizedStream;
    std::unique_ptr<Anonymizer> anonymizer = Network::writeXml(anonymizedStream, network, ExportOptions(properties));

    const auto& simpleAnonymizerTmp = dynamic_cast<const SimpleAnonymizer&>(*anonymizer);
    BOOST_CHECK_EQUAL(19, simpleAnonymizerTmp.getStringCount());

    // write mapping file
    std::stringstream mappingStream;
    anonymizer->write(mappingStream);

    BOOST_CHECK(anonymizedStream.str().find("VLGEN") == std::string::npos);

    // read using a new SimpleAnonymizer instance
    std::unique_ptr<Anonymizer> simpleAnonymizer = stdcxx::make_unique<SimpleAnonymizer>();
    simpleAnonymizer->read(mappingStream);
    Network::readXml(anonymizedStream, ImportOptions(), *simpleAnonymizer);

    std::stringstream deanonymizedStream;
    Network::writeXml(deanonymizedStream, network);

    BOOST_CHECK_EQUAL(ref, deanonymizedStream.str());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
