/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCNODE_HPP
#define POWSYBL_IIDM_DCNODE_HPP

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/NetworkRef.hpp>

#include <powsybl/stdcxx/reference.hpp>

#include <vector>

namespace powsybl {

namespace iidm {

class DcNode : public Identifiable {
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

public:
    double getNominalV() const;

    DcNode& setNominalV(double nominalV);
    
    void remove();

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

    double m_nominalV;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCNODE_HPP
