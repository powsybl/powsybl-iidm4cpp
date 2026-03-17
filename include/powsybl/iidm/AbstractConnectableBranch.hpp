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

    virtual stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups1() const override;
    virtual stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups1()override;
    virtual const stdcxx::optional<std::string>& getSelectedOperationalLimitsGroupId1() const override;
    virtual stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup1(const std::string& id) const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup1(const std::string& id) override;
    virtual stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup1() const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup1() override;
    virtual OperationalLimitsGroup& newOperationalLimitsGroup1(const std::string& id) override;
    virtual void setSelectedOperationalLimitsGroup1(const std::string& id) override;
    virtual void removeOperationalLimitsGroup1(const std::string& id) override;
    virtual void cancelSelectedOperationalLimitsGroup1() override;
    /**
     * Get the OperationalLimitsGroup selected on side 1 or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup1() override;
    /**
     * Get the OperationalLimitsGroup corresponding to the given id from side 1 or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup1(const std::string& id) override;

    virtual stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups2() const override;
    virtual stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups2()override;
    virtual const stdcxx::optional<std::string>& getSelectedOperationalLimitsGroupId2() const override;
    virtual stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup2(const std::string& id) const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup2(const std::string& id) override;
    virtual stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup2() const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup2() override;
    virtual OperationalLimitsGroup& newOperationalLimitsGroup2(const std::string& id) override;
    virtual void setSelectedOperationalLimitsGroup2(const std::string& id) override;
    virtual void removeOperationalLimitsGroup2(const std::string& id) override;
    virtual void cancelSelectedOperationalLimitsGroup2() override;
    /**
     * Get the OperationalLimitsGroup selected on side 2 or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup2() override;
    /**
     * Get the OperationalLimitsGroup corresponding to the given id from side 2 or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup2(const std::string& id) override;

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

private:
    OperationalLimitsGroups m_operationalLimitsHolder1;

    OperationalLimitsGroups m_operationalLimitsHolder2;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ABSTRACTCONNECTABLEBRANCH_HPP
