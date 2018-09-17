/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerVoltageLevelTopology.hpp"

#include <powsybl/AssertionError.hpp>
#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include "NodeBreakerVoltageLevel.hpp"
#include "NodeTerminal.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

PowsyblException createSwitchNotFoundException(const std::string& switchId) {
    return PowsyblException(logging::format("Switch %1% not found", switchId));
}

CalculatedBusBreakerTopology::CalculatedBusBreakerTopology(NodeBreakerVoltageLevel& voltageLevel) :
    CalculatedBusTopology(voltageLevel) {

}

CalculatedBusTopology::SwitchPredicate CalculatedBusBreakerTopology::createSwitchPredicate() const {
    return [](const stdcxx::Reference<Switch>& aSwitch) {
        return aSwitch.get().isOpen() || aSwitch.get().isRetained();
    };
}

stdcxx::Reference<CalculatedBus> CalculatedBusBreakerTopology::getBus1(const std::string& switchId, bool throwException) {
    stdcxx::optional<unsigned long> e = getVoltageLevel().getEdge(switchId, throwException);
    stdcxx::Reference<Switch> aSwitch = getRetainedSwitch(e);
    if (static_cast<bool>(aSwitch)) {
        unsigned long v = getVoltageLevel().getGraph().getVertex1(*e);
        return getBus(v);
    }

    if (throwException) {
        throw createSwitchNotFoundException(switchId);
    }

    return stdcxx::ref<CalculatedBus>();
}

stdcxx::Reference<CalculatedBus> CalculatedBusBreakerTopology::getBus2(const std::string& switchId, bool throwException) {
    stdcxx::optional<unsigned long> e = getVoltageLevel().getEdge(switchId, throwException);
    stdcxx::Reference<Switch> aSwitch = getRetainedSwitch(e);
    if (static_cast<bool>(aSwitch)) {
        unsigned long v = getVoltageLevel().getGraph().getVertex2(*e);
        return getBus(v);
    }

    if (throwException) {
        throw createSwitchNotFoundException(switchId);
    }

    return stdcxx::ref<CalculatedBus>();
}

stdcxx::Reference<Switch> CalculatedBusBreakerTopology::getRetainedSwitch(const stdcxx::optional<unsigned long>& e) const {
    if (static_cast<bool>(e)) {
        const auto& aSwitch = getVoltageLevel().getGraph().getEdgeObject(*e);
        if (aSwitch.get().isRetained()) {
            return aSwitch;
        }
    }

    return stdcxx::ref<Switch>();
}

stdcxx::Reference<Switch> CalculatedBusBreakerTopology::getSwitch(const std::string& switchId, bool throwException) {
    stdcxx::optional<unsigned long> e = getVoltageLevel().getEdge(switchId, false);
    stdcxx::Reference<Switch> aSwitch = getRetainedSwitch(e);
    if (throwException && !aSwitch) {
        throw createSwitchNotFoundException(switchId);
    }

    return aSwitch;
}

bool CalculatedBusBreakerTopology::isBusValid(const node_breaker_voltage_level::Graph& /*graph*/, const std::vector<unsigned long>& vertices, const std::vector<std::reference_wrapper<NodeTerminal> >& /*terminals*/) const {
    return !vertices.empty();
}

CalculatedBusTopology::CalculatedBusTopology(powsybl::iidm::NodeBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
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
        throw PowsyblException(logging::format("Bus %1% not found in voltage level %2%", id, m_voltageLevel.getId()));
    }

    return bus;
}

std::vector<std::reference_wrapper<CalculatedBus> > CalculatedBusTopology::getBuses() {
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
    const auto& graph = m_voltageLevel.getGraph();
    graph.traverse(node, [this, &connectableBus](unsigned long /*v1*/, unsigned long /*e*/, unsigned long v2) {
        connectableBus = getBus(v2);

        return static_cast<bool>(connectableBus) ? math::TraverseResult::TERMINATE : math::TraverseResult::CONTINUE;
    });

    // if nothing found, just take the first bus
    if (! static_cast<bool>(connectableBus)) {
        const auto& buses = getBuses();
        if (buses.empty()) {
            throw AssertionError("Should not happen");
        }

        return stdcxx::ref<Bus>(buses.at(0));
    }

    return stdcxx::ref<Bus>(connectableBus);
}

const NodeBreakerVoltageLevel& CalculatedBusTopology::getVoltageLevel() const {
    return m_voltageLevel;
}

NodeBreakerVoltageLevel& CalculatedBusTopology::getVoltageLevel() {
    return m_voltageLevel;
}

void CalculatedBusTopology::invalidateCache() {
    if (static_cast<bool>(m_cache)) {
        for (auto& bus : m_cache->getBuses()) {
            bus.get().invalidate();
        }
        m_cache.reset();
    }
}

