/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_FLOWSLIMITSHOLDER_HPP
#define POWSYBL_IIDM_FLOWSLIMITSHOLDER_HPP

#include <functional>
#include <vector>

#include <powsybl/iidm/OperationalLimits.hpp>
#include <powsybl/iidm/OperationalLimitsGroup.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class ActivePowerLimits;
class ActivePowerLimitsAdder;
class ApparentPowerLimits;
class ApparentPowerLimitsAdder;
class CurrentLimits;
class CurrentLimitsAdder;
class Identifiable;

class FlowsLimitsHolder {
public:
    FlowsLimitsHolder() = default;

    FlowsLimitsHolder(const FlowsLimitsHolder&) = default;

    FlowsLimitsHolder(FlowsLimitsHolder&&) = default;

    virtual ~FlowsLimitsHolder() noexcept = default;

    FlowsLimitsHolder& operator=(const FlowsLimitsHolder&) = default;

    FlowsLimitsHolder& operator=(FlowsLimitsHolder&&) = default;

    virtual stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups() const = 0;
    virtual stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups() = 0;

    virtual const stdcxx::optional<std::string>& getSelectedOperationalLimitsGroupId() const = 0;

    virtual stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup(const std::string& id) const = 0;
    virtual stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup(const std::string& id) = 0;

    virtual stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup() const = 0;
    virtual stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup() = 0;

    virtual stdcxx::Reference<OperationalLimitsGroup> newOperationalLimitsGroup(const std::string& id) = 0;

    virtual void setSelectedOperationalLimitsGroup(const std::string& id) = 0;

    virtual void removeOperationalLimitsGroup(const std::string& id) = 0;

    virtual void cancelSelectedOperationalLimitsGroup() = 0;

    virtual stdcxx::CReference<CurrentLimits> getCurrentLimits() const;
    virtual stdcxx::Reference<CurrentLimits> getCurrentLimits();

    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimits() const;
    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimits();

    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits() const;
    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits();

    virtual ActivePowerLimitsAdder newActivePowerLimits() = 0;

    virtual ApparentPowerLimitsAdder newApparentPowerLimits() = 0;

    virtual CurrentLimitsAdder newCurrentLimits() = 0;

// protected:
//     void setIdentifiable(Identifiable& identifiable);

// private:
//     OperationalLimitsHolder m_operationalLimitsHolder;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_FLOWSLIMITSHOLDER_HPP

