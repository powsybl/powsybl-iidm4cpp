/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONNECTEDCOMPONENTSMANAGER_HPP
#define POWSYBL_IIDM_CONNECTEDCOMPONENTSMANAGER_HPP

#include <powsybl/iidm/AbstractComponentsManager.hpp>
#include <powsybl/iidm/ConnectedComponent.hpp>
#include <powsybl/iidm/Ref.hpp>

namespace powsybl {

namespace iidm {

class ConnectedComponentsManager : public AbstractComponentsManager<ConnectedComponent> {
public:
    explicit ConnectedComponentsManager(Network& network);

protected:  // AbstractComponentsManager
    void fillAdjacencyList(const std::map<std::string, unsigned long>& id2num, std::vector<std::vector<unsigned long>>& adjacencyList) const override;

protected:
    std::unique_ptr<ConnectedComponent> createComponent(unsigned long num, unsigned long size) override;

    void setComponentNumber(Bus& bus, const stdcxx::optional<unsigned long>& num) override;

private:  // AbstractComponentsManager
    const std::string& getComponentLabel() const override;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONNECTEDCOMPONENTSMANAGER_HPP
