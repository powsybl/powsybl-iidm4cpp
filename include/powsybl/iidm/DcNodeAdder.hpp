/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCNODEADDER_HPP
#define POWSYBL_IIDM_DCNODEADDER_HPP

#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/IdentifiableAdder.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Network;

class DcNodeAdder : public IdentifiableAdder<DcNode, DcNodeAdder> {
public:
    explicit DcNodeAdder(Network& network);
    explicit DcNodeAdder(Network& network, Network& subnetwork);
    ~DcNodeAdder() noexcept override = default;

    DcNode& add() override;

    DcNodeAdder& setNominalV(double nominalV);

protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    Network& m_network;
    stdcxx::Reference<Network> m_subNetworkRef;

    double m_nominalV = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCNODEADDER_HPP
