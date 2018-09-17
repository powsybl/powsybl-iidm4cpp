/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODEBREAKERVOLTAGELEVEL_HPP
#define POWSYBL_IIDM_NODEBREAKERVOLTAGELEVEL_HPP

#include <map>
#include <string>

#include <powsybl/iidm/VoltageLevel.hpp>

#include "NodeBreakerVoltageLevelBusNamingStrategy.hpp"
#include "NodeBreakerVoltageLevelGraph.hpp"
#include "NodeBreakerVoltageLevelState.hpp"
#include "NodeBreakerVoltageLevelViews.hpp"
#include "StateArray.hpp"

namespace powsybl {

namespace iidm {

class NodeBreakerVoltageLevel : public VoltageLevel {
public: // VoltageLevel
    void attach(Terminal& terminal, bool test) override;

    void clean() override;

    bool connect(Terminal& terminal) override;

    void detach(Terminal& terminal) override;

    bool disconnect(Terminal& terminal) override;

    const BusBreakerView& getBusBreakerView() const override;

    BusBreakerView& getBusBreakerView() override;

    const BusView& getBusView() const override;

    BusView& getBusView() override;

    const NodeBreakerView& getNodeBreakerView() const override;

    NodeBreakerView& getNodeBreakerView() override;

    const TopologyKind& getTopologyKind() const override;

    void invalidateCache() override;

public:
    NodeBreakerVoltageLevel(const std::string& id, const std::string& name, Substation& substation,
                            double nominalVoltage, double lowVoltageLimit, double highVoltagelimit);

    ~NodeBreakerVoltageLevel() noexcept override = default;

    Switch& addSwitch(std::unique_ptr<Switch>&& ptrSwitch, unsigned long node1, unsigned long node2);

    bool isConnected(const Terminal& terminal) const;

protected: // Stateful
    void allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteStateArrayElement(unsigned long index) override;

    void extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceStateArraySize(unsigned long number) override;

private:
    void checkTerminal(Terminal& terminal) const;

    node_breaker_voltage_level::BusNamingStrategy& getBusNamingStrategy();

    node_breaker_voltage_level::CalculatedBusBreakerTopology& getCalculatedBusBreakerTopology();

    node_breaker_voltage_level::CalculatedBusTopology& getCalculatedBusTopology();

    stdcxx::optional<unsigned long> getEdge(const std::string& switchId, bool throwException) const;

    const node_breaker_voltage_level::Graph& getGraph() const;

    unsigned long getNode1(const std::string& switchId) const;

    unsigned long getNode2(const std::string& switchId) const;

    unsigned long getNodeCount() const;

    stdcxx::Reference<Switch> getSwitch(const std::string& switchId) const;

    unsigned long getSwitchCount() const;

    stdcxx::Reference<Terminal> getTerminal(unsigned long node) const;

    void removeSwitch(const std::string& switchId);

    void setNodeCount(unsigned long nodeCount);

private:
    friend class node_breaker_voltage_level::NodeBreakerViewImpl;

    friend class node_breaker_voltage_level::BusBreakerViewImpl;

    friend class node_breaker_voltage_level::BusViewImpl;

    friend class node_breaker_voltage_level::CalculatedBusBreakerTopology;

    friend class node_breaker_voltage_level::CalculatedBusTopology;

private:
    node_breaker_voltage_level::Graph m_graph;

    std::map<std::string, unsigned long> m_switches;

    node_breaker_voltage_level::BusNamingStrategy m_busNamingStrategy;

    StateArray<node_breaker_voltage_level::StateImpl> m_states;

    node_breaker_voltage_level::NodeBreakerViewImpl m_nodeBreakerView;

    node_breaker_voltage_level::BusBreakerViewImpl m_busBreakerView;

    node_breaker_voltage_level::BusViewImpl m_busView;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NODEBREAKERVOLTAGELEVEL_HPP
