/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_MATH_UNDIRECTEDGRAPH_HXX
#define POWSYBL_MATH_UNDIRECTEDGRAPH_HXX

#include <powsybl/math/UndirectedGraph.hpp>

#include <limits>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/counting_range.hpp>

#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace math {

template <typename V, typename E>
unsigned long UndirectedGraph<V, E>::addEdge(unsigned long v1, unsigned long v2, const stdcxx::Reference<E>& object) {
    checkVertex(v1);
    checkVertex(v2);

    unsigned long e;
    std::unique_ptr<Edge> edge = stdcxx::make_unique<Edge>(v1, v2, object);
    if (m_removedEdges.empty()) {
        e = m_edges.size();
        m_edges.emplace_back(std::move(edge));
    } else {
        e = *m_removedEdges.begin();
        m_removedEdges.erase(m_removedEdges.begin());

        m_edges[e] = std::move(edge);
    }

    invalidateAdjacencyList();

    return e;
}

template <typename V, typename E>
unsigned long UndirectedGraph<V, E>::addVertex() {
    unsigned long v;

    std::unique_ptr<Vertex> vertex = stdcxx::make_unique<Vertex>();
    if (m_availableVertices.empty()) {
        v = m_vertices.size();
        m_vertices.emplace_back(std::move(vertex));
    } else {
        v = *m_availableVertices.begin();
        m_availableVertices.erase(m_availableVertices.begin());

        m_vertices[v] = std::move(vertex);
    }

    invalidateAdjacencyList();

    return v;
}

template <typename V, typename E>
void UndirectedGraph<V, E>::addVertexIfNotPresent(unsigned long v) {
    if (v < m_vertices.size()) {
        const auto& it = m_availableVertices.find(v);
        if (it != m_availableVertices.end()) {
            m_vertices[v] = stdcxx::make_unique<Vertex>();
            m_availableVertices.erase(it);
        }
    } else {
        for (unsigned int i = m_vertices.size(); i < v; ++i) {
            m_availableVertices.insert(i);
        }
        m_vertices.resize(v + 1);
        m_vertices[v] = stdcxx::make_unique<Vertex>();
    }
    invalidateAdjacencyList();
}

template <typename V, typename E>
void UndirectedGraph<V, E>::checkEdge(unsigned long e) const {
    if (e >= m_edges.size() || !m_edges[e]) {
        throw PowsyblException(stdcxx::format("Edge %1% not found", e));
    }
}

template <typename V, typename E>
void UndirectedGraph<V, E>::checkVertex(unsigned long v) const {
    if (v >= m_vertices.size() || !m_vertices[v]) {
        throw PowsyblException(stdcxx::format("Vertex %1% not found", v));
    }
}

template <typename V, typename E>
void UndirectedGraph<V, E>::cleanVertices(unsigned long v) {
    for (unsigned long i = 0; i <= v; ++i) {
        const auto& it = m_availableVertices.find(v - i);
        if (it == m_availableVertices.end()) {
            return;
        }
        m_availableVertices.erase(it);
        m_vertices.pop_back();
    }
}

template <typename V, typename E>
std::vector<typename UndirectedGraph<V, E>::Path> UndirectedGraph<V, E>::findAllPaths(unsigned long v, const VertexVisitor& pathComplete, const EdgeVisitor& pathCanceled) const {
    std::vector<Path> paths;

    std::vector<bool> encountered(m_vertices.size(), false);
    Path path;
    findAllPaths(v, pathComplete, pathCanceled, path, encountered, paths);

    std::sort(paths.begin(), paths.end(), [](const Path& path1, const Path& path2) {
        return path1.size() < path2.size();
    });

    return paths;
}

template <typename V, typename E>
void UndirectedGraph<V, E>::findAllPaths(unsigned long v, const VertexVisitor& pathComplete, const EdgeVisitor& pathCanceled,
                                         const Path& path, std::vector<bool>& encountered, std::vector<Path>& paths) const {
    checkVertex(v);

    encountered[v] = true;

    const std::vector<std::vector<unsigned long> >& adjacencyList = getAdjacencyList();
    const std::vector<unsigned long>& adjacentEdges = adjacencyList[v];

    for (const auto& e : adjacentEdges) {
        const std::unique_ptr<Edge>& edge = m_edges[e];
        if (pathCanceled(edge->getObject())) {
            continue;
        }

        unsigned long v1 = edge->getVertex1();
        unsigned long v2 = edge->getVertex2();

        UndirectedGraph<V, E>::Path path2 = path;
        std::vector<bool> encountered2 = encountered;
        if (v == v2) {
            if (findAllPaths(e, v1, pathComplete, pathCanceled, path2, encountered2, paths)) {
                continue;
            }
        } else if (v == v1) {
            if (findAllPaths(e, v2, pathComplete, pathCanceled, path2, encountered2, paths)) {
                continue;
            }
        } else {
            throw PowsyblException(stdcxx::format("Edge %1% is not connected to vertex %2%", e, v));
        }
    }
}

