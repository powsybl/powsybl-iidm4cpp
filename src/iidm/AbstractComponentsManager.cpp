/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/AbstractComponentsManager.hpp>

#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Component.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DcBus.hpp>
#include <powsybl/iidm/DcLine.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/math/GraphUtil.hpp>
#include <powsybl/stdcxx/format.hpp>

#include <chrono>

namespace powsybl {

namespace iidm {

AbstractComponentsManager::AbstractComponentsManager(Network& network, const ComponentFactory& factory, bool ac, bool dc) :
    m_network(network),
    m_factory(factory),
    m_ac(ac),
    m_dc(dc) {
}

AbstractComponentsManager::AbstractComponentsManager(Network& network, AbstractComponentsManager&& manager, bool ac, bool dc) noexcept :
    m_network(network),
    m_factory(std::move(manager.m_factory)),
    m_ac(ac),
    m_dc(dc) {
    for (auto& component : manager.m_components) {
        m_components.emplace_back(m_factory(network, component->getNum(), component->getSize()));
    }
}

void AbstractComponentsManager::fillAdjacencyList(const std::map<std::string, unsigned long>& busId2num, std::vector<std::vector<unsigned long>>& adjacencyList) const {
    fillAcAdjacencyList(busId2num, adjacencyList);
    fillDcAdjacencyList(busId2num, adjacencyList);
    fillAcDcAdjacencyList(busId2num, adjacencyList);
}
void AbstractComponentsManager::fillAcAdjacencyList(const std::map<std::string, unsigned long>& busId2num, std::vector<std::vector<unsigned long>>& adjacencyList) const {
    if(!m_ac) {
        return;
    }
    for (const Line& line : getNetwork().getLines()) {
        const auto& bus1 = line.getTerminal1().getBusView().getBus();
        const auto& bus2 = line.getTerminal2().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, busId2num, adjacencyList);
    }
    for (const TieLine& tl : getNetwork().getTieLines()) {
        const auto& bus1 = tl.getDanglingLine1().getTerminal().getBusView().getBus();
        const auto& bus2 = tl.getDanglingLine2().getTerminal().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, busId2num, adjacencyList);
    }
    for (const TwoWindingsTransformer& twt : getNetwork().getTwoWindingsTransformers()) {
        const auto& bus1 = twt.getTerminal1().getBusView().getBus();
        const auto& bus2 = twt.getTerminal2().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, busId2num, adjacencyList);
    }
    for (const ThreeWindingsTransformer& twt : getNetwork().getThreeWindingsTransformers()) {
        const auto& busA = twt.getLeg1().getTerminal().getBusView().getBus();
        const auto& busB = twt.getLeg2().getTerminal().getBusView().getBus();
        const auto& busC = twt.getLeg3().getTerminal().getBusView().getBus();
        addToAdjacencyList(busA, busB, busId2num, adjacencyList);
        addToAdjacencyList(busA, busC, busId2num, adjacencyList);
        addToAdjacencyList(busB, busC, busId2num, adjacencyList);
    }
    // AC/DC Converters with two AC terminals (AC synchronous component) are not included here
    // The converter does not synchronize the 2 AC Terminals
    // The converter does not impose phase or frequency alignment
    // The adjacency of the 2 AC Terminals is however added in the case of ac && dc (connected component)
}
void AbstractComponentsManager::fillDcAdjacencyList(const std::map<std::string, unsigned long>& busId2num, std::vector<std::vector<unsigned long>>& adjacencyList) const {
    if(!m_dc) {
        return;
    }
    for (const DcLine& dcLine : getNetwork().getDcLines()) {
        const auto& dcBus1 = dcLine.getDcTerminal1().getDcBus();
        const auto& dcBus2 = dcLine.getDcTerminal2().getDcBus();
        addToAdjacencyList(dcBus1, dcBus2, busId2num, adjacencyList);
    }
    for (const AcDcConverter& acDcConverter : getNetwork().getDcConnectables<AcDcConverter>()) {
        const auto& dcBus1 = acDcConverter.getDcTerminal1().getDcBus();
        const auto& dcBus2 = acDcConverter.getDcTerminal2().getDcBus();
        addToAdjacencyList(dcBus1, dcBus2, busId2num, adjacencyList);
    }
}
void AbstractComponentsManager::fillAcDcAdjacencyList(const std::map<std::string, unsigned long>& busId2num, std::vector<std::vector<unsigned long>>& adjacencyList) const {
    if(!m_ac || !m_dc){
        return;
    }
    for (const HvdcLine& line : getNetwork().getHvdcLines()) {
        const auto& bus1 = line.getConverterStation1().get().getTerminal().getBusView().getBus();
        const auto& bus2 = line.getConverterStation2().get().getTerminal().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, busId2num, adjacencyList);
    }
    for (const AcDcConverter& acDcConverter : getNetwork().getDcConnectables<AcDcConverter>()) {
        const auto& bus1 = acDcConverter.getTerminal1().getBusView().getBus();
        const auto& dcBus1 = acDcConverter.getDcTerminal1().getDcBus();
        const auto& dcBus2 = acDcConverter.getDcTerminal2().getDcBus();
        addToAdjacencyList(bus1, dcBus1, busId2num, adjacencyList);
        addToAdjacencyList(bus1, dcBus2, busId2num, adjacencyList);

        const auto& acTerminal2 = acDcConverter.getTerminal2();
        if(static_cast<bool>(acTerminal2)) {
            const auto& bus2 = acTerminal2.get().getBusView().getBus();
            addToAdjacencyList(bus1, bus2, busId2num, adjacencyList);
            addToAdjacencyList(bus2, dcBus1, busId2num, adjacencyList);
            addToAdjacencyList(bus2, dcBus2, busId2num, adjacencyList);
        }
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
    reset();

    unsigned long num = 0UL;
    std::map<std::string, unsigned long> busId2num;
    std::vector<std::reference_wrapper<Bus>> num2AcBus;
    std::vector<std::reference_wrapper<DcBus>> num2DcBus;
    if(m_ac) {
        for (Bus& bus : getNetwork().getBusView().getBuses()) {
            num2AcBus.emplace_back(std::ref(bus));
            busId2num[bus.getId()] = num;
            num++;
        }
    }
    if(m_dc) {
        for (DcBus& dcBus: getNetwork().getDcBuses()) {
            num2DcBus.emplace_back(std::ref(dcBus));
            busId2num[dcBus.getId()] = num;
            num++;
        }
    }
    unsigned long nbAcBuses = num2AcBus.size();

    std::vector<std::vector<unsigned long>> adjacencyList(num);
    for (auto& v : adjacencyList) {
        v.reserve(3);
    }
    fillAdjacencyList(busId2num, adjacencyList);

    const auto& result = math::GraphUtil::computeConnectedComponents(adjacencyList);
    const auto& componentNumbers = result.getComponentNumber();
    const auto& componentSizes = result.getComponentSize();

    m_components.reserve(componentSizes.size());
    for (unsigned long i = 0UL; i < componentSizes.size(); i++) {
        m_components.emplace_back(m_factory(m_network, i, componentSizes[i]));
    }

    for (unsigned long i = 0UL; i < componentNumbers.size(); i++) {
        if(i < nbAcBuses) {
            Bus& bus = num2AcBus[i].get();
            setComponentNumber(bus, componentNumbers[i]);
        } else {
            DcBus& dcBus = num2DcBus[i-nbAcBuses].get();
            setComponentNumber(dcBus, componentNumbers[i]);
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = endTime - startTime;
    logger.debug(stdcxx::format("%1% components computed in %2% ms", getComponentLabel(), diff.count() * 1000.0));
}

void AbstractComponentsManager::reset() {
    if(m_ac) {
        for (Bus& bus : getNetwork().getBusBreakerView().getBuses()) {
            setComponentNumber(bus, stdcxx::optional<unsigned long>());
        }
    }
    if(m_dc) {
        for (DcBus& dcBus : getNetwork().getDcBuses()) {
            setComponentNumber(dcBus, stdcxx::optional<unsigned long>());
        }
    }
}

}  // namespace iidm

}  // namespace powsybl
