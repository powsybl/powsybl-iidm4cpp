/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TIELINEADDER_HPP
#define POWSYBL_IIDM_TIELINEADDER_HPP

#include <powsybl/iidm/IdentifiableAdder.hpp>

#include <powsybl/iidm/TieLine.hpp>

namespace powsybl {

namespace iidm {

class Network;

class TieLineAdder : public IdentifiableAdder<TieLine, TieLineAdder> {
public:

    explicit TieLineAdder(Network& network);
    explicit TieLineAdder(Network& network, const std::string& subNetworkId);

    ~TieLineAdder() noexcept override = default;

    TieLine& add() override;

    TieLineAdder& setBoundaryLine1(const std::string& id);

    TieLineAdder& setBoundaryLine2(const std::string& id);

protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    Network& m_network;
    std::string m_subnetworkId;

    std::string m_blId1;
    std::string m_blId2;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TIELINEADDER_HPP
