/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/xml/XmlStreamWriter.hpp>

#include <libxml/xmlwriter.h>

#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlCharConversion.hpp>
#include <powsybl/xml/XmlStreamException.hpp>

#include "XmlEncoding.hpp"

namespace powsybl {

namespace xml {

XmlStreamWriter::XmlStreamWriter(std::ostream& stream, bool indent) :
    m_stream(stream),
    m_indent(indent),
    m_writer(nullptr) {

}

void XmlStreamWriter::setPrefix(const std::string& prefix, const std::string& uri) {
    if (prefix.empty()) {
        throw XmlStreamException("Prefix cannot be empty");
    }

    const std::string& fullPrefix = "xmlns:" + prefix;
    writeAttribute(fullPrefix, uri);
}

void XmlStreamWriter::writeAttribute(const std::string& attributeName, bool attributeValue) {
    writeAttribute(attributeName, std::string(attributeValue ? "true" : "false"));
}

void XmlStreamWriter::writeAttribute(const std::string& attributeName, const char* attributeValue) {
    writeAttribute(attributeName, std::string(attributeValue));
}

void XmlStreamWriter::writeAttribute(const std::string& attributeName, double attributeValue) {
    if (!std::isnan(attributeValue)) {
        writeAttribute(attributeName, stdcxx::to_string(attributeValue));
    }
}

void XmlStreamWriter::writeAttribute(const std::string& attributeName, int attributeValue) {
    writeAttribute(attributeName, std::to_string(attributeValue));
}

void XmlStreamWriter::writeAttribute(const std::string& attributeName, const std::string& attributeValue) {
    int written = xmlTextWriterWriteAttribute(m_writer.get(), S2XML(attributeName), S2XML(attributeValue));
    if (written < 0) {
        throw XmlStreamException(stdcxx::format("Failed to write attribute %1%", attributeName));
    }
}

void XmlStreamWriter::writeAttribute(const std::string& attributeName, long attributeValue) {
    writeAttribute(attributeName, std::to_string(attributeValue));
}

void XmlStreamWriter::writeAttribute(const std::string& attributeName, unsigned long attributeValue) {
    writeAttribute(attributeName, std::to_string(attributeValue));
}

void XmlStreamWriter::writeCharacters(const std::string& content) {
    int written = xmlTextWriterWriteString(m_writer.get(), S2XML(content));
    if (written < 0) {
        throw XmlStreamException(stdcxx::format("Failed to write characters %1%", content));
    }
}

void XmlStreamWriter::writeEmptyElement(const std::string& uri, const std::string& elementName) {
    writeStartElement(uri, elementName);
    writeEndElement();
}

void XmlStreamWriter::writeEndDocument() {
    // deleting the xmlTextWriter pointer automatically calls xmlTextWriterFlush() and write to stream
    // m_writer is instanciated in writeStartElement, so it must be deleted here
    m_writer.reset();
}

void XmlStreamWriter::writeEndElement() {
    int written = xmlTextWriterEndElement(m_writer.get());
    if (written < 0) {
        throw XmlStreamException("Failed to write end element");
    }
}

void XmlStreamWriter::writeOptionalAttribute(const std::string& attributeName, bool attributeValue, bool absentValue) {
    if (attributeValue != absentValue) {
        writeAttribute(attributeName, attributeValue);
    }
}

void XmlStreamWriter::writeOptionalAttribute(const std::string& attributeName, const char* attributeValue, const char* absentValue) {
    if (strcmp(attributeValue, absentValue) != 0) {
        writeAttribute(attributeName, attributeValue);
    }
}

void XmlStreamWriter::writeOptionalAttribute(const std::string& attributeName, double attributeValue) {
    writeAttribute(attributeName, attributeValue);
}

void XmlStreamWriter::writeOptionalAttribute(const std::string& attributeName, double attributeValue, double absentValue) {
    if (!std::isnan(attributeValue) && !stdcxx::isEqual(attributeValue, absentValue)) {
        writeAttribute(attributeName, stdcxx::to_string(attributeValue));
    }
}

void XmlStreamWriter::writeOptionalAttribute(const std::string& attributeName, int attributeValue, int absentValue) {
    if (attributeValue != absentValue) {
        writeAttribute(attributeName, std::to_string(attributeValue));
    }
}

void XmlStreamWriter::writeOptionalAttribute(const std::string& attributeName, unsigned long attributeValue, unsigned long absentValue) {
    if (attributeValue != absentValue) {
        writeAttribute(attributeName, std::to_string(attributeValue));
    }
}

void XmlStreamWriter::writeOptionalAttribute(const std::string& attributeName, const std::string& attributeValue,
                                             const std::string& absentValue) {
    if (attributeValue != absentValue) {
        writeAttribute(attributeName, attributeValue);
    }
}

void XmlStreamWriter::writeStartDocument(const std::string& encoding, const std::string& version) {
    if (m_writer) {
        throw XmlStreamException("XmlStreamWriter::WriteStartDocument should be call once");
    }

    xmlCharEncodingHandlerPtr encoder = xmlFindCharEncodingHandler(encoding.c_str());
    if (encoder == nullptr) {
        throw XmlStreamException(stdcxx::format("Unable to get encoder for encoding %1%", encoding));
    }

    // initialize writer
    auto writeCallback = [](void* context, const char* buffer, int len) {
        XmlStreamWriter& writer = *static_cast<XmlStreamWriter*>(context);

        size_t beforeWrite = writer.m_stream.tellp();
        writer.m_stream.write(buffer, len);
        size_t afterWrite = writer.m_stream.tellp();

        return static_cast<int>(afterWrite - beforeWrite);
    };

    auto closeCallback = [](void* /*context*/) {
        return 0;
    };

    auto deleteWriterCallback = [](void* ptr) {
        xmlFreeTextWriter(static_cast<xmlTextWriterPtr>(ptr));
    };

    xmlOutputBufferPtr buffer = xmlOutputBufferCreateIO(writeCallback, closeCallback, this, encoder);

    m_writer = XmlStreamWriterPtr(xmlNewTextWriter(buffer), deleteWriterCallback);

    if (!m_writer) {
        throw XmlStreamException(stdcxx::format("Unable to create xml writer"));
    }

    int ok = xmlTextWriterSetIndent(m_writer.get(), m_indent ? 1 : 0);
    if (ok == 0) {
        ok = xmlTextWriterSetIndentString(m_writer.get(), S2XML(std::string(4, ' ')));
    }

    if (ok < 0) {
        throw XmlStreamException(stdcxx::format("Cannot set indentation"));
    }

    int written = xmlTextWriterStartDocument(m_writer.get(), version.c_str(), encoding.c_str(), nullptr);
    if (written < 0) {
        throw XmlStreamException("Failed to write start document");
    }
}

void XmlStreamWriter::writeStartElement(const std::string& prefix, const std::string& elementName) {
    const std::string& fullElementName = prefix.empty() ? elementName : prefix + ":" + elementName;
    int written = xmlTextWriterStartElement(m_writer.get(), S2XML(fullElementName));
    if (written < 0) {
        throw XmlStreamException(stdcxx::format("Failed to write start element %1%", fullElementName));
    }
}

}  // namespace xml

}  // namespace powsybl