template <typename V, typename E>
bool UndirectedGraph<V, E>::findAllPaths(unsigned long e, unsigned long v, const VertexVisitor& pathComplete, const EdgeVisitor& pathCanceled,
                                         Path& path, std::vector<bool>& encountered, std::vector<std::vector<unsigned long> >& paths) const {
    if (encountered[v]) {
        return false;
    }

    const std::unique_ptr<Vertex>& vertex = m_vertices[v];
    path.push_back(e);
    if (pathComplete(vertex->getObject())) {
        paths.emplace_back(std::move(path));
        return true;
    }

    findAllPaths(v, pathComplete, pathCanceled, path, encountered, paths);
    return false;
}

template <typename V, typename E>
const std::vector<std::vector<unsigned long> >& UndirectedGraph<V, E>::getAdjacencyList() const {
    static unsigned long s_neighborsCapacity = 2;

    std::lock_guard<std::mutex> lock(m_adjacencyListMutex);

    if (m_adjacencyList.size() != m_vertices.size()) {
        m_adjacencyList.resize(m_vertices.size());
        for (unsigned long v = 0; v < m_vertices.size(); ++v) {
            if (m_vertices[v]) {
                m_adjacencyList[v].reserve(s_neighborsCapacity);
            }
        }

        for (unsigned long e = 0; e < m_edges.size(); ++e) {
            const std::unique_ptr<Edge>& edge = m_edges[e];
            if (edge) {
                m_adjacencyList[edge->getVertex1()].push_back(e);
                m_adjacencyList[edge->getVertex2()].push_back(e);
            }
        }
    }

    return m_adjacencyList;
}

template <typename V, typename E>
unsigned long UndirectedGraph<V, E>::getEdgeCount() const {
    return m_edges.size() - m_removedEdges.size();
}

template <typename V, typename E>
const stdcxx::Reference<E>& UndirectedGraph<V, E>::getEdgeObject(unsigned long e) const {
    checkEdge(e);

    return m_edges[e]->getObject();
}

template <typename V, typename E>
typename UndirectedGraph<V, E>::template const_range<E> UndirectedGraph<V, E>::getEdgeObjects() const {
    const auto& filter = [](const std::unique_ptr<Edge>& edge) {
        return static_cast<bool>(edge);
    };

    return m_edges | boost::adaptors::filtered(filter) | boost::adaptors::indirected | boost::adaptors::transformed(Edge::map);
}

template <typename V, typename E>
typename UndirectedGraph<V, E>::template range<E> UndirectedGraph<V, E>::getEdgeObjects() {
    const auto& filter = [](const std::unique_ptr<Edge>& edge) {
        return static_cast<bool>(edge);
    };

    return m_edges | boost::adaptors::filtered(filter) | boost::adaptors::indirected | boost::adaptors::transformed(Edge::map);
}

template <typename V, typename E>
typename UndirectedGraph<V, E>::template const_range<E> UndirectedGraph<V, E>::getEdgeObjects(unsigned long v1, unsigned long v2) const {
    checkVertex(v1);
    checkVertex(v2);

    const std::vector<std::vector<unsigned long> >& adjacencyList = getAdjacencyList();
    const std::vector<unsigned long>& adjacentEdges = adjacencyList[v1];

    const auto& edgeMapper = [this](const unsigned long& e) {
        return m_edges[e].get();
    };
    const auto& edgeFilter = [v1, v2](const Edge& edge) {
        return ((edge->getVertex1() == v1 && edge->getVertex2() == v2) || (edge->getVertex1() == v2 && edge->getVertex2() == v1));
    };

    return adjacentEdges |
        boost::adaptors::transformed(edgeMapper) |
        boost::adaptors::filtered(edgeFilter) |
        boost::adaptors::transformed(Edge::map);
}

template <typename V, typename E>
typename UndirectedGraph<V, E>::template const_range<unsigned long> UndirectedGraph<V, E>::getEdges() const {
    const auto& filter = [this](const unsigned long e) {
        return static_cast<bool>(m_edges[e]);
    };

    return boost::counting_range(0UL, static_cast<unsigned long>(m_edges.size())) | boost::adaptors::filtered(filter);
}

