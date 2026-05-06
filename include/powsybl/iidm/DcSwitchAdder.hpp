/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCSWITCHADDER_HPP
#define POWSYBL_IIDM_DCSWITCHADDER_HPP

#include <powsybl/iidm/DcSwitch.hpp>
#include <powsybl/iidm/IdentifiableAdder.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class DcNode;
class DcSwitch;

class DcSwitchAdder : public IdentifiableAdder<DcSwitch, DcSwitchAdder> {
public:
    explicit DcSwitchAdder(Network& network);
    explicit DcSwitchAdder(Network& network, const std::string& subnetworkId);
    ~DcSwitchAdder() noexcept override = default;

    DcSwitch& add() override;

    DcSwitchAdder& setOpen(bool open);
    DcSwitchAdder& setKind(const DcSwitchKind& kind);
    DcSwitchAdder& setDcNode1(const std::string& dcNodeId1);
    DcSwitchAdder& setDcNode2(const std::string& dcNodeId1);

protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    Network& m_network;
    std::string m_subnetworkId;

    stdcxx::optional<bool> m_open;
    stdcxx::optional<DcSwitchKind> m_kind;
    std::string m_dcNodeId1;
    std::string m_dcNodeId2;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCSWITCHADDER_HPP
