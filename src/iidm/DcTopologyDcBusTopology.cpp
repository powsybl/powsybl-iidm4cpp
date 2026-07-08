/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcTopologyDcBusTopology.hpp>


#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/DcSwitch.hpp>
#include <powsybl/iidm/DcTopologyModel.hpp>
#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/util/Identifiables.hpp>

namespace powsybl {

namespace iidm {

namespace dc_topology_model {

DcBusTopology::DcBusTopology(DcTopologyModel& dcTopologyModel) :
    m_dcTopologyModel(dcTopologyModel) {
}

DcBusTopology::DcBusTopology(DcTopologyModel& dcTopologyModel, DcBusTopology&& dcBusTopology) noexcept :
    m_dcTopologyModel(dcTopologyModel),
    m_cache(std::move(dcBusTopology.m_cache)) {
}

unsigned long DcBusTopology::getDcBusCount() {
    updateCache();

    return m_cache->getDcBusCount();
}

stdcxx::Reference<DcBus> DcBusTopology::getDcBus(const std::string& dcBusId) {
    updateCache();

    return m_cache->getDcBus(dcBusId);
}

stdcxx::Reference<DcBus> DcBusTopology::getDcBusOfDcNode(const std::string& dcNodeId) {
    updateCache();

    return m_cache->getDcBusOfDcNode(dcNodeId);
}

stdcxx::range<DcBus> DcBusTopology::getDcBuses() {
    updateCache();

    return m_cache->getDcBuses();
}

void DcBusTopology::invalidateCache() {
    if (static_cast<bool>(m_cache)) {
        for (auto& dcBus : m_cache->getDcBuses()) {
            dcBus.invalidate();
        }
        m_cache.reset();
    }
}

void DcBusTopology::updateCache() {
    if (static_cast<bool>(m_cache)) {
        return;
    }

    DcBusCache::DcBusById dcBusById; // std::map<std::string, std::unique_ptr<DcBus> >;
    //mapping between Dc Nodes and Dc Buses
    DcBusCache::DcBusByNodeId dcBusByNode; // std::map<std::string, stdcxx::Reference<DcBus> >;

    const auto& graph = m_dcTopologyModel.m_graph;

    std::vector<bool> encountered(graph->getVertexCapacity(), false);
    for (unsigned long v : graph->getVertices()) {
        if(!encountered[v]) {

            DcBus::DcNodeSet dcNodeSet;
            dcNodeSet.push_back(std::ref(graph->getVertexObject(v).get()));

            graph->traverse(v, math::TraversalType::DEPTH_FIRST, [&dcNodeSet, &graph](unsigned long /*v1*/, unsigned long e, unsigned long v2) {
                stdcxx::Reference<DcSwitch> dcSwitch = graph->getEdgeObject(e);
                if(!dcSwitch || dcSwitch.get().isOpen()) {
                    return math::TraverseResult::TERMINATE_PATH; 
                }
                dcNodeSet.push_back(std::ref(graph->getVertexObject(v2).get()));
                return math::TraverseResult::CONTINUE;
            }, encountered);

            if(isDcBusValid(dcNodeSet)) {
                std::unique_ptr<DcBus> ptrDcBus = createDcBus(dcNodeSet);

                const auto& it = dcBusById.insert(std::make_pair(ptrDcBus->getId(), std::move(ptrDcBus)));
                const std::reference_wrapper<DcBus>& dcBus = std::ref(*it.first->second);

                std::for_each(dcNodeSet.begin(), dcNodeSet.end(), [&dcBusByNode, &dcBus](const std::reference_wrapper<DcNode>& dcNode) {
                    dcBusByNode.insert(std::make_pair(dcNode.get().getId(), dcBus));
                });
            }
        }
    }

    m_cache = stdcxx::make_unique<DcBusCache>(std::move(dcBusById), std::move(dcBusByNode));
}

std::unique_ptr<DcBus> DcBusTopology::createDcBus(const DcBus::DcNodeSet& dcNodes) const {
    if(dcNodes.empty()) {
        throw PowsyblException("DcBus set of DcNodes is empty");
    }

    Network& network = m_dcTopologyModel.getNetwork();
    //Use ordered set to retrieve the "lowest" node (by Id)
    std::set<std::string> dcNodesIds;
    for (const auto& dcNode : dcNodes) {
        dcNodesIds.emplace(dcNode.get().getId());
    }
    const DcNode& lowestNode = network.getDcNode(*dcNodesIds.begin()); 

    const std::string& dcBusId = Identifiables::getUniqueId( stdcxx::format("%1%_dcBus", lowestNode.getId()), [&network](const std::string& id) {
            return static_cast<bool>(network.find(id));
    });

    return stdcxx::make_unique<DcBus>(dcBusId, lowestNode.getOptionalName(), lowestNode.isFictitious(), dcNodes);
}

bool DcBusTopology::isDcBusValid(const DcBus::DcNodeSet& dcNodes) const {
    //DcBus is valid if at least one DcConnectable is connected, i.e. at least one connected DcTerminal
    for (const auto& dcNode : dcNodes) {
        if(dcNode.get().getConnectedDcTerminalCount() > 0) {
            return true;
        }
    }
    return false;
}

}  // namespace dc_topology_model

}  // namespace iidm

}  // namespace powsybl
