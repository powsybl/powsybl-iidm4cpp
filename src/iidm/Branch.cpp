/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Branch.hpp>

#include <limits>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/util/LimitViolationUtils.hpp>
#include <powsybl/stdcxx/MessageFormat.hpp>
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

Branch::Branch(const std::string& id, const std::string& name, const ConnectableType& connectableType) :
    Connectable(id, name, connectableType) {
}

bool Branch::checkPermanentLimit(const Side& side) const {
    return checkPermanentLimit(side, 1.0);
}

bool Branch::checkPermanentLimit(const Side& side, double limitReduction) const {
    return LimitViolationUtils::checkPermanentLimit(*this, side, limitReduction, getTerminal(side).getI());
}

bool Branch::checkPermanentLimit1() const {
    return checkPermanentLimit1(1.0);
}

bool Branch::checkPermanentLimit1(double limitReduction) const {
    return checkPermanentLimit(Side::ONE, limitReduction);
}

bool Branch::checkPermanentLimit2() const {
    return checkPermanentLimit2(1.0);
}

bool Branch::checkPermanentLimit2(double limitReduction) const {
    return checkPermanentLimit(Side::TWO, limitReduction);
}

std::unique_ptr<Branch::Overload> Branch::checkTemporaryLimits(const Side& side) const {
    return checkTemporaryLimits(side, 1.0);
}

std::unique_ptr<Branch::Overload> Branch::checkTemporaryLimits(const Side& side, double limitReduction) const {
    return LimitViolationUtils::checkTemporaryLimits(*this, side, limitReduction, getTerminal(side).getI());
}

std::unique_ptr<Branch::Overload> Branch::checkTemporaryLimits1() const {
    return checkTemporaryLimits1(1.0);
}

std::unique_ptr<Branch::Overload> Branch::checkTemporaryLimits1(double limitReduction) const {
    return checkTemporaryLimits(Side::ONE, limitReduction);
}

std::unique_ptr<Branch::Overload> Branch::checkTemporaryLimits2() const {
    return checkTemporaryLimits2(1.0);
}

std::unique_ptr<Branch::Overload> Branch::checkTemporaryLimits2(double limitReduction) const {
    return checkTemporaryLimits(Side::TWO, limitReduction);
}

stdcxx::CReference<CurrentLimits> Branch::getCurrentLimits(const Side& side) const {
    return stdcxx::cref(getCurrentLimitsPtr(side));
}

stdcxx::Reference<CurrentLimits> Branch::getCurrentLimits(const Side& side) {
    return stdcxx::ref(getCurrentLimitsPtr(side));
}

stdcxx::CReference<CurrentLimits> Branch::getCurrentLimits1() const {
    return getCurrentLimits(Side::ONE);
}

stdcxx::Reference<CurrentLimits> Branch::getCurrentLimits1() {
    return getCurrentLimits(Side::ONE);
}

stdcxx::CReference<CurrentLimits> Branch::getCurrentLimits2() const {
    return getCurrentLimits(Side::TWO);
}

stdcxx::Reference<CurrentLimits> Branch::getCurrentLimits2() {
    return getCurrentLimits(Side::TWO);
}

const std::unique_ptr<CurrentLimits>& Branch::getCurrentLimitsPtr(const Side& side) const {
    switch (side) {
        case Side::ONE:
            return m_limits1;

        case Side::TWO:
            return m_limits2;

        default:
            throw AssertionError(stdcxx::format("Unexpected side value: %1%", side));
    }
}

unsigned long Branch::getOverloadDuration() const {
    std::unique_ptr<Overload> o1 = checkTemporaryLimits1();
    std::unique_ptr<Overload> o2 = checkTemporaryLimits2();

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

bool Branch::isOverloaded() const {
    return isOverloaded(1.0);
}

bool Branch::isOverloaded(double limitReduction) const {
    return checkPermanentLimit1(limitReduction) || checkPermanentLimit2(limitReduction);
}

CurrentLimitsAdder<Branch::Side, Branch> Branch::newCurrentLimits1() {
    return CurrentLimitsAdder<Branch::Side, Branch>(Side::ONE, *this);
}

CurrentLimitsAdder<Branch::Side, Branch> Branch::newCurrentLimits2() {
    return CurrentLimitsAdder<Branch::Side, Branch>(Side::TWO, *this);
}

void Branch::setCurrentLimits(const Branch::Side& side, std::unique_ptr<CurrentLimits> limits) {
    switch (side) {
        case Side::ONE:
            m_limits1 = std::move(limits);
            break;

        case Side::TWO:
            m_limits2 = std::move(limits);
            break;

        default:
            throw AssertionError(stdcxx::format("Unexpected side value: %1%", side));
    }
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
