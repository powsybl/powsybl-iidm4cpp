/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcSwitchAdder.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

DcSwitchAdder::DcSwitchAdder(Network& network) :
    m_network(network) {
}
DcSwitchAdder::DcSwitchAdder(Network& network, const std::string& subnetworkId) :
    DcSwitchAdder(network) {
    m_subnetworkId = subnetworkId;
}

const Network& DcSwitchAdder::getNetwork() const {
    return m_network;
}

Network& DcSwitchAdder::getNetwork() {
    return m_network;
}

const std::string& DcSwitchAdder::getTypeDescription() const {
    static std::string s_typeDescription = "DC Switch";
    return s_typeDescription;
}


DcSwitch& DcSwitchAdder::add() {
    std::string id = checkAndGetUniqueId();

    DcNode& dcNode1 = checkAndGetDcNode(getNetwork(), *this, m_dcNodeId1, "dcNode1");
    DcNode& dcNode2 = checkAndGetDcNode(getNetwork(), *this, m_dcNodeId2, "dcNode2");

    std::string parentNetworkId = (!m_subnetworkId.empty()) ? m_subnetworkId : m_network.getId();
    checkSameParentNetwork(parentNetworkId, *this, dcNode1, dcNode2);
    if(!m_kind) {
        throw ValidationException(*this, "kind is not set");
    }
    if(!m_open) {
        throw ValidationException(*this, "open is not set");
    }

    auto ptrDcSwitch = std::unique_ptr<DcSwitch>(new DcSwitch(id, getName(), isFictitious(), dcNode1, dcNode2, *m_kind, *m_open));
    auto& dcSwitch = getNetwork().checkAndAdd<DcSwitch>(std::move(ptrDcSwitch));
    return dcSwitch;
}

DcSwitchAdder& DcSwitchAdder::setOpen(bool open) {
    m_open = open;
    return *this;
}
DcSwitchAdder& DcSwitchAdder::setKind(const DcSwitchKind& kind) {
    m_kind = kind;
    return *this;
}
DcSwitchAdder& DcSwitchAdder::setDcNode1(const std::string& dcNodeId1) {
    m_dcNodeId1 = dcNodeId1;
    return *this;
}
DcSwitchAdder& DcSwitchAdder::setDcNode2(const std::string& dcNodeId2) {
    m_dcNodeId2 = dcNodeId2;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
