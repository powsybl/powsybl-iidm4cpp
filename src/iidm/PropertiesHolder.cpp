/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/PropertiesHolder.hpp>

namespace powsybl {

namespace iidm {

bool PropertiesHolder::hasProperty() const {
    return !m_properties.isEmpty();
}

bool PropertiesHolder::hasProperty(const std::string& key) const {
    return m_properties.contains(key);
}

const std::string& PropertiesHolder::getProperty(const std::string& key) const {
    return m_properties.get(key);
}

const std::string& PropertiesHolder::getProperty(const std::string& key, const std::string& defaultValue) const {
    return m_properties.get(key, defaultValue);
}

stdcxx::optional<std::string> PropertiesHolder::setProperty(const std::string& key, const std::string& value) {
    return m_properties.set(key, value);
}

bool PropertiesHolder::removeProperty(const std::string& key) {
    return static_cast<bool>(m_properties.remove(key));
}

stdcxx::const_range<std::string> PropertiesHolder::getPropertyNames() const {
    return m_properties.getKeys();
}

}  // namespace iidm

}  // namespace powsybl
