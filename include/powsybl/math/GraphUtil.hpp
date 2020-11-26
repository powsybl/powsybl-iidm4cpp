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

namespace GraphUtil {

using AdjacencyList = std::vector<std::vector<unsigned long>>;

ConnectedComponentsComputationResult computeConnectedComponents(const AdjacencyList& adjacencyList);

}  // namespace GraphUtil

}  // namespace math

}  // namespace powsybl

#endif  // POWSYBL_MATH_GRAPHUTIL_HPP
