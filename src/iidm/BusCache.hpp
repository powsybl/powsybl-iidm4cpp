/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSCACHE_HPP
#define POWSYBL_IIDM_BUSCACHE_HPP

#include <functional>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class ConfiguredBus;
class MergedBus;

namespace bus_breaker_voltage_level {

class BusCache {
public:
    using MergedBusById = std::map<std::string, std::unique_ptr<MergedBus> >;

    using MergedBusByConfiguredBus = std::map<std::reference_wrapper<ConfiguredBus>, std::reference_wrapper<MergedBus>, stdcxx::less<ConfiguredBus> >;

public:
    BusCache(MergedBusById mergedBus, MergedBusByConfiguredBus mapping);

    ~BusCache() = default;

    stdcxx::Reference<MergedBus> getMergedBus(const std::string& id) const;

    stdcxx::Reference<MergedBus> getMergedBus(const stdcxx::Reference<ConfiguredBus>& bus) const;

    std::vector<std::reference_wrapper<MergedBus> > getMergedBuses() const;

private:
    MergedBusById m_mergedBus;

    MergedBusByConfiguredBus m_mapping;
};

}  // namespace bus_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSCACHE_HPP
