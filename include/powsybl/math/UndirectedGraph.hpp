/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_MATH_UNDIRECTEDGRAPH_HPP
#define POWSYBL_MATH_UNDIRECTEDGRAPH_HPP

#include <memory>
#include <mutex>
#include <set>
#include <vector>

#include <powsybl/math/Traverser.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace math {

template <typename V, typename E>
class UndirectedGraph {
public:
    typedef std::vector<unsigned long> Path;

    typedef std::function<bool(const stdcxx::Reference<E>&)> EdgeVisitor;

    typedef std::function<bool(const stdcxx::Reference<V>&)> VertexVisitor;

public:
    UndirectedGraph() = default;

    ~UndirectedGraph() noexcept = default;

    unsigned long addEdge(unsigned long v1, unsigned long v2, const stdcxx::Reference<E>& object);

    unsigned long addVertex();

    std::vector<Path> findAllPaths(unsigned long v, const VertexVisitor& pathComplete, const EdgeVisitor& pathCanceled) const;

    unsigned long getEdgeCount() const;

    const stdcxx::Reference<E>& getEdgeObject(unsigned long e) const;

    std::vector<stdcxx::Reference<E> > getEdgeObjects() const;

    std::vector<stdcxx::Reference<E> > getEdgeObjects(unsigned long v1, unsigned long v2) const;

    std::set<unsigned long> getEdges() const;

    unsigned long getMaxVertex() const;

    unsigned long getVertex1(unsigned long e) const;

    unsigned long getVertex2(unsigned long e) const;

    unsigned long getVertexCount() const;

    const stdcxx::Reference<V>& getVertexObject(unsigned long v) const;

    std::vector<stdcxx::Reference<V> > getVertexObjects() const;

    std::set<unsigned long> getVertices() const;

    void removeAllEdges();

    void removeAllVertices();

    stdcxx::Reference<E> removeEdge(unsigned long e);

    void removeIsolatedVertices();

    stdcxx::Reference<V> removeVertex(unsigned long v);

    void setVertexObject(unsigned long v, const stdcxx::Reference<V>& object);

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
    Edge(unsigned long v1, unsigned long v2, const stdcxx::Reference<E>& object);

    ~Edge() noexcept = default;

    const stdcxx::Reference<E>& getObject() const;

    unsigned long getVertex1() const;

    unsigned long getVertex2() const;

    void setObject(const stdcxx::Reference<E>& object);

private:
    unsigned long m_vertex1;

    unsigned long m_vertex2;

    stdcxx::Reference<E> m_object;
};

template <typename V, typename E>
class UndirectedGraph<V, E>::Vertex {
public:
    Vertex();

    ~Vertex() noexcept = default;

    const stdcxx::Reference<V>& getObject() const;

    void setObject(const stdcxx::Reference<V>& object);

private:
    stdcxx::Reference<V> m_object;
};

}  // namespace math

}  // namespace powsybl

#include <powsybl/math/UndirectedGraph.hxx>

#endif  // POWSYBL_MATH_UNDIRECTEDGRAPH_HPP
