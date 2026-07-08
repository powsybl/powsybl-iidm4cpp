/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCNODE_HPP
#define POWSYBL_IIDM_DCNODE_HPP

#include <powsybl/iidm/DcTopologyVisitable.hpp>
#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/NetworkRef.hpp>

#include <powsybl/stdcxx/reference.hpp>
#include <list>
#include <vector>

namespace powsybl {

namespace iidm {

class Component;
class DcBus;

class DcNode : public Identifiable, public DcTopologyVisitable {
public:
    ~DcNode() noexcept override = default;

public:  // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

    const Network& getParentNetwork() const override;

    Network& getParentNetwork() override;

    const IdentifiableType& getType() const override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

protected:  // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

public:
    double getNominalV() const;

    DcNode& setNominalV(double nominalV);

    double getV() const;

    DcNode& setV(double v);

    stdcxx::CReference<DcBus> getDcBus() const;

    stdcxx::Reference<DcBus> getDcBus();

    void remove();

public:  // DcTopologyVisitable
    unsigned long getDcTerminalCount() const override;

    stdcxx::const_range<DcTerminal> getDcTerminals() const override;

    stdcxx::range<DcTerminal> getDcTerminals() override;

    unsigned long getConnectedDcTerminalCount() const override;

    stdcxx::const_range<DcTerminal> getConnectedDcTerminals() const override;

    stdcxx::range<DcTerminal> getConnectedDcTerminals() override;

    virtual void visitConnectedEquipments(DcTopologyVisitor& visitor) override;

    virtual void visitConnectedOrConnectableEquipments(DcTopologyVisitor& visitor) override;


    void addDcTerminal(DcTerminal& dcTerminal);
    void removeDcTerminal(DcTerminal& dcTerminal);


    void setConnectedComponentNumber(const stdcxx::optional<unsigned long>& connectedComponentNumber);
    void setDcComponentNumber(const stdcxx::optional<unsigned long>& dcComponentNumber);

    stdcxx::CReference<Component> getConnectedComponent() const;
    stdcxx::Reference<Component> getConnectedComponent();

    stdcxx::CReference<Component> getDcComponent() const;
    stdcxx::Reference<Component> getDcComponent();



protected:
    DcNode(Network& rootNetwork, const std::string& id, const std::string& name, bool fictitious, double nominalV);
    DcNode(Network& rootNetwork, Network& subnetwork, const std::string& id, const std::string& name, bool fictitious, double nominalV);
    friend class DcNodeAdder;

private:
    void setNetworkRef(Network& network);
    friend class NetworkIndex;

private:
    NetworkRef m_network;
    stdcxx::Reference<Network> m_subnetworkRef;

    std::list<std::reference_wrapper<DcTerminal> > m_dcTerminals;

    double m_nominalV;

    std::vector<double> m_v;

    std::vector<stdcxx::optional<unsigned long>> m_connectedComponentNumber;

    std::vector<stdcxx::optional<unsigned long>> m_dcComponentNumber;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCNODE_HPP
