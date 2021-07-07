/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_EXTENSIONXMLSERIALIZER_HPP
#define POWSYBL_IIDM_CONVERTER_XML_EXTENSIONXMLSERIALIZER_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <powsybl/iidm/Extendable.hpp>
#include <powsybl/iidm/ExtensionProvider.hpp>

namespace powsybl {

namespace iidm {

class Extension;
class Identifiable;

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

/**
 * An ExtensionProvider able to serialize/deserialize extensions from XML
 *
 * An ExtensionXmlSerializer can have several versions with one XSD schema per version: the XML serialization/deserialization
 * of an extension is versionable.
 */
class ExtensionXmlSerializer : public ExtensionProvider {
public:
    /**
     * Construct an XML serializer for an extension
     *
     * @param extensionName the name of the extension
     * @param categoryName the category of the extension
     * @param namespacePrefix the XML prefix of the extension
     */
    ExtensionXmlSerializer(std::string&& extensionName, std::string&& categoryName, std::string&& namespacePrefix);

    ~ExtensionXmlSerializer() noexcept override = default;

    /**
     * Check if a XML version of the extension is supported
     *
     * @param extensionVersion The XML version for this extension
     *
     * @throws an {@link PowsyblException} if the version is not supported.
     */
    virtual void checkExtensionVersionSupported(const std::string& extensionVersion) const;

    /**
     * Return the name of this serializer
     *
     * @return the name of this serializer
     */
    const std::string& getName() const;

    /**
     * Return the XML prefix of the extension
     *
     * @return the XML prefix of the extension
     */
    const std::string& getNamespacePrefix() const;

    /**
     * Return the namespace URI of the extension in the latest version of its serialization
     *
     * @return the namespace URI of the extension in the latest version
     */
    virtual const std::string& getNamespaceUri() const = 0;

    /**
     * Return the namespace URI of the extension in the given version of its serialization
     *
     * @param extensionVersion the version of its serialization
     *
     * @return the namespace URI of the extension in the given version
     */
    virtual const std::string& getNamespaceUri(const std::string& extensionVersion) const = 0;

    /**
     * Return the current version of this serializer
     *
     * @return the current version of this serializer
     */
    virtual const std::string& getVersion() const;

    /**
     * Return true if the extension is serializable, false else
     *
     * @return true if the extension is serializable, false else
     */
    virtual bool isSerializable(const Extension& extension) const;

    /**
     * Create an extension from its XML serialization
     *
     * @param extendable The holder of the new extension
     * @param context The current XML context
     *
     * @return The added extension
     *
     * @throw An {@link AssertionError} if the extension is not compatible with the extendable type
     */
    virtual Extension& read(Extendable& extendable, NetworkXmlReaderContext& context) const = 0;

    /**
     * Write an extension in XML
     *
     * @param extension The extension to write to XML
     * @param context The current XML context
     */
    virtual void write(const Extension& extension, NetworkXmlWriterContext& context) const = 0;

protected:
    /**
     * A utility function to cast a generic extension into its specific type E
     *
     * @tparam E The expected type of the extension
     *
     * @param extension The extension to cast
     *
     * @return The extension casted into E if the extension is an instance of E
     *
     * @throw a {@link PowsyblException} if the extension is not compatible with the expected type
     */
    template <typename E>
    const E& safeCast(const Extension& extension) const;

private:
    std::string m_namespacePrefix;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hxx>

#endif  // POWSYBL_IIDM_CONVERTER_XML_EXTENSIONXMLSERIALIZER_HPP
