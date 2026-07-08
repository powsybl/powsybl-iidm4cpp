/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcNode.hpp>

#include <powsybl/iidm/ConnectedComponentsManager.hpp>
#include <powsybl/iidm/DcConnectable.hpp>
#include <powsybl/iidm/DcComponentsManager.hpp>
#include <powsybl/iidm/DcTerminal.hpp>
#include <powsybl/iidm/DcTopologyModel.hpp>
#include <powsybl/iidm/DcTopologyVisitor.hpp>
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

void DcNode::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Identifiable::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_v[index] = m_v[sourceIndex];
        m_connectedComponentNumber[index] = m_connectedComponentNumber[sourceIndex];
        m_dcComponentNumber[index] = m_dcComponentNumber[sourceIndex];
    }
}

void DcNode::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Identifiable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_v.resize(m_v.size() + number, m_v[sourceIndex]);
    m_connectedComponentNumber.resize(m_connectedComponentNumber.size() + number, m_connectedComponentNumber[sourceIndex]);
    m_dcComponentNumber.resize(m_dcComponentNumber.size() + number, m_dcComponentNumber[sourceIndex]);
}

void DcNode::reduceVariantArraySize(unsigned long number) {
    Identifiable::reduceVariantArraySize(number);

    m_v.resize(m_v.size() - number);
    m_connectedComponentNumber.resize(m_connectedComponentNumber.size() - number);
    m_dcComponentNumber.resize(m_dcComponentNumber.size() - number);
}

double DcNode::getNominalV() const {
    return m_nominalV;
}

DcNode& DcNode::setNominalV(double nominalV) {
    checkNominalVoltage(*this, nominalV);
    m_nominalV = nominalV;
    return *this;
}

double DcNode::getV() const {
    return m_v[getNetwork().getVariantIndex()];
}

DcNode& DcNode::setV(double v) {
    m_v[getNetwork().getVariantIndex()] = v;
    return *this;
}

stdcxx::CReference<DcBus> DcNode::getDcBus() const {
    return getParentNetwork().getDcTopologyModel().getDcBusOfDcNode(getId());
}

stdcxx::Reference<DcBus> DcNode::getDcBus() {
    return getParentNetwork().getDcTopologyModel().getDcBusOfDcNode(getId());
}

void DcNode::remove() {
    Network& network = getNetwork();

    auto dcTerminals  = getDcTerminals();
    if(!dcTerminals.empty()) {
        throw PowsyblException(stdcxx::format("Cannot remove DC Node '%1%' because DC Connectable '%2%' is connected to it", getId(), dcTerminals.front().getDcConnectable().get().getId()));
    }

    for(const auto& dcSwitch : network.getDcSwitches()) {
        if(stdcxx::areSame(dcSwitch.getDcNode1(), *this) || stdcxx::areSame(dcSwitch.getDcNode2(), *this)) {
            throw PowsyblException(stdcxx::format("Cannot remove DC Node '%1%' because DC Switch '%2%' is connected to it", getId(), dcSwitch.getId()));
        }
    }

    getParentNetwork().getDcTopologyModel().removeDcNode(getId());
    network.remove(*this);
}

DcNode::DcNode(Network& rootNetwork, const std::string& id, const std::string& name, bool fictitious, double nominalV) :
    Identifiable(id, name, fictitious),
    m_network(rootNetwork),
    m_subnetworkRef(),
    m_nominalV(nominalV),
    m_v(rootNetwork.getVariantManager().getVariantArraySize(), stdcxx::nan()),
    m_connectedComponentNumber(rootNetwork.getVariantManager().getVariantArraySize(), stdcxx::optional<unsigned long>()),
    m_dcComponentNumber(rootNetwork.getVariantManager().getVariantArraySize(), stdcxx::optional<unsigned long>()) {
}
DcNode::DcNode(Network& rootNetwork, Network& subnetwork, const std::string& id, const std::string& name, bool fictitious, double nominalV) :
    DcNode(rootNetwork, id, name, fictitious, nominalV) {
    m_subnetworkRef = subnetwork;
}


