/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ABSTRACTCONNECTEDCOMPONENTSMANAGER_HXX
#define POWSYBL_IIDM_ABSTRACTCONNECTEDCOMPONENTSMANAGER_HXX

#include <powsybl/iidm/AbstractConnectedComponentsManager.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/NetworkViews.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

template <typename C>
AbstractConnectedComponentsManager<C>::AbstractConnectedComponentsManager() :
    AbstractComponentsManager<C>("Connected") {
}

template <typename C>
void AbstractConnectedComponentsManager<C>::fillAdjacencyList(const std::map<std::string, unsigned long>& id2num, std::vector<std::vector<unsigned long>>& adjacencyList) const {
    AbstractComponentsManager<C>::fillAdjacencyList(id2num, adjacencyList);
    for (const HvdcLine& line : this->getNetwork().getHvdcLines()) {
        const auto& bus1 = line.getConverterStation1().get().getTerminal().getBusView().getBus();
        const auto& bus2 = line.getConverterStation2().get().getTerminal().getBusView().getBus();
        this->addToAdjacencyList(bus1, bus2, id2num, adjacencyList);
    }
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ABSTRACTCONNECTEDCOMPONENTSMANAGER_HXX
