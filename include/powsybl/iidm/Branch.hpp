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

#include <list>

namespace powsybl {

namespace iidm {

class ActivePowerLimits;
class ActivePowerLimitsAdder;
class ApparentPowerLimits;
class ApparentPowerLimitsAdder;
class CurrentLimits;
class CurrentLimitsAdder;

class FlowsLimitsHolder;
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

    std::vector<std::unique_ptr<Overload>> checkAllTemporaryLimits(const TwoSides& side, const LimitType& type) const;

    std::vector<std::unique_ptr<Overload>> checkAllTemporaryLimits(const TwoSides& side, double limitReduction, const LimitType& type) const;

    virtual stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups1() const;
    virtual stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups1();
    virtual stdcxx::optional<std::string> getSelectedOperationalLimitsGroupId1() const;
    virtual const std::list<std::string>& getAllSelectedOperationalLimitsGroupIds1() const;
    virtual stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup1(const std::string& id) const;
    virtual stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup1(const std::string& id);
    virtual stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup1() const;
    virtual stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup1();
    virtual stdcxx::const_range<OperationalLimitsGroup> getAllSelectedOperationalLimitsGroups1() const;
    virtual stdcxx::range<OperationalLimitsGroup> getAllSelectedOperationalLimitsGroups1();
    virtual OperationalLimitsGroup& newOperationalLimitsGroup1(const std::string& id);
    virtual void setSelectedOperationalLimitsGroup1(const std::string& id);
    virtual void addSelectedOperationalLimitsGroups1(const std::list<std::string>& ids);
    virtual void addSelectedOperationalLimitsGroupsByPredicate1(const stdcxx::Predicate<std::string>& operationalLimitsGroupIdPredicate);
    virtual void removeOperationalLimitsGroup1(const std::string& id);
    virtual void cancelSelectedOperationalLimitsGroup1();
    virtual void deselectOperationalLimitsGroups1(const std::list<std::string>& ids);
    /**
     * Get the OperationalLimitsGroup selected on side 1 or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup1();
    /**
     * Get the OperationalLimitsGroup corresponding to the given id from side 1 or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup1(const std::string& id);


    virtual stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups2() const;
    virtual stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups2();
    virtual stdcxx::optional<std::string> getSelectedOperationalLimitsGroupId2() const;
    virtual const std::list<std::string>& getAllSelectedOperationalLimitsGroupIds2() const;
    virtual stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup2(const std::string& id) const;
    virtual stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup2(const std::string& id);
    virtual stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup2() const;
    virtual stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup2();
    virtual stdcxx::const_range<OperationalLimitsGroup> getAllSelectedOperationalLimitsGroups2() const;
    virtual stdcxx::range<OperationalLimitsGroup> getAllSelectedOperationalLimitsGroups2();
    virtual OperationalLimitsGroup& newOperationalLimitsGroup2(const std::string& id);
    virtual void setSelectedOperationalLimitsGroup2(const std::string& id);
    virtual void addSelectedOperationalLimitsGroups2(const std::list<std::string>& ids);
    virtual void addSelectedOperationalLimitsGroupsByPredicate2(const stdcxx::Predicate<std::string>& operationalLimitsGroupIdPredicate);
    virtual void removeOperationalLimitsGroup2(const std::string& id);
    virtual void cancelSelectedOperationalLimitsGroup2();
    virtual void deselectOperationalLimitsGroups2(const std::list<std::string>& ids);
    /**
     * Get the OperationalLimitsGroup selected on side 2 or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup2();
    /**
     * Get the OperationalLimitsGroup corresponding to the given id from side 2 or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup2(const std::string& id);

    const std::list<std::string>& getAllSelectedOperationalLimitsGroupIds(const TwoSides& side) const;
    stdcxx::const_range<OperationalLimitsGroup> getAllSelectedOperationalLimitsGroups(const TwoSides& side) const;
    stdcxx::range<OperationalLimitsGroup> getAllSelectedOperationalLimitsGroups(const TwoSides& side);
    void addSelectedOperationalLimitsGroups(const TwoSides& side, const std::list<std::string>& ids);
    void addSelectedOperationalLimitsGroupsByPredicate(const TwoSides& side, const stdcxx::Predicate<std::string>& operationalLimitsGroupIdPredicate);
    void deselectOperationalLimitsGroups(const TwoSides& side, const std::list<std::string>& ids);

    stdcxx::CReference<ActivePowerLimits> getActivePowerLimits(const TwoSides& side) const;

    stdcxx::Reference<ActivePowerLimits> getActivePowerLimits(const TwoSides& side);

    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimits1() const;

    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimits1();

    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimits2() const;

    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimits2();

    stdcxx::const_range<ActivePowerLimits> getAllSelectedActivePowerLimits(const TwoSides& side) const;
    stdcxx::range<ActivePowerLimits> getAllSelectedActivePowerLimits(const TwoSides& side);
    virtual stdcxx::const_range<ActivePowerLimits> getAllSelectedActivePowerLimits1() const;
    virtual stdcxx::range<ActivePowerLimits> getAllSelectedActivePowerLimits1();
    virtual stdcxx::const_range<ActivePowerLimits> getAllSelectedActivePowerLimits2() const;
    virtual stdcxx::range<ActivePowerLimits> getAllSelectedActivePowerLimits2();

    stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits(const TwoSides& side) const;

    stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits(const TwoSides& side);

    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits1() const ;

    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits1();

    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits2() const;

    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits2();

    stdcxx::const_range<ApparentPowerLimits> getAllSelectedApparentPowerLimits(const TwoSides& side) const;
    stdcxx::range<ApparentPowerLimits> getAllSelectedApparentPowerLimits(const TwoSides& side);
    virtual stdcxx::const_range<ApparentPowerLimits> getAllSelectedApparentPowerLimits1() const;
    virtual stdcxx::range<ApparentPowerLimits> getAllSelectedApparentPowerLimits1();
    virtual stdcxx::const_range<ApparentPowerLimits> getAllSelectedApparentPowerLimits2() const;
    virtual stdcxx::range<ApparentPowerLimits> getAllSelectedApparentPowerLimits2();

    stdcxx::CReference<CurrentLimits> getCurrentLimits(const TwoSides& side) const;

    stdcxx::Reference<CurrentLimits> getCurrentLimits(const TwoSides& side);

    virtual stdcxx::CReference<CurrentLimits> getCurrentLimits1() const;

    virtual stdcxx::Reference<CurrentLimits> getCurrentLimits1();

    virtual stdcxx::CReference<CurrentLimits> getCurrentLimits2() const;

    virtual stdcxx::Reference<CurrentLimits> getCurrentLimits2();

    stdcxx::const_range<CurrentLimits> getAllSelectedCurrentLimits(const TwoSides& side) const;
    stdcxx::range<CurrentLimits> getAllSelectedCurrentLimits(const TwoSides& side);
    virtual stdcxx::const_range<CurrentLimits> getAllSelectedCurrentLimits1() const;
    virtual stdcxx::range<CurrentLimits> getAllSelectedCurrentLimits1();
    virtual stdcxx::const_range<CurrentLimits> getAllSelectedCurrentLimits2() const;
    virtual stdcxx::range<CurrentLimits> getAllSelectedCurrentLimits2();

    stdcxx::CReference<LoadingLimits> getLimits(const LimitType& type, const TwoSides& side) const;

    stdcxx::Reference<LoadingLimits> getLimits(const LimitType& type, const TwoSides& side);

    stdcxx::const_range<LoadingLimits> getAllSelectedLoadingLimits(const LimitType& type, const TwoSides& side) const;
    stdcxx::range<LoadingLimits> getAllSelectedLoadingLimits(const LimitType& type, const TwoSides& side);

    virtual unsigned long getOverloadDuration() const;

    TwoSides getSide(const Terminal& terminal) const;

    const Terminal& getTerminalFromSide(const TwoSides& side) const;

    Terminal& getTerminalFromSide(const TwoSides& side);

    const Terminal& getTerminalFromVoltageLevel(const std::string& voltageLevelId) const;

    Terminal& getTerminalFromVoltageLevel(const std::string& voltageLevelId);

    virtual const Terminal& getTerminal1() const = 0;

    virtual Terminal& getTerminal1() = 0;

    virtual const Terminal& getTerminal2() const = 0;

    virtual Terminal& getTerminal2() = 0;

    /**
     * Only checks permanent limits for limitType::CURRENT
     */
    bool isOverloaded() const;

