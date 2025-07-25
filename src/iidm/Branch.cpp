/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Branch.hpp>

#include <limits>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/util/LimitViolationUtils.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

Branch::Overload::Overload(const CurrentLimits::TemporaryLimit& temporaryLimit, const std::string& previousLimitName, double previousLimit) :
    m_temporaryLimit(temporaryLimit),
    m_previousLimitName(previousLimitName),
    m_previousLimit(previousLimit) {
}

double Branch::Overload::getPreviousLimit() const {
    return m_previousLimit;
}

const std::string& Branch::Overload::getPreviousLimitName() const {
    return m_previousLimitName;
}

const CurrentLimits::TemporaryLimit& Branch::Overload::getTemporaryLimit() const {
    return m_temporaryLimit;
}

Branch::Branch(const std::string& id, const std::string& name, bool fictitious) :
    Connectable(id, name, fictitious),
    m_operationalLimitsHolder1(*this, "limits1"),
    m_operationalLimitsHolder2(*this, "limits2") {
}

bool Branch::checkPermanentLimit(const Side& side, const LimitType& type) const {
    return checkPermanentLimit(side, 1.0, type);
}

bool Branch::checkPermanentLimit(const Side& side, double limitReduction, const LimitType& type) const {
    switch (side) {
        case Side::ONE:
            return checkPermanentLimit1(limitReduction, type);

        case Side::TWO:
            return checkPermanentLimit2(limitReduction, type);

        default:
            throw AssertionError(stdcxx::format("Unexpected side %1%", side));
    }
}

bool Branch::checkPermanentLimit1(const LimitType& type) const {
    return checkPermanentLimit1(1.0, type);
}

bool Branch::checkPermanentLimit1(double limitReduction, const LimitType& type) const {
    return LimitViolationUtils::checkPermanentLimit(*this, Side::ONE, limitReduction, getValueForLimit(getTerminal1(), type), type);
}

bool Branch::checkPermanentLimit2(const LimitType& type) const {
    return checkPermanentLimit2(1.0, type);
}

bool Branch::checkPermanentLimit2(double limitReduction, const LimitType& type) const {
    return LimitViolationUtils::checkPermanentLimit(*this, Side::TWO, limitReduction, getValueForLimit(getTerminal2(), type), type);
}

std::unique_ptr<Branch::Overload> Branch::checkTemporaryLimits(const Side& side, const LimitType& type) const {
    return checkTemporaryLimits(side, 1.0, type);
}

std::unique_ptr<Branch::Overload> Branch::checkTemporaryLimits(const Side& side, double limitReduction, const LimitType& type) const {
    switch (side) {
        case Side::ONE:
            return checkTemporaryLimits1(limitReduction, type);

        case Side::TWO:
            return checkTemporaryLimits2(limitReduction, type);

        default:
            throw AssertionError(stdcxx::format("Unexpected side %1%", side));
    }
}

std::unique_ptr<Branch::Overload> Branch::checkTemporaryLimits1(const LimitType& type) const {
    return checkTemporaryLimits1(1.0, type);
}

std::unique_ptr<Branch::Overload> Branch::checkTemporaryLimits1(double limitReduction, const LimitType& type) const {
    return LimitViolationUtils::checkTemporaryLimits(*this, Side::ONE, limitReduction, getValueForLimit(getTerminal1(), type), type);
}

std::unique_ptr<Branch::Overload> Branch::checkTemporaryLimits2(const LimitType& type) const {
    return checkTemporaryLimits2(1.0, type);
}

std::unique_ptr<Branch::Overload> Branch::checkTemporaryLimits2(double limitReduction, const LimitType& type) const {
    return LimitViolationUtils::checkTemporaryLimits(*this, Side::TWO, limitReduction, getValueForLimit(getTerminal2(), type), type);
}

stdcxx::CReference<ActivePowerLimits> Branch::getActivePowerLimits(const Side& side) const {
    switch (side) {
        case Side::ONE:
            return getActivePowerLimits1();
        case Side::TWO:
            return getActivePowerLimits2();
        default:
            throw AssertionError(stdcxx::format("Unexpected side: %1%", side));
    }
}

stdcxx::Reference<ActivePowerLimits> Branch::getActivePowerLimits(const Side& side) {
    return stdcxx::ref(const_cast<const Branch*>(this)->getActivePowerLimits(side));
}