unsigned long DcNode::getDcTerminalCount() const {
    return boost::size(getDcTerminals());
}
stdcxx::const_range<DcTerminal> DcNode::getDcTerminals() const {
    const auto& mapper = stdcxx::map<std::reference_wrapper<DcTerminal>, DcTerminal>;
    return m_dcTerminals | boost::adaptors::transformed(mapper);
}
stdcxx::range<DcTerminal> DcNode::getDcTerminals() {
    const auto& mapper = stdcxx::map<std::reference_wrapper<DcTerminal>, DcTerminal>;
    return m_dcTerminals | boost::adaptors::transformed(mapper);
}

unsigned long DcNode::getConnectedDcTerminalCount() const {
    return boost::size(getConnectedDcTerminals());
}
stdcxx::const_range<DcTerminal> DcNode::getConnectedDcTerminals() const {
    const auto& filter = [](const DcTerminal& dcTerminal) {
        return dcTerminal.isConnected();
    };
    const auto& mapper = stdcxx::map<std::reference_wrapper<DcTerminal>, DcTerminal>;

    return m_dcTerminals | boost::adaptors::transformed(mapper) | boost::adaptors::filtered(filter);
}
stdcxx::range<DcTerminal> DcNode::getConnectedDcTerminals() {
    const auto& filter = [](const DcTerminal& dcTerminal) {
        return dcTerminal.isConnected();
    };
    const auto& mapper = stdcxx::map<std::reference_wrapper<DcTerminal>, DcTerminal>;

    return m_dcTerminals | boost::adaptors::transformed(mapper) | boost::adaptors::filtered(filter);
}

void DcNode::visitConnectedEquipments(DcTopologyVisitor& visitor) {
    DcTopologyVisitor::visitDcEquipments(getConnectedDcTerminals(), visitor);
}

void DcNode::visitConnectedOrConnectableEquipments(DcTopologyVisitor& visitor) {
    DcTopologyVisitor::visitDcEquipments(getDcTerminals(), visitor);
}

void DcNode::addDcTerminal(DcTerminal& dcTerminal) {
    m_dcTerminals.push_back(std::ref(dcTerminal));
}
void DcNode::removeDcTerminal(DcTerminal& dcTerminal) {
    const auto& itFind = std::find_if(m_dcTerminals.begin(), m_dcTerminals.end(), [&dcTerminal](const std::reference_wrapper<DcTerminal>& item) {
        return stdcxx::areSame(dcTerminal, item.get());
    });

    if (itFind != m_dcTerminals.end()) {
        m_dcTerminals.erase(itFind);
    } else {
        throw PowsyblException(stdcxx::format("DcTerminal %1% not found", dcTerminal));
    }
}

void DcNode::setConnectedComponentNumber(const stdcxx::optional<unsigned long>& connectedComponentNumber) {
    unsigned long variantIndex = getNetwork().getVariantIndex();
    m_connectedComponentNumber[variantIndex] = connectedComponentNumber;
}
void DcNode::setDcComponentNumber(const stdcxx::optional<unsigned long>& dcComponentNumber) {
    unsigned long variantIndex = getNetwork().getVariantIndex();
    m_dcComponentNumber[variantIndex] = dcComponentNumber;
}

stdcxx::CReference<Component> DcNode::getConnectedComponent() const {
    auto& ccm = const_cast<ConnectedComponentsManager&>(getNetwork().getConnectedComponentsManager());
    ccm.update();
    return stdcxx::cref<Component>(ccm.getComponent(m_connectedComponentNumber[getNetwork().getVariantIndex()]));
}
stdcxx::Reference<Component> DcNode::getConnectedComponent() {
    return stdcxx::ref(static_cast<const DcNode*>(this)->getConnectedComponent());
}

stdcxx::CReference<Component> DcNode::getDcComponent() const {
    auto& dcm = const_cast<DcComponentsManager&>(getNetwork().getDcComponentsManager());
    dcm.update();
    return stdcxx::cref<Component>(dcm.getComponent(m_dcComponentNumber[getNetwork().getVariantIndex()]));
}
stdcxx::Reference<Component> DcNode::getDcComponent() {
    return stdcxx::ref(static_cast<const DcNode*>(this)->getDcComponent());
}


}  // namespace iidm

}  // namespace powsybl
