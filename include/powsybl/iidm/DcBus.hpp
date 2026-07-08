/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCBUS_HPP
#define POWSYBL_IIDM_DCBUS_HPP

#include <powsybl/iidm/DcTopologyVisitable.hpp>
#include <powsybl/iidm/Identifiable.hpp>

#include <powsybl/stdcxx/reference.hpp>
#include <powsybl/stdcxx/set.hpp>

namespace powsybl {

namespace iidm {

class Component;
class DcNode;
class DcTerminal;

/**
 * A DC Bus is formed on a connected DcNode, or a set of connected DcNodes inside a DcTopologyModel.
 */
class DcBus : public Identifiable, public DcTopologyVisitable {

public:
    using DcNodeSet = stdcxx::LinkedHashSet<std::reference_wrapper<DcNode>, stdcxx::hash<std::reference_wrapper<DcNode> >, stdcxx::equal_to<DcNode> >;

public:  // Identifiable
    /**
     * Retrieve the first DcNode's Network
     */
    const Network& getNetwork() const override;
    /**
     * Retrieve the first DcNode's Network
     */
    Network& getNetwork() override;
    /**
     * Retrieve the first DcNode's parent Network
     */
    const Network& getParentNetwork() const override;
    /**
     * Retrieve the first DcNode's parent Network
     */
    Network& getParentNetwork() override;

    const IdentifiableType& getType() const override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

public:
    DcBus(const std::string& id, const std::string& name, bool fictitious, DcNodeSet dcNodes);

    ~DcBus() noexcept override = default;

public:  //DcTopologyVisitable
    unsigned long getDcTerminalCount() const override;

    stdcxx::const_range<DcTerminal> getDcTerminals() const override;

    stdcxx::range<DcTerminal> getDcTerminals() override;

    unsigned long getConnectedDcTerminalCount() const override;

    stdcxx::const_range<DcTerminal> getConnectedDcTerminals() const override;

    stdcxx::range<DcTerminal> getConnectedDcTerminals() override;

    void visitConnectedEquipments(DcTopologyVisitor& visitor) override;

    void visitConnectedOrConnectableEquipments(DcTopologyVisitor& visitor) override;


    /**
     * Get the connected component that this bus is part of
     */
    stdcxx::CReference<Component> getConnectedComponent() const;
    /**
     * Get the connected component that this bus is part of
     */
    stdcxx::Reference<Component> getConnectedComponent();


    /**
     * Return true if this DC bus belongs to the main connected component
     */
    bool isInMainConnectedComponent() const;

    /**
     * Get the DC Component that this bus is part of
     */
    stdcxx::CReference<Component> getDcComponent() const;
    /**
     * Get the DC Component that this bus is part of
     */
    stdcxx::Reference<Component> getDcComponent();

    double getV();
    DcBus& setV(double v);

    /**
     * Get the DC nodes linked to this bus
     */
    stdcxx::const_range<DcNode> getDcNodes() const;
    /**
     * Get the DC nodes linked to this bus
     */
    stdcxx::range<DcNode> getDcNodes();

    /**
     * Invalidate this bus after topology changed
     */
    void invalidate();

private:
    void checkValidity() const;

private:

    void setConnectedComponentNumber(const stdcxx::optional<unsigned long>& connectedComponentNumber);
    friend class ConnectedComponentsManager;

    void setDcComponentNumber(const stdcxx::optional<unsigned long>& dcComponentNumber);
    friend class DcComponentsManager;

private:
    DcNodeSet m_dcNodes;

    bool m_valid = true;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCBUS_HPP
