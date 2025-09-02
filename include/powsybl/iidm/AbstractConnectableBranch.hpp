/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ABSTRACTCONNECTABLEBRANCH_HPP
#define POWSYBL_IIDM_ABSTRACTCONNECTABLEBRANCH_HPP

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/OperationalLimitsHolder.hpp>

namespace powsybl {

namespace iidm {

class ActivePowerLimits;
class ActivePowerLimitsAdder;
class ApparentPowerLimits;
class ApparentPowerLimitsAdder;
class CurrentLimitsAdder;

class AbstractConnectableBranch : public Branch, public Connectable {

public:
    ~AbstractConnectableBranch() noexcept override = default;

    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimits1() const override;

    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimits1() override;

    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimits2() const override;

    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimits2() override;

    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits1() const override;

    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits1() override;

    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits2() const override;

    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits2() override;

    virtual stdcxx::CReference<CurrentLimits> getCurrentLimits1() const override;

    virtual stdcxx::Reference<CurrentLimits> getCurrentLimits1() override;

    virtual stdcxx::CReference<CurrentLimits> getCurrentLimits2() const override;

    virtual stdcxx::Reference<CurrentLimits> getCurrentLimits2() override;

    virtual const Terminal& getTerminal1() const override;

    virtual Terminal& getTerminal1() override;

    virtual const Terminal& getTerminal2() const override;

    virtual Terminal& getTerminal2() override;

    virtual ActivePowerLimitsAdder newActivePowerLimits1() override;

    virtual ActivePowerLimitsAdder newActivePowerLimits2() override;

    virtual ApparentPowerLimitsAdder newApparentPowerLimits1() override;

    virtual ApparentPowerLimitsAdder newApparentPowerLimits2() override;

    virtual CurrentLimitsAdder newCurrentLimits1() override;

    virtual CurrentLimitsAdder newCurrentLimits2() override;

protected:
    AbstractConnectableBranch();

private:
    //friend class CurrentLimitsAdder;

private:
    OperationalLimitsHolder m_operationalLimitsHolder1;

    OperationalLimitsHolder m_operationalLimitsHolder2;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ABSTRACTCONNECTABLEBRANCH_HPP
