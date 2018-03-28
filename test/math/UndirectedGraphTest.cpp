/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vector>

#include <gtest/gtest.h>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/math/UndirectedGraph.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace math {

class V {
public:
    V(std::string name) :
        m_name(std::move(name)) {
    }

    const std::string& getName() const {
        return m_name;
    }

private:
    std::string m_name;
};

class E {
public:
    E(std::string name) :
        m_name(std::move(name)) {
    }

    const std::string& getName() const {
        return m_name;
    }

private:
    std::string m_name;
};

TEST(UndirectedGraph, constructor) {
    UndirectedGraph<V, E> graph;

    ASSERT_EQ(0ul, graph.getEdgeCount());
    ASSERT_EQ(0ul, graph.getVertexCount());
}

TEST(UndirectedGraph, addEdge) {
    UndirectedGraph<V, E> graph;

    unsigned long v1 = graph.addVertex();
    unsigned long v2 = graph.addVertex();
    ASSERT_EQ(2ul, graph.getVertexCount());

    unsigned long e = graph.addEdge(v1, v2, stdcxx::optref<E>());
    ASSERT_EQ(1ul, graph.getEdgeCount());
    ASSERT_EQ(0ul, e);
}

TEST(UndirectedGraph, addVertex) {
    UndirectedGraph<V, E> graph;
    graph.addVertex();
    ASSERT_EQ(1ul, graph.getVertexCount());
}

TEST(UndirectedGraph, getEdgeObject) {
    E expected("edge");

    UndirectedGraph<V, E> graph;
    graph.addVertex();
    graph.addVertex();
    graph.addEdge(0, 1, stdcxx::optref(expected));

    const stdcxx::Optional<E>& edge = graph.getEdgeObject(0);
    ASSERT_TRUE(stdcxx::areSame(expected, edge.get()));

    const std::vector<stdcxx::Optional<E> >& objects = graph.getEdgeObjects();
    ASSERT_EQ(1ul, objects.size());
    ASSERT_TRUE(stdcxx::areSame(expected, objects.at(0).get()));
}

TEST(UndirectedGraph, getEdges) {
    std::set<unsigned long> expected = {0, 1};

    UndirectedGraph<V, E> graph;
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addEdge(0, 1, stdcxx::optref<E>());
    graph.addEdge(0, 2, stdcxx::optref<E>());

    const std::set<unsigned long>& edges = graph.getEdges();

    ASSERT_EQ(expected, edges);
}

TEST(UndirectedGraph, getMaxVertex) {
    std::set<unsigned long> expected;

    UndirectedGraph<V, E> graph;

    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    expected = {0, 1, 2};
    ASSERT_EQ(3ul, graph.getMaxVertex());
    ASSERT_EQ(expected, graph.getVertices());

    graph.removeVertex(0);
    graph.removeVertex(1);
    expected = {2};
    ASSERT_EQ(3ul, graph.getMaxVertex());
    ASSERT_EQ(expected, graph.getVertices());

    graph.addVertex();
    expected = {0, 2};
    ASSERT_EQ(3ul, graph.getMaxVertex());
    ASSERT_EQ(expected, graph.getVertices());

    graph.removeVertex(2);
    expected = {0};
    ASSERT_EQ(2ul, graph.getMaxVertex());
    ASSERT_EQ(expected, graph.getVertices());
}

TEST(UndirectedGraph, getVertexObject) {
    V expected("vertex");

    UndirectedGraph<V, E> graph;
    graph.addVertex();

    ASSERT_FALSE(graph.getVertexObject(0));
    ASSERT_TRUE(!graph.getVertexObject(0));

    graph.setVertexObject(0, stdcxx::optref(expected));
    ASSERT_TRUE(stdcxx::areSame(expected, graph.getVertexObject(0).get()));

    const std::vector<stdcxx::Optional<V> >& objects = graph.getVertexObjects();
    ASSERT_EQ(1ul, objects.size());
    ASSERT_TRUE(stdcxx::areSame(expected, objects.at(0).get()));
}

