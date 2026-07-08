/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcTopologyDcBusCache.hpp>

#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/map.hpp>

namespace powsybl {

namespace iidm {

namespace dc_topology_model {

DcBusCache::DcBusCache(DcBusById busById, DcBusByNodeId busByNodeMapping) :
    m_busById(std::move(busById)),
    m_busByNode(std::move(busByNodeMapping)) {
};

stdcxx::Reference<DcBus> DcBusCache::getDcBus(const std::string& id) const {
    const auto& it = m_busById.find(id);

    return (it == m_busById.end()) ? stdcxx::Reference<DcBus>() : stdcxx::ref<DcBus>(it->second);
}

stdcxx::Reference<DcBus> DcBusCache::getDcBusOfDcNode(const std::string& dcNodeId) const {
    const auto& it = m_busByNode.find(dcNodeId);

    return (it == m_busByNode.end()) ? stdcxx::Reference<DcBus>() : it->second;
}

unsigned long DcBusCache::getDcBusCount() const {
    return m_busById.size();
}

stdcxx::const_range<DcBus> DcBusCache::getDcBuses() const {
    return boost::adaptors::values(m_busById) | boost::adaptors::indirected;
}

stdcxx::range<DcBus> DcBusCache::getDcBuses() {
    return boost::adaptors::values(m_busById) | boost::adaptors::indirected;
}

}  // namespace dc_topology_model

}  // namespace iidm

}  // namespace powsybl
