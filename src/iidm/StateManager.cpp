/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/StateManager.hpp>

#include <algorithm>

#include <powsybl/iidm/MultipleStateContext.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/StateContextGuard.hpp>
#include <powsybl/iidm/Stateful.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

constexpr unsigned long StateManager::INITIAL_STATE_INDEX;

StateManager::StateManager(Network& network) :
    m_network(network),
    m_stateContext(stdcxx::make_unique<MultipleStateContext>(INITIAL_STATE_INDEX)),
    m_stateMutex(),
    m_statesById(),
    m_unusedIndexes(),
    m_stateArraySize(INITIAL_STATE_INDEX + 1) {

    m_statesById.insert(std::make_pair(getInitialStateId(), INITIAL_STATE_INDEX));
}

StateManager::StateManager(StateManager&& stateManager) noexcept :
    m_network(stateManager.m_network),
    m_stateContext(std::move(stateManager.m_stateContext)),
    m_stateMutex(),
    m_statesById(std::move(stateManager.m_statesById)),
    m_unusedIndexes(std::move(stateManager.m_unusedIndexes)),
    m_stateArraySize(stateManager.m_stateArraySize) {
}

iterator_traits<Stateful>::iterator StateManager::begin() {
    return m_network.begin<Stateful>();
}

void StateManager::cloneState(const std::string& sourceStateId, const std::string& targetStateId) {
    cloneState(sourceStateId, { targetStateId });
}

void StateManager::cloneState(const std::string& sourceStateId, const std::initializer_list<std::string>& targetStateIds) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<StateManager>();

    if (targetStateIds.size() == 0) {
        throw PowsyblException("Empty target state id list");
    }

    logger.debug("Creating states %1%", logging::toString(targetStateIds));

    std::lock_guard<std::mutex> lock(m_stateMutex);

    unsigned long sourceIndex = getStateIndex(sourceStateId);
    unsigned long initStateArraySize = m_stateArraySize;
    unsigned long extendedCount = 0;
    std::set<unsigned long> recycled;
    for (const auto& targetStateId : targetStateIds) {
        if (m_statesById.find(targetStateId) != m_statesById.end()) {
            throw PowsyblException(logging::format("Target state '%1%' already exists", targetStateId));
        }
        if (m_unusedIndexes.empty()) {
            // extend state array size
            m_statesById.insert(std::make_pair(targetStateId, m_stateArraySize));
            ++m_stateArraySize;
            ++extendedCount;
        } else {
            // recycle an index
            unsigned long index = *m_unusedIndexes.begin();
            m_unusedIndexes.erase(m_unusedIndexes.begin());

            m_statesById.insert(std::make_pair(targetStateId, index));
            recycled.insert(index);
        }
    }

    if (!recycled.empty()) {
        std::for_each(begin(), end(), [recycled, sourceIndex](Stateful& stateful) {
            stateful.allocateStateArrayElement(recycled, sourceIndex);
        });
        logger.trace("Recycling state array indexes %1%", logging::toString(recycled));
    }
    if (extendedCount > 0) {
        std::for_each(begin(), end(), [initStateArraySize, extendedCount, sourceIndex](Stateful& stateful) {
            stateful.extendStateArraySize(initStateArraySize, extendedCount, sourceIndex);
        });
        logger.trace("Extending state array size to %1% (+%2%)", m_stateArraySize, extendedCount);
    }
}

iterator_traits<Stateful>::iterator StateManager::end() {
    return m_network.end<Stateful>();
}

void StateManager::forEachState(const std::function<void()>& function) {
    std::lock_guard<std::mutex> lock(m_stateMutex);

    StateContextGuard context(*m_stateContext);

    for (const auto& it : m_statesById) {
        m_stateContext->setStateIndex(it.second);
        function();
    }
}

const std::string& StateManager::getInitialStateId() {
    static std::string s_initialStateId("InitialState");

    return s_initialStateId;
}

unsigned long StateManager::getStateArraySize() const {
    return m_stateArraySize;
}

std::set<std::string> StateManager::getStateIds() const {
    std::lock_guard<std::mutex> lock(m_stateMutex);

    std::set<std::string> stateIds;
    std::for_each(m_statesById.begin(), m_statesById.end(), [&stateIds](const std::pair<std::string, int>& entry) {
        stateIds.insert(entry.first);
    });

    return stateIds;
}

unsigned long StateManager::getStateIndex() const {
    return m_stateContext->getStateIndex();
}

unsigned long StateManager::getStateIndex(const std::string& stateId) const {
    const auto& it = m_statesById.find(stateId);
    if (it == m_statesById.end()) {
        throw PowsyblException(logging::format("State '%1%' not found", stateId));
    }

    return it->second;
}

const std::string& StateManager::getWorkingStateId() const {
    std::lock_guard<std::mutex> lock(m_stateMutex);

    unsigned long index = m_stateContext->getStateIndex();
    const auto& it = std::find_if(m_statesById.begin(), m_statesById.end(), [&](const std::pair<std::string, unsigned long>& entry)
    {
        return entry.second == index;
    });

    return it->first;
}

void StateManager::removeState(const std::string& stateId) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<StateManager>();

    std::lock_guard<std::mutex> lock(m_stateMutex);

    if (stateId == getInitialStateId()) {
        throw PowsyblException("Removing initial state is forbidden");
    }

    unsigned long index = getStateIndex(stateId);
    m_statesById.erase(stateId);

    logger.debug("Removing state '%1%'", stateId);

    if (index == m_stateArraySize - 1) {
        // remove consecutive unused index starting from the end
        unsigned long stateCount = 1;
        for (unsigned long i = index; i > INITIAL_STATE_INDEX; --i) {
            const auto& it = std::find(m_unusedIndexes.begin(), m_unusedIndexes.end(), i - 1);
            if (it == m_unusedIndexes.end()) {
                break;
            }

            m_unusedIndexes.erase(it);
            ++stateCount;
        }

        std::for_each(begin(), end(), [stateCount](Stateful& stateful) {
            stateful.reduceStateArraySize(stateCount);
        });

        m_stateArraySize -= stateCount;
        logger.trace("Reducing state array size to %1%", m_stateArraySize);
    } else {
        m_unusedIndexes.insert(index);

        std::for_each(begin(), end(), [index](Stateful& stateful) {
            stateful.deleteStateArrayElement(index);
        });

        logger.trace("Deleting state array element at index %1%", index);
    }

    m_stateContext->resetIfStateIndexIs(index);
}

void StateManager::setWorkingState(const std::string& stateId) {
    std::lock_guard<std::mutex> lock(m_stateMutex);

    unsigned long index = getStateIndex(stateId);
    m_stateContext->setStateIndex(index);
}

}

}
