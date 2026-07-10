/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcTopologyModel.hpp>

#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/iidm/DcLine.hpp>
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

void DcTopologyModel::addNextDcTerminals(DcTerminal& dcTerminal, DcTerminalSet& nextDcTerminals) {
    DcConnectable& otherDcConnectable = dcTerminal.getDcConnectable();
    if (stdcxx::isInstanceOf<DcLine>(otherDcConnectable)) {
        auto& dcLine = dynamic_cast<DcLine&>(otherDcConnectable);
        if (stdcxx::areSame(dcLine.getDcTerminal1(), dcTerminal)) {
            nextDcTerminals.emplace(dcLine.getDcTerminal2());
        } else if (stdcxx::areSame(dcLine.getDcTerminal2(), dcTerminal)) {
            nextDcTerminals.emplace(dcLine.getDcTerminal1());
        } else {
            throw AssertionError(stdcxx::format("%1% is not one of the DcLine '%2%' terminals", dcTerminal, dcLine.getId()));
        }
    } else if (stdcxx::isInstanceOf<AcDcConverter>(otherDcConnectable)) {
        auto& acdcConverter = dynamic_cast<AcDcConverter&>(otherDcConnectable);
        if (stdcxx::areSame(acdcConverter.getDcTerminal1(), dcTerminal)) {
            nextDcTerminals.emplace(acdcConverter.getDcTerminal2());
        } else if (stdcxx::areSame(acdcConverter.getDcTerminal2(), dcTerminal)) {
            nextDcTerminals.emplace(acdcConverter.getDcTerminal1());
        } else {
            throw AssertionError(stdcxx::format("%1% is not one of the AcDcConverter '%2%' terminals", dcTerminal, acdcConverter.getId()));
        }
    }
}
math::TraverseResult DcTopologyModel::getTraverserResult(DcTerminalSet& visitedDcTerminals, DcTerminal& dcTerminal, DcTerminal::DcTopologyTraverser& traverser) {
    if (visitedDcTerminals.insert(dcTerminal).second) {
        return traverser.traverse(dcTerminal, dcTerminal.isConnected());
    }
    return math::TraverseResult::TERMINATE_PATH;
}

bool DcTopologyModel::connect(DcTerminal& dcTerminal) {
    // Already connected ?
    if (dcTerminal.isConnected()) {
        return false;
    }

    dcTerminal.setConnected(true);
    return true;
}
bool DcTopologyModel::disconnect(DcTerminal& dcTerminal) {
    // Already disconnected ?
    if (!dcTerminal.isConnected()) {
        return false;
    }

    dcTerminal.setConnected(false);
    return true;
}

bool DcTopologyModel::traverse(DcTerminal& dcTerminal, DcTerminal::DcTopologyTraverser& traverser, math::TraversalType traversalType) const {
    DcTerminalSet traversedDcTerminals;
    return traverse(dcTerminal, traverser, traversedDcTerminals, traversalType);
}

bool DcTopologyModel::traverse(DcTerminal& dcTerminal, DcTerminal::DcTopologyTraverser& traverser, DcTerminalSet& traversedTerminals, math::TraversalType traversalType) const {
    //Check traversing the terminal itself:
    math::TraverseResult termTraverseResult = getTraverserResult(traversedTerminals, dcTerminal, traverser);
    if (termTraverseResult == math::TraverseResult::TERMINATE_TRAVERSER) {
        return false;
    }

    if(termTraverseResult == math::TraverseResult::CONTINUE) {
        //Continue traversal on adjacent dc terminals:
        DcTerminalSet nextDcTerminals;
        addNextDcTerminals(dcTerminal, nextDcTerminals);

        //Check traversing the dcTerminals connected to the same DcNode:
        unsigned long v = *getVertex(dcTerminal.getDcNode().getId(), true);
        assertGraph();
        DcNode& dcNode = m_graph->getVertexObject(v).get();
        for (DcTerminal& t : dcNode.getDcTerminals()) {
            math::TraverseResult tTraverseResult = getTraverserResult(traversedTerminals, t, traverser);
            if (tTraverseResult == math::TraverseResult::TERMINATE_TRAVERSER) {
                return false;
            }
            if (tTraverseResult == math::TraverseResult::CONTINUE) {
                addNextDcTerminals(t, nextDcTerminals);
            }
        }

        //Then go through the graph to visit other connected DcNodes:
        bool traversalTerminated = !m_graph->traverse(v, traversalType, 
            [this, &nextDcTerminals, &traverser, &traversedTerminals](unsigned long /*v1*/, unsigned long e, unsigned long v2) {
                DcSwitch& dcSwitch = m_graph->getEdgeObject(e).get();
                const stdcxx::range<DcTerminal>& otherNodeDcTerminals = m_graph->getVertexObject(v2).get().getDcTerminals();
                math::TraverseResult switchTraverseResult = traverser.traverse(dcSwitch);
                if(switchTraverseResult == math::TraverseResult::CONTINUE && !otherNodeDcTerminals.empty()) {
                    //At lest one DcTerminal on the other node :
                    DcTerminal& otherDcTerminal = *otherNodeDcTerminals.begin();
                    math::TraverseResult otherDcTerminalResult = getTraverserResult(traversedTerminals, otherDcTerminal, traverser);
                    if(otherDcTerminalResult == math::TraverseResult::CONTINUE) {
                        addNextDcTerminals(otherDcTerminal, nextDcTerminals);
                    }
                    return otherDcTerminalResult;
                }
                return switchTraverseResult;
            }
        );

        if(traversalTerminated) {
            return false;
        }

        //Look into the next Dc Terminals
        for (DcTerminal& nextDcTerminal : nextDcTerminals) {
            if (!nextDcTerminal.traverse(traverser, traversedTerminals, traversalType)) {
                return false;
            }
        }
    }
    return true;
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
