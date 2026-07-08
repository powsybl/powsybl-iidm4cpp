/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcTopologyModel.hpp>

#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/DcSwitch.hpp>
#include <powsybl/iidm/DcTerminal.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

DcTopologyModel::DcTopologyModel(Network& owner) :
    m_owner(owner),
    m_variants(*this, [this]() { return stdcxx::make_unique<dc_topology_model::DcBusTopologyVariantImpl>(*this); }),
    m_dcBusView(*this) {
        m_graph = stdcxx::make_unique<DcTopologyModel::Graph>();
}

DcTopologyModel::DcTopologyModel(Network& owner, DcTopologyModel&& dcTopologyModel) noexcept :
    m_owner(owner),
    m_dcNodes(std::move(dcTopologyModel.m_dcNodes)),
    m_dcSwitches(std::move(dcTopologyModel.m_dcSwitches)),
    m_graph(std::move(dcTopologyModel.m_graph)),
    m_variants(*this, std::move(dcTopologyModel.m_variants)),
    m_dcBusView(*this)
    {

}

void DcTopologyModel::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    m_variants.allocateVariantArrayElement(indexes, [this, sourceIndex]() { return m_variants.copy(sourceIndex); });
}

void DcTopologyModel::deleteVariantArrayElement(unsigned long index) {
    m_variants.deleteVariantArrayElement(index);
}

void DcTopologyModel::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    m_variants.extendVariantArraySize(initVariantArraySize, number, [this, sourceIndex]() { return m_variants.copy(sourceIndex); });
}

void DcTopologyModel::reduceVariantArraySize(unsigned long number) {
    m_variants.reduceVariantArraySize(number);
}

const Network& DcTopologyModel::getNetwork() const {
    return m_owner;
}

Network& DcTopologyModel::getNetwork() {
    return m_owner;
}

void DcTopologyModel::assertGraph() const {
    if(!static_cast<bool>(m_graph)) {
        throw PowsyblException(stdcxx::format("Graph missing from DcTopologyModel of Network %1%", getNetwork().getId())); 
    }
}

stdcxx::optional<unsigned long> DcTopologyModel::getVertex(const std::string& dcNodeId, bool throwException) const {
    checkNotEmpty(dcNodeId, "DC Node id is null");

    const auto& it = m_dcNodes.find(dcNodeId);
    if (it != m_dcNodes.end()) {
        return {it->second};
    }
    if (!throwException) {
        return {};
    }

    throw PowsyblException(stdcxx::format("DC Node '%1%' not found in the Network '%2%' DC topology model", dcNodeId, getNetwork().getId()));

}

void DcTopologyModel::addDcNodeToTopology(DcNode& dcNode) {
    assertGraph();
    unsigned long v = m_graph->addVertex();

    m_graph->setVertexObject(v, stdcxx::ref(dcNode));
    m_dcNodes.insert(std::make_pair(dcNode.getId(), v));
    invalidateAllVariantsCache();
}
void DcTopologyModel::addDcSwitchToTopology(DcSwitch& dcSwitch, std::string dcNodeId1, std::string dcNodeId2) {
    assertGraph();
    unsigned long v1 = *getVertex(dcNodeId1, true);
    unsigned long v2 = *getVertex(dcNodeId2, true);

    unsigned long e = m_graph->addEdge(v1, v2, stdcxx::ref(dcSwitch));
    m_dcSwitches.insert(std::make_pair(dcSwitch.getId(), e));
    invalidateAllVariantsCache();
}

