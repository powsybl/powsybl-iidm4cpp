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

class Network;
class VoltageLevel;

class NetworkIndex {
public:
    NetworkIndex() = default;

    NetworkIndex(const NetworkIndex&) = delete;

    NetworkIndex(NetworkIndex&&) noexcept = delete;

    NetworkIndex(Network& network, NetworkIndex&& networkIndex) noexcept;

    ~NetworkIndex() noexcept = default;

    NetworkIndex& operator=(const NetworkIndex&) = delete;

    NetworkIndex& operator=(NetworkIndex&&) noexcept = delete;

    bool addAlias(const Identifiable& obj, const std::string& alias);

    template <typename T>
    T& checkAndAdd(std::unique_ptr<T>&& identifiable);

    template <typename T>
    const T& get(const std::string& id) const;

    template <typename T>
    T& get(const std::string& id);

    template <typename T, typename U = T>
    stdcxx::const_range<U> getAll() const;

    template <typename T, typename U = T>
    stdcxx::range<U> getAll();

    template <typename T>
    unsigned long getObjectCount() const;

    template <typename T>
    stdcxx::CReference<T> find(const std::string& id) const;

    template <typename T>
    stdcxx::Reference<T> find(const std::string& id);

    void remove(Identifiable& identifiable);

    void removeAlias(const Identifiable& obj, const std::string& alias);

private:
    static void checkId(const std::string& id);

private:
    class Deleter {
    public:
        void operator()(Identifiable* ptr) const;
    };

    using IdentifiableById = std::map<std::string, std::unique_ptr<Identifiable, Deleter> >;

    using Identifiables = std::vector<std::reference_wrapper<Identifiable> >;

    using IdentifiablesByType = std::unordered_map<std::type_index, Identifiables>;

private:
    IdentifiableById m_objectsById;

    mutable IdentifiablesByType m_objectsByType;

    std::map<std::string, std::string> m_idByAlias;
};

template <>
const Identifiable& NetworkIndex::get(const std::string& id) const;

template <>
Identifiable& NetworkIndex::get(const std::string& id);

template <>
unsigned long NetworkIndex::getObjectCount<VoltageLevel>() const;

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/NetworkIndex.hxx>

#endif  // POWSYBL_IIDM_NETWORKINDEX_HPP
