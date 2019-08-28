/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_COMPONENTSMANAGER_HPP
#define POWSYBL_IIDM_COMPONENTSMANAGER_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <powsybl/iidm/Component.hpp>

#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class Component;
class Network;

class ComponentsManager {
public:
    virtual ~ComponentsManager() noexcept = default;

    stdcxx::CReference<Component> getComponent(long num) const;

    stdcxx::Reference<Component> getComponent(long num);

    stdcxx::const_range<Component> getConnectedComponents() const;

    stdcxx::range<Component> getConnectedComponents();

    void invalidate();

    void update();

protected:
    explicit ComponentsManager(Network& network);

    void addToAdjacencyList(const stdcxx::CReference<Bus>& bus1, const stdcxx::CReference<Bus>& bus2, const std::map<std::string, long>& id2num, std::vector<std::vector<long>>& adjacencyList) const;

    virtual std::unique_ptr<Component> createComponent(long num, unsigned long size) = 0;

    virtual void fillAdjacencyList(const std::map<std::string, long>& id2num, std::vector<std::vector<long>>& adjacencyList) const;

    virtual const std::string& getComponentLabel() const = 0;

    const Network& getNetwork() const;

    Network& getNetwork();

    virtual void setComponentNumber(Bus& bus, long num) const = 0;

private:
    Network& m_network;

    std::vector<std::unique_ptr<Component>> m_components;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_COMPONENTSMANAGER_HPP
