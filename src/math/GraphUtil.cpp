/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/math/GraphUtil.hpp>

namespace powsybl {

namespace math {

namespace GraphUtil {

struct ConnectedComponent {
    unsigned long size;

    unsigned long orderedNumber;
};

void computeConnectedComponents(unsigned long v1, unsigned long c, std::vector<unsigned long>& componentSize,
                                const AdjacencyList & adjacencyList, std::vector<stdcxx::optional<unsigned long>>& componentNumber) {
    componentNumber[v1] = c;
    ++componentSize[c];
    const std::vector<unsigned long>& ls = adjacencyList[v1];
    for (unsigned long v2 : ls) {
        if (!componentNumber[v2]) {
            computeConnectedComponents(v2, c, componentSize, adjacencyList, componentNumber);
        }
    }
}

ConnectedComponentsComputationResult computeConnectedComponents(const AdjacencyList& adjacencyList) {
    // componentNumber contains for each bus, its component number
    std::vector<stdcxx::optional<unsigned long>> componentNumber(adjacencyList.size());

    // componentSize contains the size (number of buses) of each component
    std::vector<unsigned long> componentSize(adjacencyList.size());

    // Fill componentSize and componentNumber based on the adjacency list
    unsigned long c = 0;
    for (unsigned long v = 0; v < adjacencyList.size(); v++) {
        if (!componentNumber[v]) {
            computeConnectedComponents(v, c, componentSize, adjacencyList, componentNumber);
            c++;
        }
    }

    // Initialize the components with their computed size and prepare the orderedComponents with references
    std::vector<ConnectedComponent> components(c);
    std::vector<std::reference_wrapper<ConnectedComponent>> orderedComponents;
    orderedComponents.reserve(components.size());
    for (unsigned long i = 0; i < c; ++i) {
        auto& component = components[i];
        component.size = componentSize[i];
        orderedComponents.emplace_back(component);
    }

    // Sort the components by their size (the main component is the bigger one)
    std::sort(orderedComponents.begin(), orderedComponents.end(), [](const std::reference_wrapper<ConnectedComponent>& c1, const std::reference_wrapper<ConnectedComponent>& c2) {
        return c2.get().size < c1.get().size;
    });

    // Assign the component number based on their order
    for (unsigned long i = 0; i < orderedComponents.size(); ++i) {
        orderedComponents[i].get().orderedNumber = i;
    }

    // componentSize contains the size (number of buses) of each component
    componentSize.resize(orderedComponents.size());
    for (const auto& comp : orderedComponents) {
        componentSize[comp.get().orderedNumber] = comp.get().size;
    }

    // componentNumber contains for each bus, its component number
    for (auto& it : componentNumber) {
        const auto& comp = components[*it];
        it = comp.orderedNumber;
    }

    return ConnectedComponentsComputationResult(componentNumber, componentSize);
}

}  // namespace GraphUtil

}  // namespace math

}  // namespace powsybl
