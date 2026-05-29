/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcGroundAdder.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

DcGroundAdder::DcGroundAdder(Network& network) :
    m_network(network) {
}
DcGroundAdder::DcGroundAdder(Network& network, const std::string& subNetworkId) :
    DcGroundAdder(network) {
    m_subnetworkId = subNetworkId;
}

DcGroundAdder& DcGroundAdder::setR(double r) {
    m_r = r;
    return *this;
}

DcGroundAdder& DcGroundAdder::setDcNode(const std::string& dcNodeId) {
    m_dcNodeId = dcNodeId;
    return *this;
}

DcGroundAdder& DcGroundAdder::setConnected(bool connected) {
    m_connected = connected;
    return *this;
}

DcGround& DcGroundAdder::add() {
    std::string id = checkAndGetUniqueId();

    DcNode& dcNode = checkAndGetDcNode(getNetwork(), *this, m_dcNodeId, "dcNode");

    std::string parentNetworkId = (!m_subnetworkId.empty()) ? m_subnetworkId : m_network.getId();
    checkSameParentNetwork(parentNetworkId, *this, dcNode);
    checkDoubleParamPositive(*this, m_r, "r");

    std::unique_ptr<DcGround> ptrDcGround = stdcxx::make_unique<DcGround>(id, getName(), isFictitious(), m_r);

    std::unique_ptr<DcTerminal> ptrDcTerminal = stdcxx::make_unique<DcTerminal>(dcNode, m_connected);
    ptrDcGround->addDcTerminal(std::move(ptrDcTerminal));

    auto& dcGround = getNetwork().checkAndAdd<DcGround>(std::move(ptrDcGround));
    return dcGround;
}

const Network& DcGroundAdder::getNetwork() const {
    return m_network;
}

Network& DcGroundAdder::getNetwork() {
    return m_network;
}

const std::string& DcGroundAdder::getTypeDescription() const {
    static std::string s_typeDescription = "DC Ground";
    return s_typeDescription;
}

}  // namespace iidm

}  // namespace powsybl
