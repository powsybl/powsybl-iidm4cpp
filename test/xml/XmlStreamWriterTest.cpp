/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sstream>

#include <boost/test/unit_test.hpp>

#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamException.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace xml {

BOOST_AUTO_TEST_SUITE(XmlWriterTestSuite)

BOOST_AUTO_TEST_CASE(XmlWriteApiWriteElementAttribute) {

    std::stringstream ss;
    powsybl::xml::XmlStreamWriter writer(ss, false);

    writer.writeStartDocument("UTF-8", "1.0");

    writer.writeStartElement("ns", "element");

    writer.setPrefix("ns", "http://ns/1_0");
    writer.setPrefix("ns2", "http://ns2/1_0");

    writer.writeAttribute("elementAttr", "elementAttrValue");
    writer.writeAttribute("elementAttr2", 1);
    writer.writeAttribute("elementAttr3", 1.0);
    writer.writeOptionalAttribute("elementAttr4", 1.0, stdcxx::nan());
    writer.writeOptionalAttribute("elementAttr5", stdcxx::nan(), stdcxx::nan());
    writer.writeOptionalAttribute("elementAttr6", "a", "");

    // add a sub-element
    writer.writeStartElement("", "subElement");
    writer.writeAttribute("subElementAttr", "subElementAttrValue");
    writer.writeEndElement();

    writer.writeEndElement();

    writer.writeEndDocument();

    std::string result = ss.str();

    BOOST_CHECK(result.find("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") != std::string::npos);
    BOOST_CHECK(result.find(
        "<ns:element xmlns:ns=\"http://ns/1_0\" xmlns:ns2=\"http://ns2/1_0\" elementAttr=\"elementAttrValue\" "
        "elementAttr2=\"1\" elementAttr3=\"1.000000\" elementAttr4=\"1.000000\" elementAttr6=\"a\">"
        "<subElement subElementAttr=\"subElementAttrValue\"/>"
        "</ns:element>") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(FailureEncoding) {

    std::stringstream ss;
    powsybl::xml::XmlStreamWriter writer(ss, false);

    POWSYBL_ASSERT_THROW(writer.writeStartDocument("aaa", "-1.0"), powsybl::xml::XmlStreamException,
                         "Unable to get encoder for encoding aaa");
}

BOOST_AUTO_TEST_CASE(FailureStartElementFails) {

    std::stringstream ss;
    powsybl::xml::XmlStreamWriter writer(ss, false);

    writer.writeStartDocument("UTF-8", "1.0");

    POWSYBL_ASSERT_THROW(writer.writeStartElement("", ""), powsybl::xml::XmlStreamException,
                         "Failed to write start element ");
}

BOOST_AUTO_TEST_CASE(FailureStartElement) {

    std::stringstream ss;
    powsybl::xml::XmlStreamWriter writer(ss, false);

    writer.writeStartDocument("UTF-8", "1.0");
    writer.writeStartElement("", "network");

    POWSYBL_ASSERT_THROW(writer.writeAttribute("", ""), powsybl::xml::XmlStreamException,
                         "Failed to write attribute ");
}

BOOST_AUTO_TEST_CASE(FailureSetPrefix) {

    std::stringstream ss;
    powsybl::xml::XmlStreamWriter writer(ss, false);

    writer.writeStartDocument("UTF-8", "1.0");
    writer.writeStartElement("", "network");

    POWSYBL_ASSERT_THROW(writer.setPrefix("", ""), powsybl::xml::XmlStreamException, "Prefix cannot be empty");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace powsybl
