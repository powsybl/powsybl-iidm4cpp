/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCTOPOLOGYVIEW_HPP
#define POWSYBL_IIDM_DCTOPOLOGYVIEW_HPP

#include <powsybl/iidm/DcBus.hpp>

#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class DcTopologyModel;

namespace dc_topology_model {

class DcBusView {
public:
    stdcxx::const_range<DcBus> getDcBuses() const;

    stdcxx::range<DcBus> getDcBuses();

    unsigned long getDcBusCount() const;

    stdcxx::CReference<DcBus> getDcBus(const std::string& dcBusId) const;

    stdcxx::Reference<DcBus> getDcBus(const std::string& dcBusId);

    stdcxx::CReference<DcBus> getDcBusOfDcNode(const std::string& dcNodeId) const;

    stdcxx::Reference<DcBus> getDcBusOfDcNode(const std::string& dcNodeId);

public:
    explicit DcBusView(DcTopologyModel& dcTopologyModel);

    ~DcBusView() noexcept = default;

private:
    DcTopologyModel& m_dcTopologyModel;
};

}  // namespace dc_topology_model

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCTOPOLOGYVIEW_HPP