/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VARIANTMANAGER_HPP
#define POWSYBL_IIDM_VARIANTMANAGER_HPP

#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>

#include <powsybl/iidm/VariantContext.hpp>
#include <powsybl/iidm/iterators/Iterators.hpp>

namespace powsybl {

namespace iidm {

class Network;

class VariantManager {
public:
    static const std::string& getInitialVariantId();

public:
    explicit VariantManager(Network& network);

    VariantManager(const VariantManager& variantManager) = delete;

    VariantManager(VariantManager&& variantManager) noexcept;

    ~VariantManager() noexcept = default;

    VariantManager& operator=(const VariantManager& variantManager) = delete;

    void cloneVariant(const std::string& sourceVariantId, const std::string& targetVariantId);

    void cloneVariant(const std::string& sourceVariantId, const std::initializer_list<std::string>& targetVariantIds);

    void forEachVariant(const std::function<void()>& function);

    unsigned long getVariantArraySize() const;

    std::set<std::string> getVariantIds() const;

    unsigned long getVariantIndex() const;

    std::set<unsigned long> getVariantIndices() const;

    const std::string& getWorkingVariantId() const;

    void removeVariant(const std::string& variantId);

    void setWorkingVariant(const std::string& variantId);

private:
    static constexpr unsigned long INITIAL_VARIANT_INDEX = 0;

private:
    iterator_traits<MultiVariantObject>::iterator begin();

    iterator_traits<MultiVariantObject>::iterator end();

    unsigned long getVariantIndex(const std::string& variantId) const;

private:
    Network& m_network;

    std::unique_ptr<VariantContext> m_variantContext;

    mutable std::mutex m_variantMutex;

    std::unordered_map<std::string, unsigned long> m_variantsById;

    std::set<unsigned long> m_unusedIndexes;

    unsigned long m_variantArraySize;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VARIANTMANAGER_HPP
