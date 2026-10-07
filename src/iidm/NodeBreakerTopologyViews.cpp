/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include "NodeBreakerTopologyViews.hpp"

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/cast.hpp>

#include "CalculatedBus.hpp"
#include "NodeBreakerTopologyModel.hpp"
#include "NodeTerminal.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_topology_model {

BusBreakerViewImpl::BusBreakerViewImpl(NodeBreakerTopologyModel& topologyModel) :
    m_topologyModel(topologyModel) {
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus(const std::string& busId) const {
    return stdcxx::cref<Bus>(m_topologyModel.getCalculatedBusBreakerTopology().getBus(busId, false));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus(const std::string& busId) {
    return stdcxx::ref<Bus>(m_topologyModel.getCalculatedBusBreakerTopology().getBus(busId, false));
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus1(const std::string& switchId) const {
    return stdcxx::cref<Bus>(m_topologyModel.getCalculatedBusBreakerTopology().getBus1(switchId, true));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus1(const std::string& switchId) {
    return stdcxx::ref<Bus>(m_topologyModel.getCalculatedBusBreakerTopology().getBus1(switchId, true));
}

stdcxx::CReference<Bus> BusBreakerViewImpl::getBus2(const std::string& switchId) const {
    return stdcxx::cref<Bus>(m_topologyModel.getCalculatedBusBreakerTopology().getBus2(switchId, true));
}

stdcxx::Reference<Bus> BusBreakerViewImpl::getBus2(const std::string& switchId) {
    return stdcxx::ref<Bus>(m_topologyModel.getCalculatedBusBreakerTopology().getBus2(switchId, true));
}

unsigned long BusBreakerViewImpl::getBusCount() const {
    return m_topologyModel.getCalculatedBusBreakerTopology().getBusCount();
}

stdcxx::const_range<Bus> BusBreakerViewImpl::getBuses() const {
    const auto& calculatedBuses = m_topologyModel.getCalculatedBusBreakerTopology().getBuses();

    const auto& mapper = stdcxx::upcast<CalculatedBus, Bus>;

    return calculatedBuses | boost::adaptors::transformed(mapper);
}

stdcxx::range<Bus> BusBreakerViewImpl::getBuses() {
    const auto& calculatedBuses = m_topologyModel.getCalculatedBusBreakerTopology().getBuses();

    const auto& mapper = stdcxx::upcast<CalculatedBus, Bus>;

    return calculatedBuses | boost::adaptors::transformed(mapper);
}

stdcxx::CReference<Switch> BusBreakerViewImpl::getSwitch(const std::string& switchId) const {
    return m_topologyModel.getCalculatedBusBreakerTopology().getSwitch(switchId, true);
}

stdcxx::Reference<Switch> BusBreakerViewImpl::getSwitch(const std::string& switchId) {
    return stdcxx::ref(m_topologyModel.getCalculatedBusBreakerTopology().getSwitch(switchId, true));
}

unsigned long BusBreakerViewImpl::getSwitchCount() const {
    return m_topologyModel.getCalculatedBusBreakerTopology().getSwitchCount();
}

stdcxx::const_range<Switch> BusBreakerViewImpl::getSwitches() const {
    return m_topologyModel.getCalculatedBusBreakerTopology().getSwitches();
}

stdcxx::range<Switch> BusBreakerViewImpl::getSwitches() {
    return m_topologyModel.getCalculatedBusBreakerTopology().getSwitches();
}

BusAdder BusBreakerViewImpl::newBus() {
    throw AssertionError("Not implemented");
}

VoltageLevel::BusBreakerView::SwitchAdder BusBreakerViewImpl::newSwitch() {
    throw AssertionError("Not implemented");
}

void BusBreakerViewImpl::removeAllBuses() {
    throw AssertionError("Not implemented");
}

void BusBreakerViewImpl::removeAllSwitches() {
    throw AssertionError("Not implemented");
}

void BusBreakerViewImpl::removeBus(const std::string& /*busId*/) {
    throw AssertionError("Not implemented");
}

void BusBreakerViewImpl::removeSwitch(const std::string& /*switchId*/) {
    throw AssertionError("Not implemented");
}

void BusBreakerViewImpl::traverse(const Bus& /*bus*/, const TopologyTraverser& /*traverser*/) {
    throw PowsyblException("Not supported in a node/breaker topology");
}

BusViewImpl::BusViewImpl(NodeBreakerTopologyModel& voltageLevel) :
    m_topologyModel(voltageLevel) {
}

stdcxx::CReference<Bus> BusViewImpl::getBus(const std::string& busId) const {
    return stdcxx::cref<Bus>(m_topologyModel.getCalculatedBusTopology().getBus(busId, false));
}

stdcxx::Reference<Bus> BusViewImpl::getBus(const std::string& busId) {
    return stdcxx::ref<Bus>(m_topologyModel.getCalculatedBusTopology().getBus(busId, false));
}

unsigned long BusViewImpl::getBusCount() const {
    return m_topologyModel.getCalculatedBusTopology().getBusCount();
}

stdcxx::const_range<Bus> BusViewImpl::getBuses() const {
    const auto& calculatedBuses = m_topologyModel.getCalculatedBusTopology().getBuses();

    const auto& mapper = stdcxx::upcast<CalculatedBus, Bus>;

    return calculatedBuses | boost::adaptors::transformed(mapper);
}

stdcxx::range<Bus> BusViewImpl::getBuses() {
    const auto& calculatedBuses = m_topologyModel.getCalculatedBusTopology().getBuses();

    const auto& mapper = stdcxx::upcast<CalculatedBus, Bus>;

    return calculatedBuses | boost::adaptors::transformed(mapper);
}


stdcxx::CReference<Bus> BusViewImpl::getMergedBus(const std::string& busbarSectionId) const {
    auto& terminal = dynamic_cast<NodeTerminal&>(m_topologyModel.getNodeBreakerView().getBusbarSection(busbarSectionId).get().getTerminal());

    return stdcxx::cref<Bus>(m_topologyModel.getCalculatedBusTopology().getBus(terminal.getNode()));
}

stdcxx::Reference<Bus> BusViewImpl::getMergedBus(const std::string& busbarSectionId) {
    auto& terminal = dynamic_cast<NodeTerminal&>(m_topologyModel.getNodeBreakerView().getBusbarSection(busbarSectionId).get().getTerminal());

    return stdcxx::ref<Bus>(m_topologyModel.getCalculatedBusTopology().getBus(terminal.getNode()));
}

NodeBreakerViewImpl::NodeBreakerViewImpl(NodeBreakerTopologyModel& voltageLevel) :
    m_topologyModel(voltageLevel) {
        unsigned long variantArraySize = m_topologyModel.getNetwork().getVariantManager().getVariantArraySize();
        m_fictitiousP0ByNode.resize(variantArraySize, std::map<unsigned long, double>());
        m_fictitiousQ0ByNode.resize(variantArraySize, std::map<unsigned long, double>());
}

void NodeBreakerViewImpl::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (const auto& index : indexes) {
        m_fictitiousP0ByNode[index] = m_fictitiousP0ByNode[sourceIndex];
        m_fictitiousQ0ByNode[index] = m_fictitiousQ0ByNode[sourceIndex];
    }
}

void NodeBreakerViewImpl::deleteVariantArrayElement(unsigned long index) {
    m_fictitiousP0ByNode[index].clear();
    m_fictitiousQ0ByNode[index].clear();
}

void NodeBreakerViewImpl::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_fictitiousP0ByNode.resize(m_fictitiousP0ByNode.size() + number, m_fictitiousP0ByNode[sourceIndex]);
    m_fictitiousQ0ByNode.resize(m_fictitiousQ0ByNode.size() + number, m_fictitiousQ0ByNode[sourceIndex]);
}

void NodeBreakerViewImpl::reduceVariantArraySize(unsigned long number) {
    m_fictitiousP0ByNode.resize(m_fictitiousP0ByNode.size() - number);
    m_fictitiousQ0ByNode.resize(m_fictitiousQ0ByNode.size() - number);
}

double NodeBreakerViewImpl::getFictitiousP0(unsigned long node) const {
    //get map of the current Variant index
    const auto& fictP0ByNode = m_fictitiousP0ByNode.at(m_topologyModel.getNetwork().getVariantIndex());

    const auto& it = fictP0ByNode.find(node);
    if(it!=fictP0ByNode.cend()) {
        return it->second;
    }
    return stdcxx::nan();
}

double NodeBreakerViewImpl::getFictitiousQ0(unsigned long node) const {
    //get map of the current Variant index
    const auto& fictQ0ByNode = m_fictitiousQ0ByNode.at(m_topologyModel.getNetwork().getVariantIndex());

    const auto& it = fictQ0ByNode.find(node);
    if(it!=fictQ0ByNode.cend()) {
        return it->second;
    }
    return stdcxx::nan();
}

stdcxx::CReference<BusbarSection> NodeBreakerViewImpl::getBusbarSection(const std::string& bbsId) const {
    return stdcxx::cref(m_topologyModel.getVoltageLevel().getConnectable<BusbarSection>(bbsId));
}

stdcxx::Reference<BusbarSection> NodeBreakerViewImpl::getBusbarSection(const std::string& bbsId) {
    return m_topologyModel.getVoltageLevel().getConnectable<BusbarSection>(bbsId);
}

unsigned long NodeBreakerViewImpl::getBusbarSectionCount() const {
    return m_topologyModel.getConnectableCount<BusbarSection>();
}

stdcxx::const_range<BusbarSection> NodeBreakerViewImpl::getBusbarSections() const {
    return m_topologyModel.getConnectables<BusbarSection>();
}

stdcxx::range<BusbarSection> NodeBreakerViewImpl::getBusbarSections() {
    return m_topologyModel.getConnectables<BusbarSection>();
}

unsigned long NodeBreakerViewImpl::getInternalConnectionCount() const {
    return m_topologyModel.getInternalConnectionCount();
}

stdcxx::const_range<NodeBreakerViewImpl::InternalConnection> NodeBreakerViewImpl::getInternalConnections() const {
    const auto& filter = [this](const unsigned long& e) {
        return !m_topologyModel.getGraph().getEdgeObject(e);
    };

    const auto& mapper = [this](const unsigned long& e) {
        unsigned long node1 = m_topologyModel.getGraph().getVertex1(e);
        unsigned long node2 = m_topologyModel.getGraph().getVertex2(e);

        return InternalConnection(node1, node2);
    };

    return m_topologyModel.getGraph().getEdges() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

long NodeBreakerViewImpl::getMaximumNodeIndex() const {
    return m_topologyModel.getMaximumNodeIndex();
}

unsigned long NodeBreakerViewImpl::getNode1(const std::string& switchId) const {
    return m_topologyModel.getNode1(switchId);
}

unsigned long NodeBreakerViewImpl::getNode2(const std::string& switchId) const {
    return m_topologyModel.getNode2(switchId);
}

stdcxx::const_range<unsigned long> NodeBreakerViewImpl::getNodes() const {
    return m_topologyModel.getGraph().getVertices();
}

stdcxx::CReference<Terminal> NodeBreakerViewImpl::getOptionalTerminal(unsigned long node) const {
    if (m_topologyModel.getGraph().vertexExists(node)) {
        return stdcxx::cref<Terminal>(m_topologyModel.getGraph().getVertexObject(node));
    }
    return stdcxx::cref<Terminal>();
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getOptionalTerminal(unsigned long node) {
    if (m_topologyModel.getGraph().vertexExists(node)) {
        return stdcxx::ref<Terminal>(m_topologyModel.getGraph().getVertexObject(node));
    }
    return stdcxx::ref<Terminal>();
}

stdcxx::CReference<Switch> NodeBreakerViewImpl::getSwitch(const std::string& switchId) const {
    return m_topologyModel.getSwitch(switchId);
}

stdcxx::Reference<Switch> NodeBreakerViewImpl::getSwitch(const std::string& switchId) {
    return stdcxx::ref(m_topologyModel.getSwitch(switchId));
}

unsigned long NodeBreakerViewImpl::getSwitchCount() const {
    return m_topologyModel.getSwitchCount();
}

stdcxx::const_range<Switch> NodeBreakerViewImpl::getSwitches() const {
    const auto& filter = [](const stdcxx::Reference<Switch>& sw) {
        return static_cast<bool>(sw);
    };

    const auto& mapper = stdcxx::map<stdcxx::Reference<Switch>, Switch>;

    return m_topologyModel.getGraph().getEdgeObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

stdcxx::range<Switch> NodeBreakerViewImpl::getSwitches() {
    const auto& filter = [](const stdcxx::Reference<Switch>& sw) {
        return static_cast<bool>(sw);
    };

    const auto& mapper = stdcxx::map<stdcxx::Reference<Switch>, Switch>;

    return m_topologyModel.getGraph().getEdgeObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

stdcxx::CReference<Terminal> NodeBreakerViewImpl::getTerminal(unsigned long node) const {
    return stdcxx::cref(m_topologyModel.getTerminal(node));
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getTerminal(unsigned long node) {
    return m_topologyModel.getTerminal(node);
}

stdcxx::CReference<Terminal> NodeBreakerViewImpl::getTerminal1(const std::string& switchId) const {
    return getTerminal(getNode1(switchId));
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getTerminal1(const std::string& switchId) {
    return getTerminal(getNode1(switchId));
}

stdcxx::CReference<Terminal> NodeBreakerViewImpl::getTerminal2(const std::string& switchId) const {
    return getTerminal(getNode2(switchId));
}

stdcxx::Reference<Terminal> NodeBreakerViewImpl::getTerminal2(const std::string& switchId) {
    return getTerminal(getNode2(switchId));
}

bool NodeBreakerViewImpl::hasAttachedEquipment(unsigned long node) const {
    return m_topologyModel.getGraph().vertexExists(node);
}

double NodeBreakerViewImpl::hasFictitiousP0() const {
    //get map of the current variant index:
    const auto& fictP0ByNode = m_fictitiousP0ByNode.at(m_topologyModel.getNetwork().getVariantIndex());
    return !fictP0ByNode.empty();
}

double NodeBreakerViewImpl::hasFictitiousQ0() const {
    //get map of the current Variant index
    const auto& fictQ0ByNode = m_fictitiousQ0ByNode.at(m_topologyModel.getNetwork().getVariantIndex());
    return !fictQ0ByNode.empty();
}

NodeBreakerViewImpl::SwitchAdder NodeBreakerViewImpl::newBreaker() {
    return SwitchAdder(m_topologyModel.getVoltageLevel()).setKind(SwitchKind::BREAKER);
}

BusbarSectionAdder NodeBreakerViewImpl::newBusbarSection() {
    return BusbarSectionAdder(m_topologyModel.getVoltageLevel());
}

NodeBreakerViewImpl::SwitchAdder NodeBreakerViewImpl::newDisconnector() {
    return SwitchAdder(m_topologyModel.getVoltageLevel()).setKind(SwitchKind::DISCONNECTOR);
}

NodeBreakerViewImpl::InternalConnectionAdder NodeBreakerViewImpl::newInternalConnection() {
    return InternalConnectionAdder(m_topologyModel.getVoltageLevel());
}

NodeBreakerViewImpl::SwitchAdder NodeBreakerViewImpl::newSwitch() {
    return SwitchAdder(m_topologyModel.getVoltageLevel());
}

void NodeBreakerViewImpl::removeInternalConnections(unsigned long node1, unsigned long node2) {
    m_topologyModel.removeInternalConnections(node1, node2);
}

void NodeBreakerViewImpl::removeSwitch(const std::string& switchId) {
    m_topologyModel.removeSwitch(switchId);
}

voltage_level::NodeBreakerView& NodeBreakerViewImpl::setFictitiousP0(unsigned long node, double p0) {
    const Network& network = m_topologyModel.getNetwork();
    checkP0(m_topologyModel.getVoltageLevel(), p0, network.getMinimumValidationLevel());

    //get map of the current Variant index
    auto& fictP0ByNode = m_fictitiousP0ByNode.at(m_topologyModel.getNetwork().getVariantIndex());

    //save the value, or remove if value is nan or 0.0
    if(std::isnan(p0) || p0 == 0.0) {
        fictP0ByNode.erase(node);
    } else {
        fictP0ByNode[node] = p0;
    }

    return *this;
}

voltage_level::NodeBreakerView& NodeBreakerViewImpl::setFictitiousQ0(unsigned long node, double q0) {
    const Network& network = m_topologyModel.getNetwork();
    checkQ0(m_topologyModel.getVoltageLevel(), q0, network.getMinimumValidationLevel());

    //get map of the current Variant index
    auto& fictQ0ByNode = m_fictitiousQ0ByNode.at(m_topologyModel.getNetwork().getVariantIndex());

    //save the value, or remove if value is nan or 0.0
    if(std::isnan(q0) || q0 == 0.0) {
        fictQ0ByNode.erase(node);
    } else {
        fictQ0ByNode[node] = q0;
    }

    return *this;
}

void NodeBreakerViewImpl::traverse(unsigned long node, const TopologyTraverser& traverser) const {
    math::Traverser graphTraverser = [this, &traverser](unsigned long v1, unsigned long e, unsigned long v2) {
        return traverser(v1, m_topologyModel.getGraph().getEdgeObject(e), v2);
    };

    m_topologyModel.getGraph().traverse(node, math::TraversalType::DEPTH_FIRST, graphTraverser);
}

void NodeBreakerViewImpl::traverse(stdcxx::const_range<unsigned long>& nodes, const TopologyTraverser& traverser) const {
    powsybl::math::Traverser graphTraverser = [this, &traverser](unsigned long v1, unsigned long e, unsigned long v2) {
        return traverser(v1, m_topologyModel.getGraph().getEdgeObject(e), v2);
    };

    m_topologyModel.getGraph().traverse(nodes, math::TraversalType::DEPTH_FIRST, graphTraverser);
}

}  // namespace node_breaker_topology_model

}  // namespace iidm

}  // namespace powsybl
