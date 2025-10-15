/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Branch.hpp>

#include <limits>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/ActivePowerLimits.hpp>
#include <powsybl/iidm/ApparentPowerLimits.hpp>
#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/util/LimitViolationUtils.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

bool Branch::checkPermanentLimit(const TwoSides& side, const LimitType& type) const {
    return checkPermanentLimit(side, 1.0, type);
}

bool Branch::checkPermanentLimit(const TwoSides& side, double limitReduction, const LimitType& type) const {
    switch (side) {
        case TwoSides::ONE:
            return checkPermanentLimit1(limitReduction, type);

        case TwoSides::TWO:
            return checkPermanentLimit2(limitReduction, type);

        case TwoSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected TwoSides value: %1%", side));
    }
}

bool Branch::checkPermanentLimit1(const LimitType& type) const {
    return checkPermanentLimit1(1.0, type);
}

bool Branch::checkPermanentLimit1(double limitReduction, const LimitType& type) const {
    double limitValue = LimitViolationUtils::getValueForLimit(getTerminal1(), type);
    return LimitViolationUtils::checkPermanentLimit(*this, TwoSides::ONE, limitReduction, limitValue, type);
}

bool Branch::checkPermanentLimit2(const LimitType& type) const {
    return checkPermanentLimit2(1.0, type);
}

bool Branch::checkPermanentLimit2(double limitReduction, const LimitType& type) const {
    double limitValue = LimitViolationUtils::getValueForLimit(getTerminal2(), type);
    return LimitViolationUtils::checkPermanentLimit(*this, TwoSides::TWO, limitReduction, limitValue, type);
}

std::unique_ptr<Overload> Branch::checkTemporaryLimits(const TwoSides& side, const LimitType& type) const {
    return checkTemporaryLimits(side, 1.0, type);
}

std::unique_ptr<Overload> Branch::checkTemporaryLimits(const TwoSides& side, double limitReduction, const LimitType& type) const {
    switch (side) {
        case TwoSides::ONE:
            return checkTemporaryLimits1(limitReduction, type);

        case TwoSides::TWO:
            return checkTemporaryLimits2(limitReduction, type);

        case TwoSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected TwoSides value: %1%", side));
    }
}

std::unique_ptr<Overload> Branch::checkTemporaryLimits1(const LimitType& type) const {
    return checkTemporaryLimits1(1.0, type);
}

std::unique_ptr<Overload> Branch::checkTemporaryLimits1(double limitReduction, const LimitType& type) const {
    double limitValue = LimitViolationUtils::getValueForLimit(getTerminal1(), type);
    return LimitViolationUtils::checkTemporaryLimits(*this, TwoSides::ONE, limitReduction, limitValue, type);
}

std::unique_ptr<Overload> Branch::checkTemporaryLimits2(const LimitType& type) const {
    return checkTemporaryLimits2(1.0, type);
}

std::unique_ptr<Overload> Branch::checkTemporaryLimits2(double limitReduction, const LimitType& type) const {
    double limitValue = LimitViolationUtils::getValueForLimit(getTerminal2(), type);
    return LimitViolationUtils::checkTemporaryLimits(*this, TwoSides::TWO, limitReduction, limitValue, type);
}

stdcxx::CReference<ActivePowerLimits> Branch::getActivePowerLimits(const TwoSides& side) const {
    switch (side) {
        case TwoSides::ONE:
            return getActivePowerLimits1();
        case TwoSides::TWO:
            return getActivePowerLimits2();
        case TwoSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected TwoSides value: %1%", side));
    }
}

stdcxx::Reference<ActivePowerLimits> Branch::getActivePowerLimits(const TwoSides& side) {
    return stdcxx::ref(const_cast<const Branch*>(this)->getActivePowerLimits(side));
}

stdcxx::CReference<ApparentPowerLimits> Branch::getApparentPowerLimits(const TwoSides& side) const {
    switch (side) {
        case TwoSides::ONE:
            return getApparentPowerLimits1();
        case TwoSides::TWO:
            return getApparentPowerLimits2();
        case TwoSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected TwoSides value: %1%", side));
    }
}

