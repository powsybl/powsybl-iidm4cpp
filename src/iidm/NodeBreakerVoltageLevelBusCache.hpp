/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELBUSCACHE_HPP
#define POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELBUSCACHE_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <powsybl/stdcxx/reference.hpp>

#include "CalculatedBus.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

class BusCache {
public:
    using CalculatedBusByNode = std::vector<stdcxx::Reference<CalculatedBus> >;

    using CalculatedBusById = std::map<std::string, std::unique_ptr<CalculatedBus> >;

public:
    BusCache(CalculatedBusByNode busByNode, CalculatedBusById busById);

    ~BusCache() noexcept = default;

    stdcxx::Reference<CalculatedBus> getBus(unsigned long node) const;

    stdcxx::Reference<CalculatedBus> getBus(const std::string& id) const;

    stdcxx::const_range<CalculatedBus> getBuses() const;

    stdcxx::range<CalculatedBus> getBuses();

private:
    CalculatedBusByNode m_busByNode;

    CalculatedBusById m_busById;
};

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELBUSCACHE_HPP
