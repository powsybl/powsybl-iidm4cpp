/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BRANCH_HPP
#define POWSYBL_IIDM_BRANCH_HPP

#include <iosfwd>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/OperationalLimitsHolder.hpp>

namespace powsybl {

namespace iidm {

class ActivePowerLimits;
class ActivePowerLimitsAdder;
class ApparentPowerLimits;
class ApparentPowerLimitsAdder;
class CurrentLimitsAdder;

class Branch : public Connectable {
public:
    enum class Side : unsigned char {
        ONE,
        TWO
    };

public:
    class Overload {
    public:
        Overload(const CurrentLimits::TemporaryLimit& temporaryLimit, const std::string& previousLimitName, double previousLimit);

        ~Overload() noexcept = default;

        double getPreviousLimit() const;

        const std::string& getPreviousLimitName() const;

        const CurrentLimits::TemporaryLimit& getTemporaryLimit() const;

    private:
        CurrentLimits::TemporaryLimit m_temporaryLimit;

        std::string m_previousLimitName;

        double m_previousLimit;
    };

public:
    ~Branch() noexcept override = default;

    bool checkPermanentLimit(const Side& side) const;

    bool checkPermanentLimit(const Side& side, double limitReduction) const;

    bool checkPermanentLimit1() const;

    bool checkPermanentLimit1(double limitReduction) const;

    bool checkPermanentLimit2() const;

    bool checkPermanentLimit2(double limitReduction) const;

    std::unique_ptr<Overload> checkTemporaryLimits(const Side& side) const;

    std::unique_ptr<Overload> checkTemporaryLimits(const Side& side, double limitReduction) const;

    std::unique_ptr<Overload> checkTemporaryLimits1() const;

    std::unique_ptr<Overload> checkTemporaryLimits1(double limitReduction) const;

    std::unique_ptr<Overload> checkTemporaryLimits2() const;

    std::unique_ptr<Overload> checkTemporaryLimits2(double limitReduction) const;

    stdcxx::CReference<ActivePowerLimits> getActivePowerLimits(const Side& side) const;

    stdcxx::Reference<ActivePowerLimits> getActivePowerLimits(const Side& side);

    stdcxx::CReference<ActivePowerLimits> getActivePowerLimits1() const;

    stdcxx::Reference<ActivePowerLimits> getActivePowerLimits1();

    stdcxx::CReference<ActivePowerLimits> getActivePowerLimits2() const;

    stdcxx::Reference<ActivePowerLimits> getActivePowerLimits2();

    stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits(const Side& side) const;

    stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits(const Side& side);

    stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits1() const;

    stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits1();

    stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits2() const;

    stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits2();

    stdcxx::CReference<CurrentLimits> getCurrentLimits(const Side& side) const;

    stdcxx::Reference<CurrentLimits> getCurrentLimits(const Side& side);

    stdcxx::CReference<CurrentLimits> getCurrentLimits1() const;

    stdcxx::Reference<CurrentLimits> getCurrentLimits1();

    stdcxx::CReference<CurrentLimits> getCurrentLimits2() const;

    stdcxx::Reference<CurrentLimits> getCurrentLimits2();

    unsigned long getOverloadDuration() const;

    Side getSide(const Terminal& terminal) const;

    const Terminal& getTerminal(const Side& side) const;

    Terminal& getTerminal(const Side& side);

    const Terminal& getTerminal(const std::string& voltageLevelId) const;

    Terminal& getTerminal(const std::string& voltageLevelId);

    const Terminal& getTerminal1() const;

    Terminal& getTerminal1();

    const Terminal& getTerminal2() const;

    Terminal& getTerminal2();

    bool isOverloaded() const;

    bool isOverloaded(double limitReduction) const;

    ActivePowerLimitsAdder newActivePowerLimits1();

    ActivePowerLimitsAdder newActivePowerLimits2();

    ApparentPowerLimitsAdder newApparentPowerLimits1();

    ApparentPowerLimitsAdder newApparentPowerLimits2();

    CurrentLimitsAdder newCurrentLimits1();

    CurrentLimitsAdder newCurrentLimits2();

protected:
    Branch(const std::string& id, const std::string& name, bool fictitious, const ConnectableType& connectableType);

private:
    friend class CurrentLimitsAdder;

private:
    OperationalLimitsHolder m_operationalLimitsHolder1;

    OperationalLimitsHolder m_operationalLimitsHolder2;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BRANCH_HPP
