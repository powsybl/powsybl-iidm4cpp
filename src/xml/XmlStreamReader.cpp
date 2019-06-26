/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/xml/XmlStreamReader.hpp>

#include <libxml/xmlreader.h>

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

    auto closeCallback = [](void*) {
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

std::string XmlStreamReader::getAttributeValue(const std::string& attributeName) const {
    std::unique_ptr<xmlChar> value = getAttributeValue(attributeName, true);
    return XML2S(value.get());
}

std::unique_ptr<xmlChar>
XmlStreamReader::getAttributeValue(const std::string& attributeName, bool throwException) const {
    checkNodeType(*m_reader, XML_READER_TYPE_ELEMENT);

    std::unique_ptr<xmlChar> value(xmlTextReaderGetAttribute(m_reader.get(), S2XML(attributeName)));
    if (!value && throwException) {
        throw XmlStreamException(logging::format("Attribute %1% does not exists", attributeName.c_str()));
    }

    return value;
}

int XmlStreamReader::getCurrentNodeType() const {
    return xmlTextReaderNodeType(m_reader.get());
}

std::string XmlStreamReader::getDefaultNamespace() const {
    std::unique_ptr<xmlChar> namespaceXml(xmlTextReaderLookupNamespace(m_reader.get(), nullptr));
    return namespaceXml ? XML2S(namespaceXml.get()) : "";
}

std::string XmlStreamReader::getLocalName() const {
    checkNodeType(*m_reader, XML_READER_TYPE_ELEMENT);

    std::unique_ptr<xmlChar> localNameXml(xmlTextReaderLocalName(m_reader.get()));
    if (!localNameXml) {
        throw XmlStreamException("Element does not have any local name");
    }

    return XML2S(localNameXml.get());
}

std::string XmlStreamReader::getNamespace(const std::string& prefix) const {
    if (prefix.empty()) {
        return getDefaultNamespace();
    }

    std::unique_ptr<xmlChar> namespaceXml(xmlTextReaderLookupNamespace(m_reader.get(), S2XML(prefix)));
    if (!namespaceXml) {
        throw XmlStreamException(logging::format("Unknown prefix %1%", prefix));
    }
    return XML2S(namespaceXml.get());
}

int XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName, int defaultValue) const {
    std::unique_ptr<xmlChar> value = getAttributeValue(attributeName, false);
    try {
        return static_cast<bool>(value) ? std::stoi(XML2S(value.get())) : defaultValue;
    } catch (std::invalid_argument& error) {
        throw XmlStreamException(logging::format("Unable to convert attribute %1% to int", attributeName));
    }
}

double XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName, double defaultValue) const {
    std::unique_ptr<xmlChar> value = getAttributeValue(attributeName, false);
    try {
        return static_cast<bool>(value) ? std::stod(XML2S(value.get())) : defaultValue;
    } catch (const std::invalid_argument& error) {
        throw XmlStreamException(logging::format("Unable to convert attribute %1% to double", attributeName));
    }
}

std::string
XmlStreamReader::getOptionalAttributeValue(const std::string& attributeName, const std::string& defaultValue) const {
    std::unique_ptr<xmlChar> value = getAttributeValue(attributeName, false);
    return static_cast<bool>(value) ? XML2S(value.get()) : defaultValue;
}

std::string XmlStreamReader::getPrefix() const {
    checkNodeType(*m_reader, XML_READER_TYPE_ELEMENT);

    std::string prefix;
    std::unique_ptr<xmlChar> prefixXml(xmlTextReaderPrefix(m_reader.get()));
    if (prefixXml) {
        prefix = XML2S(prefixXml.get());
    }
    return prefix;
}

int XmlStreamReader::next() const {
    xmlTextReaderRead(m_reader.get());
    return getCurrentNodeType();
}

void XmlStreamReader::readUntilEndElement(const std::string& elementName, const ReadCallback& callback) const {
    int res = xmlTextReaderRead(m_reader.get());

    std::string name;

    int nodeType = getCurrentNodeType();
    if (nodeType == XML_READER_TYPE_ELEMENT) {
        name = getLocalName();
    }

    while (res == 1 && nodeType != XML_READER_TYPE_END_ELEMENT && name != elementName) {
        if (nodeType == XML_READER_TYPE_ELEMENT) {
            callback(*this);
        }

        res = xmlTextReaderRead(m_reader.get());
        nodeType = getCurrentNodeType();
        if (nodeType == XML_READER_TYPE_ELEMENT) {
            name = getLocalName();
        }
    }

    if (res != 1) {
        throw XmlStreamException(logging::format("Unexpected end of file: expecting %1%", elementName.c_str()));
    }
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
