/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORKINDEX_HPP
#define POWSYBL_IIDM_NETWORKINDEX_HPP

#include <typeindex>
#include <unordered_map>
#include <vector>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class NetworkIndex {
public:
    template <typename T>
    using const_range = stdcxx::range<const T&>;

    template <typename T>
    using range = stdcxx::range<T&>;

public:
    NetworkIndex() = default;

    NetworkIndex(const NetworkIndex& networkIndex) = delete;

    NetworkIndex(NetworkIndex&&) = default;

    ~NetworkIndex() noexcept = default;

    NetworkIndex& operator=(const NetworkIndex& networkIndex) = delete;

    template <typename T>
    T& checkAndAdd(std::unique_ptr<T>&& identifiable);

    template <typename T>
    const T& get(const std::string& id) const;

    template <typename T>
    T& get(const std::string& id);

    template <typename T, typename U = T>
    const_range<U> getAll() const;

    template <typename T, typename U = T>
    range<U> getAll();

    template <typename T>
    unsigned long getObjectCount() const;

    template <typename T>
    stdcxx::CReference<T> find(const std::string& id) const;

    template <typename T>
    stdcxx::Reference<T> find(const std::string& id);

    void remove(Identifiable& identifiable);

private:
    static void checkId(const std::string& id);

private:
    using IdentifiableById = std::unordered_map<std::string, std::unique_ptr<Identifiable> >;

    using Identifiables = std::vector<std::reference_wrapper<Identifiable> >;

    using IdentifiablesByType = std::unordered_map<std::type_index, Identifiables>;

private:
    IdentifiableById m_objectsById;

    mutable IdentifiablesByType m_objectsByType;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/NetworkIndex.hxx>

#endif  // POWSYBL_IIDM_NETWORKINDEX_HPP
