/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OBJECTSTOREITERATOR_HPP
#define POWSYBL_IIDM_OBJECTSTOREITERATOR_HPP

namespace powsybl {

namespace iidm {

class ObjectStore;

template <typename T>
class ObjectStoreIterator {
public:
    ObjectStoreIterator(const ObjectStoreIterator& it) = default;

    T& operator*() const;

    T& operator()() const;

    operator T&() const;

    ObjectStoreIterator operator++();

    ObjectStoreIterator& operator++(int i);

    bool operator==(const ObjectStoreIterator& it) const;

    bool operator!=(const ObjectStoreIterator& it) const;

private:
    ObjectStoreIterator(const ObjectStore& objectStore, int index);

    friend class ObjectStore;

private:
    void check() const;

private:
    std::reference_wrapper<const ObjectStore> m_objectStore;

    int m_index;
};

}

}

#endif  // POWSYBL_IIDM_OBJECTSTOREITERATOR_HPP
