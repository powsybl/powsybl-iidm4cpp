/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OBJECTSTORE_HXX
#define POWSYBL_IIDM_OBJECTSTORE_HXX

#include <powsybl/iidm/ObjectStore.hpp>

#include <iostream>
#include <sstream>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

template <typename T>
ObjectStore::iterator<T> ObjectStore::begin() {
    return ObjectStore::iterator<T>(*this, 0);
}

template <typename T>
ObjectStore::const_iterator<T> ObjectStore::cbegin() const {
    return ObjectStore::const_iterator<T>(*this, 0);
}

template <typename T>
ObjectStore::iterator<const T> ObjectStore::cend() const {
    const auto& it = m_objectsByType.find(typeid(T).name());

    int index = (it == m_objectsByType.end()) ? 0 : it->second.size();

    return ObjectStore::const_iterator<T>(*this, index);
}

template <typename T>
ObjectStore::iterator<T> ObjectStore::end() {
    const auto& it = m_objectsByType.find(typeid(T).name());

    int index = (it == m_objectsByType.end()) ? 0 : it->second.size();

    return ObjectStore::iterator<T>(*this, index);
}

template <typename T>
T& ObjectStore::checkAndAdd(std::unique_ptr<T>&& identifiable) {
    assert(identifiable);

    checkId(identifiable->getId());
    if (m_objectsById.find(identifiable->getId()) != m_objectsById.end()) {
        throw PowsyblException(std::string("Object (") + typeid(T).name() + ") '" + identifiable->getId() + "' already exists");
    }

    auto it = m_objectsById.emplace(std::make_pair(identifiable->getId(), std::move(identifiable)));

    std::reference_wrapper<Identifiable> refIdentifiable = dynamic_cast<Identifiable&>(*it.first->second.get());
    m_objectsByType[typeid(T).name()].emplace_back(std::move(refIdentifiable));

    return dynamic_cast<T&>(*it.first->second);
}

template<typename T>
T& ObjectStore::get(const std::string& id) const {
    checkId(id);

    const auto& it = m_objectsById.find(id);
    if (it == m_objectsById.end()) {
        std::ostringstream oss;
        oss << "Unable to find to identifiable '" + id + "'";
        throw PowsyblException(oss.str());
    } else {
        T* identifiable = dynamic_cast<T*>(it->second.get());
        if (identifiable == nullptr) {
            std::ostringstream oss;
            oss << "Identifiable '" << id << "' is not a " << typeid(T).name();
            throw PowsyblException(oss.str());
        } else {
            return *identifiable;
        }
    }
}

template <typename T>
unsigned int ObjectStore::getObjectCount() const {
    const auto& it = m_objectsByType.find(typeid(T).name());
    if (it != m_objectsByType.end()) {
        return it->second.size();
    }
    return 0;
}

}

}

#endif  // POWSYBL_IIDM_OBJECTSTORE_HXX
