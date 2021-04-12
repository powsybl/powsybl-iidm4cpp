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

class FlowsLimitsHolder {
public:
    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimits() const = 0;

    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimits() = 0;

    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits() const = 0;

    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits() = 0;

    virtual stdcxx::CReference<CurrentLimits> getCurrentLimits() const = 0;

    virtual stdcxx::Reference<CurrentLimits> getCurrentLimits() = 0;

    virtual stdcxx::const_range<OperationalLimits> getOperationalLimits() const = 0;

    virtual stdcxx::range<OperationalLimits> getOperationalLimits() = 0;

    virtual ActivePowerLimitsAdder newActivePowerLimits() = 0;

    virtual ApparentPowerLimitsAdder newApparentPowerLimits() = 0;

    virtual CurrentLimitsAdder newCurrentLimits() = 0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_FLOWSLIMITSHOLDER_HPP

