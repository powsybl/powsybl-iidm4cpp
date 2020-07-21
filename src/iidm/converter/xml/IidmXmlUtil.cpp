/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

PowsyblException createException(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& version, const IidmXmlVersion& contextVersion, const std::string& reason) {
    return PowsyblException(stdcxx::format("%1%.%2% is %3% for IIDM-XML version %4%. %5%%6%", rootElementName, elementName, errorMessage, contextVersion.toString("."), reason, version.toString(".")));
}

void IidmXmlUtil::assertMaximumVersion(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& maxVersion, const IidmXmlVersion& contextVersion) {
    if (contextVersion > maxVersion) {
        throw createException(rootElementName, elementName, errorMessage, maxVersion, contextVersion, "IIDM-XML version should be <= ");
    }
}

void IidmXmlUtil::assertMinimumVersion(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& minVersion, const IidmXmlVersion& contextVersion) {
    if (contextVersion < minVersion) {
        throw createException(rootElementName, elementName, errorMessage, minVersion, contextVersion, "IIDM-XML version should be >= ");
    }
}

void IidmXmlUtil::assertMinimumVersionIfNotDefault(bool valueIsNotDefault, const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& minVersion, const IidmXmlVersion& contextVersion) {
    if (valueIsNotDefault) {
        assertMinimumVersion(rootElementName, elementName, errorMessage, minVersion, contextVersion);
    }
}

void IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(bool valueIsNotDefault, const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& minVersion, const IidmXmlVersion& contextVersion, const std::function<void()>& runnable) {
    if (valueIsNotDefault) {
        assertMinimumVersion(rootElementName, elementName, errorMessage, minVersion, contextVersion);
        runnable();
    }
}

void IidmXmlUtil::assertStrictMaximumVersion(const std::string& rootElementName, const std::string& elementName, const char* errorMessage, const IidmXmlVersion& maxVersion, const IidmXmlVersion& contextVersion) {
    if (contextVersion >= maxVersion) {
        throw createException(rootElementName, elementName, errorMessage, maxVersion, contextVersion, "IIDM-XML version should be < ");
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
    writeAttributeFromMinimumVersion(rootElementName, attributeName, value != defaultValue, errorMessage, minVersion, context.getVersion(), [&context, &attributeName, &value]() {
        context.getWriter().writeAttribute(attributeName, value);
    });
}

void IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(const std::string& rootElementName, const std::string& attributeName, double value, const char* errorMessage, const IidmXmlVersion& minVersion, NetworkXmlWriterContext& context) {
    writeDoubleAttributeFromMinimumVersion(rootElementName, attributeName, value, stdcxx::nan(), errorMessage, minVersion, context);
}

void IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(const std::string& rootElementName, const std::string& attributeName, double value, double defaultValue, const char* errorMessage, const IidmXmlVersion& minVersion, NetworkXmlWriterContext& context) {
    writeAttributeFromMinimumVersion(rootElementName, attributeName, !stdcxx::isEqual(value, defaultValue), errorMessage, minVersion, context.getVersion(), [&context, &attributeName, &value]() {
        context.getWriter().writeAttribute(attributeName, value);
    });
}

void IidmXmlUtil::writeAttributeFromMinimumVersion(const std::string& rootElementName, const std::string& attributeName, bool isNotDefaultValue, const char* errorMessage, const IidmXmlVersion& minVersion, const IidmXmlVersion& contextVersion, const std::function<void()>& write) {
    if (contextVersion >= minVersion) {
        write();
    } else {
        assertMinimumVersionIfNotDefault(isNotDefaultValue, rootElementName, attributeName, errorMessage, minVersion, contextVersion);
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
