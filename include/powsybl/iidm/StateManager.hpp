/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_STATEMANAGER_HPP
#define POWSYBL_IIDM_STATEMANAGER_HPP

#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>

#include <powsybl/iidm/StateContext.hpp>
#include <powsybl/iidm/iterators/Iterators.hpp>

namespace powsybl {

namespace iidm {

class Network;

class StateManager {
public:
    static const std::string& getInitialStateId();

public:
    explicit StateManager(Network& network);

    StateManager(StateManager&& stateManager) noexcept;

    void cloneState(const std::string& sourceStateId, const std::string& targetStateId);

    void cloneState(const std::string& sourceStateId, const std::initializer_list<std::string>& targetStateIds);

    void forEachState(const std::function<void()>& function);

    unsigned long getStateArraySize() const;

    std::set<std::string> getStateIds() const;

    unsigned long getStateIndex() const;

    const std::string& getWorkingStateId() const;

    void removeState(const std::string& stateId);

    void setWorkingState(const std::string& stateId);

private:
    static constexpr unsigned long INITIAL_STATE_INDEX = 0;

private:
    iterator_traits<Stateful>::iterator begin();

    iterator_traits<Stateful>::iterator end();

    unsigned long getStateIndex(const std::string& stateId) const;

private: // Non copyable
    StateManager(const StateManager& stateManager) = delete;

    StateManager& operator=(const StateManager& stateManager) = delete;

private:
    Network& m_network;

    std::unique_ptr<StateContext> m_stateContext;

    mutable std::mutex m_stateMutex;

    std::unordered_map<std::string, unsigned long> m_statesById;

    std::set<unsigned long> m_unusedIndexes;

    unsigned long m_stateArraySize;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_STATEMANAGER_HPP
