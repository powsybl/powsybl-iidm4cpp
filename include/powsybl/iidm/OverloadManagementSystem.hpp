/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OVERLOADMANAGEMENTSYSTEM_HPP
#define POWSYBL_IIDM_OVERLOADMANAGEMENTSYSTEM_HPP

#include <powsybl/iidm/AutomationSystem.hpp>

#include <powsybl/iidm/OverloadManagementSystemTripping.hpp>
#include <powsybl/iidm/ThreeSides.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Substation;

class OverloadManagementSystem : public AutomationSystem {
public:
    using Tripping = overload_management_system::Tripping;

public:  // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

    const Network& getParentNetwork() const override;

    Network& getParentNetwork() override;

    const IdentifiableType& getType() const override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

public: // AutomationSystem
    void remove() override;

public:
    ~OverloadManagementSystem() noexcept override = default;

    stdcxx::CReference<Substation> getSubstation() const;

    stdcxx::Reference<Substation> getSubstation();

    const std::string& getMonitoredElementId() const;

    const ThreeSides& getMonitoredSide() const;

    std::vector<std::reference_wrapper<Tripping>> getTrippings();
    stdcxx::const_range<Tripping> getTrippings() const;

private:
    void checkMonitoredElementId(const std::string& monitoredElementId);

    OverloadManagementSystem(const stdcxx::Reference<Substation>& substation, const std::string& id, const std::string& name, bool fictitious, bool enabled,
                            const std::string& monitoredElementId, const ThreeSides& monitoredSide);

    OverloadManagementSystem& addTripping(std::unique_ptr<Tripping>&& tripping);
    friend class OverloadManagementSystemAdder;

private:
    stdcxx::Reference<Substation> m_substation;

    std::string m_monitoredElementId;

    ThreeSides m_monitoredSide;

    std::vector<std::unique_ptr<Tripping>> m_trippings;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_OVERLOADMANAGEMENTSYSTEM_HPP
