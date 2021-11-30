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
#include <powsybl/iidm/OperationalLimitsHolder.hpp>
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
    FlowsLimitsHolder(Identifiable& identifiable, std::string&& attributeName);

    explicit FlowsLimitsHolder(std::string&& attributeName);

    FlowsLimitsHolder(const FlowsLimitsHolder&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of OperationalLimitsHolder is not marked noexcept
    FlowsLimitsHolder(FlowsLimitsHolder&&) = default;  // NOSONAR

    ~FlowsLimitsHolder() noexcept = default;

    FlowsLimitsHolder& operator=(const FlowsLimitsHolder&) = default;

    FlowsLimitsHolder& operator=(FlowsLimitsHolder&&) = default;

    stdcxx::CReference<ActivePowerLimits> getActivePowerLimits() const;

    stdcxx::Reference<ActivePowerLimits> getActivePowerLimits();

    stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits() const;

    stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits();

    stdcxx::CReference<CurrentLimits> getCurrentLimits() const;

    stdcxx::Reference<CurrentLimits> getCurrentLimits();

    stdcxx::const_range<OperationalLimits> getOperationalLimits() const;

    stdcxx::range<OperationalLimits> getOperationalLimits();

    ActivePowerLimitsAdder newActivePowerLimits();

    ApparentPowerLimitsAdder newApparentPowerLimits();

    CurrentLimitsAdder newCurrentLimits();

protected:
    void setIdentifiable(Identifiable& identifiable);

private:
    OperationalLimitsHolder m_operationalLimitsHolder;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_FLOWSLIMITSHOLDER_HPP

