/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_XML_XMLSTREAMREADER_HPP
#define POWSYBL_XML_XMLSTREAMREADER_HPP

#include <functional>
#include <iosfwd>
#include <memory>
#include <string>

#include <libxml/xmlreader.h>

#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/xml/XmlString.hpp>

namespace powsybl {

namespace xml {

class XmlStreamReader {
public:
    using ReadCallback = std::function<void()>;

public:
    explicit XmlStreamReader(std::istream& stream);

    XmlStreamReader(std::istream& stream, const std::string& encoding);

    ~XmlStreamReader() = default;

    template <typename T>
    T getAttributeValue(const std::string& attributeName) const;

    std::string getAttributeValue(const std::string& attributeName) const;

    std::string getDefaultNamespace() const;

    std::string getLocalName() const;

    std::string getNamespace(const std::string& prefix) const;

    std::string getNamespace(const std::string& prefix, const std::string& defaultValue) const;

    std::string getNamespaceOrDefault(const std::string& prefix) const;

    template <typename T>
    stdcxx::optional<T> getOptionalAttributeValue(const std::string& attributeName) const;

    bool getOptionalAttributeValue(const std::string& attributeName, bool defaultValue) const;

    std::string getOptionalAttributeValue(const std::string& attributeName, const char* defaultValue) const;

    int getOptionalAttributeValue(const std::string& attributeName, int defaultValue) const;

    double getOptionalAttributeValue(const std::string& attributeName, double defaultValue) const;

    unsigned long getOptionalAttributeValue(const std::string& attributeName, unsigned long defaultValue) const;

    std::string getOptionalAttributeValue(const std::string& attributeName, const std::string& defaultValue) const;

    std::string getPrefix() const;

    std::string getText() const;

    std::string readCharacters() const;

    std::string readUntilEndElement(const std::string& elementName) const;

    std::string readUntilEndElement(const std::string& elementName, const ReadCallback& callback) const;

    void readUntilNextElement() const;

    void skipComments() const;

private:
    XmlString getAttributeValue(const std::string& attributeName, bool throwException) const;

    int getCurrentNodeType() const;

    int next() const;

private:
    using XmlTextReaderPtr = std::unique_ptr<xmlTextReader, std::function<void(xmlTextReader*)>>;

private:
    std::istream& m_stream;

    XmlTextReaderPtr m_reader;
};

}  // namespace xml

}  // namespace powsybl

#endif //POWSYBL_XML_XMLSTREAMREADER_HPP
