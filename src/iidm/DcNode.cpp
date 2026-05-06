/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcNode.hpp>

#include <powsybl/iidm/DcConnectable.hpp>
#include <powsybl/iidm/DcTerminal.hpp>
#include <powsybl/iidm/DcSwitch.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

const Network& DcNode::getNetwork() const {
    if (m_network) {
        return m_network.get();
    }
    throw PowsyblException(stdcxx::format("%1% %2% has no network", getTypeDescription(), getId()));
}

Network& DcNode::getNetwork() {
    return const_cast<Network&>(static_cast<const DcNode*>(this)->getNetwork());
}

void DcNode::setNetworkRef(Network& network) {
    m_network.set(network);
}

const Network& DcNode::getParentNetwork() const {
    if(static_cast<bool>(m_subnetworkRef)) {
        return m_subnetworkRef.get();
    }
    return getNetwork();
}

Network& DcNode::getParentNetwork() {
    return const_cast<Network&>(static_cast<const DcNode*>(this)->getParentNetwork());
}

const IdentifiableType& DcNode::getType() const {
    static IdentifiableType s_type = IdentifiableType::DC_NODE;
    return s_type;
}

const std::string& DcNode::getTypeDescription() const {
    static std::string s_typeDescription = "DC Node";
    return s_typeDescription;
}

double DcNode::getNominalV() const {
    return m_nominalV;
}

DcNode& DcNode::setNominalV(double nominalV) {
    checkNominalVoltage(*this, nominalV);
    m_nominalV = nominalV;
    return *this;
}

void DcNode::remove() {
    Network& network = getNetwork();

    //To check and to be improved once DC topology is implemented:
    for(const auto& dcConnectable : network.getDcConnectables()) {
        for(const auto & dcTerminal : dcConnectable.getDcTerminals()) {
            if(stdcxx::areSame(dcTerminal.get().getDcNode(), *this)) {
                throw PowsyblException(stdcxx::format("Cannot remove DC Node '%1%' because DC Connectable '%2%' is connected to it", getId(), dcConnectable.getId()));
            }
        }
    }

    for(const auto& dcSwitch : network.getDcSwitches()) {
        if(stdcxx::areSame(dcSwitch.getDcNode1(), *this) || stdcxx::areSame(dcSwitch.getDcNode2(), *this)) {
            throw PowsyblException(stdcxx::format("Cannot remove DC Node '%1%' because DC Switch '%2%' is connected to it", getId(), dcSwitch.getId()));
        }
    }

    network.remove(*this);
}

DcNode::DcNode(Network& rootNetwork, const std::string& id, const std::string& name, bool fictitious, double nominalV) :
    Identifiable(id, name, fictitious),
    m_network(rootNetwork),
    m_subnetworkRef(),
    m_nominalV(nominalV) {
}
DcNode::DcNode(Network& rootNetwork, Network& subnetwork, const std::string& id, const std::string& name, bool fictitious, double nominalV) :
    DcNode(rootNetwork, id, name, fictitious, nominalV) {
    m_subnetworkRef = subnetwork;
}

}  // namespace iidm

}  // namespace powsybl
