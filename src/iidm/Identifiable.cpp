/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Identifiable.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace stdcxx {

template <>
std::string toString(const powsybl::iidm::Identifiable& value) {
    return value.getId();
}

}  // namespace stdcxx

namespace powsybl {

namespace iidm {

Identifiable::Identifiable(const std::string& id, const std::string& name) :
    m_id(checkNotEmpty(id, "Invalid id")),
    m_name(name) {
}

Identifiable::Identifiable(Identifiable&& identifiable) noexcept :
    Validable(std::move(identifiable)),
    Extendable(std::move(identifiable)),
    m_id(std::move(identifiable.m_id)),
    m_name(std::move(identifiable.m_name)),
    m_properties(std::move(identifiable.m_properties)) {
}

const std::string& Identifiable::getId() const {
    return m_id;
}

std::string Identifiable::getMessageHeader() const {
    return getTypeDescription() + " '" + m_id + "': ";
}

const std::string& Identifiable::getName() const {
    return m_name.empty() ? m_id : m_name;
}

const std::string& Identifiable::getProperty(const std::string& key) const {
    return m_properties.get(key);
}

const std::string& Identifiable::getProperty(const std::string& key, const std::string& defaultValue) const {
    return m_properties.get(key, defaultValue);
}

stdcxx::const_range<std::string> Identifiable::getPropertyNames() const {
    return m_properties.getKeys();
}

bool Identifiable::hasProperty() const {
    return !m_properties.isEmpty();
}

bool Identifiable::hasProperty(const std::string& key) const {
    return m_properties.contains(key);
}

stdcxx::optional<std::string> Identifiable::setProperty(const std::string& key, const std::string& value) {
    return m_properties.set(key, value);
}

std::ostream& operator<<(std::ostream& stream, const Identifiable& identifiable) {
    stream << stdcxx::toString(identifiable);

    return stream;
}

}  // namespace iidm

}  // namespace powsybl
