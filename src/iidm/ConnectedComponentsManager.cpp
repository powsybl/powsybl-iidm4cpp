/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ConnectedComponentsManager.hpp>

#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

ConnectedComponentsManager::ConnectedComponentsManager(Network& network) :
    m_network(network) {
}

std::unique_ptr<ConnectedComponent> ConnectedComponentsManager::createComponent(unsigned long num, unsigned long size) {
    return stdcxx::make_unique<ConnectedComponent>(num, size, m_network.get());
}

void ConnectedComponentsManager::fillAdjacencyList(const std::map<std::string, unsigned long>& id2num, std::vector<std::vector<unsigned long>>& adjacencyList) const {
    AbstractComponentsManager<ConnectedComponent>::fillAdjacencyList(id2num, adjacencyList);
    for (const HvdcLine& line : this->getNetwork().getHvdcLines()) {
        const auto& bus1 = line.getConverterStation1().get().getTerminal().getBusView().getBus();
        const auto& bus2 = line.getConverterStation2().get().getTerminal().getBusView().getBus();
        addToAdjacencyList(bus1, bus2, id2num, adjacencyList);
    }
}

const std::string& ConnectedComponentsManager::getComponentLabel() const {
    static std::string s_label = "Connected";
    return s_label;
}

const Network& ConnectedComponentsManager::getNetwork() const {
    return m_network.get();
}

Network& ConnectedComponentsManager::getNetwork() {
    return m_network.get();
}

void ConnectedComponentsManager::setComponentNumber(Bus& bus, const stdcxx::optional<unsigned long>& num) {
    bus.setConnectedComponentNumber(num);
}

void ConnectedComponentsManager::setNetworkRef(Network& network) {
    m_network.set(network);
}

}  // namespace iidm

}  // namespace powsybl
