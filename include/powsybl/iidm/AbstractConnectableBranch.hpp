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
    virtual stdcxx::Reference<OperationalLimitsGroup> newOperationalLimitsGroup1(const std::string& id) override;
    virtual void setSelectedOperationalLimitsGroup1(const std::string& id) override;
    virtual void removeOperationalLimitsGroup1(const std::string& id) override;
    virtual void cancelSelectedOperationalLimitsGroup1() override;

    virtual stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups2() const override;
    virtual stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups2()override;
    virtual const stdcxx::optional<std::string>& getSelectedOperationalLimitsGroupId2() const override;
    virtual stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup2(const std::string& id) const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup2(const std::string& id) override;
    virtual stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup2() const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup2() override;
    virtual stdcxx::Reference<OperationalLimitsGroup> newOperationalLimitsGroup2(const std::string& id) override;
    virtual void setSelectedOperationalLimitsGroup2(const std::string& id) override;
    virtual void removeOperationalLimitsGroup2(const std::string& id) override;
    virtual void cancelSelectedOperationalLimitsGroup2() override;

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
    OperationalLimitsGroups m_operationalLimitsHolder1;

    OperationalLimitsGroups m_operationalLimitsHolder2;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ABSTRACTCONNECTABLEBRANCH_HPP
