/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BRANCH_HPP
#define POWSYBL_IIDM_BRANCH_HPP

#include <cstdint>
#include <ostream>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>

namespace powsybl {

namespace iidm {

class Branch : public Connectable {
public:
    enum class Side : unsigned int {
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

    stdcxx::CReference<CurrentLimits> getCurrentLimits(const Side& side) const;

    stdcxx::Reference<CurrentLimits> getCurrentLimits(const Side& side);

    stdcxx::CReference<CurrentLimits> getCurrentLimits1() const;

    stdcxx::Reference<CurrentLimits> getCurrentLimits1();

    stdcxx::CReference<CurrentLimits> getCurrentLimits2() const;

    stdcxx::Reference<CurrentLimits> getCurrentLimits2();

    int32_t getOverloadDuration() const;

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

    CurrentLimitsAdder<Side, Branch> newCurrentLimits1();

    CurrentLimitsAdder<Side, Branch> newCurrentLimits2();

protected:
    Branch(const std::string& id, const std::string& name, const ConnectableType& connectableType);

private:
    const std::unique_ptr<CurrentLimits>& getCurrentLimitsPtr(const Side& side) const;

    void setCurrentLimits(const Side& side, std::unique_ptr<CurrentLimits> limits);

private:
    friend class CurrentLimitsAdder<Side, Branch>;

private:
    std::unique_ptr<CurrentLimits> m_limits1;

    std::unique_ptr<CurrentLimits> m_limits2;
};

std::string getSideName(const Branch::Side& side);

std::ostream& operator<<(std::ostream& stream, const Branch::Side& side);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BRANCH_HPP
