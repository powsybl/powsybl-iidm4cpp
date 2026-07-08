/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCTOPOLOGYDCBUSTOPOLOGY_HPP
#define POWSYBL_IIDM_DCTOPOLOGYDCBUSTOPOLOGY_HPP

#include <powsybl/iidm/DcBus.hpp>
#include <powsybl/iidm/DcTopologyDcBusCache.hpp>

#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class DcTopologyModel;

namespace dc_topology_model {

class DcBusTopology {
public:
    explicit DcBusTopology(DcTopologyModel& dcTopologyModel);

    DcBusTopology(DcTopologyModel& dcTopologyModel, DcBusTopology&& dcBusTopology) noexcept;

    ~DcBusTopology() noexcept = default;

    unsigned long getDcBusCount();

    stdcxx::Reference<DcBus> getDcBus(const std::string& dcBusId);

    stdcxx::Reference<DcBus> getDcBusOfDcNode(const std::string& dcNodeId);

    stdcxx::range<DcBus> getDcBuses();

    void invalidateCache();

    void updateCache();

private:
    std::unique_ptr<DcBus> createDcBus(const DcBus::DcNodeSet& dcNodes) const;

    bool isDcBusValid(const DcBus::DcNodeSet& dcNodes) const;

private:
    DcTopologyModel& m_dcTopologyModel;

    std::unique_ptr<DcBusCache> m_cache;
};

}  // namespace dc_topology_model

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCTOPOLOGYDCBUSTOPOLOGY_HPP