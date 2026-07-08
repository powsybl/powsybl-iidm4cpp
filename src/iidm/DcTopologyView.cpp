/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcTopologyView.hpp>

#include <powsybl/iidm/DcTopologyModel.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace dc_topology_model {

DcBusView::DcBusView(DcTopologyModel& dcTopologyModel) :
    m_dcTopologyModel(dcTopologyModel) {
}

stdcxx::const_range<DcBus> DcBusView::getDcBuses() const {
    return m_dcTopologyModel.getDcBusTopology().getDcBuses();
}

stdcxx::range<DcBus> DcBusView::getDcBuses() {
    return m_dcTopologyModel.getDcBusTopology().getDcBuses();
}

unsigned long DcBusView::getDcBusCount() const {
    return m_dcTopologyModel.getDcBusTopology().getDcBusCount();
}

stdcxx::CReference<DcBus> DcBusView::getDcBus(const std::string& dcBusId) const {
    return stdcxx::cref(m_dcTopologyModel.getDcBusTopology().getDcBus(dcBusId));
}

stdcxx::Reference<DcBus> DcBusView::getDcBus(const std::string& dcBusId) {
    return stdcxx::ref(m_dcTopologyModel.getDcBusTopology().getDcBus(dcBusId));
}

stdcxx::CReference<DcBus> DcBusView::getDcBusOfDcNode(const std::string& dcNodeId) const {
    return stdcxx::cref(m_dcTopologyModel.getDcBusTopology().getDcBusOfDcNode(dcNodeId));
}

stdcxx::Reference<DcBus> DcBusView::getDcBusOfDcNode(const std::string& dcNodeId) {
    return stdcxx::ref(m_dcTopologyModel.getDcBusTopology().getDcBusOfDcNode(dcNodeId));
}

}  // namespace dc_topology_model

}  // namespace iidm

}  // namespace powsybl