bool CalculatedBusTopology::isBusValid(const node_breaker_voltage_level::Graph& graph, const std::vector<unsigned long>& vertices, const std::vector<std::reference_wrapper<NodeTerminal> >& /*terminals*/) const {
    unsigned long feederCount = 0;
    unsigned long branchCount = 0;
    unsigned long busbarSectionCount = 0;

    for (unsigned long vertex : vertices) {
        const auto& terminal = graph.getVertexObject(vertex);
        if (static_cast<bool>(terminal)) {
            const auto& connectable = terminal.get().getConnectable();
            const auto& connectableType = connectable.get().getConnectableType();

            switch (connectableType) {
                case ConnectableType::LINE:
                case ConnectableType::TWO_WINDINGS_TRANSFORMER:
                case ConnectableType::THREE_WINDINGS_TRANSFORMER:
                case ConnectableType::HVDC_CONVERTER_STATION:
                    ++branchCount;
                    ++feederCount;
                    break;

                case ConnectableType::LOAD:
                case ConnectableType::GENERATOR:
                case ConnectableType::SHUNT_COMPENSATOR:
                case ConnectableType::DANGLING_LINE:
                case ConnectableType::STATIC_VAR_COMPENSATOR:
                    ++feederCount;
                    break;

                case ConnectableType::BUSBAR_SECTION:
                    ++busbarSectionCount;
                    break;

                default:
                    throw AssertionError(logging::format("Unexpected ConnectableType value: %1%", connectableType));
            }
        }
    }
    return (busbarSectionCount >= 1 && feederCount >= 1) ||
           (branchCount >= 1 && feederCount >= 2);
}

void CalculatedBusTopology::traverse(unsigned long v, std::vector<bool>& encountered, const CalculatedBusTopology::SwitchPredicate& terminate, BusCache::CalculatedBusById& busById, BusCache::CalculatedBusByNode& busByNode) {
    if (!encountered[v]) {
        std::vector<unsigned long> vertices(1, v);

        const auto& graph = m_voltageLevel.getGraph();
        graph.traverse(v, [&graph, &terminate, &vertices](unsigned long /*v1*/, unsigned long e, unsigned long v2) {
            const stdcxx::Reference<Switch> aSwitch = graph.getEdgeObject(e);
            if (static_cast<bool>(aSwitch) && terminate(aSwitch)) {
                return math::TraverseResult::TERMINATE;
            }

            vertices.push_back(v2);
            return math::TraverseResult::CONTINUE;
        }, encountered);

        std::string busId = m_voltageLevel.getBusNamingStrategy().getName();
        std::vector<std::reference_wrapper<NodeTerminal> > terminals;
        terminals.reserve(vertices.size());
        for (unsigned long vertex : vertices) {
            const auto& terminal = graph.getVertexObject(vertex);
            if (static_cast<bool>(terminal)) {
                terminals.emplace_back(std::ref(terminal.get()));
            }
        }

        if (isBusValid(graph, vertices, terminals)) {
            std::unique_ptr<CalculatedBus> ptrCalculatedBus = stdcxx::make_unique<CalculatedBus>(busId, m_voltageLevel, std::move(terminals));
            const auto& it = busById.insert(std::make_pair(busId, std::move(ptrCalculatedBus)));
            const std::reference_wrapper<CalculatedBus>& calculatedBus = std::ref(*it.first->second);

            for (unsigned long vertex : vertices) {
                busByNode[vertex] = calculatedBus;
            }
        }
    }
}

void CalculatedBusTopology::updateCache() {
    updateCache(createSwitchPredicate());
}

void CalculatedBusTopology::updateCache(const SwitchPredicate& terminate) {
    if (static_cast<bool>(m_cache)) {
        return;
    }

    logging::Logger& logger = logging::LoggerFactory::getLogger<CalculatedBusTopology>();
    logger.trace(logging::format("Update bus topology of voltage level %1%", m_voltageLevel.getId()));

    const auto& graph = m_voltageLevel.getGraph();

    BusCache::CalculatedBusById busById;
    BusCache::CalculatedBusByNode busByNode;
    busByNode.reserve(graph.getMaxVertex());

    std::vector<bool> encountered(graph.getMaxVertex(), false);
    for (unsigned long e : graph.getEdges()) {
        traverse(graph.getVertex1(e), encountered, terminate, busById, busByNode);
        traverse(graph.getVertex2(e), encountered, terminate, busById, busByNode);
    }

    m_cache = stdcxx::make_unique<BusCache>(std::move(busByNode), std::move(busById));

    // TODO(mathbagu) logger.trace(logging::format("Found buses %1%", logging::toString(m_cache->getBuses())));
}

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl
