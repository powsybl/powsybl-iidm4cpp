/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/ImportOptions.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

ImportOptions& ImportOptions::addExtension(const std::string& extension) {
    m_extensions.insert(extension);
    return *this;
}

bool ImportOptions::isThrowExceptionIfExtensionNotFound() const {
    return m_throwExceptionIfExtensionNotFound;
}

ImportOptions& ImportOptions::setExtensions(const std::set<std::string>& extensions) {
    m_extensions = extensions;
    return *this;
}

ImportOptions& ImportOptions::setThrowExceptionIfExtensionNotFound(bool throwExceptionIfExtensionNotFound) {
    m_throwExceptionIfExtensionNotFound = throwExceptionIfExtensionNotFound;
    return *this;
}

bool ImportOptions::withExtension(const std::string& extension) const {
    return m_extensions.empty() || m_extensions.find(extension) != m_extensions.end();
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
