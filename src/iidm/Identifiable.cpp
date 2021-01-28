/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Identifiable.hpp>

#include <boost/range/adaptor/map.hpp>
#include <boost/range/join.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace stdcxx {

template <>
std::string toString(const powsybl::iidm::Identifiable& value) {
    return value.getId();
}

}  // namespace stdcxx

namespace powsybl {

namespace iidm {

Identifiable::Identifiable(const std::string& id, const std::string& name, bool fictitious) :
    m_id(checkNotEmpty(id, "Invalid id")),
    m_name(name),
    m_fictitious(fictitious) {
}

void Identifiable::addAlias(const std::string& alias) {
    addAlias(alias, false);
}

void Identifiable::addAlias(const std::string& alias, bool ensureAliasUnicity) {
    addAlias(alias, "", ensureAliasUnicity);
}

void Identifiable::addAlias(const std::string& alias, const std::string& aliasType) {
    addAlias(alias, aliasType, false);
}

void Identifiable::addAlias(const std::string& alias, const char* aliasType) {
    addAlias(alias, std::string(aliasType), false);
}

void Identifiable::addAlias(const std::string& alias, const std::string& aliasType, bool ensureAliasUnicity) {
    std::string uniqueAlias = alias;
    if (ensureAliasUnicity) {
        uniqueAlias = util::Identifiables::getUniqueId(alias, [this](const std::string& alias) {
            return static_cast<bool>(getNetwork().find(alias));
        });
    }
    if (!aliasType.empty() && m_aliasesByType.find(aliasType) != m_aliasesByType.end()) {
        throw PowsyblException(stdcxx::format("%1% already has an alias of type %2%", m_id, aliasType));
    }
    if (getNetwork().getIndex().addAlias(*this, uniqueAlias)) {
        if (!aliasType.empty()) {
            m_aliasesByType[aliasType] = uniqueAlias;
        } else {
            m_aliasesWithoutType.emplace(uniqueAlias);
        }
    }
}

void Identifiable::addAlias(const std::string& alias, const char* aliasType, bool ensureAliasUnicity) {
    addAlias(alias, std::string(aliasType), ensureAliasUnicity);
}

void Identifiable::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (Extension& extension : getExtensions()) {
        if (stdcxx::isInstanceOf<MultiVariantObject>(extension)) {
            dynamic_cast<MultiVariantObject&>(extension).allocateVariantArrayElement(indexes, sourceIndex);
        }
    }
}

void Identifiable::deleteVariantArrayElement(unsigned long index) {
    for (Extension& extension : getExtensions()) {
        if (stdcxx::isInstanceOf<MultiVariantObject>(extension)) {
            dynamic_cast<MultiVariantObject&>(extension).deleteVariantArrayElement(index);
        }
    }
}

void Identifiable::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    for (Extension& extension : getExtensions()) {
        if (stdcxx::isInstanceOf<MultiVariantObject>(extension)) {
            dynamic_cast<MultiVariantObject&>(extension).extendVariantArraySize(initVariantArraySize, number, sourceIndex);
        }
    }
}

stdcxx::const_range<std::string> Identifiable::getAliases() const {
    return boost::range::join(m_aliasesWithoutType, m_aliasesByType | boost::adaptors::map_values);
}

stdcxx::optional<std::string> Identifiable::getAliasFromType(const std::string& aliasType) const {
    auto it = m_aliasesByType.find(aliasType);
    return it != m_aliasesByType.end() ? it->second : stdcxx::optional<std::string>();
}

std::string Identifiable::getAliasType(const std::string& alias) const {
    if (m_aliasesWithoutType.find(alias) != m_aliasesWithoutType.end()) {
        return "";
    }
    auto it = std::find_if(m_aliasesByType.begin(), m_aliasesByType.end(), [&alias](const std::pair<std::string, std::string>& entry) {
        return entry.second == alias;
    });
    return it != m_aliasesByType.end() ? it->first : "";
}

const std::string& Identifiable::getId() const {
    return m_id;
}

std::string Identifiable::getMessageHeader() const {
    return getTypeDescription() + " '" + m_id + "': ";
}

const std::string& Identifiable::getNameOrId() const {
    return m_name.empty() ? m_id : m_name;
}

const std::string& Identifiable::getOptionalName() const {
    return m_name;
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

bool Identifiable::hasAliases() const {
    return !m_aliasesWithoutType.empty() || !m_aliasesByType.empty();
}

bool Identifiable::hasProperty() const {
    return !m_properties.isEmpty();
}

bool Identifiable::hasProperty(const std::string& key) const {
    return m_properties.contains(key);
}

bool Identifiable::isFictitious() const {
    return m_fictitious;
}

void Identifiable::reduceVariantArraySize(unsigned long number) {
    for (Extension& extension : getExtensions()) {
        if (stdcxx::isInstanceOf<MultiVariantObject>(extension)) {
            dynamic_cast<MultiVariantObject&>(extension).reduceVariantArraySize(number);
        }
    }
}

void Identifiable::removeAlias(const std::string& alias) {
    getNetwork().getIndex().removeAlias(*this, alias);
    auto it = std::find_if(m_aliasesByType.begin(), m_aliasesByType.end(), [&alias](const std::pair<std::string, std::string>& entry) {
        return entry.second == alias;
    });
    if (it != m_aliasesByType.end()) {
        m_aliasesByType.erase(it);
    } else {
        m_aliasesWithoutType.erase(alias);
    }
}

void Identifiable::setFictitious(bool fictitious) {
    m_fictitious = fictitious;
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
