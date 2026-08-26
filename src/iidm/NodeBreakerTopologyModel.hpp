/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODEBREAKERTOPOLOGYMODEL_HPP
#define POWSYBL_IIDM_NODEBREAKERTOPOLOGYMODEL_HPP

#include <map>
#include <set>
#include <string>

#include <powsybl/iidm/VariantArray.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "NodeBreakerTopologyBusNamingStrategy.hpp"
#include "NodeBreakerTopologyGraph.hpp"
#include "NodeBreakerTopologyVariant.hpp"
#include "NodeBreakerTopologyViews.hpp"
#include "NodeTerminalViews.hpp"

namespace powsybl {

namespace iidm {

class NodeBreakerTopologyModel : public TopologyModel {
public: // TopologyModel
    void attach(Terminal& terminal, bool test) override;

    bool connect(Terminal& terminal) override;
    bool connect(Terminal& terminal, const stdcxx::Predicate<Switch>& isTypeSwitchToOperate) override;
    /**
     * Retrieve a list of switches that verify the given predicate and that when all are closed, should connect the given Terminal with the shortest path.
     * Switches are appended to the given vector.
     *
     * @return true if there is at least one path found that contains an appropriate Switch. false otherwise (meaning there is no path to connect the Terminal for that predicate)
     */
    bool getConnectingSwitches(Terminal& terminal, const stdcxx::Predicate<Switch>& isTypeSwitchToOperate, std::vector<stdcxx::Reference<Switch>>& switchesForConnection);

    void detach(Terminal& terminal) override;

    bool disconnect(Terminal& terminal) override;
    bool disconnect(Terminal& terminal, const stdcxx::Predicate<Switch>& isSwitchOpenable) override;
    /**
     * Retrieve a list of switches that verify the given predicate and that when all are opened, should disconnect the given Terminal.
     * Switches are appended to the given vector. Depending on the topology graph, the same switch could be added in this list several times.
     *
     * @return true if there is at least one openable Switch on each path leading from the given Terminal to another one,
     * false otherwise (meaning there is at least one path that cannot be opened to disconnect the Terminal for that predicate)
     */
    bool getDisconnectingSwitches(Terminal& terminal, const stdcxx::Predicate<Switch>& isSwitchOpenable, std::vector<stdcxx::Reference<Switch>>& switchesForDisconnection);

    const BusBreakerView& getBusBreakerView() const override;

    BusBreakerView& getBusBreakerView() override;

    const BusView& getBusView() const override;

    BusView& getBusView() override;

    /**
     * Get the highest index of used nodes (i.e. attached to an equipment, a switch or an internal connection) in the voltage level.
     *
     * @return the highest index of used nodes.
     */
    long getMaximumNodeIndex() const;

    const NodeBreakerView& getNodeBreakerView() const override;

    NodeBreakerView& getNodeBreakerView() override;

    unsigned long getSwitchCount() const override;

    stdcxx::const_range<Switch> getSwitches() const override;

    stdcxx::range<Switch> getSwitches() override;

    const TopologyKind& getTopologyKind() const override;

    void invalidateCache(bool exceptBusBreakerView = false) override;

    bool traverse(NodeTerminal& terminal, Terminal::TopologyTraverser& traverser, math::TraversalType traversalType) const;

    bool traverse(NodeTerminal& terminal, Terminal::TopologyTraverser& traverser, TerminalSet& traversedTerminals, math::TraversalType traversalType) const;

public:
    NodeBreakerTopologyModel(VoltageLevel& voltageLevel);

    ~NodeBreakerTopologyModel() noexcept override = default;

    void addInternalConnection(unsigned long node1, unsigned long node2);

    Switch& addSwitch(std::unique_ptr<Switch>&& ptrSwitch, unsigned long node1, unsigned long node2);

    bool isConnected(const Terminal& terminal) const;

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

protected: // TopologyModel
    stdcxx::const_range<Terminal> getTerminals() const override;

    stdcxx::range<Terminal> getTerminals() override;

protected:  // TopologyModel
    void removeTopology() override;

    void removeSwitchFromTopology(const std::string& switchId);
    friend class VoltageLevel;

private:
    static math::TraverseResult getTraverseResult(TerminalSet& visitedTerminals, NodeTerminal& terminal, Terminal::TopologyTraverser& traverser);

private:
    void checkTerminal(Terminal& terminal) const;

    void clean();

    node_breaker_topology_model::BusNamingStrategy& getBusNamingStrategy();

    const node_breaker_topology_model::CalculatedBusBreakerTopology& getCalculatedBusBreakerTopology() const;

    node_breaker_topology_model::CalculatedBusBreakerTopology& getCalculatedBusBreakerTopology();

    node_breaker_topology_model::CalculatedBusTopology& getCalculatedBusTopology();

    stdcxx::optional<unsigned long> getEdge(const std::string& switchId, bool throwException) const;

    const node_breaker_topology_model::Graph& getGraph() const;

    unsigned long getInternalConnectionCount() const;

    unsigned long getNode1(const std::string& switchId) const;

    unsigned long getNode2(const std::string& switchId) const;

    stdcxx::CReference<Switch> getSwitch(const std::string& switchId) const;

    stdcxx::CReference<Terminal> getTerminal(unsigned long node) const;

    stdcxx::Reference<Terminal> getTerminal(unsigned long node);

    void removeInternalConnections(unsigned long node1, unsigned long node2);

    void removeSwitch(const std::string& switchId);

private:
    friend class node_breaker_topology_model::NodeBreakerViewImpl;

    friend class node_breaker_topology_model::BusBreakerViewImpl;

    friend class node_breaker_topology_model::BusViewImpl;

    friend class node_breaker_topology_model::CalculatedBusBreakerTopology;

    friend class node_breaker_topology_model::CalculatedBusTopology;

    friend class node_terminal::NodeBreakerViewImpl;

    friend class node_terminal::BusBreakerViewImpl;

    friend class node_terminal::BusViewImpl;

private:

    node_breaker_topology_model::Graph m_graph;

    std::map<std::string, unsigned long> m_switches;

    node_breaker_topology_model::BusNamingStrategy m_busNamingStrategy;

    node_breaker_topology_model::VariantArray m_variants;

    node_breaker_topology_model::NodeBreakerViewImpl m_nodeBreakerView;

    node_breaker_topology_model::BusBreakerViewImpl m_busBreakerView;

    node_breaker_topology_model::BusViewImpl m_busView;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NODEBREAKERVOLTAGELEVEL_HPP
