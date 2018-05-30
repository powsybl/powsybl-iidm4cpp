/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ITERATORS_NETWORKITERATOR_HXX
#define POWSYBL_IIDM_ITERATORS_NETWORKITERATOR_HXX

#include <memory>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/ObjectStore.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename Iterator>
NetworkIterator<T, Iterator>::NetworkIterator(ObjectStore& objectStore) :
    m_current(objectStore.m_objectsById.begin()),
    m_end(objectStore.m_objectsById.end()) {

    advance(false);
}

template <typename T, typename Iterator>
NetworkIterator<T, Iterator>::NetworkIterator(const ObjectStore& objectStore) :
    m_current(objectStore.m_objectsById.cbegin()),
    m_end(objectStore.m_objectsById.cend()) {

    advance(false);
}

template <typename T, typename Iterator>
T& NetworkIterator<T, Iterator>::operator*() const {
    return static_cast<T&>(*this);
}

template <typename T, typename Iterator>
T& NetworkIterator<T, Iterator>::operator()() const {
    return static_cast<T&>(*this);
}

template <typename T, typename Iterator>
NetworkIterator<T, Iterator>::operator T&() const {
    check();

    return dynamic_cast<T&>(*m_current->second.get());
}

template <typename T, typename Iterator>
NetworkIterator<T, Iterator>& NetworkIterator<T, Iterator>::operator++() {
    check();

    advance(true);

    return *this;
}

template <typename T, typename Iterator>
NetworkIterator<T, Iterator> NetworkIterator<T, Iterator>::operator++(int /*unused*/) {
    check();

    NetworkIterator<T, Iterator> tmp(*this);
    advance(true);

    return tmp;
}

template <typename T, typename Iterator>
bool NetworkIterator<T, Iterator>::operator==(const NetworkIterator<T, Iterator>& it) const {
    // Two iterators are equal if
    //   - current and end positions of this iterator are equal to the other iterator (same position of the same collection)
    //   - current and end positions of both iterators are equal (both are end iterators)
    return ((m_current == it.m_current) && (m_end == it.m_end)) ||
        ((m_current == m_end) && (it.m_current == it.m_end));
}

template <typename T, typename Iterator>
bool NetworkIterator<T, Iterator>::operator!=(const NetworkIterator<T, Iterator>& it) const {
    return !(*this == it);
}

template <typename T, typename Iterator>
void NetworkIterator<T, Iterator>::advance(bool increment) {
    if (increment) {
        ++m_current;
    }

    while ((m_current != m_end) && (!stdcxx::isInstanceOf<T>(m_current->second))) {
        ++m_current;
    }
}

template <typename T, typename Iterator>
void NetworkIterator<T, Iterator>::check() const {
    if (m_current == m_end) {
        throw PowsyblException("Invalid iterators");
    }
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ITERATORS_NETWORKITERATOR_HXX
