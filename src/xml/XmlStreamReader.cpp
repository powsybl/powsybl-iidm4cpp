/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/xml/XmlStreamReader.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <libxml/xmlreader.h>

#include <powsybl/AssertionError.hpp>
#include <powsybl/logging/MessageFormat.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/xml/XmlCharConversion.hpp>
#include <powsybl/xml/XmlStreamException.hpp>

#include "XmlEncoding.hpp"

namespace powsybl {

namespace xml {

void checkNodeType(xmlTextReader& reader, const xmlReaderTypes& expected) {
    int nodeType = xmlTextReaderNodeType(&reader);
    if (nodeType != expected) {
        throw XmlStreamException(logging::format("Unexpected node type: %1% (expected: %2%)", nodeType, expected));
    }
}

XmlStreamReader::XmlStreamReader(std::istream& stream) :
    XmlStreamReader(stream, DEFAULT_ENCODING) {
}

XmlStreamReader::XmlStreamReader(std::istream& stream, const std::string& encoding) :
    m_stream(stream),
    m_reader(nullptr) {

    static const int XML_PARSE_NO_OPTION = 0;

    auto readCallback = [](void* context, char* buffer, int len) {
        XmlStreamReader& reader = *static_cast<XmlStreamReader*>(context);
        reader.m_stream.read(buffer, len);

        return static_cast<int>(reader.m_stream.gcount());
    };

    auto closeCallback = [](void* /*context*/) {
        return 0;
    };

    auto deleteReaderCallback = [](void* ptr) {
        xmlFreeTextReader(static_cast<xmlTextReaderPtr>(ptr));
    };

    m_reader = XmlTextReaderPtr(
        xmlReaderForIO(readCallback, closeCallback, this, "", encoding.c_str(), XML_PARSE_NO_OPTION),
        deleteReaderCallback);

    next();
}

template <>
bool XmlStreamReader::getAttributeValue(const std::string& attributeName) const {
    XmlString value = getAttributeValue(attributeName, false);
    const std::string& booleanStr = XML2S(value.get());
    return boost::iequals(booleanStr, "true");
}

template <>
double XmlStreamReader::getAttributeValue(const std::string& attributeName) const {
    XmlString value = getAttributeValue(attributeName, true);
    return std::stod(XML2S(value.get()));
}

template <>
int XmlStreamReader::getAttributeValue(const std::string& attributeName) const {
    XmlString value = getAttributeValue(attributeName, true);
    return std::stoi(XML2S(value.get()));
}

template <>
long XmlStreamReader::getAttributeValue(const std::string& attributeName) const {
    XmlString value = getAttributeValue(attributeName, true);
    return std::stol(XML2S(value.get()));
}

template <>
unsigned long XmlStreamReader::getAttributeValue(const std::string& attributeName) const {
    XmlString value = getAttributeValue(attributeName, false);
    return std::stoul(XML2S(value.get()));
}

std::string XmlStreamReader::getAttributeValue(const std::string& attributeName) const {
    XmlString value = getAttributeValue(attributeName, true);
    return XML2S(value.get());
}

XmlString XmlStreamReader::getAttributeValue(const std::string& attributeName, bool throwException) const {
    checkNodeType(*m_reader, XML_READER_TYPE_ELEMENT);

    XmlString value(xmlTextReaderGetAttribute(m_reader.get(), S2XML(attributeName)));
    if (!value && throwException) {
        throw XmlStreamException(logging::format("Attribute %1% does not exists", attributeName.c_str()));
    }

    return value;
}

int XmlStreamReader::getCurrentNodeType() const {
    return xmlTextReaderNodeType(m_reader.get());
}

std::string XmlStreamReader::getDefaultNamespace() const {
    XmlString namespaceXml(xmlTextReaderLookupNamespace(m_reader.get(), nullptr));
    return namespaceXml ? XML2S(namespaceXml.get()) : "";
}

std::string XmlStreamReader::getLocalName() const {
    XmlString localNameXml(xmlTextReaderLocalName(m_reader.get()));
    if (!localNameXml) {
        throw XmlStreamException("Element does not have any local name");
    }

    return XML2S(localNameXml.get());
}

std::string XmlStreamReader::getNamespace(const std::string& prefix) const {
    if (prefix.empty()) {
        return getDefaultNamespace();
    }

    XmlString namespaceXml(xmlTextReaderLookupNamespace(m_reader.get(), S2XML(prefix)));
    if (!namespaceXml) {
        throw XmlStreamException(logging::format("Unknown prefix %1%", prefix));
    }
    return XML2S(namespaceXml.get());
}

std::string XmlStreamReader::getNamespace(const std::string& prefix, const std::string& defaultValue) const {
    XmlString namespaceXml(xmlTextReaderLookupNamespace(m_reader.get(), S2XML(prefix)));
    return namespaceXml ? XML2S(namespaceXml.get()) : defaultValue;
}

std::string XmlStreamReader::getNamespaceOrDefault(const std::string& prefix) const {
    return getNamespace(prefix, getDefaultNamespace());
}

template <>
stdcxx::optional<bool> XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName) const {
    stdcxx::optional<bool> value;

    const XmlString& str = getAttributeValue(attributeName, false);
    if (str) {
        value = boost::iequals(XML2S(str.get()), "true");
    }

    return value;
}

template <>
stdcxx::optional<int> XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName) const {
    stdcxx::optional<int> value;

    const XmlString& str = getAttributeValue(attributeName, false);
    if (str) {
        value = std::stoi(XML2S(str.get()));
    }

    return value;
}

