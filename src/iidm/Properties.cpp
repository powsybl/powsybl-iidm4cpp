/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Properties.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

std::map<std::string, std::string>::iterator Properties::begin() {
    return m_properties.begin();
}

std::map<std::string, std::string>::const_iterator Properties::cbegin() const {
    return m_properties.cbegin();
}

std::map<std::string, std::string>::const_iterator Properties::cend() const {
    return m_properties.cend();
}

Properties& Properties::clear() {
    m_properties.clear();
    return *this;
}

std::map<std::string, std::string>::iterator Properties::end() {
    return m_properties.end();
}

const std::string& Properties::get(const std::string& key) const {
    const auto& it = m_properties.find(key);
    if (it == m_properties.end()) {
        throw PowsyblException(logging::format("Property %1% does not exist", key));
    }

    return it->second;
}

const std::string& Properties::get(const std::string& key, const std::string& defaultValue) const {
    const auto& it = m_properties.find(key);
    return (it != m_properties.end()) ? it->second : defaultValue;
}

bool Properties::contains(const std::string& key) const {
    return m_properties.find(key) != m_properties.end();
}

bool Properties::isEmpty() const {
    return m_properties.empty();
}

Properties& Properties::remove(const std::string& key) {
    m_properties.erase(key);
    return *this;
}

Properties& Properties::set(const std::string& key, const std::string& value) {
    const auto& res = m_properties.insert(std::make_pair(key, value));
    if (!res.second) {
        res.first->second = value;
    }
    return *this;
}

unsigned long Properties::size() const {
    return m_properties.size();
}

}  // namespace iidm

}  // namespace powsybl
