/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UNDIRECTEDGRAPH_HPP
#define POWSYBL_IIDM_UNDIRECTEDGRAPH_HPP

#include <memory>
#include <mutex>
#include <set>
#include <vector>

#include <powsybl/math/Traverser.hpp>
#include <powsybl/stdcxx/optional_reference_wrapper.hpp>

namespace powsybl {

namespace math {

template <typename V, typename E>
class UndirectedGraph {
public:
    typedef std::vector<unsigned long> Path;

    typedef std::function<bool(const stdcxx::Optional<E>&)> EdgeVisitor;

    typedef std::function<bool(const stdcxx::Optional<V>&)> VertexVisitor;

public:
    UndirectedGraph();

    ~UndirectedGraph() = default;

    unsigned long addEdge(unsigned long v1, unsigned long v2, const stdcxx::Optional<E>& object);

    unsigned long addVertex();

    std::vector<Path> findAllPaths(unsigned long v, const VertexVisitor& pathComplete, const EdgeVisitor& pathCanceled) const;

    unsigned long getEdgeCount() const;

    const stdcxx::Optional<E>& getEdgeObject(unsigned long e) const;

    std::vector<stdcxx::Optional<E> > getEdgeObjects() const;

    std::vector<stdcxx::Optional<E> > getEdgeObjects(unsigned long v1, unsigned long v2) const;

    std::set<unsigned long> getEdges() const;

    unsigned long getMaxVertex() const;

    unsigned long getVertex1(unsigned long e) const;

    unsigned long getVertex2(unsigned long e) const;

    unsigned long getVertexCount() const;

    const stdcxx::Optional<V>& getVertexObject(unsigned long v) const;

    std::vector<stdcxx::Optional<V> > getVertexObjects() const;

    std::set<unsigned long> getVertices() const;

    void removeAllEdges();

    void removeAllVertices();

    stdcxx::Optional<E> removeEdge(unsigned long e);

    stdcxx::Optional<V> removeVertex(unsigned long v);

    void setVertexObject(unsigned long v, const stdcxx::Optional<V>& object);

    void traverse(unsigned long v, const Traverser& traverser) const;

    void traverse(unsigned long v, const Traverser& traverser, std::vector<bool>& encountered) const;

private:
    class Edge;

    class Vertex;

private:
    void checkEdge(unsigned long e) const;

    void checkVertex(unsigned long v) const;

    void findAllPaths(unsigned long v, const VertexVisitor& pathComplete, const EdgeVisitor& pathCanceled,
                      Path& path, std::vector<bool>& encountered, std::vector<Path>& paths) const;

    bool findAllPaths(unsigned long e, unsigned long v, const VertexVisitor& pathComplete, const EdgeVisitor& pathCanceled,
                      Path& path, std::vector<bool>& encountered, std::vector<Path>& paths) const;

    const std::vector<std::vector<unsigned long> >& getAdjacencyList() const;

    void invalidateAdjacencyList();

private:
    std::vector<std::unique_ptr<Vertex> > m_vertices;

    std::vector<std::unique_ptr<Edge> > m_edges;

    std::set<unsigned long> m_removedVertices;

    std::set<unsigned long> m_removedEdges;

    mutable std::mutex m_adjacencyListMutex;

    mutable std::vector<std::vector<unsigned long> > m_adjacencyList;
};

template <typename V, typename E>
class UndirectedGraph<V, E>::Edge {
public:
    Edge(unsigned long v1, unsigned long v2, const stdcxx::Optional<E>& object);

    ~Edge() = default;

    const stdcxx::Optional<E>& getObject() const;

    unsigned long getVertex1() const;

    unsigned long getVertex2() const;

    void setObject(const stdcxx::Optional<E>& object);

private:
    unsigned long m_vertex1;

    unsigned long m_vertex2;

    stdcxx::Optional<E> m_object;
};

template <typename V, typename E>
class UndirectedGraph<V, E>::Vertex {
public:
    Vertex();

    ~Vertex() = default;

    const stdcxx::Optional<V>& getObject() const;

    void setObject(const stdcxx::Optional<V>& object);

private:
    stdcxx::Optional<V> m_object;
};

}

}

#include <powsybl/math/UndirectedGraph.hxx>

#endif  // POWSYBL_IIDM_UNDIRECTEDGRAPH_HPP
