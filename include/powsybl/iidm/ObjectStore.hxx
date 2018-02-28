/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OBJECTSTORE_HXX
#define POWSYBL_IIDM_OBJECTSTORE_HXX

#include <powsybl/iidm/ObjectStore.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/logging/MessageFormat.hpp>
#include <powsybl/stdcxx/demangle.hpp>

namespace powsybl {

namespace iidm {

template <typename T>
ObjectStore::iterator<T> ObjectStore::begin() {
    return ObjectStore::iterator<T>(*this);
}

template <typename T>
ObjectStore::const_iterator<T> ObjectStore::cbegin() const {
    return typename ObjectStore::const_iterator<T>(*this);
}

template <typename T>
ObjectStore::const_iterator<T> ObjectStore::cend() const {
    return typename ObjectStore::const_iterator<T>();
}

template <typename T>
T& ObjectStore::checkAndAdd(std::unique_ptr<T>&& identifiable) {
    assert(identifiable);
    checkId(identifiable->getId());

    auto other = m_objectsById.find(identifiable->getId());
    if (other != m_objectsById.end()) {
        throw PowsyblException(logging::format("Object '%1%' already exists (%2%)", identifiable->getId(), stdcxx::demangle(*other->second)));
    }

    auto it = m_objectsById.emplace(std::make_pair(identifiable->getId(), std::move(identifiable)));

    std::reference_wrapper<Identifiable> refIdentifiable = dynamic_cast<Identifiable&>(*it.first->second.get());
    m_objectsByType[typeid(T)].emplace_back(std::move(refIdentifiable));

    return dynamic_cast<T&>(*it.first->second);
}

template <typename T>
ObjectStore::iterator<T> ObjectStore::end() {
    return ObjectStore::iterator<T>();
}

template<typename T>
T& ObjectStore::get(const std::string& id) const {
    checkId(id);

    const auto& it = m_objectsById.find(id);
    if (it == m_objectsById.end()) {
        throw PowsyblException(logging::format("Unable to find to the identifiable '%1%'", id));
    } else {
        T* identifiable = dynamic_cast<T*>(it->second.get());
        if (identifiable == nullptr) {
            throw PowsyblException(logging::format("Identifiable '%1%' is not a %2%", id, stdcxx::demangle<T>()));
        } else {
            return *identifiable;
        }
    }
}

template <typename T>
unsigned long ObjectStore::getObjectCount() const {
    const auto& it = m_objectsByType.find(typeid(T));
    if (it != m_objectsByType.end()) {
        return it->second.size();
    }
    return 0;
}

}

}

#endif  // POWSYBL_IIDM_OBJECTSTORE_HXX
