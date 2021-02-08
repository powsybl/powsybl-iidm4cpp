/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/AbstractComponentsManager.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Component.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/math/GraphUtil.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

AbstractComponentsManager::AbstractComponentsManager(Network& network, const ComponentFactory& factory) :
    m_network(network),
    m_factory(factory) {
}

AbstractComponentsManager::AbstractComponentsManager(Network& network, AbstractComponentsManager&& manager) noexcept :
    m_network(network),
    m_factory(std::move(manager.m_factory)) {
    for (auto& component : manager.m_components) {
        m_components.emplace_back(m_factory(network, component->getNum(), component->getSize()));
    }
}

void AbstractComponentsManager::addToAdjacencyList(const stdcxx::CReference<Bus>& bus1, const stdcxx::CReference<Bus>& bus2, const std::map<std::string, unsigned long>& id2num, std::vector<std::vector<unsigned long>>& adjacencyList) const {
    if (bus1 && bus2) {
        unsigned long busNum1 = id2num.find(bus1.get().getId())->second;
        unsigned long busNum2 = id2num.find(bus2.get().getId())->second;
        adjacencyList[busNum1].emplace_back(busNum2);
        adjacencyList[busNum2].emplace_back(busNum1);
    }
}

void AbstractComponentsManager::fillAdjacencyList(const std::map<std::string, unsigned long>& id2num, std::vector<std::vector<unsigned long>>& adjacencyList) const {
    for (const Line& line : getNetwork().getLines()) {
        const auto& bus1 = line.getTerminal1().getBusView().getBus();
        const auto& bus2 = line.getTerminal2().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, id2num, adjacencyList);
    }
    for (const TwoWindingsTransformer& twt : getNetwork().getTwoWindingsTransformers()) {
        const auto& bus1 = twt.getTerminal1().getBusView().getBus();
        const auto& bus2 = twt.getTerminal2().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, id2num, adjacencyList);
    }
    for (const ThreeWindingsTransformer& twt : getNetwork().getThreeWindingsTransformers()) {
        const auto& bus1 = twt.getLeg1().getTerminal().getBusView().getBus();
        const auto& bus2 = twt.getLeg2().getTerminal().getBusView().getBus();
        const auto& bus3 = twt.getLeg3().getTerminal().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, id2num, adjacencyList);
        addToAdjacencyList(bus1, bus3, id2num, adjacencyList);
        addToAdjacencyList(bus2, bus3, id2num, adjacencyList);
    }
}

stdcxx::CReference<Component> AbstractComponentsManager::getComponent(const stdcxx::optional<unsigned long>& num) const {
    // update() must not be put here, but explicitly called each time before because update may
    // trigger a new component computation and so on a change in the value of the num component already passed
    // (and outdated consequently) in parameter of this method
    return num ? stdcxx::cref(*m_components[*num]) : stdcxx::cref<Component>();
}

stdcxx::const_range<Component> AbstractComponentsManager::getConnectedComponents() const {
    const_cast<AbstractComponentsManager*>(this)->update();

    return m_components | boost::adaptors::indirected;
}

stdcxx::range<Component> AbstractComponentsManager::getConnectedComponents() {
    update();
    return m_components | boost::adaptors::indirected;
}

const Network& AbstractComponentsManager::getNetwork() const {
    return m_network;
}

Network& AbstractComponentsManager::getNetwork() {
    return m_network;
}

void AbstractComponentsManager::invalidate() {
    m_components.clear();
    m_components.shrink_to_fit();
}

void AbstractComponentsManager::update() {
    if (!m_components.empty()) {
        return;
    }

    logging::Logger& logger = logging::LoggerFactory::getLogger<AbstractComponentsManager>();

    auto startTime = std::chrono::high_resolution_clock::now();

    // reset
    for (Bus& bus : getNetwork().getBusBreakerView().getBuses()) {
        setComponentNumber(bus, stdcxx::optional<unsigned long>());
    }

    unsigned long num = 0UL;
    std::map<std::string, unsigned long> id2num;
    std::vector<std::reference_wrapper<Bus>> num2bus;
    for (Bus& bus : getNetwork().getBusView().getBuses()) {
        num2bus.emplace_back(std::ref(bus));
        id2num[bus.getId()] = num;
        num++;
    }
    std::vector<std::vector<unsigned long>> adjacencyList(num);
    for (auto& v : adjacencyList) {
        v.reserve(3);
    }
    fillAdjacencyList(id2num, adjacencyList);

    const auto& result = math::GraphUtil::computeConnectedComponents(adjacencyList);
    const auto& componentNumbers = result.getComponentNumber();
    const auto& componentSizes = result.getComponentSize();

    m_components.reserve(componentSizes.size());
    for (unsigned long i = 0UL; i < componentSizes.size(); i++) {
        m_components.emplace_back(m_factory(m_network, i, componentSizes[i]));
    }

    for (unsigned long i = 0UL; i < componentNumbers.size(); i++) {
        Bus& bus = num2bus[i].get();
        setComponentNumber(bus, componentNumbers[i]);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = endTime - startTime;
    logger.debug(stdcxx::format("%1% components computed in %2% ms", getComponentLabel(), diff.count() * 1000.0));
}

}  // namespace iidm

}  // namespace powsybl
