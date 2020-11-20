/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_MATH_GRAPHUTIL_HPP
#define POWSYBL_MATH_GRAPHUTIL_HPP

#include <vector>

#include <powsybl/math/ConnectedComponentsComputationResult.hpp>
#include <powsybl/math/UndirectedGraph.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace math {

class GraphUtil {
public:
    using ConnectedComponentsComputationResult = graph_util::ConnectedComponentsComputationResult;

public:
    static ConnectedComponentsComputationResult computeConnectedComponents(const std::vector<std::vector<unsigned long>>& adjacencyList);

public:
    GraphUtil() = delete;

    template <typename V, typename E>
    void removeIsolatedVertices(UndirectedGraph<V, E>& graph);

private:
    static void computeConnectedComponents(unsigned long v1, unsigned long c, std::vector<unsigned long>& componentSize, const std::vector<std::vector<unsigned long>>& adjacencyList, std::vector<stdcxx::optional<unsigned long>>& componentNumber);
};

}  // namespace math

}  // namespace powsybl

#include <powsybl/math/GraphUtil.hxx>

#endif  // POWSYBL_MATH_GRAPHUTIL_HPP