    /**
     * Only checks permanent limits for limitType::CURRENT
     */
    bool isOverloaded(double limitReduction) const;

    /** \deprecated
     * Use OperationalLimitsGroup::newActivePowerLimits() instead.
     */
    virtual ActivePowerLimitsAdder newActivePowerLimits1() = 0;
    /** \deprecated
     * Use OperationalLimitsGroup::newActivePowerLimits(const ActivePowerLimits& activePowerLimits) instead.
     */
    ActivePowerLimitsAdder newActivePowerLimits1(const ActivePowerLimits& activePowerLimits);

    /** \deprecated
     * Use OperationalLimitsGroup::newActivePowerLimits() instead.
     */
    virtual ActivePowerLimitsAdder newActivePowerLimits2() = 0;
    /** \deprecated
     * Use OperationalLimitsGroup::newActivePowerLimits(const ActivePowerLimits& activePowerLimits) instead.
     */
    ActivePowerLimitsAdder newActivePowerLimits2(const ActivePowerLimits& activePowerLimits);

    /** \deprecated
     * Use OperationalLimitsGroup::newApparentPowerLimits() instead.
     */
    virtual ApparentPowerLimitsAdder newApparentPowerLimits1() = 0;
    /** \deprecated
     * Use OperationalLimitsGroup::newApparentPowerLimits(const ApparentPowerLimits& apparentPowerLimits) instead.
     */
    ApparentPowerLimitsAdder newApparentPowerLimits1(const ApparentPowerLimits& apparentPowerLimits);

