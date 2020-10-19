/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_MATH_GRAPHUTIL_HPP
#define POWSYBL_MATH_GRAPHUTIL_HPP

#include <vector>

#include <powsybl/math/UndirectedGraph.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace math {

class GraphUtil {
public:
    class ConnectedComponentsComputationResult {
    public:
        ConnectedComponentsComputationResult(const std::vector<stdcxx::optional<unsigned long>>& componentNumber, const std::vector<unsigned long>& orderedComponents);

        const std::vector<stdcxx::optional<unsigned long>>& getComponentNumber() const;

        std::vector<stdcxx::optional<unsigned long>>& getComponentNumber();

        const std::vector<unsigned long>& getComponentSize() const;

        std::vector<unsigned long>& getComponentSize();

    private:

        std::vector<stdcxx::optional<unsigned long>> m_componentNumber;

        std::vector<unsigned long> m_componentSize;
    };

public:
    static ConnectedComponentsComputationResult computeConnectedComponents(const std::vector<std::vector<unsigned long>>& adjacencyList);

    template <typename V, typename E>
    void removeIsolatedVertices(UndirectedGraph<V, E>& graph);

public:
    GraphUtil() = delete;

private:
    static void computeConnectedComponents(unsigned long v1, unsigned long c, std::vector<unsigned long>& componentSize, const std::vector<std::vector<unsigned long>>& adjacencyList, std::vector<stdcxx::optional<unsigned long>>& componentNumber);

private:
    class ConnectedComponent {
    public:
        explicit ConnectedComponent(unsigned long size);

        unsigned long getOrderedNumber() const;

        unsigned long getSize() const;

        ConnectedComponent& setOrderedNumber(unsigned long orderedNumber);

    private:
        unsigned long m_size;

        unsigned long m_orderedNumber = 0UL;
    };
};

}  // namespace math

}  // namespace powsybl

#include <powsybl/math/GraphUtil.hxx>

#endif  // POWSYBL_MATH_GRAPHUTIL_HPP
