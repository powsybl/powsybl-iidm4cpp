/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcNodeAdder.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

DcNodeAdder::DcNodeAdder(Network& network) :
    m_network(network) {
}

DcNodeAdder::DcNodeAdder(Network& network, Network& subnetwork) :
    m_network(network),
    m_subNetworkRef(stdcxx::ref<Network>(subnetwork)) {
}

const Network& DcNodeAdder::getNetwork() const {
    return m_network;
}

Network& DcNodeAdder::getNetwork() {
    return m_network;
}

const std::string& DcNodeAdder::getTypeDescription() const {
    static std::string s_typeDescription = "DC Node";
    return s_typeDescription;
}


DcNode& DcNodeAdder::add() {
    std::string id = checkAndGetUniqueId();
    checkNominalVoltage(*this, m_nominalV);

    std::unique_ptr<DcNode> ptrDcNode;
    if(static_cast<bool>(m_subNetworkRef)) {
        ptrDcNode = std::unique_ptr<DcNode>(new DcNode(m_network, m_subNetworkRef.get(), id, getName(), isFictitious(), m_nominalV));
    } else {
        ptrDcNode = std::unique_ptr<DcNode>(new DcNode(m_network, id, getName(), isFictitious(), m_nominalV));
    }
    auto& dcNode = getNetwork().checkAndAdd<DcNode>(std::move(ptrDcNode));

    Network& parentNetwork = (static_cast<bool>(m_subNetworkRef)) ? m_subNetworkRef.get() : getNetwork();
    parentNetwork.getDcTopologyModel().addDcNodeToTopology(dcNode);

    return dcNode;
}


DcNodeAdder& DcNodeAdder::setNominalV(double nominalV) {
    m_nominalV = nominalV;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