void DcTopologyModel::removeDcNode(std::string dcNodeId) {
    assertGraph();
    const auto& it = m_dcNodes.find(dcNodeId);
    if (it == m_dcNodes.end()) {
        throw PowsyblException(stdcxx::format("DC Node '%1%' not found in the Network '%2%' DC topology model", dcNodeId, getNetwork().getId()));
    }

    const auto& dcNode = m_graph->removeVertex(it->second);
    m_dcNodes.erase(it);
    getNetwork().remove(dcNode.get());
    invalidateAllVariantsCache();
}
void DcTopologyModel::removeDcSwitch(std::string dcSwitchId) {
    assertGraph();
    const auto& it = m_dcSwitches.find(dcSwitchId);
    if (it == m_dcSwitches.end()) {
        throw PowsyblException(stdcxx::format("DC Switch '%1%' not found in the Network '%2%' DC topology model", dcSwitchId, getNetwork().getId()));
    }

    const auto& dcSwitch = m_graph->removeEdge(it->second);
    m_dcSwitches.erase(it);
    getNetwork().remove(dcSwitch.get());
    invalidateAllVariantsCache();
}
void DcTopologyModel::removeAllDcNodes() {
    assertGraph();
    if (m_graph->getEdgeCount() > 0) {
        throw PowsyblException(stdcxx::format("Cannot remove all DC Nodes because there is still some DC Switches in the Network '%1%' DC topology model", getNetwork().getId()));
    }
    for (const auto& it : m_graph->getVertexObjects()) {
        getNetwork().remove(it.get());
    }

    m_graph->removeAllVertices();
    m_dcNodes.clear();
    invalidateAllVariantsCache();
}
void DcTopologyModel::removeAllDcSwitches() {
    assertGraph();
    for (const auto& it : m_graph->getEdgeObjects()) {
        getNetwork().remove(it.get());
    }
    m_graph->removeAllEdges();
    m_dcSwitches.clear();
    invalidateAllVariantsCache();
}

void DcTopologyModel::invalidateCache() {
    m_variants.get().getDcBusTopology().invalidateCache();
    // DC topology does not affect at all AC topology,
    // synchronous (ac) components do not need to be invalidated,
    // only dc components and connected (ac+dc) components are invalidated.
    getNetwork().getConnectedComponentsManager().invalidate();
    getNetwork().getDcComponentsManager().invalidate();
}

void DcTopologyModel::attach(DcTerminal& dcTerminal) {
    DcNode& dcNode = dcTerminal.getDcNode();
    dcNode.addDcTerminal(dcTerminal);

    invalidateAllVariantsCache();
}
void DcTopologyModel::detach(DcTerminal& dcTerminal) {
    DcNode& dcNode = dcTerminal.getDcNode();
    dcNode.removeDcTerminal(dcTerminal);

    invalidateAllVariantsCache();
}

void DcTopologyModel::invalidateAllVariantsCache() {
    getNetwork().getVariantManager().forEachVariant( [this]() {
        invalidateCache();
    });
}

const dc_topology_model::DcBusTopology& DcTopologyModel::getDcBusTopology() const {
    return m_variants.get().getDcBusTopology();
}
dc_topology_model::DcBusTopology& DcTopologyModel::getDcBusTopology() {
    return m_variants.get().getDcBusTopology();
}

const dc_topology_model::DcBusView& DcTopologyModel::getDcBusView() const {
    return m_dcBusView;
}
dc_topology_model::DcBusView& DcTopologyModel::getDcBusView() {
    return m_dcBusView;
}

stdcxx::const_range<DcBus> DcTopologyModel::getDcBuses() const {
    return getDcBusView().getDcBuses();
}

stdcxx::range<DcBus> DcTopologyModel::getDcBuses() {
    return getDcBusTopology().getDcBuses();
}

unsigned long DcTopologyModel::getDcBusCount() const {
    return getDcBusView().getDcBusCount();
}

stdcxx::CReference<DcBus> DcTopologyModel::getDcBus(const std::string& dcBusId) const {
    return getDcBusView().getDcBus(dcBusId);
}

stdcxx::Reference<DcBus> DcTopologyModel::getDcBus(const std::string& dcBusId) {
    return getDcBusView().getDcBus(dcBusId);
}

stdcxx::CReference<DcBus> DcTopologyModel::getDcBusOfDcNode(const std::string& dcNodeId) const {
    return getDcBusView().getDcBusOfDcNode(dcNodeId);
}

stdcxx::Reference<DcBus> DcTopologyModel::getDcBusOfDcNode(const std::string& dcNodeId) {
    return getDcBusView().getDcBusOfDcNode(dcNodeId);
}


}  // namespace iidm

}  // namespace powsybl
