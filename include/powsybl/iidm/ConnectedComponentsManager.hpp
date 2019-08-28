/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONNECTEDCOMPONENTSMANAGER_HPP
#define POWSYBL_IIDM_CONNECTEDCOMPONENTSMANAGER_HPP

#include <powsybl/iidm/ComponentsManager.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class Network;

namespace network {

class VariantImpl;

}  // namespace network

class ConnectedComponentsManager : public ComponentsManager {
public:
    ~ConnectedComponentsManager() noexcept override = default;

protected: // ComponentsManager
    std::unique_ptr<Component> createComponent(long num, unsigned long size) override;

    void fillAdjacencyList(const std::map<std::string, long>& id2num, std::vector<std::vector<long>>& adjacencyList) const override;

    const std::string& getComponentLabel() const override;

    void setComponentNumber(Bus& bus, long num) const override;

private:
    explicit ConnectedComponentsManager(Network& network);

    friend class network::VariantImpl;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONNECTEDCOMPONENTSMANAGER_HPP
