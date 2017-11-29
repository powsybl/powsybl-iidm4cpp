/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OBJECTSTOREITERATOR_HXX
#define POWSYBL_IIDM_OBJECTSTOREITERATOR_HXX

namespace powsybl {

namespace iidm {

template <typename T>
ObjectStoreIterator<T>::ObjectStoreIterator(const ObjectStore& objectStore, int index) :
    m_objectStore(objectStore),
    m_index(index) {
}

template <typename T>
T& ObjectStoreIterator<T>::operator*() const {
    return static_cast<T&>(*this);
}

template <typename T>
T& ObjectStoreIterator<T>::operator()() const {
    return static_cast<T&>(*this);
}

template <typename T>
ObjectStoreIterator<T>::operator T&() const {
    check();

    const auto& it = m_objectStore.get().m_objectsByType.find(typeid(T).name());
    const std::reference_wrapper<Identifiable> identifiable = it->second.at(m_index);

    return dynamic_cast<T&>(identifiable.get());
}

template <typename T>
ObjectStoreIterator<T> ObjectStoreIterator<T>::operator++() {
    check();

    ObjectStore::iterator<T> tmp(*this);
    m_index++;

    return tmp;
}

template <typename T>
ObjectStoreIterator<T>& ObjectStoreIterator<T>::operator++(int i) {
    check();

    m_index++;

    return *this;
}

template <typename T>
bool ObjectStoreIterator<T>::operator==(const ObjectStoreIterator<T>& it) const {
    return (&m_objectStore.get() == &it.m_objectStore.get()) && (m_index == it.m_index);
}

template <typename T>
bool ObjectStoreIterator<T>::operator!=(const ObjectStoreIterator<T>& it) const {
    return !(*this == it);
}

template <typename T>
void ObjectStoreIterator<T>::check() const {
    if (m_index == -1) {
        throw PowsyblException("Invalid iterator");
    }

    const auto& it = m_objectStore.get().m_objectsByType.find(typeid(T).name());
    if (it == m_objectStore.get().m_objectsByType.end()) {
        std::ostringstream oss;
        oss << "Invalid iterator: unable to find " << typeid(T).name() << " list";
        throw PowsyblException(oss.str());
    }

    if (m_index >= it->second.size()) {
        std::ostringstream oss;
        oss << "Invalid iterator: index (" << m_index << ") out of range [0, " << it->second.size() << "[";
        throw PowsyblException(oss.str());
    }
}


}

}

#endif  // POWSYBL_IIDM_OBJECTSTOREITERATOR_HXX
