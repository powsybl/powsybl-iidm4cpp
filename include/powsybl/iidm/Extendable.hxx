/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENDABLE_HXX
#define POWSYBL_IIDM_EXTENDABLE_HXX

#include <powsybl/iidm/Extendable.hpp>

#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

template <typename E, typename>
stdcxx::CReference<E> Extendable::findExtension() const {
    stdcxx::CReference<E> extension;

    const auto& it = m_extensionsByType.find(typeid(E));
    if (it != m_extensionsByType.end()) {
        extension = stdcxx::cref(dynamic_cast<const E&>(it->second.get()));
    }

    return extension;
}

template <typename E, typename>
stdcxx::Reference<E> Extendable::findExtension() {
    stdcxx::Reference<E> extension;

    const auto& it = m_extensionsByType.find(typeid(E));
    if (it != m_extensionsByType.end()) {
        extension = stdcxx::ref(dynamic_cast<E&>(it->second.get()));
    }

    return extension;
}

template <typename E, typename>
const E& Extendable::getExtension() const {
    const auto& it = m_extensionsByType.find(typeid(E));
    if (it == m_extensionsByType.end()) {
        throw PowsyblException(stdcxx::format("Extension %1% not found", stdcxx::demangle<E>()));
    }
    return dynamic_cast<E&>(it->second.get());
}

template <typename E, typename>
E& Extendable::getExtension() {
    return const_cast<E&>(static_cast<const Extendable*>(this)->getExtension<E>());
}

template <typename E, typename>
stdcxx::CReference<E> Extendable::getExtensionByName(const std::string& name) const {
    auto it = m_extensionsByName.find(name);
    if (it != m_extensionsByName.end() && stdcxx::isInstanceOf<E>(*it->second)) {
        return stdcxx::cref<E>(dynamic_cast<const E&>(*it->second));
    }
    return stdcxx::cref<E>();
}

template <typename E, typename>
stdcxx::Reference<E> Extendable::getExtensionByName(const std::string& name) {
    return stdcxx::ref(const_cast<const Extendable*>(this)->getExtensionByName<E>(name));
}

template <typename Adder, typename>
Adder Extendable::newExtension() {
    return Adder(*this);
}

template <typename E, typename>
void Extendable::removeExtension() {
    const auto& it = m_extensionsByType.find(typeid(E));
    if (it != m_extensionsByType.end()) {
        m_extensionsByName.erase(it->second.get().getName());
        m_extensionsByType.erase(it);
    }
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENDABLE_HXX
