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

#include <powsybl/test/AssertionUtils.hpp>

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

    BOOST_CHECK_EQUAL(0UL, graph.getEdgeCount());
    BOOST_CHECK_EQUAL(0UL, graph.getVertexCount());
}

BOOST_AUTO_TEST_CASE(addEdge) {
    UndirectedGraph<V, E> graph;

    unsigned long v1 = graph.addVertex();
    unsigned long v2 = graph.addVertex();
    BOOST_CHECK_EQUAL(2UL, graph.getVertexCount());

    unsigned long e = graph.addEdge(v1, v2, stdcxx::ref<E>());
    BOOST_CHECK_EQUAL(1UL, graph.getEdgeCount());
    BOOST_CHECK_EQUAL(0UL, e);
}

BOOST_AUTO_TEST_CASE(addVertex) {
    UndirectedGraph<V, E> graph;
    graph.addVertex();
    BOOST_CHECK_EQUAL(1UL, graph.getVertexCount());
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
    BOOST_CHECK_EQUAL(1UL, boost::size(objects));
    for (const auto& obj : objects) {
        BOOST_TEST(stdcxx::areSame(expected, obj.get()));
    }

    graph.addVertex();
    graph.addVertex();
    graph.removeVertex(2UL);
    BOOST_CHECK(graph.vertexExists(0UL));
    BOOST_CHECK(graph.vertexExists(1UL));
    BOOST_CHECK(!graph.vertexExists(2UL));
    BOOST_CHECK(graph.vertexExists(3UL));
    BOOST_CHECK(!graph.vertexExists(4UL));
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
    BOOST_CHECK_EQUAL(3UL, graph.getMaxVertex());
    const auto& vertices = graph.getVertices();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), std::begin(vertices), std::end(vertices));

    graph.removeVertex(0);
    graph.removeVertex(1);
    expected = {2};
    BOOST_CHECK_EQUAL(3UL, graph.getMaxVertex());
    const auto& vertices2 = graph.getVertices();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), std::begin(vertices2), std::end(vertices2));

    graph.addVertex();
    expected = {0, 2};
    BOOST_CHECK_EQUAL(3UL, graph.getMaxVertex());
    const auto& vertices3 = graph.getVertices();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), std::begin(vertices3), std::end(vertices3));

    graph.removeVertex(2);
    expected = {0};
    BOOST_CHECK_EQUAL(1UL, graph.getMaxVertex());
    const auto& vertices4 = graph.getVertices();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), std::begin(vertices4), std::end(vertices4));

    graph.addVertexIfNotPresent(3);
    expected = {0, 3};
    BOOST_CHECK_EQUAL(2UL, graph.getVertexCount());
    BOOST_CHECK_EQUAL(4UL, graph.getMaxVertex());
    const auto& vertices5 = graph.getVertices();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), std::begin(vertices5), std::end(vertices5));

    graph.addVertexIfNotPresent(1);
    expected = {0, 1, 3};
    BOOST_CHECK_EQUAL(3UL, graph.getVertexCount());
    BOOST_CHECK_EQUAL(4UL, graph.getMaxVertex());
    const auto& vertices6 = graph.getVertices();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), std::begin(vertices6), std::end(vertices6));

    graph.removeVertex(3);
    expected = {0, 1};
    BOOST_CHECK_EQUAL(2UL, graph.getVertexCount());
    BOOST_CHECK_EQUAL(2UL, graph.getMaxVertex());
    const auto& vertices7 = graph.getVertices();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.cbegin(), expected.cend(), std::begin(vertices7), std::end(vertices7));

    graph.removeAllVertices();
    BOOST_CHECK_EQUAL(0UL, graph.getVertexCount());
    BOOST_CHECK_EQUAL(0UL, graph.getMaxVertex());
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
    BOOST_CHECK_EQUAL(1UL, boost::size(objects));
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
    BOOST_CHECK_EQUAL(3UL, paths.size());
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
    BOOST_CHECK_EQUAL(3UL, graph.getVertexCount());

    unsigned long e1 = graph.addEdge(0, 1, stdcxx::ref(expectedEdge1));
    BOOST_CHECK_EQUAL(1UL, graph.getEdgeCount());

    POWSYBL_ASSERT_THROW(graph.removeAllVertices(), PowsyblException, "Cannot remove all vertices because there is still some edges in the graph");
    const stdcxx::Reference<E>& edge1 = graph.removeEdge(e1);
    POWSYBL_ASSERT_THROW(graph.removeEdge(e1), PowsyblException, "Edge 0 not found");
    BOOST_CHECK_EQUAL(0UL, graph.getEdgeCount());
    BOOST_TEST(stdcxx::areSame(expectedEdge1, edge1.get()));

    unsigned long e2 = graph.addEdge(0, 1, stdcxx::ref(expectedEdge2));
    unsigned long e3 = graph.addEdge(1, 2, stdcxx::ref(expectedEdge3));
    BOOST_CHECK_EQUAL(0UL, e2);
    BOOST_CHECK_EQUAL(1UL, e3);

    const stdcxx::Reference<E>& edge2 = graph.removeEdge(e2);
    BOOST_CHECK_EQUAL(1UL, graph.getEdgeCount());
    BOOST_TEST(stdcxx::areSame(expectedEdge2, edge2.get()));

    unsigned long e4 = graph.addEdge(0, 1, stdcxx::ref(expectedEdge4));
    BOOST_CHECK_EQUAL(0UL, e4);

    const stdcxx::Reference<E>& edge3 = graph.removeEdge(e3);
    const stdcxx::Reference<E>& edge4 = graph.removeEdge(e4);
    BOOST_CHECK_EQUAL(0UL, graph.getEdgeCount());
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
    BOOST_CHECK_EQUAL(1UL, graph.getVertexCount());
    BOOST_TEST(stdcxx::areSame(expected, vertex1.get()));
}


    /**
     *           0
     *           |
     *         ---------
     *         |   |   |
     *         1   2   3
     *         |   |   |
     *         -----   |
     *           |     |
     *           4     |
     *           |     |
     *           -------
     *              |
     *              5
     *  edges:
     *  0 <-> 1 : 0
     *  0 <-> 2 : 1
     *  0 <-> 3 : 2
     *  1 <-> 4 : 3
     *  2 <-> 4 : 4
     *  4 <-> 5 : 5
     *  3 <-> 5 : 6
     *
     *  all paths (edge numbers) between vertex 0 and 5:
     *  0, 3, 5
     *  1, 4, 5
     *  2, 6
     */
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
    graph.addEdge(2, 4, stdcxx::ref<E>());
    graph.addEdge(4, 5, stdcxx::ref<E>());
    graph.addEdge(3, 5, stdcxx::ref<E>());

    const Traverser& traverser = [](unsigned long v1, unsigned long e, unsigned long v2) {
        if (v1 == 4 && e == 3 && v2 == 1) {
            return TraverseResult::TERMINATE_PATH;
        }
        if (v1 == 4 && e == 4 && v2 == 2) {
            return TraverseResult::TERMINATE_PATH;
        }
        if (v1 == 5 && e == 6 && v2 == 3) {
            return TraverseResult::TERMINATE_PATH;
        }
        return TraverseResult::CONTINUE;
    };

    std::vector<bool> encountered(graph.getVertexCount());
    std::fill(encountered.begin(), encountered.end(), false);
    std::vector<bool> encounteredExpected = {false, false, false, false, true, true};
    graph.traverse(5, TraversalType::DEPTH_FIRST ,traverser, encountered);
    BOOST_CHECK_EQUAL_COLLECTIONS(encountered.begin(), encountered.end(), encounteredExpected.begin(), encounteredExpected.end());
    std::fill(encountered.begin(), encountered.end(), false);
    graph.traverse(5, TraversalType::BREADTH_FIRST ,traverser, encountered);
    BOOST_CHECK_EQUAL_COLLECTIONS(encountered.begin(), encountered.end(), encounteredExpected.begin(), encounteredExpected.end());

    std::fill(encountered.begin(), encountered.end(), false);
    const Traverser& traverser2 = [&](unsigned long v1, unsigned long /*e*/, unsigned long v2) {
        encountered[v1] = true;
        return v2 == 1 || v2 == 2 || v2 == 3 ? TraverseResult::TERMINATE_PATH : TraverseResult::CONTINUE;
    };
    graph.traverse(4, TraversalType::DEPTH_FIRST, traverser2);
    // Only vertex 4 and 5 encountered
    std::vector<bool> encounteredExpected2 = {false, false, false, false, true, true};
    BOOST_CHECK_EQUAL_COLLECTIONS(encountered.begin(), encountered.end(), encounteredExpected2.begin(), encounteredExpected2.end());
    std::fill(encountered.begin(), encountered.end(), false);
    graph.traverse(4, TraversalType::BREADTH_FIRST, traverser2);
    BOOST_CHECK_EQUAL_COLLECTIONS(encountered.begin(), encountered.end(), encounteredExpected2.begin(), encounteredExpected2.end());

    const Traverser& traverser3 = [&](unsigned long v1, unsigned long /*e*/, unsigned long v2) {
        encountered[v1] = true;
        return v2 == 0 ? TraverseResult::TERMINATE_TRAVERSER : TraverseResult::CONTINUE;
    };
    std::fill(encountered.begin(), encountered.end(), false);
    graph.traverse(5, TraversalType::DEPTH_FIRST, traverser3, encountered);
    // Only vertices on first path encountering 0 are encountered
    std::vector<bool> encounteredDepthExpected3 = {false, true, false, false, true, true};
    BOOST_CHECK_EQUAL_COLLECTIONS(encountered.begin(), encountered.end(), encounteredDepthExpected3.begin(), encounteredDepthExpected3.end());
    std::fill(encountered.begin(), encountered.end(), false);
    std::vector<bool> encounteredBreadthExpected3 = {false, false, false, true, true, true};
    graph.traverse(5, TraversalType::BREADTH_FIRST, traverser3);
    BOOST_CHECK_EQUAL_COLLECTIONS(encountered.begin(), encountered.end(), encounteredBreadthExpected3.begin(), encounteredBreadthExpected3.end());


    std::fill(encountered.begin(), encountered.end(), false);
    std::vector<bool> eEncountered(graph.getEdgeCount());
    std::fill(eEncountered.begin(), eEncountered.end(), false);
    const Traverser& traverser4 = [&](unsigned long /*v1*/, unsigned long e, unsigned long /*v2*/) {
        eEncountered[e] = true;
        return TraverseResult::CONTINUE;
    };

    graph.traverse(5, TraversalType::BREADTH_FIRST, traverser4, encountered);
    //All vertices and edges are encountered :
    std::vector<bool> encounteredBreadthExpected4 = {true, true, true, true, true, true};
    std::vector<bool> eEncounteredBreadthExpected4 = {true, true, true, true, true, true, true};
    BOOST_CHECK_EQUAL_COLLECTIONS(encountered.begin(), encountered.end(), encounteredBreadthExpected4.begin(), encounteredBreadthExpected4.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(eEncountered.begin(), eEncountered.end(), eEncounteredBreadthExpected4.begin(), eEncounteredBreadthExpected4.end());

    struct GraphPath
    {
        unsigned long v1;
        unsigned long e;
        unsigned long v2;

        bool operator!=(const GraphPath& reference) const {
            return v1 != reference.v1 ||e != reference.e ||v2 != reference.v2 ;
        }

    };

    std::vector<GraphPath> breadthFirstexpected = {
                {5, 5, 4},
                {5, 6, 3},
                {4, 3, 1},
                {4, 4, 2},
                {3, 2, 0},
                {1, 0, 0},
                {2, 1, 0}};
    std::vector<GraphPath> depthFirstExpected = {
                {5, 5, 4},
                {4, 3, 1},
                {1, 0, 0},
                {0, 1, 2},
                {2, 4, 4},
                {0, 2, 3},
                {3, 6, 5}};
    
    // Check that all edges and vertices are traversed in the right order when traversing the graph with no stopping point
    std::vector<GraphPath> pathsBf;
    std::vector<GraphPath> pathsDf;
    graph.traverse(5, TraversalType::BREADTH_FIRST, [&](unsigned long v1, unsigned long e,unsigned long  v2){
            pathsBf.push_back({v1, e, v2});
            return TraverseResult::CONTINUE;
        });
    graph.traverse(5, TraversalType::DEPTH_FIRST, [&](unsigned long v1, unsigned long e, unsigned long v2){
            pathsDf.push_back({v1, e, v2});
            return TraverseResult::CONTINUE;
        });

    auto itpathBf = pathsBf.begin();
    auto itpathBfExp = breadthFirstexpected.begin();
    for(; itpathBf != pathsBf.end() && itpathBfExp != breadthFirstexpected.end(); ++itpathBf, ++itpathBfExp) {
        BOOST_CHECK(!( *itpathBf != *itpathBfExp ));
    }
    auto itpathDf = pathsDf.begin();
    auto itpathDfExp = depthFirstExpected.begin();
    for(; itpathDf != pathsDf.end() && itpathDfExp != depthFirstExpected.end(); ++itpathDf, ++itpathDfExp) {
        BOOST_CHECK(!( *itpathDf != *itpathDfExp ));
    }

    // Check all calls done when traversing the graph with one stopping point at vertex 0 when arriving from 3
    // to ensure the edge 0 and 1 still get traversed even if the destination vertex 0 is already encountered
    std::vector<GraphPath> pathsWithStoppingPoint;
    graph.traverse(5, TraversalType::BREADTH_FIRST, [&](unsigned long v1, unsigned long e, unsigned long v2){
            pathsWithStoppingPoint.push_back({v1, e, v2});
            return (v1 == 3 && v2 == 0) ? TraverseResult::TERMINATE_PATH : TraverseResult::CONTINUE;
        });
    auto itpathStop = pathsWithStoppingPoint.begin();
    auto itpathExp = breadthFirstexpected.begin();
    for(; itpathStop != pathsWithStoppingPoint.end() && itpathExp != breadthFirstexpected.end(); ++itpathStop, ++itpathExp) {
        BOOST_CHECK(!( *itpathStop != *itpathExp ));
    }

}

    /**
     *           0
     *           |
     *         -------
     *         |  |  |
     *         -------
     *           |
     *           1
     *           |
     *           2
     *
     *  edges:
     *  0 <-> 1 : 0
     *  0 <-> 1 : 1
     *  1 <-> 2 : 2
     *  0 <-> 1 : 3
     */
