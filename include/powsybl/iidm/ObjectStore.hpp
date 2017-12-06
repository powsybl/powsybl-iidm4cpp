/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OBJECTSTORE_HPP
#define POWSYBL_IIDM_OBJECTSTORE_HPP

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/ObjectStoreIterator.hpp>

namespace powsybl {

namespace iidm {

class ObjectStore {
public:
    template <typename T> friend class ObjectStoreIterator;

    template <typename T> using iterator = ObjectStoreIterator<T>;

    template <typename T> using const_iterator = ObjectStoreIterator<const T>;;

    template <typename T> iterator<T> begin();

    template <typename T> const_iterator<T> cbegin() const;

    template <typename T> const_iterator<T> cend() const;

    template <typename T> iterator<T> end();

public:
    template <typename T> T& checkAndAdd(std::unique_ptr<T>&& identifiable);

    template <typename T> T& get(const std::string& id) const;

    template <typename T> unsigned int getObjectCount() const;

private:
    static void checkId(const std::string& id);

private:
    std::unordered_map<std::string, std::unique_ptr<Identifiable> > m_objectsById;

    std::unordered_map<std::string, std::vector<std::reference_wrapper<Identifiable> > > m_objectsByType;
};

}

}

#include <powsybl/iidm/ObjectStore.hxx>
#include <powsybl/iidm/ObjectStoreIterator.hxx>

#endif  // POWSYBL_IIDM_OBJECTSTORE_HPP
