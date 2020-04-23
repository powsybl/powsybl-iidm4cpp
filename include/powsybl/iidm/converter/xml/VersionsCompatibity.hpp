/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_VERSIONSCOMPATIBITY_HPP
#define POWSYBL_IIDM_CONVERTER_XML_VERSIONSCOMPATIBITY_HPP

#include <map>
#include <string>
#include <vector>

#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

using VersionsCompatibility = std::map<std::string, std::vector<std::string> >;

/**
 * A builder to ease the creation of a versions compatibility map.
 */
class VersionsCompatibilityBuilder {
public:
    /**
     * Create a builder for {@link VersionsCompatibility}
     */
    VersionsCompatibilityBuilder() = default;

    VersionsCompatibilityBuilder(const VersionsCompatibilityBuilder&) = delete;

    VersionsCompatibilityBuilder(VersionsCompatibilityBuilder&&) noexcept = delete;

    /**
     * Destructor
     */
    ~VersionsCompatibilityBuilder() = default;

    VersionsCompatibilityBuilder& operator=(const VersionsCompatibilityBuilder&) = delete;

    VersionsCompatibilityBuilder& operator=(VersionsCompatibilityBuilder&&) noexcept = delete;

    VersionsCompatibility build();

    VersionsCompatibilityBuilder& put(const IidmXmlVersion& iidmVersion, std::initializer_list<std::string> extensionVersions);

    VersionsCompatibilityBuilder& put(std::string&& iidmVersion, std::initializer_list<std::string> extensionVersions);

private:
    VersionsCompatibility m_versionsCompatibility;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_VERSIONSCOMPATIBITY_HPP
