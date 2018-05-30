/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ITERATORS_NETWORKITERATOR_HPP
#define POWSYBL_IIDM_ITERATORS_NETWORKITERATOR_HPP

namespace powsybl {

namespace iidm {

class ObjectStore;

/**
 * Forward iterator over the IdentifiableById map. Skip all the Identifiable which are not compatible with T.
 */
template <typename T, typename Iterator>
class NetworkIterator {
public:
    NetworkIterator() = default;

    NetworkIterator(const NetworkIterator&) = default;

    NetworkIterator(ObjectStore& objectStore);

    NetworkIterator(const ObjectStore& objectStore);

    NetworkIterator& operator=(const NetworkIterator&) = default;

    T& operator*() const;

    T& operator()() const;

    operator T&() const;

    NetworkIterator& operator++();

    NetworkIterator operator++(int i);

    bool operator==(const NetworkIterator& it) const;

    bool operator!=(const NetworkIterator& it) const;

private:
    void advance(bool increment);

    void check() const;

private:
    Iterator m_current;

    Iterator m_end;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ITERATORS_NETWORKITERATOR_HPP
