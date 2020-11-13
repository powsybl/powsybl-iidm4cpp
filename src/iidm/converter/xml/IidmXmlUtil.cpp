/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

std::string message(const std::string& elementName, const char* errorMessage, const IidmXmlVersion& version, const IidmXmlVersion& contextVersion, const std::string& reason) {
    return stdcxx::format("%1% is %2% for IIDM-XML version %3%. %4%%5%", elementName, errorMessage, contextVersion.toString("."), reason, version.toString("."));
}

PowsyblException createException(const std::string& elementName, const char* errorMessage, const IidmXmlVersion& version, const IidmXmlVersion& contextVersion, const std::string& reason) {
    return PowsyblException(message(elementName, errorMessage, version, contextVersion, reason));
}

PowsyblException createException(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& version, const IidmXmlVersion& contextVersion, const std::string& reason) {
    return createException(stdcxx::format("%1%.%2%", rootElementName, elementName), errorMessage, version, contextVersion, reason);
}

void createExceptionOrLogError(const std::string& elementName, const char* errorMessage, const IidmXmlVersion& refVersion, const std::string& reason, const NetworkXmlWriterContext& context) {
    static logging::Logger& S_LOGGER = logging::LoggerFactory::getLogger<IidmXmlUtil>();
    if (context.getOptions().getIidmVersionIncompatibilityBehavior() == ExportOptions::IidmVersionIncompatibilityBehavior::THROW_EXCEPTION) {
        throw createException(elementName, errorMessage, refVersion, context.getVersion(), reason);
    }
    if (context.getOptions().getIidmVersionIncompatibilityBehavior() == ExportOptions::IidmVersionIncompatibilityBehavior::LOG_ERROR) {
        S_LOGGER.error(message(elementName, errorMessage, refVersion, context.getVersion(), reason));
    } else {
        throw AssertionError(stdcxx::format("Unexpected behaviour: %1%", context.getOptions().getIidmVersionIncompatibilityBehavior()));
    }
}

void createExceptionOrLogError(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& refVersion, const std::string& reason, const NetworkXmlWriterContext& context) {
    createExceptionOrLogError(stdcxx::format("%1%.%2%", rootElementName, elementName), errorMessage, refVersion, reason, context);
}

void IidmXmlUtil::assertMaximumVersion(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& maxVersion, const NetworkXmlWriterContext& context) {
    if (context.getVersion() > maxVersion) {
        createExceptionOrLogError(rootElementName, elementName, errorMessage, maxVersion, MAXIMUM_REASON, context);
    }
}

void IidmXmlUtil::assertMinimumVersion(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& minVersion, const NetworkXmlReaderContext& context) {
    if (context.getVersion() < minVersion) {
        throw createException(rootElementName, elementName, errorMessage, minVersion, context.getVersion(), MINIMUM_REASON);
    }
}

void IidmXmlUtil::assertMinimumVersion(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& minVersion, const NetworkXmlWriterContext& context) {
    if (context.getVersion() < minVersion) {
        createExceptionOrLogError(rootElementName, elementName, errorMessage, minVersion, MINIMUM_REASON, context);
    }
}

void IidmXmlUtil::assertMinimumVersionIfNotDefault(bool valueIsNotDefault, const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& minVersion, const NetworkXmlWriterContext& context) {
    if (valueIsNotDefault) {
        assertMinimumVersion(rootElementName, elementName, errorMessage, minVersion, context);
    }
}

void IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(bool valueIsNotDefault, const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& minVersion, const NetworkXmlWriterContext& context, const std::function<void()>& runnable) {
    if (valueIsNotDefault) {
        assertMinimumVersion(rootElementName, elementName, errorMessage, minVersion, context);
        runnable();
    }
}

void IidmXmlUtil::assertStrictMaximumVersion(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& maxVersion, const NetworkXmlWriterContext& context) {
    if (context.getVersion() >= maxVersion) {
        createExceptionOrLogError(rootElementName, elementName, errorMessage, maxVersion, "IIDM-XML version should be < ", context);
    }
}

void IidmXmlUtil::runFromMinimumVersion(const IidmXmlVersion& minVersion, const IidmXmlVersion& contextVersion, const std::function<void()>& runnable) {
    if (contextVersion >= minVersion) {
        runnable();
    }
}

void IidmXmlUtil::runUntilMaximumVersion(const IidmXmlVersion& maxVersion, const IidmXmlVersion& contextVersion, const std::function<void()>& runnable) {
    if (contextVersion <= maxVersion) {
        runnable();
    }
}

void IidmXmlUtil::writeBooleanAttributeFromMinimumVersion(const std::string& rootElementName, const std::string& attributeName, bool value, bool defaultValue, const char* errorMessage, const IidmXmlVersion& minVersion, NetworkXmlWriterContext& context) {
    writeAttributeFromMinimumVersion(rootElementName, attributeName, value != defaultValue, errorMessage, minVersion, context, [&context, &attributeName, &value]() {
        context.getWriter().writeAttribute(attributeName, value);
    });
}

void IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(const std::string& rootElementName, const std::string& attributeName, double value, const char* errorMessage, const IidmXmlVersion& minVersion, NetworkXmlWriterContext& context) {
    writeDoubleAttributeFromMinimumVersion(rootElementName, attributeName, value, stdcxx::nan(), errorMessage, minVersion, context);
}

void IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(const std::string& rootElementName, const std::string& attributeName, double value, double defaultValue, const char* errorMessage, const IidmXmlVersion& minVersion, NetworkXmlWriterContext& context) {
    writeAttributeFromMinimumVersion(rootElementName, attributeName, !stdcxx::isEqual(value, defaultValue), errorMessage, minVersion, context, [&context, &attributeName, &value]() {
        context.getWriter().writeAttribute(attributeName, value);
    });
}

void IidmXmlUtil::writeAttributeFromMinimumVersion(const std::string& rootElementName, const std::string& attributeName, bool isNotDefaultValue, const char* errorMessage, const IidmXmlVersion& minVersion, const NetworkXmlWriterContext& context, const std::function<void()>& write) {
    if (context.getVersion() >= minVersion) {
        write();
    } else {
        assertMinimumVersionIfNotDefault(isNotDefaultValue, rootElementName, attributeName, errorMessage, minVersion, context);
    }
}

void IidmXmlUtil::writeULongAttributeUntilMaximumVersion(const std::string& attributeName, unsigned long value, const IidmXmlVersion& maxVersion, NetworkXmlWriterContext& context) {
    if (context.getVersion() <= maxVersion) {
        context.getWriter().writeAttribute(attributeName, value);
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
