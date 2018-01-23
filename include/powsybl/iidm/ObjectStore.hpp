/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OBJECTSTORE_HPP
#define POWSYBL_IIDM_OBJECTSTORE_HPP

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/bits/ObjectStore.hpp>
#include <powsybl/iidm/iterators/Iterators.hpp>

namespace powsybl {

namespace iidm {

class ObjectStore {
public:
    ObjectStore() = default;

    ObjectStore(ObjectStore&& objectStore) = default;

    template <typename T> using const_iterator = typename iterator_traits<T>::const_iterator;

    template <typename T> using iterator = typename iterator_traits<T>::iterator;

    template <typename T> iterator<T> begin();

    template <typename T> const_iterator<T> cbegin() const;

    template <typename T> const_iterator<T> cend() const;

    template <typename T> T& checkAndAdd(std::unique_ptr<T>&& identifiable);

    template <typename T> iterator<T> end();

    template <typename T> T& get(const std::string& id) const;

    template <typename T> unsigned long getObjectCount() const;

    void remove(Identifiable& identifiable);

private:
    static void checkId(const std::string& id);

private: // Non copyable
    ObjectStore(const ObjectStore& objectStore) = delete;

    ObjectStore& operator=(const ObjectStore& objectStore) = delete;

private:
    IdentifiableById m_objectsById;

    IdentifiablesByType m_objectsByType;

private:
    template <typename T, typename Iterator> friend class NetworkIterator;

    template <typename Base, typename Iterator, typename... Derived> friend class NetworkFastIterator;
};

}

}

#include <powsybl/iidm/ObjectStore.hxx>
#include <powsybl/iidm/iterators/Iterators.hxx>

#endif  // POWSYBL_IIDM_OBJECTSTORE_HPP