BOOST_AUTO_TEST_CASE(traverseParallelEdges) {

    UndirectedGraph<V, E> graph;
    graph.addVertex();
    graph.addVertex();
    graph.addVertex();
    graph.addEdge(0, 1, stdcxx::ref<E>()); //0
    graph.addEdge(1, 0, stdcxx::ref<E>()); //1
    graph.addEdge(1, 2, stdcxx::ref<E>()); //2
    graph.addEdge(0, 1, stdcxx::ref<E>()); //3

    struct GraphPath
    {
        unsigned long v1;
        unsigned long e;
        unsigned long v2;

        bool operator!=(const GraphPath& reference) const {
            return v1 != reference.v1 ||e != reference.e ||v2 != reference.v2 ;
        }

    };
    std::vector<GraphPath> expectedBF = {
        {0, 0, 1},
        {0, 1, 1},
        {0, 3, 1},
        {1, 2, 2}
    };
    std::vector<GraphPath> expectedDF = {
        {0, 0, 1},
        {1, 1, 0},
        {1, 2, 2},
        {1, 3, 0}
    };
    std::vector<GraphPath> pathBF;
    std::vector<GraphPath> pathDF;
    graph.traverse(0, TraversalType::BREADTH_FIRST, [&](unsigned long v1, unsigned long e, unsigned long v2){
        pathBF.push_back({v1, e, v2});
        return TraverseResult::CONTINUE;
    });
    graph.traverse(0, TraversalType::DEPTH_FIRST, [&](unsigned long v1, unsigned long e, unsigned long v2){
        pathDF.push_back({v1, e, v2});
        return TraverseResult::CONTINUE;
    });

    auto itpathBF = pathBF.begin();
    auto itpathExpBF = expectedBF.begin();
    for(; itpathBF != pathBF.end() && itpathExpBF != expectedBF.end(); ++itpathBF, ++itpathExpBF) {
        BOOST_CHECK(!( *itpathBF != *itpathExpBF ));
    }
    auto itpathDF = pathDF.begin();
    auto itpathExpDF = expectedDF.begin();
    for(; itpathDF != pathDF.end() && itpathExpDF != expectedDF.end(); ++itpathDF, ++itpathExpDF) {
        BOOST_CHECK(!( *itpathDF != *itpathExpDF ));
    }

}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace math

}  // namespace powsybl
