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
#include <powsybl/iidm/OperationalLimitsGroups.hpp>

namespace powsybl {

namespace iidm {

class AbstractConnectableBranch : public Branch, public Connectable {

public:
    ~AbstractConnectableBranch() noexcept override = default;

    /**
     * Copy every OperationalLimitsGroup from the copiedBranch into this Branch,
     * if a group already exists with the same Id, it will be overwwritten.
     * Sets the selected groups of this Branch as they are selected on the copiedBranch,
     * groups are deselected before the copy.
     */
    virtual void copyOperationalLimits(const AbstractConnectableBranch& copiedBranch);

    virtual const Terminal& getTerminal1() const override;

    virtual Terminal& getTerminal1() override;

    virtual const Terminal& getTerminal2() const override;

    virtual Terminal& getTerminal2() override;

    /** \deprecated
     * Use OperationalLimitsGroup::newActivePowerLimits() instead.
     */
    virtual ActivePowerLimitsAdder newActivePowerLimits1() override;
    /** \deprecated
     * Use OperationalLimitsGroup::newActivePowerLimits() instead.
     */
    virtual ActivePowerLimitsAdder newActivePowerLimits2() override;
    /** \deprecated
     * Use OperationalLimitsGroup::newApparentPowerLimits() instead.
     */
    virtual ApparentPowerLimitsAdder newApparentPowerLimits1() override;
    /** \deprecated
     * Use OperationalLimitsGroup::newApparentPowerLimits() instead.
     */
    virtual ApparentPowerLimitsAdder newApparentPowerLimits2() override;
    /** \deprecated
     * Use OperationalLimitsGroup::newCurrentLimits() instead.
     */
    virtual CurrentLimitsAdder newCurrentLimits1() override;
    /** \deprecated
     * Use OperationalLimitsGroup::newCurrentLimits() instead.
     */
    virtual CurrentLimitsAdder newCurrentLimits2() override;

protected:
    AbstractConnectableBranch();

    virtual const FlowsLimitsHolder& getOperationalLimitsHolder1() const override;
    virtual FlowsLimitsHolder& getOperationalLimitsHolder1() override;
    virtual const FlowsLimitsHolder& getOperationalLimitsHolder2() const override;
    virtual FlowsLimitsHolder& getOperationalLimitsHolder2() override;

private:
    OperationalLimitsGroups m_operationalLimitsHolder1;

    OperationalLimitsGroups m_operationalLimitsHolder2;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ABSTRACTCONNECTABLEBRANCH_HPP
