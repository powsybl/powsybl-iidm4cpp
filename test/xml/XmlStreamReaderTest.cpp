/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sstream>
#include <string>

#include <boost/test/unit_test.hpp>

#include <powsybl/xml/XmlStreamException.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace xml {

BOOST_AUTO_TEST_SUITE(XmlReaderTestSuite)

const std::string& xmlTest = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                             "<iidm:network xmlns=\"http://www.itesla_project.eu/schema/iidm/1_1\" "
                             "              xmlns:iidm=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                             "               id=\"aa\" "
                             "               caseDate=\"bb\" "
                             "               forecastDistance=\"cc\" "
                             "               sourceFormat=\"dd\" "
                             "               iidm:attributeNamespace=\"a\" "
                             "               attributeNoNamespace=\"ab\" "
                             "               iidm:namespacedAttributeInvalid=\"cd\" "
                             "               iidm:namespacedAttributeValid=\"2\" "
                             "               nonNamespacedAttributeInvalid=\"ef\" "
                             "               nonNamespacedAttributeValid=\"3\" "
                             "               intAttribute=\"5\" "
                             "               nonIntAttribute=\"abcdef\"> "
                             "    <iidm:substation id=\"S1\" country=\"FR\" tso=\"RTE\">"
                             "        <iidm:voltageLevel id=\"VL4\" nominalV=\"90.0\" topologyKind=\"BUS_BREAKER\">"
                             "        </iidm:voltageLevel>"
                             "    </iidm:substation>"
                             "</iidm:network>";

BOOST_AUTO_TEST_CASE(XmlReadApiGetAttribute) {

    std::stringstream ss;
    ss << xmlTest;
    powsybl::xml::XmlStreamReader reader(ss);

    BOOST_CHECK_EQUAL("iidm", reader.getPrefix());
    BOOST_CHECK_EQUAL("http://www.itesla_project.eu/schema/iidm/1_0", reader.getAttributeValue("xmlns:iidm"));
    BOOST_CHECK_EQUAL("aa", reader.getAttributeValue("id"));
    BOOST_CHECK_EQUAL("bb", reader.getAttributeValue("caseDate"));
    BOOST_CHECK_EQUAL("cc", reader.getAttributeValue("forecastDistance"));
    BOOST_CHECK_EQUAL("dd", reader.getAttributeValue("sourceFormat"));
    BOOST_CHECK_EQUAL("ab", reader.getAttributeValue("attributeNoNamespace"));

    POWSYBL_ASSERT_THROW(reader.getOptionalAttributeValue("nonNamespacedAttributeInvalid", 0),
                         powsybl::xml::XmlStreamException, "Unable to convert attribute nonNamespacedAttributeInvalid to int");
    BOOST_CHECK_EQUAL(3, reader.getOptionalAttributeValue("nonNamespacedAttributeValid", 0));


    BOOST_CHECK_EQUAL("http://www.itesla_project.eu/schema/iidm/1_0", reader.getNamespace("iidm"));
    BOOST_CHECK_EQUAL("http://www.itesla_project.eu/schema/iidm/1_1", reader.getNamespace(""));
    BOOST_CHECK_EQUAL("http://www.itesla_project.eu/schema/iidm/1_1", reader.getDefaultNamespace());
    POWSYBL_ASSERT_THROW(reader.getNamespace("test"), powsybl::xml::XmlStreamException, "Unknown prefix test");
}

BOOST_AUTO_TEST_CASE(XmlReadApiReadUntilEndElement) {

    std::stringstream ss;
    ss << xmlTest;
    powsybl::xml::XmlStreamReader reader(ss);

    xml::XmlStreamReader::ReadCallback callback = [&reader]() {
        if (reader.getLocalName() == "substation") {
            BOOST_CHECK_EQUAL("FR", reader.getAttributeValue("country"));
        } else if (reader.getLocalName() == "voltageLevel") {
            BOOST_CHECK_EQUAL("BUS_BREAKER", reader.getAttributeValue("topologyKind"));
        } else {
            BOOST_FAIL("Unknown node : " + reader.getLocalName());
        }
    };

    reader.readUntilEndElement("network", callback);
}

BOOST_AUTO_TEST_CASE(XmlReadApiReadSpecialChars) {

    const std::string& specialChars = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                      "<network "
                                      "    latin=\"éèàïû\" "
                                      "    norvegian=\"å/Å ø/Ø æ/Æ\"> "
                                      "</network>";
    std::stringstream ss;
    ss << specialChars;
    powsybl::xml::XmlStreamReader reader(ss);

    BOOST_CHECK_EQUAL("éèàïû", reader.getAttributeValue("latin"));
    BOOST_CHECK_EQUAL("å/Å ø/Ø æ/Æ", reader.getAttributeValue("norvegian"));
}

BOOST_AUTO_TEST_CASE(skipCommentsTest) {

    const std::string& txt = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                             "<!-- comment --> "
                             "<!-- comment2 --> "
                             "<network "
                             "    latin=\"éèàïû\" "
                             "    norvegian=\"å/Å ø/Ø æ/Æ\"> "
                             "</network>";

    std::stringstream ss;
    ss << txt;
    powsybl::xml::XmlStreamReader reader(ss);

    reader.skipComments();

    POWSYBL_ASSERT_THROW(reader.getAttributeValue("fakeAttribute"), XmlStreamException,
                         "Attribute fakeAttribute does not exists");
}

BOOST_AUTO_TEST_CASE(FailureErroneousAttribute) {

    std::stringstream ss;
    ss << xmlTest;
    powsybl::xml::XmlStreamReader reader(ss);

    POWSYBL_ASSERT_THROW(reader.getAttributeValue("fakeAttribute"), XmlStreamException,
                         "Attribute fakeAttribute does not exists");
}

BOOST_AUTO_TEST_CASE(FailureErroneousAttributeNS) {

    std::stringstream ss;
    ss << xmlTest;
    powsybl::xml::XmlStreamReader reader(ss);

    POWSYBL_ASSERT_THROW(reader.getAttributeValue("fakeAttribute"), XmlStreamException,
                         "Attribute fakeAttribute does not exists");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace powsybl
