/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_EXTENSIONXMLSERIALIZER_HPP
#define POWSYBL_IIDM_CONVERTER_XML_EXTENSIONXMLSERIALIZER_HPP

#include <memory>
#include <string>

#include <powsybl/iidm/Extendable.hpp>

namespace powsybl {

namespace iidm {

class Extension;
class Identifiable;

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

class ExtensionXmlSerializer {
public:
    ExtensionXmlSerializer(const std::string& extensionName,
                           const std::string& categoryName, /*const std::type_index& extensionClass,*/
                           bool subElements, const std::string& namespaceUri, const std::string& namespacePrefix);

    virtual ~ExtensionXmlSerializer() noexcept = default;

    const std::string& getCategoryName() const;

    // std::type_index getExtensionClass();

    const std::string& getExtensionName() const;

    const std::string& getNamespacePrefix() const;

    const std::string& getNamespaceUri() const;

    bool hasSubElements() const;

    virtual std::unique_ptr<Extension> read(Extendable& extendable, NetworkXmlReaderContext& context) const = 0;

    virtual void write(const Extension& extension, NetworkXmlWriterContext& context) const = 0;

protected:
    template <typename E>
    const E& safeCast(const Extension& extension) const;

private:
    std::string m_extensionName;

    std::string m_categoryName;

    // TODO(mathbagu): extensionClass

    bool m_subElements;

    // TODO(mathbague) xsdFileName;

    std::string m_namespaceUri;

    std::string m_namespacePrefix;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hxx>

#endif  // POWSYBL_IIDM_CONVERTER_XML_EXTENSIONXMLSERIALIZER_HPP
