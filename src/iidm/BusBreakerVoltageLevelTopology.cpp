/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusBreakerVoltageLevelTopology.hpp"

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/math/TraverseResult.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/reference.hpp>
#include <powsybl/stdcxx/set.hpp>

#include "BusBreakerVoltageLevel.hpp"

namespace powsybl {

namespace iidm {

namespace bus_breaker_voltage_level {

CalculatedBusTopology::CalculatedBusTopology(BusBreakerVoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

std::unique_ptr<MergedBus> CalculatedBusTopology::createMergedBus(unsigned long busCount, const MergedBus::BusSet& busSet) const {
    const std::string& mergedBusId = stdcxx::format("%1%_%2%", m_voltageLevel.getId(), busCount);
    const std::string& mergedBusName = m_voltageLevel.getOptionalName().empty() ? "" : stdcxx::format("%1%_%2%", m_voltageLevel.getOptionalName(), busCount);

    return stdcxx::make_unique<MergedBus>(mergedBusId, mergedBusName, m_voltageLevel.isFictitious(), busSet);
}

stdcxx::Reference<MergedBus> CalculatedBusTopology::getMergedBus(const std::string& id, bool throwException) {
    updateCache();

    stdcxx::Reference<MergedBus> bus = m_cache->getMergedBus(id);
    if (throwException && !bus) {
        throw PowsyblException(stdcxx::format("Bus %1% not found in voltage level %2%", id, m_voltageLevel.getId()));
    }

    return bus;
}

stdcxx::Reference<MergedBus> CalculatedBusTopology::getMergedBus(const stdcxx::Reference<ConfiguredBus>& bus) {
    updateCache();

    return m_cache->getMergedBus(bus);
}

stdcxx::range<MergedBus> CalculatedBusTopology::getMergedBuses() {
    updateCache();

    return m_cache->getMergedBuses();
}

void CalculatedBusTopology::invalidateCache() {
    if (static_cast<bool>(m_cache)) {
        for (auto& bus : m_cache->getMergedBuses()) {
            bus.invalidate();
        }

        m_cache.reset();
    }
}

bool CalculatedBusTopology::isBusValid(const MergedBus::BusSet& buses) const {
    unsigned long branchCount = 0;

    for (const auto& bus : buses) {
        for (const auto& terminal : bus.get().getConnectedTerminals()) {
            const auto& connectable = terminal.getConnectable().get();
            switch (connectable.getType()) {
                case ConnectableType::LINE:
                case ConnectableType::TWO_WINDINGS_TRANSFORMER:
                case ConnectableType::THREE_WINDINGS_TRANSFORMER:
                case ConnectableType::HVDC_CONVERTER_STATION:
                case ConnectableType::DANGLING_LINE:
                    ++branchCount;
                    break;

                case ConnectableType::LOAD:
                case ConnectableType::GENERATOR:
                case ConnectableType::BATTERY:
                case ConnectableType::SHUNT_COMPENSATOR:
                case ConnectableType::STATIC_VAR_COMPENSATOR:
                    break;

                case ConnectableType::BUSBAR_SECTION: // must not happen in a bus/breaker topology
                    throw AssertionError(stdcxx::format("Unexpected ConnectableType value: %1%", connectable.getType()));
            }
        }
    }

    return branchCount >= 1;
}

void CalculatedBusTopology::updateCache() {
    if (static_cast<bool>(m_cache)) {
        return;
    }

    unsigned long busCount = 0;

    BusCache::MergedBusById mergedBuses;
    BusCache::MergedBusByConfiguredBus mapping;

    const auto& graph = m_voltageLevel.getGraph();

    std::vector<bool> encountered(graph.getVertexCount(), false);
    for (unsigned long v : graph.getVertices()) {
        if (!encountered[v]) {

            MergedBus::BusSet busSet;
            busSet.push_back(std::ref(graph.getVertexObject(v).get()));

            graph.traverse(v, [&busSet, &graph](unsigned long /*v1*/, unsigned long e, unsigned long v2) {
                stdcxx::Reference<Switch> aSwitch = graph.getEdgeObject(e);
                if (aSwitch.get().isOpen()) {
                    return math::TraverseResult::TERMINATE;
                }

                busSet.push_back(std::ref(graph.getVertexObject(v2).get()));
                return math::TraverseResult::CONTINUE;
            }, encountered);

            if (isBusValid(busSet)) {
                std::unique_ptr<MergedBus> ptrMergedBus = createMergedBus(busCount++, busSet);

                const auto& it = mergedBuses.insert(std::make_pair(ptrMergedBus->getId(), std::move(ptrMergedBus)));
                const std::reference_wrapper<MergedBus>& mergedBus = std::ref(*it.first->second);

                std::for_each(busSet.begin(), busSet.end(), [&mapping, &mergedBus](const std::reference_wrapper<ConfiguredBus>& bus) {
                    mapping.insert(std::make_pair(bus, mergedBus));
                });
            }
        }
    }

    m_cache = stdcxx::make_unique<BusCache>(std::move(mergedBuses), std::move(mapping));
}

}  // namespace bus_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl
