/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCGROUNDADDER_HPP
#define POWSYBL_IIDM_DCGROUNDADDER_HPP

#include <powsybl/iidm/DcGround.hpp>
#include <powsybl/iidm/IdentifiableAdder.hpp>

namespace powsybl {

namespace iidm {

class Network;

class DcGroundAdder : public IdentifiableAdder<DcGround, DcGroundAdder> {
public:
    explicit DcGroundAdder(Network& network);
    explicit DcGroundAdder(Network& network, const std::string& subNetworkId);
    ~DcGroundAdder() noexcept override = default;

    DcGroundAdder& setR(double r);

    DcGroundAdder& setDcNode(const std::string& dcNodeId);

    DcGroundAdder& setConnected(bool connected);

    DcGround& add();

protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    Network& m_network;
    std::string m_subnetworkId;

    double m_r = 0.; // defaults to zero ohms
    std::string m_dcNodeId;
    bool m_connected = true; // defaults to connected

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCGROUNDADDER_HPP
