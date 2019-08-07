/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <vector>

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/math/UndirectedGraph.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "AssertionUtils.hpp"

namespace powsybl {

namespace math {

class V {
public:
    explicit V(std::string name) :
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
    explicit E(std::string name) :
        m_name(std::move(name)) {
    }

    const std::string& getName() const {
        return m_name;
    }

private:
    std::string m_name;
};

BOOST_AUTO_TEST_SUITE(UndirectedGraphTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    UndirectedGraph<V, E> graph;

    BOOST_CHECK_EQUAL(0ul, graph.getEdgeCount());
    BOOST_CHECK_EQUAL(0ul, graph.getVertexCount());
}

BOOST_AUTO_TEST_CASE(addEdge) {
    UndirectedGraph<V, E> graph;

    unsigned long v1 = graph.addVertex();
    unsigned long v2 = graph.addVertex();
    BOOST_CHECK_EQUAL(2ul, graph.getVertexCount());

    unsigned long e = graph.addEdge(v1, v2, stdcxx::ref<E>());
    BOOST_CHECK_EQUAL(1ul, graph.getEdgeCount());
    BOOST_CHECK_EQUAL(0ul, e);
}

BOOST_AUTO_TEST_CASE(addVertex) {
    UndirectedGraph<V, E> graph;
    graph.addVertex();
    BOOST_CHECK_EQUAL(1ul, graph.getVertexCount());
}

BOOST_AUTO_TEST_CASE(getEdgeObject) {
    E expected("edge");

    UndirectedGraph<V, E> graph;
    graph.addVertex();
    graph.addVertex();
    graph.addEdge(0, 1, stdcxx::ref(expected));

    const stdcxx::Reference<E>& edge = graph.getEdgeObject(0);
    BOOST_TEST(stdcxx::areSame(expected, edge.get()));

    const auto& objects = graph.getEdgeObjects();
    BOOST_CHECK_EQUAL(1ul, stdcxx::size(objects));
    for (const auto& obj : objects) {
        BOOST_TEST(stdcxx::areSame(expected, obj.get()));
    }
}

BOOST_AUTO_TEST_CASE(getEdges) {
    std::set<unsigned long> expected = {0, 1};

    UndirectedGraph<V, E> graph;
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addEdge(0, 1, stdcxx::ref<E>());
    graph.addEdge(0, 2, stdcxx::ref<E>());

    const auto& edges = graph.getEdges();

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), std::begin(edges), std::end(edges));
}

BOOST_AUTO_TEST_CASE(getMaxVertex) {
    std::set<unsigned long> expected;

    UndirectedGraph<V, E> graph;

    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    expected = {0, 1, 2};
    BOOST_CHECK_EQUAL(3ul, graph.getMaxVertex());
    const auto& vertices = graph.getVertices();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), std::begin(vertices), std::end(vertices));

    graph.removeVertex(0);
    graph.removeVertex(1);
    expected = {2};
    BOOST_CHECK_EQUAL(3ul, graph.getMaxVertex());
    const auto& vertices2 = graph.getVertices();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), std::begin(vertices2), std::end(vertices2));

    graph.addVertex();
    expected = {0, 2};
    BOOST_CHECK_EQUAL(3ul, graph.getMaxVertex());
    const auto& vertices3 = graph.getVertices();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), std::begin(vertices3), std::end(vertices3));

    graph.removeVertex(2);
    expected = {0};
    BOOST_CHECK_EQUAL(2ul, graph.getMaxVertex());
    const auto& vertices4 = graph.getVertices();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), std::begin(vertices4), std::end(vertices4));
}

BOOST_AUTO_TEST_CASE(getVertexObject) {
    V expected("vertex");

    UndirectedGraph<V, E> graph;
    graph.addVertex();

    BOOST_TEST(!graph.getVertexObject(0));
    BOOST_TEST(!graph.getVertexObject(0));

    graph.setVertexObject(0, stdcxx::ref(expected));
    BOOST_TEST(stdcxx::areSame(expected, graph.getVertexObject(0).get()));

    const auto& objects = graph.getVertexObjects();
    BOOST_CHECK_EQUAL(1ul, stdcxx::size(objects));
    for (const auto& obj : objects) {
        BOOST_TEST(stdcxx::areSame(expected, obj.get()));
    }

}