    /** \deprecated
     * Use OperationalLimitsGroup::newApparentPowerLimits() instead.
     */
    virtual ApparentPowerLimitsAdder newApparentPowerLimits2() = 0;
    /** \deprecated
     * Use OperationalLimitsGroup::newApparentPowerLimits(const ApparentPowerLimits& apparentPowerLimits) instead.
     */
    ApparentPowerLimitsAdder newApparentPowerLimits2(const ApparentPowerLimits& apparentPowerLimits);

    /** \deprecated
     * Use OperationalLimitsGroup::newCurrentLimits() instead.
     */
    virtual CurrentLimitsAdder newCurrentLimits1() = 0;
    /** \deprecated
     * Use OperationalLimitsGroup::newCurrentLimits(const CurrentLimits& currentLimits) instead.
     */
    CurrentLimitsAdder newCurrentLimits1(const CurrentLimits& currentLimits);

    /** \deprecated
     * Use OperationalLimitsGroup::newCurrentLimits() instead.
     */
    virtual CurrentLimitsAdder newCurrentLimits2() = 0;
    /** \deprecated
     * Use OperationalLimitsGroup::newCurrentLimits(const CurrentLimits& currentLimits) instead.
     */
    CurrentLimitsAdder newCurrentLimits2(const CurrentLimits& currentLimits);

protected:
    virtual const FlowsLimitsHolder& getOperationalLimitsHolder(const TwoSides& side) const;
    virtual FlowsLimitsHolder& getOperationalLimitsHolder(const TwoSides& side);
    virtual const FlowsLimitsHolder& getOperationalLimitsHolder1() const = 0;
    virtual FlowsLimitsHolder& getOperationalLimitsHolder1() = 0;
    virtual const FlowsLimitsHolder& getOperationalLimitsHolder2() const = 0;
    virtual FlowsLimitsHolder& getOperationalLimitsHolder2() = 0;

    Branch() = default;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BRANCH_HPP
