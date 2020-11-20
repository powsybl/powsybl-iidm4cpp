/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/math/GraphUtil.hpp>

namespace powsybl {

namespace math {

class ConnectedComponent {
public:
    explicit ConnectedComponent(unsigned long size);

private:
    friend class GraphUtil;

private:
    unsigned long m_size;

    unsigned long m_orderedNumber = 0UL;
};

ConnectedComponent::ConnectedComponent(unsigned long size) :
    m_size(size) {
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
    std::vector<std::shared_ptr<ConnectedComponent>> components;
    components.reserve(c);
    std::vector<std::shared_ptr<ConnectedComponent>> orderedComponents;
    orderedComponents.reserve(c);

    for (unsigned long i = 0UL; i < c; i++) {
        auto comp = std::make_shared<ConnectedComponent>(componentSize[i]);
        components.push_back(comp);
        orderedComponents.push_back(comp);
    }
    auto compare = [](const std::shared_ptr<ConnectedComponent>& o1, const std::shared_ptr<ConnectedComponent>& o2) {
        return o2->m_size < o1->m_size;
    };
    std::sort(orderedComponents.begin(), orderedComponents.end(), compare);
    for (unsigned long i = 0UL; i < orderedComponents.size(); i++) {
        orderedComponents[i]->m_orderedNumber = i;
    }

    componentSize = std::vector<unsigned long>(orderedComponents.size());
    for (const auto& cc : orderedComponents) {
        componentSize[cc->m_orderedNumber] = cc->m_size;
    }
    for (unsigned long i = 0UL; i < componentNumber.size();) {
        const auto& cc = components[*componentNumber[i]];
        componentNumber[i] = cc->m_orderedNumber;
        ++i;
    }
    return ConnectedComponentsComputationResult(std::move(componentNumber), componentSize);
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