template <typename V, typename E>
unsigned long UndirectedGraph<V, E>::getMaxVertex() const {
    return m_vertices.size();
}

template <typename V, typename E>
unsigned long UndirectedGraph<V, E>::getVertex1(unsigned long e) const {
    checkEdge(e);
    return m_edges.at(e).get()->getVertex1();
}

template <typename V, typename E>
unsigned long UndirectedGraph<V, E>::getVertex2(unsigned long e) const {
    checkEdge(e);
    return m_edges.at(e).get()->getVertex2();
}

template <typename V, typename E>
unsigned long UndirectedGraph<V, E>::getVertexCapacity() const {
    return m_vertices.size();
}

template <typename V, typename E>
unsigned long UndirectedGraph<V, E>::getVertexCount() const {
    return m_vertices.size() - m_availableVertices.size();
}

template <typename V, typename E>
const stdcxx::Reference<V>& UndirectedGraph<V, E>::getVertexObject(unsigned long v) const {
    checkVertex(v);

    return m_vertices[v]->getObject();
}

template <typename V, typename E>
typename UndirectedGraph<V, E>::template const_range<V> UndirectedGraph<V, E>::getVertexObjects() const {
    const auto& filter = [](const std::unique_ptr<Vertex>& vertex) {
        return static_cast<bool>(vertex);
    };

    return m_vertices | boost::adaptors::filtered(filter) | boost::adaptors::indirected | boost::adaptors::transformed(Vertex::map);
}

template <typename V, typename E>
typename UndirectedGraph<V, E>::template range<V> UndirectedGraph<V, E>::getVertexObjects() {
    const auto& filter = [](const std::unique_ptr<Vertex>& vertex) {
        return static_cast<bool>(vertex);
    };

    return m_vertices | boost::adaptors::filtered(filter) | boost::adaptors::indirected | boost::adaptors::transformed(Vertex::map);
}

template <typename V, typename E>
typename UndirectedGraph<V, E>::template const_range<unsigned long> UndirectedGraph<V, E>::getVertices() const {
    const auto& filter = [this](const unsigned long v) {
        return static_cast<bool>(m_vertices[v]);
    };

    return boost::counting_range(0UL, static_cast<unsigned long>(m_vertices.size())) | boost::adaptors::filtered(filter);
}

template <typename V, typename E>
void UndirectedGraph<V, E>::invalidateAdjacencyList() {
    std::lock_guard<std::mutex> lock(m_adjacencyListMutex);

    m_adjacencyList.clear();
}

template <typename V, typename E>
void UndirectedGraph<V, E>::removeAllEdges() {
    m_edges.clear();
    m_removedEdges.clear();

    invalidateAdjacencyList();
}

template <typename V, typename E>
void UndirectedGraph<V, E>::removeAllVertices() {
    if (!m_edges.empty()) {
        throw PowsyblException("Cannot remove all vertices because there is still some edges in the graph");
    }

    m_vertices.clear();
    m_availableVertices.clear();

    invalidateAdjacencyList();
}

template <typename V, typename E>
stdcxx::Reference<E> UndirectedGraph<V, E>::removeEdge(unsigned long e) {
    checkEdge(e);

    stdcxx::Reference<E> object = m_edges[e]->getObject();
    if (e == m_edges.size() - 1) {
        m_edges.pop_back();
    } else {
        m_edges[e].reset();
        m_removedEdges.insert(e);
    }

    invalidateAdjacencyList();

    return object;
}

template <typename V, typename E>
void UndirectedGraph<V, E>::removeIsolatedVertices() {

    std::set<unsigned long> connectedVertices;
    for (const auto& e : getEdges()) {
        connectedVertices.emplace(getVertex1(e));
        connectedVertices.emplace(getVertex2(e));
    }

    for (const auto& v : getVertices()) {
        if ((connectedVertices.find(v) == connectedVertices.end()) && !getVertexObject(v)) {
            removeVertex(v);
        }
    }
}

template <typename V, typename E>
stdcxx::Reference<V> UndirectedGraph<V, E>::removeVertex(unsigned long v) {
    checkVertex(v);

    for (const auto& edge : m_edges) {
        if (edge && (edge->getVertex1() == v || edge->getVertex2() == v)) {
            throw PowsyblException(stdcxx::format("An edge is connected to the vertex %1%", v));

        }
    }

    stdcxx::Reference<V> object = m_vertices[v]->getObject();
    if (v == m_vertices.size() - 1) {
        m_vertices.pop_back();
        cleanVertices(v - 1);
    } else {
        m_vertices[v].reset();
        m_availableVertices.insert(v);
    }

    invalidateAdjacencyList();

    return object;
}