template <>
stdcxx::optional<double> XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName) const {
    stdcxx::optional<double> value;

    const XmlString& str = getAttributeValue(attributeName, false);
    if (str) {
        value = std::stod(XML2S(str.get()));
    }

    return value;
}

template <>
stdcxx::optional<unsigned long> XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName) const {
    stdcxx::optional<unsigned long> value;

    const XmlString& str = getAttributeValue(attributeName, false);
    if (str) {
        value = std::stoul(XML2S(str.get()));
    }

    return value;
}

template <>
stdcxx::optional<std::string> XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName) const {
    stdcxx::optional<std::string> value;

    const auto& str = getAttributeValue(attributeName, false);
    if (str) {
        value = XML2S(str.get());
    }

    return value;
}

bool XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName, bool defaultValue) const {
    const auto& value = getOptionalAttributeValue<bool>(attributeName);
    return value.is_initialized() ? *value : defaultValue;
}

std::string XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName, const char* defaultValue) const {
    const auto& value = getOptionalAttributeValue<std::string>(attributeName);
    return value.is_initialized() ? *value : defaultValue;
}

int XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName, int defaultValue) const {
    const auto& value = getOptionalAttributeValue<int>(attributeName);
    return value.is_initialized() ? *value : defaultValue;
}

double XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName, double defaultValue) const {
    const auto& value = getOptionalAttributeValue<double>(attributeName);
    return value.is_initialized() ? *value : defaultValue;
}

unsigned long XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName, unsigned long defaultValue) const {
    const auto& value = getOptionalAttributeValue<unsigned long>(attributeName);
    return value.is_initialized() ? *value : defaultValue;
}

std::string XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName, const std::string& defaultValue) const {
    const auto& value = getOptionalAttributeValue<std::string>(attributeName);
    return value.is_initialized() ? *value : defaultValue;
}

std::string XmlStreamReader::getPrefix() const {
    checkNodeType(*m_reader, XML_READER_TYPE_ELEMENT);

    std::string prefix;
    XmlString prefixXml(xmlTextReaderPrefix(m_reader.get()));
    if (prefixXml) {
        prefix = XML2S(prefixXml.get());
    }
    return prefix;
}

std::string XmlStreamReader::getText() const {
    checkNodeType(*m_reader, XML_READER_TYPE_TEXT);

    std::string text;
    XmlString content(xmlTextReaderReadString(m_reader.get()));
    if (content) {
        text = XML2S(content.get());
    }
    return text;
}

int XmlStreamReader::next() const {
    xmlTextReaderRead(m_reader.get());
    return getCurrentNodeType();
}

std::string XmlStreamReader::readUntilEndElement(const std::string& elementName) const {
    return readUntilEndElement(elementName, [](){});
}

std::string XmlStreamReader::readUntilEndElement(const std::string& elementName, const ReadCallback& callback) const {
    std::string text;
    int emptyElement = xmlTextReaderIsEmptyElement(m_reader.get());
    if (emptyElement == -1) {
        // Error
        throw XmlStreamException(logging::format("An error occurred while reading <%1%>", elementName.c_str()));
    }
    if (emptyElement == 1) {
        // if current element is an empty element, XML_READER_TYPE_END_ELEMENT is never reached => nothing to do
        return text;
    }

    // not an empty element => read until end element
    int res = xmlTextReaderRead(m_reader.get());
    int nodeType = getCurrentNodeType();
    std::string name = getLocalName();
    if (nodeType == XML_READER_TYPE_TEXT) {
        text = getText();
    }

    while (res == 1 && (nodeType != XML_READER_TYPE_END_ELEMENT || name != elementName)) {
        if (nodeType == XML_READER_TYPE_ELEMENT) {
            callback();
        }

        res = xmlTextReaderRead(m_reader.get());
        nodeType = getCurrentNodeType();
        name = getLocalName();
        if (nodeType == XML_READER_TYPE_TEXT) {
            text = getText();
        }
    }

    if (res != 1) {
        throw XmlStreamException(logging::format("Unexpected end of file: expecting %1%", elementName.c_str()));
    }

    return text;
}

void XmlStreamReader::readUntilNextElement() const {
    int res = xmlTextReaderRead(m_reader.get());

    int nodeType = getCurrentNodeType();
    while (res == 1 && nodeType != XML_READER_TYPE_ELEMENT) {
        res = xmlTextReaderRead(m_reader.get());
        nodeType = getCurrentNodeType();
    }
}

void XmlStreamReader::skipComments() const {
    while (getCurrentNodeType() == XML_READER_TYPE_COMMENT) {
        next();
    }
}

}  // namespace xml

}  // namespace powsybl
