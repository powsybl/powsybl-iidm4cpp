/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusCache.hpp"

#include <algorithm>
#include <cassert>
#include <functional>

#include "ConfiguredBus.hpp"
#include "MergedBus.hpp"

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

std::vector<std::reference_wrapper<MergedBus> > BusCache::getMergedBuses() const {
    std::vector<std::reference_wrapper<MergedBus> > buses;
    buses.reserve(m_mergedBus.size());
    std::for_each(m_mergedBus.begin(), m_mergedBus.end(), [&buses](const std::pair<const std::string, std::unique_ptr<MergedBus> >& it) {
        buses.emplace_back(std::ref(*it.second));
    });

    return buses;
}

}  // namespace bus_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl
