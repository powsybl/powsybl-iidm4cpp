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
#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/Overload.hpp>
#include <powsybl/iidm/Terminal.hpp>


namespace powsybl {

namespace iidm {

class ActivePowerLimits;
class ActivePowerLimitsAdder;
class ApparentPowerLimits;
class ApparentPowerLimitsAdder;
class CurrentLimitsAdder;

class Branch : public virtual Identifiable {
public:
    enum class Side : unsigned char {
        ONE,
        TWO
    };

public:
    ~Branch() noexcept override = default;

    bool checkPermanentLimit(const Side& side, const LimitType& type) const;

    bool checkPermanentLimit(const Side& side, double limitReduction, const LimitType& type) const;

    bool checkPermanentLimit1(const LimitType& type) const;

    bool checkPermanentLimit1(double limitReduction, const LimitType& type) const;

    bool checkPermanentLimit2(const LimitType& type) const;

    bool checkPermanentLimit2(double limitReduction, const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits(const Side& side, const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits(const Side& side, double limitReduction, const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits1(const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits1(double limitReduction, const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits2(const LimitType& type) const;

    std::unique_ptr<Overload> checkTemporaryLimits2(double limitReduction, const LimitType& type) const;

    stdcxx::CReference<ActivePowerLimits> getActivePowerLimits(const Side& side) const;

    stdcxx::Reference<ActivePowerLimits> getActivePowerLimits(const Side& side);

    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimits1() const = 0;

    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimits1() = 0;

    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimits2() const = 0;

    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimits2() = 0;

    stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits(const Side& side) const;

    stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits(const Side& side);

    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits1() const = 0;

    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits1() = 0;

    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits2() const = 0;

    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits2() = 0;

    stdcxx::CReference<CurrentLimits> getCurrentLimits(const Side& side) const;

    stdcxx::Reference<CurrentLimits> getCurrentLimits(const Side& side);

    virtual stdcxx::CReference<CurrentLimits> getCurrentLimits1() const = 0;

    virtual stdcxx::Reference<CurrentLimits> getCurrentLimits1() = 0;

    virtual stdcxx::CReference<CurrentLimits> getCurrentLimits2() const = 0;

    virtual stdcxx::Reference<CurrentLimits> getCurrentLimits2() = 0;

    stdcxx::CReference<LoadingLimits> getLimits(const LimitType& type, const Side& side) const;

    stdcxx::Reference<LoadingLimits> getLimits(const LimitType& type, const Side& side);

    unsigned long getOverloadDuration() const;

    Side getSide(const Terminal& terminal) const;

    const Terminal& getTerminalFromSide(const Side& side) const;

    Terminal& getTerminalFromSide(const Side& side);

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

private:
    friend class CurrentLimitsAdder;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BRANCH_HPP
