/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELTOPOLOGY_HPP
#define POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELTOPOLOGY_HPP

#include <functional>
#include <memory>
#include <vector>

#include <powsybl/stdcxx/reference.hpp>

#include "BusBreakerVoltageLevelBusCache.hpp"
#include "MergedBus.hpp"

namespace powsybl {

namespace iidm {

namespace bus_breaker_voltage_level {

class CalculatedBusTopology {
public:
    explicit CalculatedBusTopology(BusBreakerVoltageLevel& voltageLevel);

    ~CalculatedBusTopology() noexcept = default;

    stdcxx::Reference<MergedBus> getMergedBus(const std::string& id, bool throwException);

    stdcxx::Reference<MergedBus> getMergedBus(const stdcxx::Reference<ConfiguredBus>& bus);

    stdcxx::range<MergedBus> getMergedBuses();

    void invalidateCache();

    void updateCache();

private:
    std::unique_ptr<MergedBus> createMergedBus(unsigned long busCount, const MergedBus::BusSet& busSet) const;

    bool isBusValid(const MergedBus::BusSet& buses) const;

private:
    BusBreakerVoltageLevel& m_voltageLevel;

    std::unique_ptr<BusCache> m_cache;
};

}  // namespace bus_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELTOPOLOGY_HPP
