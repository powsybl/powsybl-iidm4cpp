/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
    m_fictitiousP0ByNode = std::map<unsigned long, std::vector<double>>();
    m_fictitiousQ0ByNode = std::map<unsigned long, std::vector<double>>();
}

double NodeBreakerViewImpl::getFictitiousP0(unsigned long node) const {
    const auto& it = m_fictitiousP0ByNode.find(node);
    if(it!=m_fictitiousP0ByNode.cend()) {
        return it->second.at(m_topologyModel.getNetwork().getVariantIndex());
    }
    return stdcxx::nan();
}

double NodeBreakerViewImpl::getFictitiousQ0(unsigned long node) const {
    const auto& it = m_fictitiousQ0ByNode.find(node);
    if(it!=m_fictitiousQ0ByNode.cend()) {
        return it->second.at(m_topologyModel.getNetwork().getVariantIndex());
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

    if(m_fictitiousP0ByNode.find(node) == m_fictitiousP0ByNode.cend()) {
        m_fictitiousP0ByNode[node] = std::vector<double>(network.getVariantManager().getVariantArraySize(), stdcxx::nan());
    }
    m_fictitiousP0ByNode.at(node)[network.getVariantIndex()] = p0;
    
    std::set<unsigned long> nodesToRemove = clearFictitiousInjections(m_fictitiousP0ByNode);
    for(const auto& nodeRemoved : nodesToRemove) {
        m_fictitiousP0ByNode.erase(nodeRemoved);
    }
    
    return *this;
}

voltage_level::NodeBreakerView& NodeBreakerViewImpl::setFictitiousQ0(unsigned long node, double q0) {
    const Network& network = m_topologyModel.getNetwork();
    checkQ0(m_topologyModel.getVoltageLevel(), q0, network.getMinimumValidationLevel());

    if(m_fictitiousQ0ByNode.find(node) == m_fictitiousQ0ByNode.cend()) {
        m_fictitiousQ0ByNode[node] = std::vector<double>(network.getVariantManager().getVariantArraySize(), stdcxx::nan());
    }
    m_fictitiousQ0ByNode.at(node)[network.getVariantIndex()] = q0;
    
    std::set<unsigned long> nodesToRemove = clearFictitiousInjections(m_fictitiousQ0ByNode);
    for(const auto& nodeRemoved : nodesToRemove) {
        m_fictitiousQ0ByNode.erase(nodeRemoved);
    }
    
    return *this;
}

std::set<unsigned long> NodeBreakerViewImpl::clearFictitiousInjections(const std::map<unsigned long,std::vector<double>>& fictitiousInjectionsByNode) {
    std::set<unsigned long> toRemove;
    for(const auto& it : fictitiousInjectionsByNode) {
        toRemove.insert(it.first);
        for(const auto& vect : it.second) {
            if(!std::isnan(vect)) {
                toRemove.erase(it.first);
                break;
            }
        }
    }
    return toRemove;
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
