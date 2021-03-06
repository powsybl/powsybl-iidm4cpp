/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELBUSCACHE_HPP
#define POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELBUSCACHE_HPP

#include <functional>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <powsybl/stdcxx/reference.hpp>

#include "MergedBus.hpp"

namespace powsybl {

namespace iidm {

namespace bus_breaker_voltage_level {

class BusCache {
public:
    using MergedBusById = std::map<std::string, std::unique_ptr<MergedBus> >;

    using MergedBusByConfiguredBus = std::map<std::reference_wrapper<ConfiguredBus>, std::reference_wrapper<MergedBus>, stdcxx::less<ConfiguredBus> >;

public:
    BusCache(MergedBusById mergedBus, MergedBusByConfiguredBus mapping);

    ~BusCache() noexcept = default;

    stdcxx::Reference<MergedBus> getMergedBus(const std::string& id) const;

    stdcxx::Reference<MergedBus> getMergedBus(const stdcxx::Reference<ConfiguredBus>& bus) const;

    stdcxx::const_range<MergedBus> getMergedBuses() const;

    stdcxx::range<MergedBus> getMergedBuses();

private:
    MergedBusById m_mergedBus;

    MergedBusByConfiguredBus m_mapping;
};

}  // namespace bus_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELBUSCACHE_HPP
