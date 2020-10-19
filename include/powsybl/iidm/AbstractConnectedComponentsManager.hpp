/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ABSTRACTCONNECTEDCOMPONENTSMANAGER_HPP
#define POWSYBL_IIDM_ABSTRACTCONNECTEDCOMPONENTSMANAGER_HPP

#include <powsybl/iidm/AbstractComponentsManager.hpp>

namespace powsybl {

namespace iidm {

template <typename C>
class AbstractConnectedComponentsManager : public AbstractComponentsManager<C> {
protected:  // AbstractComponentsManager
    void fillAdjacencyList(const std::map<std::string, unsigned long>& id2num, std::vector<std::vector<unsigned long>>& adjacencyList) const override;

protected:
    AbstractConnectedComponentsManager();

    ~AbstractConnectedComponentsManager() noexcept override = default;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/AbstractConnectedComponentsManager.hxx>

#endif  // POWSYBL_IIDM_ABSTRACTCONNECTEDCOMPONENTSMANAGER_HPP
