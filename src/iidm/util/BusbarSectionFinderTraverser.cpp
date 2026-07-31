/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/BusbarSectionFinderTraverser.hpp>

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>


namespace powsybl {

namespace iidm {

std::string BusbarSectionFinderTraverser::findBusbarSectionId(Terminal& startTerminal) {
    checkIsNodeBreakerView(startTerminal.getVoltageLevel());

    BusbarSectionResult result = getBusbarSectionResult(startTerminal);

    if(!result.busbarSectionId.empty()) {
        return result.busbarSectionId;
    }
    if(startTerminal.getVoltageLevel().getNodeBreakerView().getBusbarSectionCount()>0) {
        return startTerminal.getVoltageLevel().getNodeBreakerView().getBusbarSections().front().getId();
    }
    return "";
}

BusbarSectionFinderTraverser::BusbarSectionResult BusbarSectionFinderTraverser::getBusbarSectionResult(Terminal& startTerminal) {
    checkIsNodeBreakerView(startTerminal.getVoltageLevel());

    BBSFinderCustomTopologyTraverser bbsFinder(startTerminal);
    startTerminal.traverse(bbsFinder, math::TraversalType::BREADTH_FIRST);

    //Sort results by depth if more than 1 path and return smallest
    if(!bbsFinder.m_results.empty()) { //At least one result , sort by depth and return the smallest
        bbsFinder.m_results.sort([](const BusbarSectionResult& rslt1, const BusbarSectionResult& rslt2) {
            return rslt1.depth < rslt2.depth;
        });
        return bbsFinder.m_results.front();
    }
    
    //Empty result
    return {"", 0UL, {"", false}, false};
}


math::TraverseResult BusbarSectionFinderTraverser::getTraverseResultOnBusbarSection(const std::map<unsigned long, NodeState>& visitedNodes,
                                                                   const Terminal& traversedTerminal,
                                                                   const BusbarSection& busbarSection,
                                                                   std::list<BusbarSectionResult>& results,
                                                                   PathType& pathType,
                                                                   stdcxx::optional<unsigned long>& resultsDepths) {
    unsigned long node = traversedTerminal.getNodeBreakerView().getNode();
    if(!visitedNodes.count(node)) {
        return math::TraverseResult::TERMINATE_PATH;
    }
    const auto& currentNodeState = visitedNodes.at(node);

    if(currentNodeState.nbOpenSwitchesOnPath == 0) {
        return getTraverseResultAllSwitchesClosed(busbarSection, currentNodeState, results, pathType, resultsDepths);
    } 
    if(!currentNodeState.lastSwitch.switchId.empty() && !currentNodeState.lastSwitch.isOpen) {
        return getTraverseResultLastSwitchClosed(busbarSection, currentNodeState, results, pathType, resultsDepths);
    } 
    if(!currentNodeState.lastSwitch.switchId.empty()) {
        return getTraverseResultOthers(busbarSection, currentNodeState, results, pathType, resultsDepths);
    }
    
    return math::TraverseResult::TERMINATE_PATH;
}

math::TraverseResult BusbarSectionFinderTraverser::getTraverseResultAllSwitchesClosed(const BusbarSection& busbarSection,
                                                                     const NodeState& currentNodeState,
                                                                     std::list<BusbarSectionResult>& results,
                                                                     PathType& pathType,
                                                                     stdcxx::optional<unsigned long>& resultsDepths) {

    switch (pathType) {
        case PathType::ALL_SWITCH_CLOSED:
            //same priority - continue
            break;
        case PathType::LAST_SWITCH_CLOSED:
        case PathType::OTHER:
        case PathType::UNDEFINED: {
            //Priority over previous results (or not yet defined), drop them
            results.clear();
            pathType = PathType::ALL_SWITCH_CLOSED;
            resultsDepths = currentNodeState.depth;
            break;
        }
        default:
            //incorrect value
            return math::TraverseResult::TERMINATE_PATH;
    }

    return addResultAndGetTraverseResult(busbarSection, currentNodeState, results, resultsDepths, true, math::TraverseResult::TERMINATE_TRAVERSER);
}

math::TraverseResult BusbarSectionFinderTraverser::getTraverseResultLastSwitchClosed(const BusbarSection& busbarSection,
                                                                    const NodeState& currentNodeState,
                                                                    std::list<BusbarSectionResult>& results,
                                                                    PathType& pathType,
                                                                    stdcxx::optional<unsigned long>& resultsDepths) {

    switch (pathType) {
        case PathType::ALL_SWITCH_CLOSED:
            //Previous results have priority
            return math::TraverseResult::TERMINATE_PATH;
        case PathType::LAST_SWITCH_CLOSED:
            //same priority - continue
            break;
        case PathType::OTHER: {
            //Priority over previous results, drop them
            results.clear();
            pathType = PathType::LAST_SWITCH_CLOSED;
            resultsDepths = currentNodeState.depth;
            break;
        }
        case PathType::UNDEFINED:
            //not yet defined
            pathType = PathType::LAST_SWITCH_CLOSED;
            break;
        default:
            //incorrect value
            return math::TraverseResult::TERMINATE_PATH;
    }

    return addResultAndGetTraverseResult(busbarSection, currentNodeState, results, resultsDepths, false, math::TraverseResult::TERMINATE_PATH);
}

math::TraverseResult BusbarSectionFinderTraverser::getTraverseResultOthers(const BusbarSection& busbarSection,
                                                          const NodeState& currentNodeState,
                                                          std::list<BusbarSectionResult>& results,
                                                          PathType& pathType,
                                                          stdcxx::optional<unsigned long>& resultsDepths) {

    switch (pathType) {
        case PathType::ALL_SWITCH_CLOSED:
        case PathType::LAST_SWITCH_CLOSED:
            //Previous results have priority
            return math::TraverseResult::TERMINATE_PATH;
        case PathType::OTHER:
            //same priority - continue
            break;
        case PathType::UNDEFINED:
            //not yet defined:
            pathType = PathType::OTHER;
            break;
        default:
            //incorrect value
            return math::TraverseResult::TERMINATE_PATH;
    }

    return addResultAndGetTraverseResult(busbarSection, currentNodeState, results, resultsDepths, false, math::TraverseResult::TERMINATE_PATH);
}

math::TraverseResult BusbarSectionFinderTraverser::addResultAndGetTraverseResult(const BusbarSection& busbarSection,
                                                                const NodeState& currentNodeState,
                                                                std::list<BusbarSectionResult>& results,
                                                                stdcxx::optional<unsigned long>& resultsDepths,
                                                                bool allClosedSwitches,
                                                                const math::TraverseResult& traverseResultIfBetterPathAlreadyFound) {

    if (resultsDepths.has_value() && *resultsDepths < currentNodeState.depth) {
        // We have already found at least one busbar section at a lower depth
        return traverseResultIfBetterPathAlreadyFound;
    }

    results.emplace_back(BusbarSectionResult{busbarSection.getId(), currentNodeState.depth, currentNodeState.lastSwitch, allClosedSwitches});
    resultsDepths = currentNodeState.depth;

    return math::TraverseResult::TERMINATE_PATH;
}


void BusbarSectionFinderTraverser::checkIsNodeBreakerView(const VoltageLevel& voltageLevel) {
    if (voltageLevel.getTopologyKind() != TopologyKind::NODE_BREAKER) {
        throw PowsyblException(stdcxx::format("BusbarSectionFinderTraverser only works with Node Breaker view and voltage level %1% is not in this topology kind", voltageLevel.getId()));
    }
}


BusbarSectionFinderTraverser::BBSFinderCustomTopologyTraverser::BBSFinderCustomTopologyTraverser(const Terminal& startTerminal) : 
    m_voltageLevel(startTerminal.getVoltageLevel()) {
    checkIsNodeBreakerView(m_voltageLevel);
    unsigned long startNode = startTerminal.getNodeBreakerView().getNode();
    m_visitedNodes.emplace(std::make_pair(startNode, NodeState{0, 0, {"", false}}));
    m_results.clear();
    m_resultsDepths.reset();
}

math::TraverseResult BusbarSectionFinderTraverser::BBSFinderCustomTopologyTraverser::traverse(Terminal& terminal, bool /*connected*/) {
    if(!stdcxx::areSame(m_voltageLevel, terminal.getVoltageLevel())) {
        return math::TraverseResult::TERMINATE_PATH;
    }

    if(Terminal::isInstanceOf<BusbarSection>(terminal)) {
        return getTraverseResultOnBusbarSection(m_visitedNodes, terminal, Terminal::map<BusbarSection>(terminal), m_results, m_pathType, m_resultsDepths);
    }

    return math::TraverseResult::CONTINUE;
}

math::TraverseResult BusbarSectionFinderTraverser::BBSFinderCustomTopologyTraverser::traverse(Switch& aSwitch) {
    unsigned long node1 = m_voltageLevel.getNodeBreakerView().getNode1(aSwitch.getId());
    unsigned long node2 = m_voltageLevel.getNodeBreakerView().getNode2(aSwitch.getId());

    unsigned long sourceNode = (m_visitedNodes.count(node1)) ? node1 : node2;
    unsigned long targetNode = (m_visitedNodes.count(node1)) ? node2 : node1;

    if(!m_visitedNodes.count(sourceNode)) {
        return math::TraverseResult::TERMINATE_PATH;
    }
    const NodeState& sourceState = m_visitedNodes.at(sourceNode);
    m_visitedNodes[targetNode] = NodeState{sourceState.depth + 1, sourceState.nbOpenSwitchesOnPath + (aSwitch.isOpen() ? 1 : 0),
                                                    {aSwitch.getId(), aSwitch.isOpen()}};

    return math::TraverseResult::CONTINUE;
}

}  // namespace iidm

}  // namespace powsybl
