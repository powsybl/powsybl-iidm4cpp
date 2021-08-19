/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSCACHE_HPP
#define POWSYBL_IIDM_BUSCACHE_HPP

#include <functional>
#include <map>
#include <mutex>
#include <string>

#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Bus;

class BusCache {
public:
    using BusSupplier = std::function<stdcxx::const_range<Bus>()>;

public:
    explicit BusCache(const BusSupplier& supplier);

    BusCache(const BusCache&) = delete;

    BusCache(BusCache&&) = delete;

    ~BusCache() = default;

    BusCache& operator=(const BusCache&) = delete;

    BusCache& operator=(BusCache&&) = delete;

    stdcxx::CReference<Bus> getBus(const std::string& id) const;

    std::map<std::string, std::reference_wrapper<const Bus>> getCache() const;

    void invalidate();

private:
    BusSupplier m_supplier;

    mutable stdcxx::optional<std::map<std::string, std::reference_wrapper<const Bus>>> m_cache;

    mutable std::mutex m_mutex;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSCACHE_HPP
