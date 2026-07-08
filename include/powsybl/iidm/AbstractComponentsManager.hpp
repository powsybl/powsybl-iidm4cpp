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
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class DcBus;
class Identifiable;
class Network;

class AbstractComponentsManager {
public:
    using ComponentFactory = std::function<std::unique_ptr<Component>(Network&, unsigned long, unsigned long)>;

public:
    AbstractComponentsManager(Network& network, const ComponentFactory& factory, bool ac, bool dc);

    AbstractComponentsManager(const AbstractComponentsManager&) = delete;

    AbstractComponentsManager(AbstractComponentsManager&&) noexcept = delete;

    AbstractComponentsManager(Network& network, AbstractComponentsManager&& manager, bool ac, bool dc) noexcept;

    virtual ~AbstractComponentsManager() noexcept = default;

    AbstractComponentsManager& operator=(const AbstractComponentsManager&) = delete;

    AbstractComponentsManager& operator=(AbstractComponentsManager&&) noexcept = delete;

    stdcxx::CReference<Component> getComponent(const stdcxx::optional<unsigned long>& num) const;

    stdcxx::const_range<Component> getConnectedComponents() const;

    stdcxx::range<Component> getConnectedComponents();

    void invalidate();

    void update();

private:
    template <typename T = Identifiable, typename U = Identifiable, typename = typename std::enable_if<std::is_base_of<Identifiable, T>::value && std::is_base_of<Identifiable, U>::value>::type>
    void addToAdjacencyList(const stdcxx::CReference<T>& bus1, const stdcxx::CReference<U>& bus2, const std::map<std::string, unsigned long>& busId2num, std::vector<std::vector<unsigned long>>& adjacencyList) const;

    void fillAdjacencyList(const std::map<std::string, unsigned long>& busId2num, std::vector<std::vector<unsigned long>>& adjacencyList) const;
    void fillAcAdjacencyList(const std::map<std::string, unsigned long>& busId2num, std::vector<std::vector<unsigned long>>& adjacencyList) const;
    void fillDcAdjacencyList(const std::map<std::string, unsigned long>& busId2num, std::vector<std::vector<unsigned long>>& adjacencyList) const;
    void fillAcDcAdjacencyList(const std::map<std::string, unsigned long>& busId2num, std::vector<std::vector<unsigned long>>& adjacencyList) const;

    const Network& getNetwork() const;

    Network& getNetwork();

    virtual const std::string& getComponentLabel() const = 0;

    void reset();

protected:
    virtual void setComponentNumber(Bus& bus, const stdcxx::optional<unsigned long>& num) = 0;
    virtual void setComponentNumber(DcBus& dcBus, const stdcxx::optional<unsigned long>& num) = 0;

private:
    Network& m_network;

    ComponentFactory m_factory;

    std::vector<std::unique_ptr<Component>> m_components;

    bool m_ac;
    bool m_dc;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/AbstractComponentsManager.hxx>

#endif  // POWSYBL_IIDM_ABSTRACTCOMPONENTSMANAGER_HPP
