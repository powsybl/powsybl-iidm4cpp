/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/BusCache.hpp>

#include <powsybl/iidm/Bus.hpp>

namespace powsybl {

namespace iidm {

BusCache::BusCache(const BusSupplier& supplier) :
    m_supplier(supplier) {
}

stdcxx::CReference<Bus> BusCache::getBus(const std::string& id) const {
    const auto& cache = getCache();
    const auto& it = cache.find(id);
    return it != cache.end() ? stdcxx::cref(it->second) : stdcxx::cref<Bus>();
}

const std::map<std::string, std::reference_wrapper<const Bus>>& BusCache::getCache() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_cache) {
        m_cache = std::map<std::string, std::reference_wrapper<const Bus>>();
        for (const Bus& bus : m_supplier()) {
            (*m_cache).emplace(bus.getId(), std::cref(bus));
        }
    }
    return *m_cache;
}

void BusCache::invalidate() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cache.reset();
}

}  // namespace iidm

}  // namespace powsybl
