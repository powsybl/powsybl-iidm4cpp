/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/VariantManager.hpp>

#include <algorithm>

#include <powsybl/iidm/MultiVariantObject.hpp>
#include <powsybl/iidm/MultipleVariantContext.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VariantContextGuard.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

constexpr unsigned long VariantManager::INITIAL_VARIANT_INDEX;

VariantManager::VariantManager(Network& network) :
    m_network(network),
    m_variantContext(stdcxx::make_unique<MultipleVariantContext>(INITIAL_VARIANT_INDEX)),
    m_variantArraySize(INITIAL_VARIANT_INDEX + 1) {

    m_variantsById.insert(std::make_pair(getInitialVariantId(), INITIAL_VARIANT_INDEX));
}

VariantManager::VariantManager(VariantManager&& variantManager) noexcept :
    m_network(variantManager.m_network),
    m_variantContext(std::move(variantManager.m_variantContext)),
    m_variantsById(std::move(variantManager.m_variantsById)),
    m_unusedIndexes(std::move(variantManager.m_unusedIndexes)),
    m_variantArraySize(variantManager.m_variantArraySize) {
}

void VariantManager::cloneVariant(const std::string& sourceVariantId, const std::string& targetVariantId) {
    cloneVariant(sourceVariantId, { targetVariantId });
}

void VariantManager::cloneVariant(const std::string& sourceVariantId, const std::initializer_list<std::string>& targetVariantIds) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<VariantManager>();

    if (targetVariantIds.size() == 0) {
        throw PowsyblException("Empty target variant id list");
    }

    logger.debug("Creating variants %1%", logging::toString(targetVariantIds));

    std::lock_guard<std::mutex> lock(m_variantMutex);

    unsigned long sourceIndex = getVariantIndex(sourceVariantId);
    unsigned long initVariantArraySize = m_variantArraySize;
    unsigned long extendedCount = 0;
    std::set<unsigned long> recycled;
    for (const auto& targetVariantId : targetVariantIds) {
        if (m_variantsById.find(targetVariantId) != m_variantsById.end()) {
            throw PowsyblException(logging::format("Target variant '%1%' already exists", targetVariantId));
        }
        if (m_unusedIndexes.empty()) {
            // extend variant array size
            m_variantsById.insert(std::make_pair(targetVariantId, m_variantArraySize));
            ++m_variantArraySize;
            ++extendedCount;
        } else {
            // recycle an index
            unsigned long index = *m_unusedIndexes.begin();
            m_unusedIndexes.erase(m_unusedIndexes.begin());

            m_variantsById.insert(std::make_pair(targetVariantId, index));
            recycled.insert(index);
        }
    }

    if (!recycled.empty()) {
        for (auto& multiVariantObject : m_network.getStatefulObjects()) {
            multiVariantObject.allocateVariantArrayElement(recycled, sourceIndex);
        }
        logger.trace("Recycling variant array indexes %1%", logging::toString(recycled));
    }
    if (extendedCount > 0) {
        for (auto& multiVariantObject : m_network.getStatefulObjects()) {
            multiVariantObject.extendVariantArraySize(initVariantArraySize, extendedCount, sourceIndex);
        }
        logger.trace("Extending variant array size to %1% (+%2%)", m_variantArraySize, extendedCount);
    }
}

void VariantManager::forEachVariant(const std::function<void()>& function) {
    std::lock_guard<std::mutex> lock(m_variantMutex);

    VariantContextGuard context(*m_variantContext);

    for (const auto& it : m_variantsById) {
        m_variantContext->setVariantIndex(it.second);
        function();
    }
}

const std::string& VariantManager::getInitialVariantId() {
    static std::string s_initialVariantId("InitialVariant");

    return s_initialVariantId;
}

unsigned long VariantManager::getVariantArraySize() const {
    return m_variantArraySize;
}

std::set<std::string> VariantManager::getVariantIds() const {
    std::lock_guard<std::mutex> lock(m_variantMutex);

    std::set<std::string> variantIds;
    std::for_each(m_variantsById.begin(), m_variantsById.end(), [&variantIds](const std::pair<std::string, unsigned long>& entry) {
        variantIds.insert(entry.first);
    });

    return variantIds;
}

unsigned long VariantManager::getVariantIndex() const {
    return m_variantContext->getVariantIndex();
}

unsigned long VariantManager::getVariantIndex(const std::string& variantId) const {
    const auto& it = m_variantsById.find(variantId);
    if (it == m_variantsById.end()) {
        throw PowsyblException(logging::format("Variant '%1%' not found", variantId));
    }

    return it->second;
}

std::set<unsigned long> VariantManager::getVariantIndices() const {
    std::lock_guard<std::mutex> lock(m_variantMutex);

    std::set<unsigned long> variantIndices;
    std::for_each(m_variantsById.begin(), m_variantsById.end(), [&variantIndices](const std::pair<std::string, unsigned long>& entry) {
        variantIndices.insert(entry.second);
    });

    return variantIndices;
}

const std::string& VariantManager::getWorkingVariantId() const {
    std::lock_guard<std::mutex> lock(m_variantMutex);

    unsigned long index = m_variantContext->getVariantIndex();
    const auto& it = std::find_if(m_variantsById.begin(), m_variantsById.end(), [index](const std::pair<std::string, unsigned long>& entry) {
        return entry.second == index;
    });

    return it->first;
}

void VariantManager::removeVariant(const std::string& variantId) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<VariantManager>();

    std::lock_guard<std::mutex> lock(m_variantMutex);

    if (variantId == getInitialVariantId()) {
        throw PowsyblException("Removing initial variant is forbidden");
    }

    unsigned long index = getVariantIndex(variantId);
    m_variantsById.erase(variantId);

    logger.debug("Removing variant '%1%'", variantId);

    if (index == m_variantArraySize - 1) {
        // remove consecutive unused index starting from the end
        unsigned long variantCount = 1;
        for (unsigned long i = index; i > INITIAL_VARIANT_INDEX; --i) {
            const auto& it = std::find(m_unusedIndexes.begin(), m_unusedIndexes.end(), i - 1);
            if (it == m_unusedIndexes.end()) {
                break;
            }

            m_unusedIndexes.erase(it);
            ++variantCount;
        }

        for (auto& multiVariantObject : m_network.getStatefulObjects()) {
            multiVariantObject.reduceVariantArraySize(variantCount);
        }

        m_variantArraySize -= variantCount;
        logger.trace("Reducing variant array size to %1%", m_variantArraySize);
    } else {
        m_unusedIndexes.insert(index);

        for (auto& multiVariantObject : m_network.getStatefulObjects()) {
            multiVariantObject.deleteVariantArrayElement(index);
        }

        logger.trace("Deleting variant array element at index %1%", index);
    }

    m_variantContext->resetIfVariantIndexIs(index);
}

void VariantManager::setWorkingVariant(const std::string& variantId) {
    std::lock_guard<std::mutex> lock(m_variantMutex);

    unsigned long index = getVariantIndex(variantId);
    m_variantContext->setVariantIndex(index);
}

}  // namespace iidm

}  // namespace powsybl
