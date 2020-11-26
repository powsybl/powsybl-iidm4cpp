/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_IIDMXMLUTIL_HPP
#define POWSYBL_IIDM_CONVERTER_XML_IIDMXMLUTIL_HPP

#include <functional>
#include <string>

#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

namespace ErrorMessage {

static const char* const MANDATORY = "mandatory";
static const char* const NOT_DEFAULT_NOT_SUPPORTED = "not defined as default and not supported";
static const char* const NOT_NULL_NOT_SUPPORTED = "not null and not supported";
static const char* const NOT_SUPPORTED = "not supported";

}  // namespace ErrorMessage

class IidmXmlUtil {
public:
    /**
     * Assert that the context's IIDM-XML version equals or is less recent than a given IIDM-XML version
     *
     * @param rootElementName the name of the XML root element
     * @param elementName the name of the XML element
     * @param errorMessage the type of error message in case of an exception is thrown
     * @param maxVersion the IIDM-XML version in which the element has been removed
     * @param context the context
     *
     * @throw a {@link PowsyblException} if the the context's version is more recent than the maxVersion
     */
    static void assertMaximumVersion(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& maxVersion, const NetworkXmlReaderContext& context);

    /**
     * Assert that the context's IIDM-XML version equals or is less recent than a given IIDM-XML version
     *
     * @param rootElementName the name of the XML root element
     * @param elementName the name of the XML element
     * @param errorMessage the type of error message in case of an exception is thrown
     * @param maxVersion the IIDM-XML version in which the element has been removed
     * @param context the context
     *
     * @throw a {@link PowsyblException} if the the context's version is more recent than the maxVersion
     */
    static void assertMaximumVersion(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& maxVersion, const NetworkXmlWriterContext& context);

    /**
     * Assert that the context's IIDM-XML version equals or is more recent than a given IIDM-XML version
     *
     * @param rootElementName the name of the XML root element
     * @param elementName the name of the XML element
     * @param errorMessage the type of error message in case of an exception is thrown
     * @param minVersion the IIDM-XML version in which the element has been added
     * @param context the context
     *
     * @throw a {@link PowsyblException} if the context's version less recent than the minVersion
     */
    static void assertMinimumVersion(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& minVersion, const NetworkXmlReaderContext& context);

    /**
     * Assert that the context's IIDM-XML version equals or is more recent than a given IIDM-XML version
     *
     * @param rootElementName the name of the XML root element
     * @param elementName the name of the XML element
     * @param errorMessage the type of error message in case of an exception is thrown
     * @param minVersion the IIDM-XML version in which the element has been added
     * @param context the context
     *
     * @throw a {@link PowsyblException} if the context's version less recent than the minVersion
     */
    static void assertMinimumVersion(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& minVersion, const NetworkXmlWriterContext& context);

    /**
     * Assert that the context's IIDM-XML version equals or is more recent than a given IIDM-XML version if the value of an attribute or the state of an equipment
     * is not default (interpretable for previous versions)
     *
     * @param valueIsNotDefault assert only if this parameter is true
     * @param rootElementName the name of the XML root element
     * @param elementName the name of the XML element
     * @param errorMessage the type of error message in case of an exception is thrown
     * @param minVersion the IIDM-XML version in which the element has been added
     * @param context the context
     *
     * @throw a {@link PowsyblException} if the context's version less recent than the minVersion
     */
    static void assertMinimumVersionIfNotDefault(bool valueIsNotDefault, const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& minVersion, const NetworkXmlReaderContext& context);

    /**
     * Assert that the context's IIDM-XML version equals or is more recent than a given IIDM-XML version if the value of an attribute or the state of an equipment
     * is not default (interpretable for previous versions)
     *
     * @param valueIsNotDefault assert only if this parameter is true
     * @param rootElementName the name of the XML root element
     * @param elementName the name of the XML element
     * @param errorMessage the type of error message in case of an exception is thrown
     * @param minVersion the IIDM-XML version in which the element has been added
     * @param context the context
     *
     * @throw a {@link PowsyblException} if the context's version less recent than the minVersion
     */
    static void assertMinimumVersionIfNotDefault(bool valueIsNotDefault, const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& minVersion, const NetworkXmlWriterContext& context);