stdcxx::Reference<ApparentPowerLimits> Branch::getApparentPowerLimits(const TwoSides& side) {
    return stdcxx::ref(const_cast<const Branch*>(this)->getApparentPowerLimits(side));
}

stdcxx::CReference<CurrentLimits> Branch::getCurrentLimits(const TwoSides& side) const {
    switch (side) {
        case TwoSides::ONE:
            return getCurrentLimits1();

        case TwoSides::TWO:
            return getCurrentLimits2();

        case TwoSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected TwoSides value: %1%", side));
    }
}

stdcxx::Reference<CurrentLimits> Branch::getCurrentLimits(const TwoSides& side) {
    return stdcxx::ref(const_cast<const Branch*>(this)->getCurrentLimits(side));
}

stdcxx::CReference<LoadingLimits> Branch::getLimits(const LimitType& type, const TwoSides& side) const {
    switch (type) {
        case LimitType::CURRENT:
            return stdcxx::cref<LoadingLimits>(getCurrentLimits(side));

        case LimitType::ACTIVE_POWER:
            return stdcxx::cref<LoadingLimits>(getActivePowerLimits(side));

        case LimitType::APPARENT_POWER:
            return stdcxx::cref<LoadingLimits>(getApparentPowerLimits(side));

        case LimitType::VOLTAGE:
        case LimitType::VOLTAGE_ANGLE:
        default:
            throw AssertionError(stdcxx::format("Getting %1% limits is not supported.", type));
    }
}

stdcxx::Reference<LoadingLimits> Branch::getLimits(const LimitType& type, const TwoSides& side) {
    return stdcxx::ref(const_cast<const Branch*>(this)->getLimits(type, side));
}

unsigned long Branch::getOverloadDuration() const {
    std::unique_ptr<Overload> o1 = checkTemporaryLimits1(LimitType::CURRENT);
    std::unique_ptr<Overload> o2 = checkTemporaryLimits2(LimitType::CURRENT);

    unsigned long duration1 = o1 ? o1->getTemporaryLimit().getAcceptableDuration() : std::numeric_limits<unsigned long>::max();
    unsigned long duration2 = o2 ? o2->getTemporaryLimit().getAcceptableDuration() : std::numeric_limits<unsigned long>::max();

    return std::min(duration1, duration2);
}

TwoSides Branch::getSide(const Terminal& terminal) const {
    if (stdcxx::areSame(terminal, getTerminal1())) {
        return TwoSides::ONE;
    }
    if (stdcxx::areSame(terminal, getTerminal2())) {
        return TwoSides::TWO;
    }

    throw AssertionError("The terminal is not connected to this branch");
}

const Terminal& Branch::getTerminalFromSide(const TwoSides& side) const {
    switch (side) {
        case TwoSides::ONE:
            return getTerminal1();

        case TwoSides::TWO:
            return getTerminal2();

        case TwoSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected TwoSides value: %1%", side));
    }
}

Terminal& Branch::getTerminalFromSide(const TwoSides& side) {
    return const_cast<Terminal&>(static_cast<const Branch*>(this)->getTerminalFromSide(side));
}

const Terminal& Branch::getTerminalFromVoltageLevel(const std::string& voltageLevelId) const {
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

Terminal& Branch::getTerminalFromVoltageLevel(const std::string& voltageLevelId) {
    return const_cast<Terminal&>(static_cast<const Branch*>(this)->getTerminalFromVoltageLevel(voltageLevelId));
}

bool Branch::isOverloaded() const {
    return isOverloaded(1.0);
}

bool Branch::isOverloaded(double limitReduction) const {
    return checkPermanentLimit1(limitReduction, LimitType::CURRENT) || checkPermanentLimit2(limitReduction, LimitType::CURRENT);
}

}  // namespace iidm

}  // namespace powsybl
