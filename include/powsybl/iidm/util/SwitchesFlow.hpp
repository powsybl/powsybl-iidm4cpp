/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_SWITCHESFLOW_HPP
#define POWSYBL_IIDM_UTIL_SWITCHESFLOW_HPP

#include <map>
#include <string>

#include <powsybl/math/UndirectedGraph.hpp>
#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class Switch;
class Terminal;
class VoltageLevel;

class SwitchesFlow {
public:
    SwitchesFlow(VoltageLevel& voltageLevel);

    bool hasFlow(const std::string& id) const;

    bool isEmpty() const;

    double getP1(const std::string& switchId) const;

    double getQ1(const std::string& switchId) const;

    double getP2(const std::string& switchId) const;

    double getQ2(const std::string& switchId) const;

private:
    class SwFlow {
    public:
        SwFlow(double p1, double q1, double p2, double q2);

    public:
        double m_p1;
        double m_p2;
        double m_q1;
        double m_q2;
    };

    class SwNode {
    public:
        SwNode(unsigned long node);

        SwNode(const Bus& bus);

        bool isNodeBreaker() const;

        SwNode& addPQ(double p, double q);

        SwNode& addFlow(double pFlow, double qFlow);

    private:
        stdcxx::optional<unsigned long> m_node;

        stdcxx::CReference<Bus> m_bus;

        double m_p;

        double m_q;

        double m_pflow;

        double m_qflow;
    };

    class SwEdge {
    public:
        SwEdge(Switch& sw, SwNode& swNode1, SwNode& swNode2);

        SwEdge(SwNode& swNode1, SwNode& swNode2);

        std::string getSwitchId() const;

        bool isSwitch() const;

    private:
        stdcxx::Reference<Switch> m_sw;
        SwNode& m_swNode1;
        SwNode& m_swNode2;
    };

private:
    static const SwNode& addSwNode(std::map<std::string, SwNode>& swNodeInjection, unsigned long node);

    static const SwNode& addSwNode(std::map<std::string, SwNode>& swNodeInjection, const Bus& bus);

    static void calculateInjectionsNodeBreaker(VoltageLevel& voltageLevel, std::map<std::string, SwNode>& swNodeInjection);

    static void calculateInjectionsBusBreaker(VoltageLevel& voltageLevel, std::map<std::string, SwNode>& swNodeInjection);

    static std::string getKey(unsigned long node);

    static std::string getKey(const Bus& bus);

    static double getTerminalP(const Terminal& terminal);

    static double getTerminalQ(const Terminal& terminal);

private:
    void assignZeroFlowToEdgesOutsideAllTrees(const math::UndirectedGraph<SwNode, SwEdge>& graph);

    void buildGraph(std::map<std::string, SwNode>& swNodeInjection, math::UndirectedGraph<SwNode, SwEdge>& graph);

    void buildGraphFromBusBreaker(std::map<std::string, SwNode>& swNodeInjection, math::UndirectedGraph<SwNode, SwEdge>& graph);

    void buildGraphFromNodeBreaker(std::map<std::string, SwNode>& swNodeInjection, math::UndirectedGraph<SwNode, SwEdge>& graph);

    void calculateInjections(std::map<std::string, SwNode>& swNodeInjection);

    void compute();

private:
    VoltageLevel& m_voltageLevel;

    std::map<std::string, SwFlow> m_switchesFlows;

    std::vector<std::unique_ptr<SwEdge>> m_edges;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_SWITCHESFLOW_HPP
