/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCTOPOLOGYMODEL_HPP
#define POWSYBL_IIDM_DCTOPOLOGYMODEL_HPP

#include <powsybl/iidm/MultiVariantObject.hpp>

#include <powsybl/iidm/DcBus.hpp>
#include <powsybl/iidm/DcTerminal.hpp>
#include <powsybl/iidm/DcTopologyVariant.hpp>
#include <powsybl/iidm/DcTopologyView.hpp>

#include <powsybl/math/UndirectedGraph.hpp>

#include <powsybl/stdcxx/map.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class DcTopologyModel : public MultiVariantObject, public PropertiesHolder {
public:
    DcTopologyModel(Network& owner);

    DcTopologyModel(DcTopologyModel&&) noexcept = delete;
    DcTopologyModel(Network& owner, DcTopologyModel&& dcTopologyModel) noexcept;

    using Graph = math::UndirectedGraph<DcNode, DcSwitch>;

protected:
    void addDcNodeToTopology(DcNode& dcNode);
    friend class DcNodeAdder;
    void addDcSwitchToTopology(DcSwitch& dcSwitch, std::string dcNodeId1, std::string dcNodeId2);
    friend class DcSwitchAdder;

public:
    void removeDcNode(std::string dcNodeId);
    void removeDcSwitch(std::string dcSwitchId);
    void removeAllDcNodes();
    void removeAllDcSwitches();

    void invalidateCache();

    void attach(DcTerminal& dcTerminal);
    void detach(DcTerminal& dcTerminal);

    bool connect(DcTerminal& dcTerminal);
    bool disconnect(DcTerminal& dcTerminal);

    bool traverse(DcTerminal& dcTerminal, DcTerminal::DcTopologyTraverser& traverser, math::TraversalType traversalType) const;

    /**
     * Traverse the full DC Topology graph according to the given TraversalType, keeping track on the traversed DC Terminals,
     * and using the given DcTerminal as point of origin.
     * 
     * @return false if the traversal has been terminated before getting through the whole graph 
     * (i.e. a math::TraverseResult::TERMINATE_TRAVERSER has been returned by the traverser),
     * true if not interrupted before the end.
     */
    bool traverse(DcTerminal& dcTerminal, DcTerminal::DcTopologyTraverser& traverser, DcTerminalSet& traversedTerminals, math::TraversalType traversalType) const; 


    void invalidateAllVariantsCache();

    stdcxx::const_range<DcBus> getDcBuses() const;

    stdcxx::range<DcBus> getDcBuses();

    unsigned long getDcBusCount() const;

    stdcxx::CReference<DcBus> getDcBus(const std::string& dcBusId) const;

    stdcxx::Reference<DcBus> getDcBus(const std::string& dcBusId);

    stdcxx::CReference<DcBus> getDcBusOfDcNode(const std::string& dcNodeId) const;

    stdcxx::Reference<DcBus> getDcBusOfDcNode(const std::string& dcNodeId);


protected:  // MultiVariantObject
    virtual void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    virtual void deleteVariantArrayElement(unsigned long index) override;

    virtual void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    virtual void reduceVariantArraySize(unsigned long number) override;

    friend class Network;

public:
    const Network& getNetwork() const;
    Network& getNetwork();

private:
    void assertGraph() const;
    stdcxx::optional<unsigned long> getVertex(const std::string& dcNodeId, bool throwException) const;

    static void addNextDcTerminals(DcTerminal& dcTerminal, DcTerminalSet& nextDcTerminals);
    static math::TraverseResult getTraverserResult(DcTerminalSet& visitedDcTerminals, DcTerminal& dcTerminal, DcTerminal::DcTopologyTraverser& traverser);

    const dc_topology_model::DcBusTopology& getDcBusTopology() const;
    dc_topology_model::DcBusTopology& getDcBusTopology();

    const dc_topology_model::DcBusView& getDcBusView() const;
    dc_topology_model::DcBusView& getDcBusView();

private:
    Network& m_owner; // Owned by a Network (or Subnetwork)

    std::map<std::string, unsigned long> m_dcNodes;
    std::map<std::string, unsigned long> m_dcSwitches;

    std::unique_ptr<Graph> m_graph;

    dc_topology_model::VariantArray m_variants;

    dc_topology_model::DcBusView m_dcBusView;

    friend class dc_topology_model::DcBusTopology;
    friend class dc_topology_model::DcBusView;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCTOPOLOGYMODEL_HPP