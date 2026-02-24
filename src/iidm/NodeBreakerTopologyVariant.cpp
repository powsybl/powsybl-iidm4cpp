/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerTopologyVariant.hpp"

#include <powsybl/stdcxx/make_unique.hpp>

#include "NodeBreakerTopologyModel.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_topology_model {

VariantImpl::VariantImpl(NodeBreakerTopologyModel& topologyModel) :
    Variant(topologyModel),
    m_calculatedBusBreakerTopology(topologyModel),
    m_calculatedBusTopology(topologyModel) {

}

std::unique_ptr<VariantImpl> VariantImpl::copy() const {
    return stdcxx::make_unique<VariantImpl>(m_owner);
}

const CalculatedBusBreakerTopology& VariantImpl::getCalculatedBusBreakerTopology() const {
    return m_calculatedBusBreakerTopology;
}

CalculatedBusBreakerTopology& VariantImpl::getCalculatedBusBreakerTopology() {
    return m_calculatedBusBreakerTopology;
}

CalculatedBusTopology& VariantImpl::getCalculatedBusTopology() {
    return m_calculatedBusTopology;
}

}  // namespace node_breaker_topology_model

}  // namespace iidm

}  // namespace powsybl
