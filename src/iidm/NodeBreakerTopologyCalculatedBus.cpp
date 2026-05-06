/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerTopologyCalculatedBus.hpp"

#include <powsybl/AssertionError.hpp>
#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include "NodeBreakerTopologyModel.hpp"
#include "NodeTerminal.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_topology_model {

PowsyblException createSwitchNotFoundException(const std::string& switchId) {
    return PowsyblException(stdcxx::format("Switch %1% not found", switchId));
}

CalculatedBusBreakerTopology::CalculatedBusBreakerTopology(NodeBreakerTopologyModel& topologyModel) :
    CalculatedBusTopology(topologyModel) {

}

CalculatedBusTopology::SwitchPredicate CalculatedBusBreakerTopology::createSwitchPredicate() const {
    return [](const stdcxx::Reference<Switch>& aSwitch) {
        return aSwitch.get().isOpen() || aSwitch.get().isRetained();
    };
}

stdcxx::Reference<CalculatedBus> CalculatedBusBreakerTopology::getBus1(const std::string& switchId, bool throwException) {
    stdcxx::optional<unsigned long> e = getTopologyModel().getEdge(switchId, throwException);
    stdcxx::Reference<Switch> aSwitch = getRetainedSwitch(e);
    if (static_cast<bool>(aSwitch)) {
        unsigned long v = getTopologyModel().getGraph().getVertex1(*e);
        return getBus(v);
    }

    if (throwException) {
        throw createSwitchNotFoundException(switchId);
    }

    return stdcxx::ref<CalculatedBus>();
}

stdcxx::Reference<CalculatedBus> CalculatedBusBreakerTopology::getBus2(const std::string& switchId, bool throwException) {
    stdcxx::optional<unsigned long> e = getTopologyModel().getEdge(switchId, throwException);
    stdcxx::Reference<Switch> aSwitch = getRetainedSwitch(e);
    if (static_cast<bool>(aSwitch)) {
        unsigned long v = getTopologyModel().getGraph().getVertex2(*e);
        return getBus(v);
    }

    if (throwException) {
        throw createSwitchNotFoundException(switchId);
    }

    return stdcxx::ref<CalculatedBus>();
}

stdcxx::Reference<Switch> CalculatedBusBreakerTopology::getRetainedSwitch(const stdcxx::optional<unsigned long>& e) const {
    if (e) {
        const auto& aSwitch = getTopologyModel().getGraph().getEdgeObject(*e);
        if (aSwitch.get().isRetained()) {
            return aSwitch;
        }
    }

    return stdcxx::ref<Switch>();
}

stdcxx::CReference<Switch> CalculatedBusBreakerTopology::getSwitch(const std::string& switchId, bool throwException) const {
    stdcxx::optional<unsigned long> e = getTopologyModel().getEdge(switchId, false);
    stdcxx::Reference<Switch> aSwitch = getRetainedSwitch(e);
    if (throwException && !aSwitch) {
        throw createSwitchNotFoundException(switchId);
    }

    return stdcxx::cref<Switch>(aSwitch);
}

unsigned long CalculatedBusBreakerTopology::getSwitchCount() const {
    unsigned long switchCount = 0;

    for (const auto& sw : getTopologyModel().getGraph().getEdgeObjects()) {
        if (static_cast<bool>(sw) && sw.get().isRetained()) {
            ++switchCount;
        }
    }

    return switchCount;
}

