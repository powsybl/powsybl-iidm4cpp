/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSBARSECTIONFINDERTRAVERSER_HPP
#define POWSYBL_IIDM_BUSBARSECTIONFINDERTRAVERSER_HPP

#include <powsybl/math/TraverseResult.hpp>

#include <powsybl/iidm/Terminal.hpp>

#include <powsybl/stdcxx/optional.hpp>

#include <list>
#include <map>
#include <string>

namespace powsybl {

namespace iidm {

class BusbarSection;

class BusbarSectionFinderTraverser {

private:
    enum class PathType : uint8_t {
        UNDEFINED = 0,
        ALL_SWITCH_CLOSED,
        LAST_SWITCH_CLOSED,
        OTHER
    };

public:
    BusbarSectionFinderTraverser() = delete;

    struct SwitchInfo {
        std::string switchId;
        bool isOpen;
    };

    struct BusbarSectionResult {
        std::string busbarSectionId;
        unsigned long depth;
        SwitchInfo lastSwitch;
        bool allClosedSwitch;
    };
private:
    struct NodeState {
        unsigned long depth;
        unsigned long nbOpenSwitchesOnPath;
        SwitchInfo lastSwitch;
    };

    class BBSFinderCustomTopologyTraverser : public Terminal::TopologyTraverser {
    public:
        BBSFinderCustomTopologyTraverser(const Terminal& startTerminal);

    public:  // voltage_level::TopologyTraverser
        math::TraverseResult traverse(Terminal& terminal, bool connected) override;

        math::TraverseResult traverse(Switch& aSwitch) override;

    private:
        friend class BusbarSectionFinderTraverser; //so enclosing class can access private members
        
        const VoltageLevel& m_voltageLevel;

        std::map<unsigned long, NodeState> m_visitedNodes;

        PathType m_pathType = PathType::UNDEFINED;

        std::list<BusbarSectionResult> m_results;

        stdcxx::optional<unsigned long> m_resultsDepths;
    };

public:

    /**
     * Returns the id of the busbar section the provided terminal corresponds the most<br>
     * The algorithm prioritizes in order:
     * <ul>
     *     <li>the paths with all switches closed</li>
     *     <li>the paths with the last switch closed</li>
     *     <li>all other paths</li>
     * </ul>
     * Traversal is performed in BREADTH_FIRST type. 
     * If multiple paths leading to busbar sections are found, the one with the lowest depth is returned.
     * If no path has been found, retrieve the first busbar Section from the VoltageLevel of this terminal.
     * Returns an empty string if the VoltageLevel does not include BusbarSection 
     */
    static std::string findBusbarSectionId(Terminal& startTerminal);

    /**
     * Provides information related to the busbar section on which the provided terminal could be connected.<br/>
     * The algorithm prioritizes in order:
     * <ul>
     *     <li>the paths with all switches closed</li>
     *     <li>the paths with the last switch closed</li>
     *     <li>all other paths</li>
     * </ul>
     * Traversal is performed in BREADTH_FIRST type. 
     * If multiple paths leading to busbar sections are found, the one with the lowest depth is returned.
     */
    static BusbarSectionResult getBusbarSectionResult(Terminal& startTerminal);

private:
    static math::TraverseResult getTraverseResultOnBusbarSection(const std::map<unsigned long, NodeState>& visitedNodes,
                                                                   const Terminal& traversedTerminal,
                                                                   const BusbarSection& busbarSection,
                                                                   std::list<BusbarSectionResult>& results,
                                                                   PathType& pathType,
                                                                   stdcxx::optional<unsigned long>& resultsDepths);

    static math::TraverseResult getTraverseResultAllSwitchesClosed(const BusbarSection& busbarSection,
                                                                     const NodeState& currentNodeState,
                                                                     std::list<BusbarSectionResult>& results,
                                                                     PathType& pathType,
                                                                     stdcxx::optional<unsigned long>& resultsDepths);

    static math::TraverseResult getTraverseResultLastSwitchClosed(const BusbarSection& busbarSection,
                                                                    const NodeState& currentNodeState,
                                                                    std::list<BusbarSectionResult>& results,
                                                                    PathType& pathType,
                                                                    stdcxx::optional<unsigned long>& resultsDepths);

    static math::TraverseResult getTraverseResultOthers(const BusbarSection& busbarSection,
                                                          const NodeState& currentNodeState,
                                                          std::list<BusbarSectionResult>& results,
                                                          PathType& pathType,
                                                          stdcxx::optional<unsigned long>& resultsDepths);

    static math::TraverseResult addResultAndGetTraverseResult(const BusbarSection& busbarSection,
                                                                const NodeState& currentNodeState,
                                                                std::list<BusbarSectionResult>& results,
                                                                stdcxx::optional<unsigned long>& resultsDepths,
                                                                bool allClosedSwitches,
                                                                const math::TraverseResult& traverseResultIfBetterPathAlreadyFound);


    static void checkIsNodeBreakerView(const VoltageLevel& voltageLevel);

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSBARSECTIONFINDERTRAVERSER_HPP
