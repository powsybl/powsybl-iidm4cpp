/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcTopologyVariant.hpp>


#include <powsybl/iidm/DcTopologyModel.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace dc_topology_model {

DcBusTopologyVariantImpl::DcBusTopologyVariantImpl(DcTopologyModel& dcTopologyModel) :
    Variant(dcTopologyModel),
    m_DcBusTopology(dcTopologyModel) {
}

DcBusTopologyVariantImpl::DcBusTopologyVariantImpl(DcTopologyModel& dcTopologyModel, DcBusTopologyVariantImpl&& variant) noexcept :
    Variant(dcTopologyModel),
    m_DcBusTopology(dcTopologyModel, std::move(variant.m_DcBusTopology)) {
}

std::unique_ptr<DcBusTopologyVariantImpl> DcBusTopologyVariantImpl::copy() const {
    return stdcxx::make_unique<DcBusTopologyVariantImpl>(m_owner);
}

const DcBusTopology& DcBusTopologyVariantImpl::getDcBusTopology() const {
    return m_DcBusTopology;
}

DcBusTopology& DcBusTopologyVariantImpl::getDcBusTopology() {
    return m_DcBusTopology;
}

}  // namespace dc_topology_model

}  // namespace iidm

}  // namespace powsybl
