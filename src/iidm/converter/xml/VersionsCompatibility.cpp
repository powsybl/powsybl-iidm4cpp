/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/VersionsCompatibity.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

VersionsCompatibility VersionsCompatibilityBuilder::build() {
    return m_versionsCompatibility;
}

VersionsCompatibilityBuilder& VersionsCompatibilityBuilder::put(const IidmXmlVersion& iidmVersion, std::initializer_list<std::string> extensionVersions) {
    return put(iidmVersion.toString("."), extensionVersions);
}

VersionsCompatibilityBuilder& VersionsCompatibilityBuilder::put(std::string iidmVersion, std::initializer_list<std::string> extensionVersions) {
    m_versionsCompatibility.emplace(std::make_pair(std::move(iidmVersion), extensionVersions));

    return *this;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
