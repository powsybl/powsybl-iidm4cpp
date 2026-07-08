/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ABSTRACTCOMPONENTSMANAGER_HXX
#define POWSYBL_IIDM_ABSTRACTCOMPONENTSMANAGER_HXX

#include <powsybl/iidm/AbstractComponentsManager.hpp>

#include <powsybl/iidm/Identifiable.hpp>

namespace powsybl {

namespace iidm {


template <typename T, typename U, typename>
void AbstractComponentsManager::addToAdjacencyList(const stdcxx::CReference<T>& bus1, const stdcxx::CReference<U>& bus2, const std::map<std::string, unsigned long>& busId2num, std::vector<std::vector<unsigned long>>& adjacencyList) const {
    if (bus1 && bus2) {
        unsigned long busNum1 = busId2num.find(bus1.get().getId())->second;
        unsigned long busNum2 = busId2num.find(bus2.get().getId())->second;
        adjacencyList[busNum1].emplace_back(busNum2);
        adjacencyList[busNum2].emplace_back(busNum1);
    }
}


}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ABSTRACTCOMPONENTSMANAGER_HXX
