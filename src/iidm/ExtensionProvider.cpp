/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ExtensionProvider.hpp>

namespace powsybl {

namespace iidm {

ExtensionProvider::ExtensionProvider(std::string&& extensionName, std::string&& categoryName) :
    m_extensionName(extensionName),
    m_categoryName(categoryName) {

}

const std::string& ExtensionProvider::getCategoryName() const {
    return m_categoryName;
}

const std::string& ExtensionProvider::getExtensionName() const {
    return m_extensionName;
}

}  // namespace iidm

}  // namespace powsybl
