/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BRANCH_HPP
#define POWSYBL_IIDM_BRANCH_HPP

#include <iosfwd>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/Overload.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/TwoSides.hpp>


namespace powsybl {

namespace iidm {

class ActivePowerLimits;
class ActivePowerLimitsAdder;
class ApparentPowerLimits;
class ApparentPowerLimitsAdder;
class CurrentLimits;
class CurrentLimitsAdder;

class OperationalLimitsGroup;

class Branch : public virtual Identifiable {
public:
    ~Branch() noexcept override = default;

    bool checkPermanentLimit(const TwoSides& side, const LimitType& type) const;

    bool checkPermanentLimit(const TwoSides& side, double limitReduction, const LimitType& type) const;

    bool checkPermanentLimit1(const LimitType& type) const;

    bool checkPermanentLimit1(double limitReduction, const LimitType& type) const;

    bool checkPermanentLimit2(const LimitType& type) const;

    bool checkPermanentLimit2(double limitReduction, const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits(const TwoSides& side, const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits(const TwoSides& side, double limitReduction, const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits1(const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits1(double limitReduction, const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits2(const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits2(double limitReduction, const LimitType& type) const;

    virtual stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups1() const = 0;
    virtual stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups1()= 0;
    virtual const stdcxx::optional<std::string>& getSelectedOperationalLimitsGroupId1() const = 0;
    virtual stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup1(const std::string& id) const = 0;
    virtual stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup1(const std::string& id) = 0;
    virtual stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup1() const = 0;
    virtual stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup1() = 0;
    virtual stdcxx::Reference<OperationalLimitsGroup> newOperationalLimitsGroup1(const std::string& id) = 0;
    virtual void setSelectedOperationalLimitsGroup1(const std::string& id) = 0;
    virtual void removeOperationalLimitsGroup1(const std::string& id) = 0;
    virtual void cancelSelectedOperationalLimitsGroup1() = 0;

    virtual stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups2() const = 0;
    virtual stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups2()= 0;
    virtual const stdcxx::optional<std::string>& getSelectedOperationalLimitsGroupId2() const = 0;
    virtual stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup2(const std::string& id) const = 0;
    virtual stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup2(const std::string& id) = 0;
    virtual stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup2() const = 0;
    virtual stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup2() = 0;
    virtual stdcxx::Reference<OperationalLimitsGroup> newOperationalLimitsGroup2(const std::string& id) = 0;
    virtual void setSelectedOperationalLimitsGroup2(const std::string& id) = 0;
    virtual void removeOperationalLimitsGroup2(const std::string& id) = 0;
    virtual void cancelSelectedOperationalLimitsGroup2() = 0;

    stdcxx::CReference<ActivePowerLimits> getActivePowerLimits(const TwoSides& side) const;

    stdcxx::Reference<ActivePowerLimits> getActivePowerLimits(const TwoSides& side);

    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimits1() const;

    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimits1();

    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimits2() const;

    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimits2();

    stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits(const TwoSides& side) const;

    stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits(const TwoSides& side);

    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits1() const ;

    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits1();

    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits2() const;

    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits2();

    stdcxx::CReference<CurrentLimits> getCurrentLimits(const TwoSides& side) const;

    stdcxx::Reference<CurrentLimits> getCurrentLimits(const TwoSides& side);

    virtual stdcxx::CReference<CurrentLimits> getCurrentLimits1() const;

    virtual stdcxx::Reference<CurrentLimits> getCurrentLimits1();

    virtual stdcxx::CReference<CurrentLimits> getCurrentLimits2() const;

    virtual stdcxx::Reference<CurrentLimits> getCurrentLimits2();

    stdcxx::CReference<LoadingLimits> getLimits(const LimitType& type, const TwoSides& side) const;

    stdcxx::Reference<LoadingLimits> getLimits(const LimitType& type, const TwoSides& side);

    unsigned long getOverloadDuration() const;

    TwoSides getSide(const Terminal& terminal) const;

    const Terminal& getTerminalFromSide(const TwoSides& side) const;

    Terminal& getTerminalFromSide(const TwoSides& side);

    const Terminal& getTerminalFromVoltageLevel(const std::string& voltageLevelId) const;

    Terminal& getTerminalFromVoltageLevel(const std::string& voltageLevelId);

    virtual const Terminal& getTerminal1() const = 0;

    virtual Terminal& getTerminal1() = 0;

    virtual const Terminal& getTerminal2() const = 0;

    virtual Terminal& getTerminal2() = 0;

    bool isOverloaded() const;

    bool isOverloaded(double limitReduction) const;

    virtual ActivePowerLimitsAdder newActivePowerLimits1() = 0;

    virtual ActivePowerLimitsAdder newActivePowerLimits2() = 0;

    virtual ApparentPowerLimitsAdder newApparentPowerLimits1() = 0;

    virtual ApparentPowerLimitsAdder newApparentPowerLimits2() = 0;

    virtual CurrentLimitsAdder newCurrentLimits1() = 0;

    virtual CurrentLimitsAdder newCurrentLimits2() = 0;

protected:
    Branch() = default;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BRANCH_HPP