stdcxx::const_range<Switch> CalculatedBusBreakerTopology::getSwitches() const {
    const auto& filter = [](const stdcxx::Reference<Switch>& sw) {
        return static_cast<bool>(sw) && sw.get().isRetained();
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<Switch>, Switch>;

    return getTopologyModel().getGraph().getEdgeObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

stdcxx::range<Switch> CalculatedBusBreakerTopology::getSwitches() {
    const auto& filter = [](const stdcxx::Reference<Switch>& sw) {
        return static_cast<bool>(sw) && sw.get().isRetained();
    };
    const auto& mapper = stdcxx::map<stdcxx::Reference<Switch>, Switch>;

    return getTopologyModel().getGraph().getEdgeObjects() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

bool CalculatedBusBreakerTopology::isBusValid(const node_breaker_topology_model::Graph& /*graph*/, const std::vector<unsigned long>& vertices, const std::vector<std::reference_wrapper<NodeTerminal> >& /*terminals*/) const {
    return !vertices.empty();
}

CalculatedBusTopology::CalculatedBusTopology(NodeBreakerTopologyModel& topologyModel) :
    m_topologyModel(topologyModel) {
}

CalculatedBusTopology::SwitchPredicate CalculatedBusTopology::createSwitchPredicate() const {
    return [](const stdcxx::Reference<Switch>& aSwitch) {
        return aSwitch.get().isOpen();
    };
}

stdcxx::Reference<CalculatedBus> CalculatedBusTopology::getBus(unsigned long node) {
    updateCache();

    return m_cache->getBus(node);
}

stdcxx::Reference<CalculatedBus> CalculatedBusTopology::getBus(const std::string& id, bool throwException) {
    updateCache();

    stdcxx::Reference<CalculatedBus> bus = m_cache->getBus(id);
    if (throwException && !bus) {
        throw PowsyblException(stdcxx::format("Bus %1% not found in voltage level %2%", id, m_topologyModel.getVoltageLevel().getId()));
    }

    return bus;
}

unsigned long CalculatedBusTopology::getBusCount() {
    updateCache();

    return m_cache->getBusCount();
}

stdcxx::range<CalculatedBus> CalculatedBusTopology::getBuses() {
    updateCache();

    return m_cache->getBuses();
}

stdcxx::Reference<Bus> CalculatedBusTopology::getConnectableBus(unsigned long node) {
    // check if the node is associated to a bus
    stdcxx::Reference<CalculatedBus> connectableBus = getBus(node);
    if (static_cast<bool>(connectableBus)) {
        return stdcxx::ref<Bus>(connectableBus);
    }

    // if not traverse the graph starting from the node (without stopping at open switches) until finding another
    // node associated to a bus
    const auto& graph = m_topologyModel.getGraph();
    graph.traverse(node, math::TraversalType::DEPTH_FIRST, [this, &connectableBus](unsigned long /*v1*/, unsigned long /*e*/, unsigned long v2) {
        if (static_cast<bool>(connectableBus)) {
            // traverse does not stop the algorithm when TERMINATE, it only stops searching in a given direction
            // this condition insures that while checking all the edges (in every direction) of a node, if a bus is found, it will not be lost
            return math::TraverseResult::TERMINATE_PATH;
        }
        connectableBus = getBus(v2);

        return static_cast<bool>(connectableBus) ? math::TraverseResult::TERMINATE_PATH : math::TraverseResult::CONTINUE;
    });

    // if nothing found, just take the first bus
    if (! static_cast<bool>(connectableBus)) {
        const auto& buses = getBuses();
        if (!boost::empty(buses)) {
            return stdcxx::ref<Bus>(buses.front());
        }

        return stdcxx::ref<Bus>();
    }

    return stdcxx::ref<Bus>(connectableBus);
}

const NodeBreakerTopologyModel& CalculatedBusTopology::getTopologyModel() const {
    return m_topologyModel;
}

NodeBreakerTopologyModel& CalculatedBusTopology::getTopologyModel() {
    return m_topologyModel;
}

void CalculatedBusTopology::invalidateCache() {
    if (static_cast<bool>(m_cache)) {
        for (auto& bus : m_cache->getBuses()) {
            bus.invalidate();
        }
        m_cache.reset();
    }
}

bool CalculatedBusTopology::isBusValid(const node_breaker_topology_model::Graph& graph, const std::vector<unsigned long>& vertices, const std::vector<std::reference_wrapper<NodeTerminal> >& /*terminals*/) const {
    unsigned long feederCount = 0;
    unsigned long branchCount = 0;
    unsigned long busbarSectionCount = 0;

    for (unsigned long vertex : vertices) {
        const auto& terminal = graph.getVertexObject(vertex);
        if (static_cast<bool>(terminal)) {
            const auto& connectable = terminal.get().getConnectable();
            const auto& connectableType = connectable.get().getType();

            switch (connectableType) {
                case IdentifiableType::LINE:
                case IdentifiableType::TWO_WINDINGS_TRANSFORMER:
                case IdentifiableType::THREE_WINDINGS_TRANSFORMER:
                case IdentifiableType::HVDC_CONVERTER_STATION:
                case IdentifiableType::DANGLING_LINE:
                    ++branchCount;
                    ++feederCount;
                    break;

                case IdentifiableType::LOAD:
                case IdentifiableType::GENERATOR:
                case IdentifiableType::BATTERY:
                case IdentifiableType::SHUNT_COMPENSATOR:
                case IdentifiableType::STATIC_VAR_COMPENSATOR:
                case IdentifiableType::LINE_COMMUTATED_CONVERTER:
                case IdentifiableType::VOLTAGE_SOURCE_CONVERTER:
                    ++feederCount;
                    break;

                case IdentifiableType::BUSBAR_SECTION:
                    ++busbarSectionCount;
                    break;
                
                case IdentifiableType::GROUND:
                    break;

                case IdentifiableType::NETWORK:
                case IdentifiableType::SUBSTATION:
                case IdentifiableType::VOLTAGE_LEVEL:
                case IdentifiableType::AREA:
                case IdentifiableType::OVERLOAD_MANAGEMENT_SYSTEM:
                case IdentifiableType::HVDC_LINE:
                case IdentifiableType::BUS:
                case IdentifiableType::SWITCH:
                case IdentifiableType::TIE_LINE:
                case IdentifiableType::DC_NODE:
                case IdentifiableType::DC_SWITCH:
                case IdentifiableType::DC_GROUND:
                case IdentifiableType::DC_LINE:
                default:
                    throw AssertionError(stdcxx::format("Unexpected IdentifiableType %1%", connectableType));
            }
        }
    }
    return (busbarSectionCount >= 1 && feederCount >= 1) ||
           (branchCount >= 1 && feederCount >= 2);
}

void CalculatedBusTopology::traverse(unsigned long v, std::vector<bool>& encountered, const CalculatedBusTopology::SwitchPredicate& terminate, BusCache::CalculatedBusById& busById, BusCache::CalculatedBusByNode& busByNode) {
    if (!encountered[v]) {
        std::vector<unsigned long> vertices(1, v);

        const auto& graph = m_topologyModel.getGraph();
        graph.traverse(v, math::TraversalType::DEPTH_FIRST, [&graph, &terminate, &vertices, &encountered](unsigned long /*v1*/, unsigned long e, unsigned long v2) {
            const stdcxx::Reference<Switch> aSwitch = graph.getEdgeObject(e);
            if (static_cast<bool>(aSwitch) && terminate(aSwitch)) {
                return math::TraverseResult::TERMINATE_PATH;
            }

            if(!encountered[v2]){
                //We need to check this as the traverser might be called twice with the same v2 from different edge
                //Note the 'encountered'  array is managed by graph::traverse so we should not update it there
                vertices.push_back(v2);
            }

            return math::TraverseResult::CONTINUE;
        }, encountered);

        const Network& network = m_topologyModel.getNetwork();
        std::string busId = Identifiables::getUniqueId(m_topologyModel.getBusNamingStrategy().getId(vertices),[&network](const std::string& id){
            return static_cast<bool>(network.find(id));
        });
        std::vector<std::reference_wrapper<NodeTerminal> > terminals;
        terminals.reserve(vertices.size());
        for (unsigned long vertex : vertices) {
            const auto& terminal = graph.getVertexObject(vertex);
            if (static_cast<bool>(terminal)) {
                terminals.emplace_back(std::ref(terminal.get()));
            }
        }

        if (isBusValid(graph, vertices, terminals)) {
            std::string busName = m_topologyModel.getBusNamingStrategy().getName(vertices);
            std::function<stdcxx::CReference<Bus>(stdcxx::CReference<Terminal>)> getBusFromTerminal = [](stdcxx::CReference<Terminal> term){
                return term.get().getBusView().getBus();
            };
            std::unique_ptr<CalculatedBus> ptrCalculatedBus = stdcxx::make_unique<CalculatedBus>(busId, busName, m_topologyModel.getVoltageLevel().isFictitious(), m_topologyModel.getVoltageLevel(), vertices, std::move(terminals),getBusFromTerminal);
            const auto& it = busById.insert(std::make_pair(busId, std::move(ptrCalculatedBus)));
            const stdcxx::Reference<CalculatedBus>& calculatedBus = stdcxx::ref(*it.first->second);

            for (unsigned long vertex : vertices) {
                busByNode[vertex] = calculatedBus;
            }
        }
    }
}

void CalculatedBusTopology::updateCache() {
    updateCache(createSwitchPredicate());
}

void CalculatedBusTopology::updateCache(const SwitchPredicate& predicate) {
    if (static_cast<bool>(m_cache)) {
        return;
    }

    logging::Logger& logger = logging::LoggerFactory::getLogger<CalculatedBusTopology>();
    logger.trace(stdcxx::format("Update bus topology of voltage level %1%", m_topologyModel.getVoltageLevel().getId()));

    const auto& graph = m_topologyModel.getGraph();

    BusCache::CalculatedBusById busById;
    BusCache::CalculatedBusByNode busByNode(graph.getMaxVertex());

    std::vector<bool> encountered(graph.getMaxVertex(), false);
    for (unsigned long v : graph.getVertices()) {
        traverse(v, encountered, predicate, busById, busByNode);
    }

    m_cache = stdcxx::make_unique<BusCache>(std::move(busByNode), std::move(busById));

    logger.trace(stdcxx::format("Found buses %1%", stdcxx::toString<CalculatedBus>(m_cache->getBuses())));
}

}  // namespace node_breaker_topology_model

}  // namespace iidm

}  // namespace powsybl
