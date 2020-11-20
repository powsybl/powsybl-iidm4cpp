/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_MATH_GRAPHUTIL_HXX
#define POWSYBL_MATH_GRAPHUTIL_HXX

#include <powsybl/math/GraphUtil.hpp>

namespace powsybl {

namespace math {

template <typename V, typename E>
void GraphUtil::removeIsolatedVertices(UndirectedGraph<V, E>& graph) {
    std::set<unsigned long> connectedVertices;
    for (unsigned long e : graph.getEdges()) {
        connectedVertices.emplace(graph.getEdgeVertex1(e));
        connectedVertices.emplace(graph.getEdgeVertex2(e));
    }

    for (unsigned long v : graph.getVertices()) {
        if (connectedVertices.find(v) == connectedVertices.end() && graph.getVertexObject(v)) {
            graph.removeVertex(v);
        }
    }
}

}  // namespace math

}  // namespace powsybl

#endif  // POWSYBL_MATH_GRAPHUTIL_HXX
