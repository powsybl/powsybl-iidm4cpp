/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/math/GraphUtil.hpp>

namespace powsybl {

namespace math {

GraphUtil::ConnectedComponentsComputationResult::ConnectedComponentsComputationResult(const std::vector<stdcxx::optional<unsigned long>>& componentNumber, const std::vector<unsigned long>& orderedComponents) :
    m_componentNumber(componentNumber),
    m_componentSize(orderedComponents) {
}

const std::vector<stdcxx::optional<unsigned long>>& GraphUtil::ConnectedComponentsComputationResult::getComponentNumber() const {
    return m_componentNumber;
}

std::vector<stdcxx::optional<unsigned long >>& GraphUtil::ConnectedComponentsComputationResult::getComponentNumber() {
    return m_componentNumber;
}

const std::vector<unsigned long>& GraphUtil::ConnectedComponentsComputationResult::getComponentSize() const {
    return m_componentSize;
}

std::vector<unsigned long>& GraphUtil::ConnectedComponentsComputationResult::getComponentSize() {
    return m_componentSize;
}

GraphUtil::ConnectedComponent::ConnectedComponent(unsigned long size) :
    m_size(size) {
}

unsigned long GraphUtil::ConnectedComponent::getOrderedNumber() const {
    return m_orderedNumber;
}

unsigned long GraphUtil::ConnectedComponent::getSize() const {
    return m_size;
}

GraphUtil::ConnectedComponent& GraphUtil::ConnectedComponent::setOrderedNumber(unsigned long orderedNumber) {
    m_orderedNumber = orderedNumber;
    return *this;
}

GraphUtil::ConnectedComponentsComputationResult GraphUtil::computeConnectedComponents(const std::vector<std::vector<unsigned long>>& adjacencyList) {
    std::vector<stdcxx::optional<unsigned long>> componentNumber(adjacencyList.size());
    unsigned long c = 0UL;
    std::vector<unsigned long> componentSize(adjacencyList.size(), 0UL);
    for (unsigned long v = 0UL; v < adjacencyList.size(); v++) {
        if (!componentNumber[v].is_initialized()) {
            computeConnectedComponents(v, c, componentSize, adjacencyList, componentNumber);
            c++;
        }
    }

    // sort components by size
    std::vector<std::shared_ptr<std::pair<unsigned long, stdcxx::optional<unsigned long>>>> components;
    components.reserve(c);
    std::vector<std::shared_ptr<std::pair<unsigned long, stdcxx::optional<unsigned long>>>> orderedComponents;
    orderedComponents.reserve(c);

    for (unsigned long i = 0UL; i < c; i++) {
        auto comp = std::make_shared<std::pair<unsigned long, stdcxx::optional<unsigned long>>>(componentSize[i], stdcxx::optional<unsigned long>());
        components.push_back(comp);
        orderedComponents.push_back(comp);
    }
    auto compare = [](const std::shared_ptr<std::pair<unsigned long, stdcxx::optional<unsigned long>>>& o1, const std::shared_ptr<std::pair<unsigned long, stdcxx::optional<unsigned long>>>& o2) {
        return o2->first < o1->first;
    };
    std::sort(orderedComponents.begin(), orderedComponents.end(), compare);
    for (unsigned long i = 0UL; i < orderedComponents.size(); i++) {
        orderedComponents[i]->second = i;
    }

    componentSize = std::vector<unsigned long>(orderedComponents.size());
    for (const auto& cc : orderedComponents) {
        componentSize[*cc->second] = cc->first;
    }
    for (unsigned long i = 0UL; i < componentNumber.size();) {
        const auto& cc = components[*componentNumber[i]];
        componentNumber[i] = cc->second;
        ++i;
    }
    return ConnectedComponentsComputationResult(componentNumber, componentSize);
}

void GraphUtil::computeConnectedComponents(unsigned long v1, unsigned long c, std::vector<unsigned long>& componentSize, const std::vector<std::vector<unsigned long>>& adjacencyList, std::vector<stdcxx::optional<unsigned long>>& componentNumber) {
    componentNumber[v1] = c;
    ++componentSize[c];
    const std::vector<unsigned long>& ls = adjacencyList[v1];
    for (unsigned long v2 : ls) {
        if (!componentNumber[v2].is_initialized()) {
            computeConnectedComponents(v2, c, componentSize, adjacencyList, componentNumber);
        }
    }
}

}  // namespace math

}  // namespace powsybl
