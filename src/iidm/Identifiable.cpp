/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Identifiable.hpp>

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
    if (getNetwork().getIndex().addAlias(*this, alias)) {
        m_aliases.emplace(alias);
    }
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

const std::set<std::string>& Identifiable::getAliases() const {
    return m_aliases;
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
    return !m_aliases.empty();
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
    m_aliases.erase(alias);
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
