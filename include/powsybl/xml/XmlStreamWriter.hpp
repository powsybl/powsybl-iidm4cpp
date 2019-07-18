/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_XML_XMLSTREAMWRITER_HPP
#define POWSYBL_XML_XMLSTREAMWRITER_HPP

#include <functional>
#include <iosfwd>
#include <memory>
#include <string>

#include <libxml/xmlwriter.h>

namespace powsybl {

namespace xml {

class XmlStreamWriter {
public:
    XmlStreamWriter(std::ostream& stream, bool indent);

    ~XmlStreamWriter() = default;

    void setPrefix(const std::string& prefix, const std::string& uri);

    void writeAttribute(const std::string& attributeName, double attributeValue);

    void writeAttribute(const std::string& attributeName, int attributeValue);

    void writeAttribute(const std::string& attributeName, const std::string& attributeValue);

    void writeAttribute(const std::string& attributeName, unsigned long attributeValue);

    void writeEmptyElement(const std::string& uri, const std::string& elementName);

    void writeEndDocument();

    void writeEndElement();

    void writeOptionalAttribute(const std::string& attributeName, double attributeValue, double absentValue);

    void writeOptionalAttribute(const std::string& attributeName, int attributeValue, int absentValue);

    void writeOptionalAttribute(const std::string& attributeName, const std::string& attributeValue,
                                const std::string& absentValue);

    void writeStartDocument(const std::string& encoding, const std::string& version);

    void writeStartElement(const std::string& prefix, const std::string& elementName);

private:
    using XmlStreamWriterPtr = std::unique_ptr<xmlTextWriter, std::function<void(xmlTextWriter*)>>;

private:
    std::ostream& m_stream;

    bool m_indent;

    XmlStreamWriterPtr m_writer;
};

}  // namespace xml

}  // namespace powsybl

#endif  // POWSYBL_XML_XMLSTREAMWRITER_HPP

