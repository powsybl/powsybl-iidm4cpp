/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCLINEADDER_HPP
#define POWSYBL_IIDM_DCLINEADDER_HPP

#include <powsybl/iidm/DcLine.hpp>
#include <powsybl/iidm/IdentifiableAdder.hpp>

namespace powsybl {

namespace iidm {

class Network;

class DcLineAdder : public IdentifiableAdder<DcLine, DcLineAdder> {
public:
    explicit DcLineAdder(Network& network);
    explicit DcLineAdder(Network& network, const std::string& subNetworkId);
    ~DcLineAdder() noexcept override = default;

    DcLineAdder& setR(double r);

    DcLineAdder& setDcNode1(const std::string& dcNodeId1);

    DcLineAdder& setConnected1(bool connected1);

    DcLineAdder& setDcNode2(const std::string& dcNodeId2);

    DcLineAdder& setConnected2(bool connected2);

    DcLine& add();

protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    Network& m_network;
    std::string m_subnetworkId;

    double m_r = stdcxx::nan();
    std::string m_dcNodeId1;
    std::string m_dcNodeId2;
    bool m_dcConnected1 = true; // defaults to connected
    bool m_dcConnected2 = true; // defaults to connected
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCLINEADDER_HPP
