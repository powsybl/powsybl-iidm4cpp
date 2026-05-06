/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcLineAdder.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

DcLineAdder::DcLineAdder(Network& network) :
    m_network(network) {
}
DcLineAdder::DcLineAdder(Network& network, const std::string& subNetworkId) :
    DcLineAdder(network) {
    m_subnetworkId = subNetworkId;
}

DcLineAdder& DcLineAdder::setR(double r) {
    m_r = r;
    return *this;
}

DcLineAdder& DcLineAdder::setDcNode1(const std::string& dcNodeId1) {
    m_dcNodeId1 = dcNodeId1;
    return *this;
}

DcLineAdder& DcLineAdder::setConnected1(bool connected1) {
    m_dcConnected1 = connected1;
    return *this;
}

DcLineAdder& DcLineAdder::setDcNode2(const std::string& dcNodeId2) {
    m_dcNodeId2 = dcNodeId2;
    return *this;
}

DcLineAdder& DcLineAdder::setConnected2(bool connected2) {
    m_dcConnected2 = connected2;
    return *this;
}

DcLine& DcLineAdder::add() {
    std::string id = checkAndGetUniqueId();

    DcNode& dcNode1 = checkAndGetDcNode(getNetwork(), *this, m_dcNodeId1, "dcNode1");
    DcNode& dcNode2 = checkAndGetDcNode(getNetwork(), *this, m_dcNodeId2, "dcNode2");

    std::string parentNetworkId = (!m_subnetworkId.empty()) ? m_subnetworkId : m_network.getId();
    checkSameParentNetwork(parentNetworkId, *this, dcNode1, dcNode2);
    checkDoubleParamPositive(*this, m_r, "r");

    std::unique_ptr<DcLine> ptrDcLine = stdcxx::make_unique<DcLine>(id, getName(), isFictitious(), m_r);

    std::unique_ptr<DcTerminal> ptrDcTerminal1 = stdcxx::make_unique<DcTerminal>(dcNode1, TwoSides::ONE, m_dcConnected1);
    std::unique_ptr<DcTerminal> ptrDcTerminal2 = stdcxx::make_unique<DcTerminal>(dcNode2, TwoSides::TWO, m_dcConnected2);
    ptrDcLine->addDcTerminal(std::move(ptrDcTerminal1));
    ptrDcLine->addDcTerminal(std::move(ptrDcTerminal2));

    auto& dcLine = getNetwork().checkAndAdd<DcLine>(std::move(ptrDcLine));
    return dcLine;
}

const Network& DcLineAdder::getNetwork() const {
    return m_network;
}

Network& DcLineAdder::getNetwork() {
    return m_network;
}

const std::string& DcLineAdder::getTypeDescription() const {
    static std::string s_typeDescription = "DC Line";
    return s_typeDescription;
}

}  // namespace iidm

}  // namespace powsybl