    /**
     * Assert that the context's IIDM-XML version equals or is more recent than a given IIDM-XML version if the value of an attribute or the state of an equipment
     * is not default (interpretable for previous versions).
     * If the value is not default and no exception has been thrown, run a given runnable.
     *
     * @param valueIsNotDefault assert only if this parameter is true
     * @param rootElementName the name of the XML root element
     * @param elementName the name of the XML element
     * @param errorMessage the type of error message in case of an exception is thrown
     * @param minVersion the IIDM-XML version in which the element has been added
     * @param context the context
     * @param runnable the function to run
     *
     * @throw a {@link PowsyblException} if the context's version is less recent than the minVersion and valueIsNotDefault is true
     */
    static void assertMinimumVersionAndRunIfNotDefault(bool valueIsNotDefault, const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& minVersion, const NetworkXmlReaderContext& context, const std::function<void()>& runnable);

    /**
     * Assert that the context's IIDM-XML version equals or is more recent than a given IIDM-XML version if the value of an attribute or the state of an equipment
     * is not default (interpretable for previous versions).
     * If the value is not default and no exception has been thrown, run a given runnable.
     *
     * @param valueIsNotDefault assert only if this parameter is true
     * @param rootElementName the name of the XML root element
     * @param elementName the name of the XML element
     * @param errorMessage the type of error message in case of an exception is thrown
     * @param minVersion the IIDM-XML version in which the element has been added
     * @param context the context
     * @param runnable the function to run
     *
     * @throw a {@link PowsyblException} if the context's version is less recent than the minVersion and valueIsNotDefault is true
     */
    static void assertMinimumVersionAndRunIfNotDefault(bool valueIsNotDefault, const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& minVersion, const NetworkXmlWriterContext& context, const std::function<void()>& runnable);

    /**
     * Assert that the context's IIDM-XML version is strictly older than a given IIDM-XML version.
     *
     * @param rootElementName the name of the XML root element
     * @param elementName the name of the XML element
     * @param errorMessage the type of error message in case of an exception is thrown
     * @param maxVersion the IIDM-XML version in which the element has been removed
     * @param context the context
     *
     * @throw a {@link PowsyblException} if the context's version is more recent than or equal to the maxVersion
     */
    static void assertStrictMaximumVersion(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& maxVersion, const NetworkXmlReaderContext& context);

    /**
     * Assert that the context's IIDM-XML version is strictly older than a given IIDM-XML version.
     *
     * @param rootElementName the name of the XML root element
     * @param elementName the name of the XML element
     * @param errorMessage the type of error message in case of an exception is thrown
     * @param maxVersion the IIDM-XML version in which the element has been removed
     * @param context the context
     *
     * @throw a {@link PowsyblException} if the context's version is more recent than or equal to the maxVersion
     */
    static void assertStrictMaximumVersion(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& maxVersion, const NetworkXmlWriterContext& context);

    /**
     * Run a given runnable if the context's IIDM-XML version equals or is more recent than a given minimum IIDM-XML version
     *
     * @param minVersion the IIDM-XML version in which the element has been added
     * @param contextVersion the context's IIDM-XML version
     * @param runnable the function to run
     */
    static void runFromMinimumVersion(const IidmXmlVersion& minVersion, const IidmXmlVersion& contextVersion, const std::function<void()>& runnable);

    /**
     * Run a given runnable if the context's IIDM-XML version equals or is older than a given maximum IIDM-XML version
     *
     * @param maxVersion the IIDM-XML version in which the element has been removed
     * @param contextVersion the context's IIDM-XML version
     * @param runnable the function to run
     */
    static void runUntilMaximumVersion(const IidmXmlVersion& maxVersion, const IidmXmlVersion& contextVersion, const std::function<void()>& runnable);

    /**
     * Write a <b>mandatory</b> boolean attribute from a given minimum IIDM-XML version
     *
     * If the context's IIDM-XML version is strictly older than the given minimum IIDM-XML version, the attribute's value <b>should be default</b>
     * (else an exception is thrown).
     *
     * @param rootElementName the name of the XML root element
     * @param attributeName the name of the XML attribute
     * @param value the value of the attribute
     * @param defaultValue the default value for this attribute
     * @param errorMessage the type of error message in case of an exception is thrown
     * @param minVersion the IIDM-XML version in which the attribute has been added
     * @param context the current XML {@link NetworkXmlWriterContext context}
     *
     * @throw a {@link PowsyblException} if the context's version is less recent than the minVersion and the value is not the default one
     */
    static void writeBooleanAttributeFromMinimumVersion(const std::string& rootElementName, const std::string& attributeName, bool value, bool defaultValue, const char* errorMessage, const IidmXmlVersion& minVersion, NetworkXmlWriterContext& context);

    /**
     * Write a <b>mandatory</b> double attribute from a given minimum IIDM-XML version
     *
     * If the context's IIDM-XML version is strictly older than the given minimum IIDM-XML version, the attribute's value <b>should be undefined</b>
     * (else an exception is thrown).
     *
     * @param rootElementName the name of the XML root element
     * @param attributeName the name of the XML attribute
     * @param value the value of the attribute
     * @param errorMessage the type of error message in case of an exception is thrown
     * @param minVersion the IIDM-XML version in which the attribute has been added
     * @param context the current XML {@link NetworkXmlWriterContext context}
     *
     * @throw a {@link PowsyblException} if the context's version is less recent than the minVersion and the value is defined (not NaN)
     */
    static void writeDoubleAttributeFromMinimumVersion(const std::string& rootElementName, const std::string& attributeName, double value, const char* errorMessage, const IidmXmlVersion& minVersion, NetworkXmlWriterContext& context);

    /**
     * Write a <b>mandatory</b> double attribute from a given minimum IIDM-XML version
     *
     * If the context's IIDM-XML version is strictly older than the given minimum IIDM-XML version, the attribute's value <b>should be default</b>
     * (else an exception is thrown).
     *
     * @param rootElementName the name of the XML root element
     * @param attributeName the name of the XML attribute
     * @param value the value of the attribute
     * @param defaultValue the default value for this attribute
     * @param errorMessage the type of error message in case of an exception is thrown
     * @param minVersion the IIDM-XML version in which the attribute has been added
     * @param context the current XML {@link NetworkXmlWriterContext context}
     *
     * @throw a {@link PowsyblException} if the context's version is less recent than the minVersion and the value is not the default one
     */
    static void writeDoubleAttributeFromMinimumVersion(const std::string& rootElementName, const std::string& attributeName, double value, double defaultValue, const char* errorMessage, const IidmXmlVersion& minVersion, NetworkXmlWriterContext& context);

    /**
     * Write a <b>mandatory</b> unsigned long attribute until a given maximum IIDM-XML version.
     *
     * If the context's IIDM-XML version is strictly more recent than the given maximum IIDM-XML version, do nothing.
     *
     * @param attributeName the name of the XML attribute
     * @param value the value of the attribute
     * @param minVersion the IIDM-XML version in which the attribute has been removed
     * @param contextVersion the context's IIDM-XML version
     */
    static void writeULongAttributeUntilMaximumVersion(const std::string& attributeName, unsigned long value, const IidmXmlVersion& maxVersion, NetworkXmlWriterContext& context);

public:
    IidmXmlUtil() = delete;

private:
    static void writeAttributeFromMinimumVersion(const std::string& rootElementName, const std::string& attributeName, bool isNotDefaultValue, const char* errorMessage, const IidmXmlVersion& minVersion, const NetworkXmlWriterContext& context, const std::function<void()>& write);
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_IIDMXMLUTIL_HPP
