/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORKINDEX_HPP
#define POWSYBL_IIDM_NETWORKINDEX_HPP

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/bits/NetworkIndex.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class NetworkIndex {
public:
    template <typename T, typename U = T>
    using range = typename network_index::range_traits<T, U>::range;

    template <typename T, typename U = T>
    using const_range = typename network_index::range_traits<T, U>::const_range;

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
    const_range<T, U> getAll() const;

    template <typename T, typename U = T>
    range<T, U> getAll();

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
    network_index::IdentifiableById m_objectsById;

    mutable network_index::IdentifiablesByType m_objectsByType;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/NetworkIndex.hxx>

#endif  // POWSYBL_IIDM_NETWORKINDEX_HPP