stdcxx::CReference<ActivePowerLimits> Branch::getActivePowerLimits1() const {
    return m_operationalLimitsHolder1.getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}

stdcxx::Reference<ActivePowerLimits> Branch::getActivePowerLimits1() {
    return m_operationalLimitsHolder1.getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}

stdcxx::CReference<ActivePowerLimits> Branch::getActivePowerLimits2() const {
    return m_operationalLimitsHolder2.getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}

stdcxx::Reference<ActivePowerLimits> Branch::getActivePowerLimits2() {
    return m_operationalLimitsHolder2.getOperationalLimits<ActivePowerLimits>(LimitType::ACTIVE_POWER);
}

stdcxx::CReference<ApparentPowerLimits> Branch::getApparentPowerLimits(const Side& side) const {
    switch (side) {
        case Side::ONE:
            return getApparentPowerLimits1();
        case Side::TWO:
            return getApparentPowerLimits2();
        default:
            throw AssertionError(stdcxx::format("Unexpected side: %1%", side));
    }
}

stdcxx::Reference<ApparentPowerLimits> Branch::getApparentPowerLimits(const Side& side) {
    return stdcxx::ref(const_cast<const Branch*>(this)->getApparentPowerLimits(side));
}

stdcxx::CReference<ApparentPowerLimits> Branch::getApparentPowerLimits1() const {
    return m_operationalLimitsHolder1.getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}

stdcxx::Reference<ApparentPowerLimits> Branch::getApparentPowerLimits1() {
    return m_operationalLimitsHolder1.getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}

stdcxx::CReference<ApparentPowerLimits> Branch::getApparentPowerLimits2() const {
    return m_operationalLimitsHolder2.getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}

stdcxx::Reference<ApparentPowerLimits> Branch::getApparentPowerLimits2() {
    return m_operationalLimitsHolder2.getOperationalLimits<ApparentPowerLimits>(LimitType::APPARENT_POWER);
}

stdcxx::CReference<CurrentLimits> Branch::getCurrentLimits(const Side& side) const {
    switch (side) {
        case Side::ONE:
            return getCurrentLimits1();

        case Side::TWO:
            return getCurrentLimits2();

        default:
            throw AssertionError(stdcxx::format("Unexpected side: %1%", side));
    }
}

stdcxx::Reference<CurrentLimits> Branch::getCurrentLimits(const Side& side) {
    return stdcxx::ref(const_cast<const Branch*>(this)->getCurrentLimits(side));
}

stdcxx::CReference<CurrentLimits> Branch::getCurrentLimits1() const {
    return m_operationalLimitsHolder1.getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}

stdcxx::Reference<CurrentLimits> Branch::getCurrentLimits1() {
    return m_operationalLimitsHolder1.getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}

stdcxx::CReference<CurrentLimits> Branch::getCurrentLimits2() const {
    return m_operationalLimitsHolder2.getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}

stdcxx::Reference<CurrentLimits> Branch::getCurrentLimits2() {
    return m_operationalLimitsHolder2.getOperationalLimits<CurrentLimits>(LimitType::CURRENT);
}

stdcxx::CReference<LoadingLimits> Branch::getLimits(const LimitType& type, const Side& side) const {
    switch (type) {
        case LimitType::CURRENT:
            return stdcxx::cref<LoadingLimits>(getCurrentLimits(side));

        case LimitType::ACTIVE_POWER:
            return stdcxx::cref<LoadingLimits>(getActivePowerLimits(side));

        case LimitType::APPARENT_POWER:
            return stdcxx::cref<LoadingLimits>(getApparentPowerLimits(side));

        case LimitType::VOLTAGE:
        default:
            throw AssertionError(stdcxx::format("Getting %1% limits is not supported.", type));
    }
}

stdcxx::Reference<LoadingLimits> Branch::getLimits(const LimitType& type, const Side& side) {
    return stdcxx::ref(const_cast<const Branch*>(this)->getLimits(type, side));
}