template <typename V, typename E>
void UndirectedGraph<V, E>::setVertexObject(unsigned long v, const stdcxx::Reference<V>& object) {
    checkVertex(v);

    m_vertices[v]->setObject(object);
}

template <typename V, typename E>
bool UndirectedGraph<V, E>::traverse(unsigned long v, const Traverser& traverser) const {
    std::vector<bool> encountered(m_vertices.size(), false);

    return traverse(v, traverser, encountered);
}

template <typename V, typename E>
bool UndirectedGraph<V, E>::traverse(const stdcxx::const_range<unsigned long>& startingVertices, const Traverser& traverser) const {
    std::vector<bool> encountered(m_vertices.size(), false);

    for (unsigned long startingVertex : startingVertices) {
        if (!encountered[startingVertex] && !traverse(startingVertex, traverser, encountered)) {
            return false;
        }
    }
    return true;
}

template <typename V, typename E>
bool UndirectedGraph<V, E>::traverse(unsigned long v, const Traverser& traverser, std::vector<bool>& encountered) const {
    checkVertex(v);

    encountered.resize(m_vertices.size(), false);

    const std::vector<std::vector<unsigned long> >& adjacencyList = getAdjacencyList();
    const std::vector<unsigned long>& adjacentEdges = adjacencyList[v];

    encountered[v] = true;
    bool keepGoing = true;
    for (unsigned long e : adjacentEdges) {
        const std::unique_ptr<Edge>& edge = m_edges[e];
        unsigned long v1 = edge->getVertex1();
        unsigned long v2 = edge->getVertex2();
        if (!encountered[v1]) {
            const TraverseResult& traverserResult = traverser(v2, e, v1);
            if (traverserResult == TraverseResult::CONTINUE) {
                encountered[v1] = true;
                keepGoing = traverse(v1, traverser, encountered);
            } else if (traverserResult == TraverseResult::TERMINATE_TRAVERSER) {
                keepGoing = false;
            }
        } else if (!encountered[v2]) {
            const TraverseResult& traverserResult = traverser(v1, e, v2);
            if (traverserResult == TraverseResult::CONTINUE) {
                encountered[v2] = true;
                keepGoing = traverse(v2, traverser, encountered);
            } else if (traverserResult == TraverseResult::TERMINATE_TRAVERSER) {
                keepGoing = false;
            }
        }
        if (!keepGoing) {
            break;
        }
    }

    return keepGoing;
}

template <typename V, typename E>
bool UndirectedGraph<V, E>::vertexExists(unsigned long v) const {
    return v < m_vertices.size() && m_vertices[v];
}

template <typename V, typename E>
UndirectedGraph<V, E>::Edge::Edge(unsigned long v1, unsigned long v2, const stdcxx::Reference<E>& object) :
    m_vertex1(v1),
    m_vertex2(v2),
    m_object(object) {
}

template <typename V, typename E>
const stdcxx::Reference<E>& UndirectedGraph<V, E>::Edge::getObject() const {
    return m_object;

}

template <typename V, typename E>
const stdcxx::Reference<E>& UndirectedGraph<V, E>::Edge::map(const typename UndirectedGraph<V, E>::Edge& edge) {
    return edge.getObject();
}

template <typename V, typename E>
unsigned long UndirectedGraph<V, E>::Edge::getVertex1() const {
    return m_vertex1;
}

template <typename V, typename E>
unsigned long UndirectedGraph<V, E>::Edge::getVertex2() const {
    return m_vertex2;
}

template <typename V, typename E>
void UndirectedGraph<V, E>::Edge::setObject(const stdcxx::Reference<E>& object) {
    m_object = object;
}

template <typename V, typename E>
UndirectedGraph<V, E>::Vertex::Vertex() :
    m_object() {
}

template <typename V, typename E>
const stdcxx::Reference<V>& UndirectedGraph<V, E>::Vertex::getObject() const {
    return m_object;
}

template <typename V, typename E>
const stdcxx::Reference<V>& UndirectedGraph<V, E>::Vertex::map(const typename UndirectedGraph<V, E>::Vertex& vertex) {
    return vertex.getObject();
}

template <typename V, typename E>
void UndirectedGraph<V, E>::Vertex::setObject(const stdcxx::Reference<V>& object) {
    m_object = object;
}

}  // namespace math

}  // namespace powsybl

#endif  // POWSYBL_MATH_UNDIRECTEDGRAPH_HXX
