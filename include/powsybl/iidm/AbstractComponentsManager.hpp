/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ABSTRACTCOMPONENTSMANAGER_HPP
#define POWSYBL_IIDM_ABSTRACTCOMPONENTSMANAGER_HPP

#include <map>
#include <string>
#include <vector>

#include <powsybl/iidm/Component.hpp>
#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class Network;

class AbstractComponentsManager {
public:
    using ComponentFactory = std::function<std::unique_ptr<Component>(Network&, unsigned long, unsigned long)>;

public:
    AbstractComponentsManager(Network& network, const ComponentFactory& factory);

    AbstractComponentsManager(const AbstractComponentsManager& manager) = delete;

    AbstractComponentsManager(AbstractComponentsManager&& manager) noexcept = delete;

    AbstractComponentsManager(Network& network, AbstractComponentsManager&& manager) noexcept;

    virtual ~AbstractComponentsManager() noexcept = default;

    AbstractComponentsManager& operator=(const AbstractComponentsManager& manager) = delete;

    AbstractComponentsManager& operator=(AbstractComponentsManager&& manager) noexcept = delete;

    stdcxx::CReference<Component> getComponent(const stdcxx::optional<unsigned long>& num) const;

    stdcxx::const_range<Component> getConnectedComponents() const;

    stdcxx::range<Component> getConnectedComponents();

    void invalidate();

    void update();

protected:
    void addToAdjacencyList(const stdcxx::CReference<Bus>& bus1, const stdcxx::CReference<Bus>& bus2, const std::map<std::string, unsigned long>& id2num, std::vector<std::vector<unsigned long>>& adjacencyList) const;

    virtual void fillAdjacencyList(const std::map<std::string, unsigned long>& id2num, std::vector<std::vector<unsigned long>>& adjacencyList) const;

    const Network& getNetwork() const;

    Network& getNetwork();

private:
    virtual const std::string& getComponentLabel() const = 0;

    virtual void setComponentNumber(Bus& bus, const stdcxx::optional<unsigned long>& num) = 0;

private:
    Network& m_network;

    ComponentFactory m_factory;

    std::vector<std::unique_ptr<Component>> m_components;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ABSTRACTCOMPONENTSMANAGER_HPP