unsigned long Branch::getOverloadDuration() const {
    std::unique_ptr<Overload> o1 = checkTemporaryLimits1(LimitType::CURRENT);
    std::unique_ptr<Overload> o2 = checkTemporaryLimits2(LimitType::CURRENT);

    unsigned long duration1 = o1 ? o1->getTemporaryLimit().getAcceptableDuration() : std::numeric_limits<unsigned long>::max();
    unsigned long duration2 = o2 ? o2->getTemporaryLimit().getAcceptableDuration() : std::numeric_limits<unsigned long>::max();

    return std::min(duration1, duration2);
}

Branch::Side Branch::getSide(const Terminal& terminal) const {
    if (stdcxx::areSame(terminal, getTerminals().at(0).get())) {
        return Side::ONE;
    }
    if (stdcxx::areSame(terminal, getTerminals().at(1).get())) {
        return Side::TWO;
    }

    throw AssertionError("The terminal is not connected to this branch");
}

const Terminal& Branch::getTerminal(const Side& side) const {
    switch (side) {
        case Side::ONE:
            return getTerminals().at(0).get();

        case Side::TWO:
            return getTerminals().at(1).get();

        default:
            throw AssertionError(stdcxx::format("Unexpected side value: %1%", side));
    }
}

Terminal& Branch::getTerminal(const Side& side) {
    return const_cast<Terminal&>(static_cast<const Branch*>(this)->getTerminal(side));
}

const Terminal& Branch::getTerminal(const std::string& voltageLevelId) const {
    bool side1 = getTerminal1().getVoltageLevel().getId() == voltageLevelId;
    bool side2 = getTerminal2().getVoltageLevel().getId() == voltageLevelId;
    if (side1 && side2) {
        throw PowsyblException(stdcxx::format("Both terminals are connected to voltage level %1%", voltageLevelId));
    }
    if (side1) {
        return getTerminal1();
    }
    if (side2) {
        return getTerminal2();
    }

    throw PowsyblException(stdcxx::format("No terminal connected to voltage level %1%", voltageLevelId));
}

Terminal& Branch::getTerminal(const std::string& voltageLevelId) {
    return const_cast<Terminal&>(static_cast<const Branch*>(this)->getTerminal(voltageLevelId));
}

const Terminal& Branch::getTerminal1() const {
    return getTerminal(Side::ONE);
}

Terminal& Branch::getTerminal1() {
    return getTerminal(Side::ONE);
}

const Terminal& Branch::getTerminal2() const {
    return getTerminal(Side::TWO);
}

Terminal& Branch::getTerminal2() {
    return getTerminal(Side::TWO);
}

double Branch::getValueForLimit(const Terminal& terminal, const LimitType& type) const {
    switch (type) {
        case LimitType::ACTIVE_POWER:
            return terminal.getP();

        case LimitType::APPARENT_POWER:
            return std::sqrt(terminal.getP() * terminal.getP() + terminal.getQ() * terminal.getQ());

        case LimitType::CURRENT:
            return terminal.getI();

        case LimitType::VOLTAGE:
        default:
            throw AssertionError(stdcxx::format("Getting %1% limits is not supported", type));
    }
}

bool Branch::isOverloaded() const {
    return isOverloaded(1.0);
}

bool Branch::isOverloaded(double limitReduction) const {
    return checkPermanentLimit1(limitReduction, LimitType::CURRENT) || checkPermanentLimit2(limitReduction, LimitType::CURRENT);
}

ActivePowerLimitsAdder Branch::newActivePowerLimits1() {
    return m_operationalLimitsHolder1.newActivePowerLimits();
}

ActivePowerLimitsAdder Branch::newActivePowerLimits2() {
    return m_operationalLimitsHolder2.newActivePowerLimits();
}

ApparentPowerLimitsAdder Branch::newApparentPowerLimits1() {
    return m_operationalLimitsHolder1.newApparentPowerLimits();
}

ApparentPowerLimitsAdder Branch::newApparentPowerLimits2() {
    return m_operationalLimitsHolder2.newApparentPowerLimits();
}

CurrentLimitsAdder Branch::newCurrentLimits1() {
    return m_operationalLimitsHolder1.newCurrentLimits();
}

CurrentLimitsAdder Branch::newCurrentLimits2() {
    return m_operationalLimitsHolder2.newCurrentLimits();
}

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<Branch::Side>() {
    static std::initializer_list<std::string> s_sideNames{
        "ONE",
        "TWO"
    };
    return s_sideNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
