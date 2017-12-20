/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OBJECTSTORE_HXX
#define POWSYBL_IIDM_OBJECTSTORE_HXX

#include <powsybl/iidm/ObjectStore.hpp>

#include <sstream>

#include <powsybl/PowsyblException.hpp>

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
        throw PowsyblException(std::string("Object '") + identifiable->getId() + "' already exists (" + other->second->getTypeDescription() + ")");
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