BOOST_AUTO_TEST_CASE(findAllPaths) {
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
    graph.setVertexObject(5, stdcxx::ref(vertex));
    graph.addEdge(0, 1, stdcxx::ref<E>());
    graph.addEdge(0, 2, stdcxx::ref<E>());
    graph.addEdge(0, 3, stdcxx::ref<E>());
    graph.addEdge(1, 4, stdcxx::ref<E>());
    graph.addEdge(2, 4, stdcxx::ref<E>());
    graph.addEdge(4, 5, stdcxx::ref<E>());
    graph.addEdge(3, 5, stdcxx::ref<E>());

    UndirectedGraph<V, E>::VertexVisitor pathComplete = [](const stdcxx::Reference<V>& vertex) {
        return vertex && vertex.get().getName() == "end";
    };
    UndirectedGraph<V, E>::EdgeVisitor pathCanceled = [](const stdcxx::Reference<E>& /*edge*/) {
        return false;
    };

    const std::vector<UndirectedGraph<V, E>::Path>& paths = graph.findAllPaths(0, pathComplete, pathCanceled);
    BOOST_CHECK_EQUAL(3ul, paths.size());
    BOOST_CHECK_EQUAL_COLLECTIONS(expected1.cbegin(), expected1.cend(), paths[0].cbegin(), paths[0].cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expected2.cbegin(), expected2.cend(), paths[1].cbegin(), paths[1].cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(expected3.cbegin(), expected3.cend(), paths[2].cbegin(), paths[2].cend());
}

BOOST_AUTO_TEST_CASE(removeEdge) {
    E expectedEdge1("edge1");
    E expectedEdge2("edge2");
    E expectedEdge3("edge3");
    E expectedEdge4("edge4");

    UndirectedGraph<V, E> graph;

    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    BOOST_CHECK_EQUAL(3ul, graph.getVertexCount());

    unsigned long e1 = graph.addEdge(0, 1, stdcxx::ref(expectedEdge1));
    BOOST_CHECK_EQUAL(1ul, graph.getEdgeCount());

    POWSYBL_ASSERT_THROW(graph.removeAllVertices(), PowsyblException, "Cannot remove all vertices because there is still some edges in the graph");
    const stdcxx::Reference<E>& edge1 = graph.removeEdge(e1);
    POWSYBL_ASSERT_THROW(graph.removeEdge(e1), PowsyblException, "Edge 0 not found");
    BOOST_CHECK_EQUAL(0ul, graph.getEdgeCount());
    BOOST_TEST(stdcxx::areSame(expectedEdge1, edge1.get()));

    unsigned long e2 = graph.addEdge(0, 1, stdcxx::ref(expectedEdge2));
    unsigned long e3 = graph.addEdge(1, 2, stdcxx::ref(expectedEdge3));
    BOOST_CHECK_EQUAL(0ul, e2);
    BOOST_CHECK_EQUAL(1ul, e3);

    const stdcxx::Reference<E>& edge2 = graph.removeEdge(e2);
    BOOST_CHECK_EQUAL(1ul, graph.getEdgeCount());
    BOOST_TEST(stdcxx::areSame(expectedEdge2, edge2.get()));

    unsigned long e4 = graph.addEdge(0, 1, stdcxx::ref(expectedEdge4));
    BOOST_CHECK_EQUAL(0ul, e4);

    const stdcxx::Reference<E>& edge3 = graph.removeEdge(e3);
    const stdcxx::Reference<E>& edge4 = graph.removeEdge(e4);
    BOOST_CHECK_EQUAL(0ul, graph.getEdgeCount());
    BOOST_TEST(stdcxx::areSame(expectedEdge3, edge3.get()));
    BOOST_TEST(stdcxx::areSame(expectedEdge4, edge4.get()));
}

BOOST_AUTO_TEST_CASE(removeVertex) {
    V expected("vertex");

    UndirectedGraph<V, E> graph;

    unsigned long v1 = graph.addVertex();
    unsigned long v2 = graph.addVertex();
    unsigned long e1 = graph.addEdge(v1, v2, stdcxx::ref<E>());
    graph.setVertexObject(v1, stdcxx::ref(expected));

    POWSYBL_ASSERT_THROW(graph.removeVertex(v1), PowsyblException, "An edge is connected to the vertex 0");

    graph.removeEdge(e1);
    const stdcxx::Reference<V>& vertex1 = graph.removeVertex(v1);
    POWSYBL_ASSERT_THROW(graph.removeVertex(v1), PowsyblException, "Vertex 0 not found");
    BOOST_CHECK_EQUAL(1ul, graph.getVertexCount());
    BOOST_TEST(stdcxx::areSame(expected, vertex1.get()));
}

BOOST_AUTO_TEST_CASE(traverse) {
    std::vector<bool> expected = {false, false, false, false, true, true};

    UndirectedGraph<V, E> graph;
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addEdge(0, 1, stdcxx::ref<E>());
    graph.addEdge(0, 2, stdcxx::ref<E>());
    graph.addEdge(0, 3, stdcxx::ref<E>());
    graph.addEdge(1, 4, stdcxx::ref<E>());
    graph.addEdge(2, 1, stdcxx::ref<E>());
    graph.addEdge(4, 5, stdcxx::ref<E>());
    graph.addEdge(3, 5, stdcxx::ref<E>());

    Traverser traverser = [](unsigned long, unsigned long e, unsigned long) {
        return (e == 3 || e == 4 || e == 6) ? TraverseResult::TERMINATE : TraverseResult::CONTINUE;
    };

    std::vector<bool> encountered(graph.getVertexCount(), false);
    graph.traverse(5, traverser, encountered);
    graph.traverse(5, traverser);

    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), encountered.cbegin(), encountered.cend());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace math

}  // namespace powsybl
