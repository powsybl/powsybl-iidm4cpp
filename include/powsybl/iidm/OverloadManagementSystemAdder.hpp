/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OVERLOADMANAGEMENTSYSTEMADDER_HPP
#define POWSYBL_IIDM_OVERLOADMANAGEMENTSYSTEMADDER_HPP

#include <powsybl/iidm/IdentifiableAdder.hpp>

#include <powsybl/iidm/OverloadManagementSystem.hpp>
#include <powsybl/iidm/OverloadManagementSystemTrippingAdder.hpp>

namespace powsybl {

namespace iidm {

class OverloadManagementSystemAdder : public IdentifiableAdder<OverloadManagementSystem, OverloadManagementSystemAdder> {
public:
    using TrippingAdder = overload_management_system::TrippingAdder;

    explicit OverloadManagementSystemAdder(Substation& substation);

    ~OverloadManagementSystemAdder() noexcept override = default;

    OverloadManagementSystem& add() override;

    std::shared_ptr<overload_management_system::SwitchTrippingAdder> newSwitchTripping();

    std::shared_ptr<overload_management_system::BranchTrippingAdder> newBranchTripping();

    std::shared_ptr<overload_management_system::ThreeWindingsTransformerTrippingAdder> newThreeWindingsTransformerTripping();

    OverloadManagementSystemAdder& setEnabled(bool enabled);

    OverloadManagementSystemAdder& setMonitoredElementId(const std::string& monitoredElementId);

    OverloadManagementSystemAdder& setMonitoredElementSide(const ThreeSides& monitoredElementSide);

protected: // IdentifiableAdder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    std::unique_ptr<OverloadManagementSystem::Tripping> createTripping(std::shared_ptr<TrippingAdder>& trippingAdder, const std::string& overloadManagementSystemId, 
                        std::set<std::string>& knownTrippingKeys);

private:
    stdcxx::Reference<Substation> m_substation;
    bool m_enabled = true;
    std::string m_monitoredElementId;
    ThreeSides m_monitoredSide;

    std::vector<std::shared_ptr<TrippingAdder>> m_trippingAdders;
    friend class overload_management_system::TrippingAdder;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_OVERLOADMANAGEMENTSYSTEMADDER_HPP
