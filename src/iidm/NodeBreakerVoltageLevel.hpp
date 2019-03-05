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
#include "NodeBreakerVoltageLevelVariant.hpp"
#include "NodeBreakerVoltageLevelViews.hpp"
#include "NodeTerminalViews.hpp"
#include "VariantArray.hpp"

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

    void addInternalConnection(unsigned long node1, unsigned long node2);

    Switch& addSwitch(std::unique_ptr<Switch>&& ptrSwitch, unsigned long node1, unsigned long node2);

    bool isConnected(const Terminal& terminal) const;

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

protected: // VoltageLevel
    std::vector<std::reference_wrapper<Terminal>> getTerminals() const override;

private:
    void checkTerminal(Terminal& terminal) const;

    node_breaker_voltage_level::BusNamingStrategy& getBusNamingStrategy();

    node_breaker_voltage_level::CalculatedBusBreakerTopology& getCalculatedBusBreakerTopology();

    node_breaker_voltage_level::CalculatedBusTopology& getCalculatedBusTopology();

    stdcxx::optional<unsigned long> getEdge(const std::string& switchId, bool throwException) const;

    const node_breaker_voltage_level::Graph& getGraph() const;

    unsigned long getInternalConnectionCount() const;

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

    friend class node_terminal::NodeBreakerViewImpl;

    friend class node_terminal::BusBreakerViewImpl;

    friend class node_terminal::BusViewImpl;

private:
    node_breaker_voltage_level::Graph m_graph;

    std::map<std::string, unsigned long> m_switches;

    node_breaker_voltage_level::BusNamingStrategy m_busNamingStrategy;

    VariantArray<node_breaker_voltage_level::VariantImpl> m_variants;

    node_breaker_voltage_level::NodeBreakerViewImpl m_nodeBreakerView;

    node_breaker_voltage_level::BusBreakerViewImpl m_busBreakerView;

    node_breaker_voltage_level::BusViewImpl m_busView;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NODEBREAKERVOLTAGELEVEL_HPP
