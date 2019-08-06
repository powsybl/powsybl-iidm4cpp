/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerVoltageLevelBusCache.hpp"

#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/map.hpp>

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

BusCache::BusCache(CalculatedBusByNode busByNode, CalculatedBusById busById) :
    m_busByNode(std::move(busByNode)),
    m_busById(std::move(busById)) {
}

stdcxx::Reference<CalculatedBus> BusCache::getBus(unsigned long node) const {
    return m_busByNode.at(node);
}

stdcxx::Reference<CalculatedBus> BusCache::getBus(const std::string& id) const {
    const auto& it = m_busById.find(id);

    return (it == m_busById.end()) ? stdcxx::Reference<CalculatedBus>() : stdcxx::ref<CalculatedBus>(it->second);
}

stdcxx::const_range<CalculatedBus> BusCache::getBuses() const {
    return boost::adaptors::values(m_busById) | boost::adaptors::indirected;
}

stdcxx::range<CalculatedBus> BusCache::getBuses() {
    return boost::adaptors::values(m_busById) | boost::adaptors::indirected;
}

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl
