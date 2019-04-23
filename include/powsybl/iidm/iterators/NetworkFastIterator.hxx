/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ITERATORS_NETWORKFASTITERATOR_HXX
#define POWSYBL_IIDM_ITERATORS_NETWORKFASTITERATOR_HXX

#include <set>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

template <typename Base, typename Iterator, typename... Derived>
NetworkFastIterator<Base, Iterator, Derived...>::NetworkFastIterator(NetworkIndex& networkIndex) {

    std::set<std::type_index> typeList = { typeid(Base), typeid(Derived)... };
    for (const auto& type : typeList) {
        const auto& it = networkIndex.m_objectsByType.find(type);
        if (it != networkIndex.m_objectsByType.end()) {
            m_ranges.push_back(std::make_pair(it->second.begin(), it->second.end()));
        }
    }

    m_currentRange = m_ranges.begin();
    advanceToNextRange(false);
}

template <typename Base, typename Iterator, typename... Derived>
NetworkFastIterator<Base, Iterator, Derived...>::NetworkFastIterator(const NetworkIndex& networkIndex) {

    std::set<std::type_index> typeList = { typeid(Base), typeid(Derived)... };
    for (const auto& type : typeList) {
        const auto& it = networkIndex.m_objectsByType.find(type);
        if (it != networkIndex.m_objectsByType.end()) {
            m_ranges.push_back(std::make_pair(it->second.cbegin(), it->second.cend()));
        }
    }

    m_currentRange = m_ranges.begin();
    advanceToNextRange(false);
}

template <typename Base, typename Iterator, typename... Derived>
Base& NetworkFastIterator<Base, Iterator, Derived...>::operator*() const {
    return static_cast<Base&>(*this);
}

template <typename Base, typename Iterator, typename... Derived>
Base& NetworkFastIterator<Base, Iterator, Derived...>::operator()() const {
    return static_cast<Base&>(*this);
}

template <typename Base, typename Iterator, typename... Derived>
NetworkFastIterator<Base, Iterator, Derived...>::operator Base&() const {
    check();

    return static_cast<Base&>(m_current->get());
}

template <typename Base, typename Iterator, typename... Derived>
NetworkFastIterator<Base, Iterator, Derived...>& NetworkFastIterator<Base, Iterator, Derived...>::operator++() {
    check();

    advance(true);

    return *this;
}

template <typename Base, typename Iterator, typename... Derived>
const NetworkFastIterator<Base, Iterator, Derived...> NetworkFastIterator<Base, Iterator, Derived...>::operator++(int /*unused*/) {
    check();

    NetworkFastIterator<Base, Iterator, Derived...> tmp(*this);
    advance(true);

    return tmp;
}

template <typename Base, typename Iterator, typename... Derived>
bool NetworkFastIterator<Base, Iterator, Derived...>::operator==(const NetworkFastIterator<Base, Iterator, Derived...>& it) const {
    // Two iterators are equal if
    //   - current and end positions of this iterator are equal to the other iterator (same position of the same collection)
    //   - current and end positions of both iterators are equal (both are end iterators)
    return ((m_current == it.m_current) && (m_end == it.m_end)) ||
           ((m_current == m_end) && (m_end == it.m_end));
}

template <typename T, typename Iterator, typename... Derived>
bool NetworkFastIterator<T, Iterator, Derived...>::operator!=(const NetworkFastIterator<T, Iterator, Derived...>& it) const {
    return !(*this == it);
}

template <typename T, typename Iterator, typename... Derived>
void NetworkFastIterator<T, Iterator, Derived...>::advance(bool increment) {
    if (increment) {
        ++m_current;
    }

    // If the iterator reaches the end of this range, advance to the next one
    if (m_current == m_end) {
        advanceToNextRange(true);
    }
}

template <typename T, typename Iterator, typename... Derived>
void NetworkFastIterator<T, Iterator, Derived...>::advanceToNextRange(bool increment) {
    if (increment) {
        ++m_currentRange;
    }

    if (m_currentRange != m_ranges.cend()) {
        // Iterate over the next range
        m_current = m_currentRange->first;
        m_end = m_currentRange->second;
    } else {
        // Reset range iterators
        m_current = Iterator();
        m_end = Iterator();
    }
}

template <typename T, typename Iterator, typename... Derived>
void NetworkFastIterator<T, Iterator, Derived...>::check() const {
    if (m_current == m_end) {
        throw PowsyblException("Invalid iterator");
    }
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ITERATORS_NETWORKFASTITERATOR_HXX
