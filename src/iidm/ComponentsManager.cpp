/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ComponentsManager.hpp>

#include <boost/range/adaptor/indirected.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/ComponentConstants.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/math/GraphUtil.hpp>

namespace powsybl {

namespace iidm {

ComponentsManager::ComponentsManager(Network& network) :
    m_network(network) {
}

void ComponentsManager::addToAdjacencyList(const stdcxx::CReference<Bus>& bus1, const stdcxx::CReference<Bus>& bus2,
    const std::map<std::string, long>& id2num, std::vector<std::vector<long>>& adjacencyList) const {
    if (static_cast<bool>(bus1) && static_cast<bool>(bus2)) {
        long busNum1 = id2num.at(bus1.get().getId());
        long busNum2 = id2num.at(bus2.get().getId());
        adjacencyList[busNum1].push_back(busNum2);
        adjacencyList[busNum2].push_back(busNum1);
    }
}

void ComponentsManager::fillAdjacencyList(const std::map<std::string, long>& id2num, std::vector<std::vector<long>>& adjacencyList) const {
    for (const auto& line : getNetwork().getLines()) {
        const auto& bus1 = line.getTerminal1().getBusView().getBus();
        const auto& bus2 = line.getTerminal2().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, id2num, adjacencyList);
    }
    for (const auto& transfo : getNetwork().getTwoWindingsTransformers()) {
        const auto& bus1 = transfo.getTerminal1().getBusView().getBus();
        const auto& bus2 = transfo.getTerminal2().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, id2num, adjacencyList);
    }
    for (const auto& transfo : getNetwork().getThreeWindingsTransformers()) {
        const auto& bus1 = transfo.getLeg1().getTerminal().get().getBusView().getBus();
        const auto& bus2 = transfo.getLeg2().getTerminal().get().getBusView().getBus();
        const auto& bus3 = transfo.getLeg3().getTerminal().get().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, id2num, adjacencyList);
        addToAdjacencyList(bus1, bus3, id2num, adjacencyList);
        addToAdjacencyList(bus2, bus3, id2num, adjacencyList);
    }
}

stdcxx::CReference<Component> ComponentsManager::getComponent(long num) const {
    return (num != ComponentConstants::INVALID_NUM) ? stdcxx::cref<Component>(m_components[num]) : stdcxx::CReference<Component>();
}

stdcxx::Reference<Component> ComponentsManager::getComponent(long num) {
    return (num != ComponentConstants::INVALID_NUM) ? stdcxx::ref<Component>(m_components[num]) : stdcxx::ref<Component>();
}

stdcxx::const_range<Component> ComponentsManager::getConnectedComponents() const {
    const_cast<ComponentsManager*>(this)->update();

    return boost::adaptors::indirect(m_components);
}

stdcxx::range<Component> ComponentsManager::getConnectedComponents() {
    update();

    return boost::adaptors::indirect(m_components);
}

const Network& ComponentsManager::getNetwork() const {
    return m_network;
}

Network& ComponentsManager::getNetwork() {
    return m_network;
}

void ComponentsManager::invalidate() {
    m_components.clear();
    m_components.shrink_to_fit();
}

void ComponentsManager::update() {
    if (!m_components.empty()) {
        return;
    }

    auto startTime = std::chrono::steady_clock::now();
    auto& logger = logging::LoggerFactory::getLogger<ComponentsManager>();

    //reset
    for (auto& b : getNetwork().getBusBreakerView().getBuses()) {
        setComponentNumber(b, ComponentConstants::INVALID_NUM);
    }

    long num = 0L;
    std::map<std::string, long> id2num;
    std::vector<std::reference_wrapper<Bus>> num2bus;

    for (auto& bus : getNetwork().getBusView().getBuses()) {
        num2bus.emplace_back(bus);
        id2num[bus.getId()] = num;
        ++num;
    }

    std::vector<std::vector<long>> adjacencyList(num);
    for (auto& v : adjacencyList) {
        v.reserve(3);
    }
    fillAdjacencyList(id2num, adjacencyList);

    const auto& result = math::GraphUtil::computeConnectedComponents(adjacencyList);
    const auto& componentNumbers = result.getComponentNumber();
    const auto& componentSizes = result.getComponentSize();

    m_components.reserve(componentSizes.size());
    for (unsigned long i = 0UL; i < componentSizes.size(); i++) {
        m_components.push_back(createComponent(i, componentSizes[i]));
    }

    for (unsigned long i = 0UL; i < componentNumbers.size(); i++) {
        auto& bus = num2bus[i].get();
        setComponentNumber(bus, componentNumbers[i]);
    }

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime);
    logger.info("%1% components computed in %2% ms", getComponentLabel(), duration.count());
}

}  // namespace iidm

}  // namespace powsybl
