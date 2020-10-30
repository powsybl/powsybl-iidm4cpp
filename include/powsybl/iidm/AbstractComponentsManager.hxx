/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ABSTRACTCOMPONENTSMANAGER_HXX
#define POWSYBL_IIDM_ABSTRACTCOMPONENTSMANAGER_HXX

#include <powsybl/iidm/AbstractComponentsManager.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/NetworkViews.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/math/GraphUtil.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

template <typename C>
AbstractComponentsManager<C>::AbstractComponentsManager(Network& network) :
    m_network(network) {
}

template <typename C>
void AbstractComponentsManager<C>::addToAdjacencyList(const stdcxx::CReference<Bus>& bus1, const stdcxx::CReference<Bus>& bus2, const std::map<std::string, unsigned long>& id2num, std::vector<std::vector<unsigned long>>& adjacencyList) const {
    if (bus1 && bus2) {
        unsigned long busNum1 = id2num.find(bus1.get().getId())->second;
        unsigned long busNum2 = id2num.find(bus2.get().getId())->second;
        adjacencyList[busNum1].emplace_back(busNum2);
        adjacencyList[busNum2].emplace_back(busNum1);
    }
}

template <typename C>
void AbstractComponentsManager<C>::fillAdjacencyList(const std::map<std::string, unsigned long>& id2num, std::vector<std::vector<unsigned long>>& adjacencyList) const {
    for (const Line& line : getNetwork().getLines()) {
        const auto& bus1 = line.getTerminal1().getBusView().getBus();
        const auto& bus2 = line.getTerminal2().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, id2num, adjacencyList);
    }
    for (const TwoWindingsTransformer& transfo : getNetwork().getTwoWindingsTransformers()) {
        const auto& bus1 = transfo.getTerminal1().getBusView().getBus();
        const auto& bus2 = transfo.getTerminal2().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, id2num, adjacencyList);
    }
    for (const ThreeWindingsTransformer& transfo : getNetwork().getThreeWindingsTransformers()) {
        const auto& bus1 = transfo.getLeg1().getTerminal().get().getBusView().getBus();
        const auto& bus2 = transfo.getLeg2().getTerminal().get().getBusView().getBus();
        const auto& bus3 = transfo.getLeg3().getTerminal().get().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, id2num, adjacencyList);
        addToAdjacencyList(bus1, bus3, id2num, adjacencyList);
        addToAdjacencyList(bus2, bus3, id2num, adjacencyList);
    }
}

template <typename C>
stdcxx::CReference<C> AbstractComponentsManager<C>::getComponent(const stdcxx::optional<unsigned long>& num) const {
    // update() must not be put here, but explicitly called each time before because update may
    // trigger a new component computation and so on a change in the value of the num component already passed
    // (and outdated consequently) in parameter of this method
    return num.is_initialized() ? stdcxx::cref(*m_components[*num]) : stdcxx::cref<C>();
}

template <typename C>
stdcxx::const_range<C> AbstractComponentsManager<C>::getConnectedComponents() const {
    update();
    return m_components | boost::adaptors::indirected;
}

template <typename C>
stdcxx::range<C> AbstractComponentsManager<C>::getConnectedComponents() {
    update();
    return m_components | boost::adaptors::indirected;
}

template <typename C>
const Network& AbstractComponentsManager<C>::getNetwork() const {
    return m_network.get();
}

template <typename C>
Network& AbstractComponentsManager<C>::getNetwork() {
    return m_network.get();
}

template <typename C>
void AbstractComponentsManager<C>::invalidate() {
    m_components.clear();
    m_components.shrink_to_fit();
}

template <typename C>
void AbstractComponentsManager<C>::update() {
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
        m_components.emplace_back(createComponent(i, componentSizes[i]));
    }

    for (unsigned long i = 0UL; i < componentNumbers.size(); i++) {
        Bus& bus = num2bus[i].get();
        setComponentNumber(bus, componentNumbers[i]);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = endTime - startTime;
    logger.debug(stdcxx::format("%1% components computed in %2% ms", getComponentLabel(), diff.count() * 1000.0));
}

template <typename C>
void AbstractComponentsManager<C>::setNetworkRef(Network& network) {
    m_network.set(network);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ABSTRACTCOMPONENTSMANAGER_HXX