TEST(UndirectedGraph, findAllPaths) {
    std::vector<unsigned long> expected1 = {2, 6};
    std::vector<unsigned long> expected2 = {0, 3, 5};
    std::vector<unsigned long> expected3 = {1, 4, 5};

    V vertex("end");
    UndirectedGraph<V, E> graph;
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.setVertexObject(5, stdcxx::optref(vertex));
    graph.addEdge(0, 1, stdcxx::optref<E>());
    graph.addEdge(0, 2, stdcxx::optref<E>());
    graph.addEdge(0, 3, stdcxx::optref<E>());
    graph.addEdge(1, 4, stdcxx::optref<E>());
    graph.addEdge(2, 4, stdcxx::optref<E>());
    graph.addEdge(4, 5, stdcxx::optref<E>());
    graph.addEdge(3, 5, stdcxx::optref<E>());

    UndirectedGraph<V, E>::VertexVisitor pathComplete = [](const stdcxx::Optional<V>& vertex) {
        return vertex && vertex.get().getName() == "end";
    };
    UndirectedGraph<V, E>::EdgeVisitor pathCanceled = [](const stdcxx::Optional<E>& /*edge*/) {
        return false;
    };

    const std::vector<UndirectedGraph<V, E>::Path>& paths = graph.findAllPaths(0, pathComplete, pathCanceled);
    ASSERT_EQ(3ul, paths.size());
    ASSERT_EQ(expected1, paths[0]);
    ASSERT_EQ(expected2, paths[1]);
    ASSERT_EQ(expected3, paths[2]);
}

TEST(UndirectedGraph, removeEdge) {
    E expectedEdge1("edge1");
    E expectedEdge2("edge2");
    E expectedEdge3("edge3");
    E expectedEdge4("edge4");

    UndirectedGraph<V, E> graph;

    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    ASSERT_EQ(3ul, graph.getVertexCount());

    unsigned long e1 = graph.addEdge(0, 1, stdcxx::optref<E>(expectedEdge1));
    ASSERT_EQ(1ul, graph.getEdgeCount());

    const stdcxx::Optional<E>& edge1 = graph.removeEdge(e1);
    ASSERT_EQ(0ul, graph.getEdgeCount());
    ASSERT_TRUE(stdcxx::areSame(expectedEdge1, edge1.get()));

    unsigned long e2 = graph.addEdge(0, 1, stdcxx::optref<E>(expectedEdge2));
    unsigned long e3 = graph.addEdge(1, 2, stdcxx::optref<E>(expectedEdge3));
    ASSERT_EQ(0ul, e2);
    ASSERT_EQ(1ul, e3);

    const stdcxx::Optional<E>& edge2 = graph.removeEdge(e2);
    ASSERT_EQ(1ul, graph.getEdgeCount());
    ASSERT_TRUE(stdcxx::areSame(expectedEdge2, edge2.get()));

    unsigned long e4 = graph.addEdge(0, 1, stdcxx::optref<E>(expectedEdge4));
    ASSERT_EQ(0ul, e4);

    const stdcxx::Optional<E>& edge3 = graph.removeEdge(e3);
    const stdcxx::Optional<E>& edge4 = graph.removeEdge(e4);
    ASSERT_EQ(0ul, graph.getEdgeCount());
    ASSERT_TRUE(stdcxx::areSame(expectedEdge3, edge3.get()));
    ASSERT_TRUE(stdcxx::areSame(expectedEdge4, edge4.get()));
}

TEST(UndirectedGraph, removeVertex) {
    V expected("vertex");

    UndirectedGraph<V, E> graph;

    unsigned long v1 = graph.addVertex();
    unsigned long v2 = graph.addVertex();
    unsigned long e1 = graph.addEdge(v1, v2, stdcxx::optref<E>());
    graph.setVertexObject(v1, stdcxx::optref(expected));

    POWSYBL_ASSERT_THROW(graph.removeVertex(v1), PowsyblException, "An edge is connected to the vertex 0");

    graph.removeEdge(e1);
    const stdcxx::Optional<V>& vertex1 = graph.removeVertex(v1);
    ASSERT_EQ(1ul, graph.getVertexCount());
    ASSERT_TRUE(stdcxx::areSame(expected, vertex1.get()));
}

TEST(UndirectedGraph, traverse) {
    std::vector<bool> expected = {false, false, false, false, true, true};

    UndirectedGraph<V, E> graph;
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addEdge(0, 1, stdcxx::optref<E>());
    graph.addEdge(0, 2, stdcxx::optref<E>());
    graph.addEdge(0, 3, stdcxx::optref<E>());
    graph.addEdge(1, 4, stdcxx::optref<E>());
    graph.addEdge(2, 1, stdcxx::optref<E>());
    graph.addEdge(4, 5, stdcxx::optref<E>());
    graph.addEdge(3, 5, stdcxx::optref<E>());

    Traverser traverser = [](unsigned long, unsigned long e, unsigned long) {
        return (e == 3 || e == 4 || e == 6) ? TraverseResult::TERMINATE : TraverseResult::CONTINUE;
    };

    std::vector<bool> encountered(graph.getVertexCount(), false);
    graph.traverse(5, traverser, encountered);

    ASSERT_EQ(expected, encountered);
}

}

}
