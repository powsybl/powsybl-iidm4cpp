/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusBreakerVoltageLevelBusCache.hpp"

#include <cassert>

#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/map.hpp>

namespace powsybl {

namespace iidm {

namespace bus_breaker_voltage_level {

BusCache::BusCache(MergedBusById mergedBus, MergedBusByConfiguredBus mapping) :
    m_mergedBus(std::move(mergedBus)),
    m_mapping(std::move(mapping)) {
}

stdcxx::Reference<MergedBus> BusCache::getMergedBus(const std::string& id) const {
    const auto& it = m_mergedBus.find(id);

    return (it == m_mergedBus.end()) ? stdcxx::ref<MergedBus>() : stdcxx::ref<MergedBus>(it->second);
}

stdcxx::Reference<MergedBus> BusCache::getMergedBus(const stdcxx::Reference<ConfiguredBus>& bus) const {
    assert(bus);

    const auto& it = m_mapping.find(std::ref(bus.get()));

    return (it == m_mapping.end()) ? stdcxx::ref<MergedBus>() : stdcxx::ref<MergedBus>(it->second);
}

stdcxx::const_range<MergedBus> BusCache::getMergedBuses() const {
    return boost::adaptors::values(m_mergedBus) | boost::adaptors::indirected;
}

stdcxx::range<MergedBus> BusCache::getMergedBuses() {
    return boost::adaptors::values(m_mergedBus) | boost::adaptors::indirected;
}

}  // namespace bus_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl
