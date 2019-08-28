/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/math/GraphUtil.hpp>

#include <algorithm>
#include <memory>
#include <utility>

#include <powsybl/iidm/ComponentConstants.hpp>

namespace powsybl {

namespace math {

namespace GraphUtil {

static void computeConnectedComponents(unsigned long v1, unsigned long c, std::vector<unsigned long>& componentSize,
    const std::vector<std::vector<long>>& adjacencyList, std::vector<long>& componentNumber);

ConnectedComponentsComputationResult::ConnectedComponentsComputationResult(std::vector<long>& componentNumber, std::vector<unsigned long>& orderedComponents) :
    m_componentNumber(std::move(componentNumber)),
    m_componentSize(std::move(orderedComponents)) {
}

const std::vector<long>& ConnectedComponentsComputationResult::getComponentNumber() const {
    return m_componentNumber;
}

const std::vector<unsigned long>& ConnectedComponentsComputationResult::getComponentSize() const {
    return m_componentSize;
}

ConnectedComponentsComputationResult computeConnectedComponents(const std::vector<std::vector<long>>& adjacencyList) {
    std::vector<long> componentNumber(adjacencyList.size(), iidm::ComponentConstants::INVALID_NUM);
    unsigned long c = 0UL;
    std::vector<unsigned long> componentSize(adjacencyList.size(), c);

    for (unsigned long v = 0UL; v < adjacencyList.size(); v++) {
        if (componentNumber[v] == iidm::ComponentConstants::INVALID_NUM) {
            computeConnectedComponents(v, c++, componentSize, adjacencyList, componentNumber);
        }
    }

    // sort components by size
    std::vector<std::shared_ptr<std::pair<unsigned long, long>>> components;
    components.reserve(c);
    std::vector<std::shared_ptr<std::pair<unsigned long, long>>> orderedComponents;
    orderedComponents.reserve(c);

    for (unsigned long i = 0UL; i < c; i++) {
        auto comp = std::make_shared<std::pair<unsigned long, long>>(componentSize[i], iidm::ComponentConstants::INVALID_NUM);
        components.push_back(comp);
        orderedComponents.push_back(comp);
    }
    auto compare = [](const std::shared_ptr<std::pair<unsigned long, long>>& o1, const std::shared_ptr<std::pair<unsigned long, long>>& o2) {
        return o2->first < o1->first;
    };
    std::sort(orderedComponents.begin(), orderedComponents.end(), compare);
    for (unsigned long i = 0UL; i < orderedComponents.size(); i++) {
        orderedComponents[i]->second = i;
    }

    componentSize = std::vector<unsigned long>(orderedComponents.size());
    for (const auto& cc : orderedComponents) {
        componentSize[cc->second] = cc->first;
    }
    for (unsigned long i = 0UL; i < componentNumber.size();) {
        const auto& cc = components[componentNumber[i]];
        componentNumber[i] = cc->second;
        ++i;
    }

    return ConnectedComponentsComputationResult(componentNumber, componentSize);
}

void computeConnectedComponents(unsigned long v1, unsigned long c, std::vector<unsigned long>& componentSize,
        const std::vector<std::vector<long>>& adjacencyList, std::vector<long>& componentNumber) {
    componentNumber[v1] = c;
    ++componentSize[c];
    const auto& ls = adjacencyList[v1];
    for (long v2 : ls) {
        if (componentNumber[v2] == iidm::ComponentConstants::INVALID_NUM) {
            computeConnectedComponents(v2, c, componentSize, adjacencyList, componentNumber);
        }
    }
}

}  // namespace GraphUtil

}  // namespace math

}  // namespace powsybl
