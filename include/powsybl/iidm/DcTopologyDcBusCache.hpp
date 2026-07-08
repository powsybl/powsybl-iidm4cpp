/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCTOPOLOGYDCBUSCACHE_HPP
#define POWSYBL_IIDM_DCTOPOLOGYDCBUSCACHE_HPP

#include <powsybl/iidm/DcBus.hpp>

#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class DcTopologyModel;

namespace dc_topology_model {

class DcBusCache {
public:
    using DcBusById = std::map<std::string, std::unique_ptr<DcBus> >;

    using DcBusByNodeId = std::map<std::string, stdcxx::Reference<DcBus> >;

public:
    DcBusCache(DcBusById busById, DcBusByNodeId busByNodeMapping);

    ~DcBusCache() noexcept = default;

    stdcxx::Reference<DcBus> getDcBus(const std::string& id) const;

    stdcxx::Reference<DcBus> getDcBusOfDcNode(const std::string& dcNodeId) const;

    unsigned long getDcBusCount() const;

    stdcxx::const_range<DcBus> getDcBuses() const;

    stdcxx::range<DcBus> getDcBuses();

private:
    DcBusById m_busById;

    DcBusByNodeId m_busByNode;
};

}  // namespace dc_topology_model

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCTOPOLOGYDCBUSCACHE_HPP