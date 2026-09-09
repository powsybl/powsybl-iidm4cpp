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

#include <powsybl/test/AssertionUtils.hpp>

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

    POWSYBL_ASSERT_THROW(reader.getOptionalAttributeValue("nonNamespacedAttributeInvalid", 0), std::invalid_argument, nullptr);
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

BOOST_AUTO_TEST_CASE(readListAttributeCSVFormated) {

    const std::string& txt = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                             "<element "
                             "    testListFormated0=\"&quot;&quot;&quot;&quot;,&quot;&quot;&quot;&quot;&quot;&quot;,&quot;&quot;&quot;&quot;&quot;&quot;&quot;&quot;,&quot;,&quot;,&quot;,&quot;&quot;,&quot;, ,A\" "
                             "    testListFormated1=\"&quot;test&quot;&quot;Value&quot;\" "
                             "    testListFormated2=\"&quot;&quot;&quot;testValue&quot;\" "
                             "    testListFormated3=\"&quot;testValue&quot;&quot;&quot;\" "
                             "    testListFormated4=\"&quot;test,Value,,,,&quot;\"  /> ";

    std::stringstream ss;
    ss << txt;
    powsybl::xml::XmlStreamReader reader(ss);

    auto list0 = reader.getOptionalArrayAttributeValue("testListFormated0","");
    auto list1 = reader.getOptionalArrayAttributeValue("testListFormated1","");
    auto list2 = reader.getOptionalArrayAttributeValue("testListFormated2","");
    auto list3 = reader.getOptionalArrayAttributeValue("testListFormated3","");
    auto list4 = reader.getOptionalArrayAttributeValue("testListFormated4","");
    BOOST_CHECK_EQUAL(7, list0.size());
    BOOST_CHECK_EQUAL(1, list1.size());
    BOOST_CHECK_EQUAL(1, list2.size());
    BOOST_CHECK_EQUAL(1, list3.size());
    BOOST_CHECK_EQUAL(1, list4.size());
    BOOST_CHECK(std::find(list0.begin(), list0.end(),"\"")!=list0.end());
    BOOST_CHECK(std::find(list0.begin(), list0.end(),"\"\"")!=list0.end());
    BOOST_CHECK(std::find(list0.begin(), list0.end(),"\"\"\"")!=list0.end());
    BOOST_CHECK(std::find(list0.begin(), list0.end(),",")!=list0.end());
    BOOST_CHECK(std::find(list0.begin(), list0.end(),",\",")!=list0.end());
    BOOST_CHECK(std::find(list0.begin(), list0.end()," ")!=list0.end());
    BOOST_CHECK(std::find(list0.begin(), list0.end(),"A")!=list0.end());
    BOOST_CHECK_EQUAL(list1.front(), "test\"Value");
    BOOST_CHECK_EQUAL(list2.front(), "\"testValue");
    BOOST_CHECK_EQUAL(list3.front(), "testValue\"");
    BOOST_CHECK_EQUAL(list4.front(), "test,Value,,,,");

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace powsybl
