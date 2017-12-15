/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORKFASTITERATOR_HPP
#define POWSYBL_IIDM_NETWORKFASTITERATOR_HPP

#include <functional>
#include <vector>
#include <utility>

namespace powsybl {

namespace iidm {

class ObjectStore;

/**
 * Iterator over the map-by-type map.
 */
template <typename Base, typename Iterator, typename... Derived>
class NetworkFastIterator {
public:
    NetworkFastIterator() = default;

    NetworkFastIterator(const NetworkFastIterator& it) = default;

    NetworkFastIterator(ObjectStore& objectStore);

    NetworkFastIterator(const ObjectStore& objectStore);

    NetworkFastIterator& operator=(const NetworkFastIterator& it) = default;

    Base& operator*() const;

    Base& operator()() const;

    operator Base&() const;

    NetworkFastIterator operator++();

    NetworkFastIterator& operator++(int i);

    bool operator==(const NetworkFastIterator& it) const;

    bool operator!=(const NetworkFastIterator& it) const;

private:
    void advance(bool increment);

    void advanceToNextRange(bool increment);

    void check() const;

private:
    typedef std::vector<std::pair<Iterator, Iterator> > RangeList;

    RangeList m_ranges;

    typename RangeList::const_iterator m_currentRange;

    Iterator m_current;

    Iterator m_end;
};

}

}

#endif  // POWSYBL_IIDM_NETWORKFASTITERATOR_HPPs
