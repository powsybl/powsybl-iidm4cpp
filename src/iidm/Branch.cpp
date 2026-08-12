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
#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimits.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/FlowsLimitsHolder.hpp>
#include <powsybl/iidm/OperationalLimitsGroup.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/util/LimitViolationUtils.hpp>
#include <powsybl/iidm/util/LoadingLimitsUtil.hpp>
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

std::vector<std::unique_ptr<Overload>> Branch::checkAllTemporaryLimits(const TwoSides& side, const LimitType& type) const {
    return checkAllTemporaryLimits(side, 1.0, type);
}

std::vector<std::unique_ptr<Overload>> Branch::checkAllTemporaryLimits(const TwoSides& side, double limitReduction, const LimitType& type) const {
    double limitValue = LimitViolationUtils::getValueForLimit(getTerminalFromSide(side), type);
    return LimitViolationUtils::checkAllTemporaryLimits(*this, side, limitReduction, limitValue, type);
}

stdcxx::const_range<OperationalLimitsGroup> Branch::getOperationalLimitsGroups1() const {
    return getOperationalLimitsHolder1().getOperationalLimitsGroups();
}
stdcxx::range<OperationalLimitsGroup> Branch::getOperationalLimitsGroups1() {
    return getOperationalLimitsHolder1().getOperationalLimitsGroups();
}
stdcxx::optional<std::string> Branch::getSelectedOperationalLimitsGroupId1() const {
    return getOperationalLimitsHolder1().getSelectedOperationalLimitsGroupId();
}
const std::list<std::string>& Branch::getAllSelectedOperationalLimitsGroupIds1() const {
    return getOperationalLimitsHolder1().getAllSelectedOperationalLimitsGroupIds();
}
stdcxx::CReference<OperationalLimitsGroup> Branch::getOperationalLimitsGroup1(const std::string& id) const {
    return getOperationalLimitsHolder1().getOperationalLimitsGroup(id);
}
stdcxx::Reference<OperationalLimitsGroup> Branch::getOperationalLimitsGroup1(const std::string& id) {
    return getOperationalLimitsHolder1().getOperationalLimitsGroup(id);
}
stdcxx::CReference<OperationalLimitsGroup> Branch::getSelectedOperationalLimitsGroup1() const {
    return getOperationalLimitsHolder1().getSelectedOperationalLimitsGroup();
}
stdcxx::Reference<OperationalLimitsGroup> Branch::getSelectedOperationalLimitsGroup1() {
    return getOperationalLimitsHolder1().getSelectedOperationalLimitsGroup();
}
stdcxx::const_range<OperationalLimitsGroup> Branch::getAllSelectedOperationalLimitsGroups1() const {
    return getOperationalLimitsHolder1().getAllSelectedOperationalLimitsGroups();
}
stdcxx::range<OperationalLimitsGroup> Branch::getAllSelectedOperationalLimitsGroups1() {
    return getOperationalLimitsHolder1().getAllSelectedOperationalLimitsGroups();
}
OperationalLimitsGroup& Branch::newOperationalLimitsGroup1(const std::string& id) {
    return getOperationalLimitsHolder1().newOperationalLimitsGroup(id);
}
void Branch::setSelectedOperationalLimitsGroup1(const std::string& id) {
    getOperationalLimitsHolder1().setSelectedOperationalLimitsGroup(id);
}
void Branch::addSelectedOperationalLimitsGroups1(const std::list<std::string>& ids) {
    getOperationalLimitsHolder1().addSelectedOperationalLimitsGroups(ids);
}
void Branch::addSelectedOperationalLimitsGroupsByPredicate1(const stdcxx::Predicate<std::string>& operationalLimitsGroupIdPredicate) {
    getOperationalLimitsHolder1().addSelectedOperationalLimitsGroupsByPredicate(operationalLimitsGroupIdPredicate);
}
void Branch::removeOperationalLimitsGroup1(const std::string& id) {
    getOperationalLimitsHolder1().removeOperationalLimitsGroup(id);
}
void Branch::cancelSelectedOperationalLimitsGroup1() {
    getOperationalLimitsHolder1().cancelSelectedOperationalLimitsGroup();
}
void Branch::deselectOperationalLimitsGroups1(const std::list<std::string>& ids) {
    getOperationalLimitsHolder1().deselectOperationalLimitsGroups(ids);
}
OperationalLimitsGroup& Branch::getOrCreateSelectedOperationalLimitsGroup1() {
    return getOperationalLimitsHolder1().getOrCreateSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& Branch::getOrCreateSelectedOperationalLimitsGroup1(const std::string& id) {
    return getOperationalLimitsHolder1().getOrCreateSelectedOperationalLimitsGroup(id);
}

stdcxx::const_range<OperationalLimitsGroup> Branch::getOperationalLimitsGroups2() const {
    return getOperationalLimitsHolder2().getOperationalLimitsGroups();
}
stdcxx::range<OperationalLimitsGroup> Branch::getOperationalLimitsGroups2() {
    return getOperationalLimitsHolder2().getOperationalLimitsGroups();
}
stdcxx::optional<std::string> Branch::getSelectedOperationalLimitsGroupId2() const {
    return getOperationalLimitsHolder2().getSelectedOperationalLimitsGroupId();
}
const std::list<std::string>& Branch::getAllSelectedOperationalLimitsGroupIds2() const {
    return getOperationalLimitsHolder2().getAllSelectedOperationalLimitsGroupIds();
}
stdcxx::CReference<OperationalLimitsGroup> Branch::getOperationalLimitsGroup2(const std::string& id) const {
    return getOperationalLimitsHolder2().getOperationalLimitsGroup(id);
}
stdcxx::Reference<OperationalLimitsGroup> Branch::getOperationalLimitsGroup2(const std::string& id) {
    return getOperationalLimitsHolder2().getOperationalLimitsGroup(id);
}
stdcxx::CReference<OperationalLimitsGroup> Branch::getSelectedOperationalLimitsGroup2() const {
    return getOperationalLimitsHolder2().getSelectedOperationalLimitsGroup();
}
stdcxx::Reference<OperationalLimitsGroup> Branch::getSelectedOperationalLimitsGroup2() {
    return getOperationalLimitsHolder2().getSelectedOperationalLimitsGroup();
}
stdcxx::const_range<OperationalLimitsGroup> Branch::getAllSelectedOperationalLimitsGroups2() const {
    return getOperationalLimitsHolder2().getAllSelectedOperationalLimitsGroups();
}
stdcxx::range<OperationalLimitsGroup> Branch::getAllSelectedOperationalLimitsGroups2() {
    return getOperationalLimitsHolder2().getAllSelectedOperationalLimitsGroups();
}
OperationalLimitsGroup& Branch::newOperationalLimitsGroup2(const std::string& id) {
    return getOperationalLimitsHolder2().newOperationalLimitsGroup(id);
}
void Branch::setSelectedOperationalLimitsGroup2(const std::string& id) {
    getOperationalLimitsHolder2().setSelectedOperationalLimitsGroup(id);
}
void Branch::addSelectedOperationalLimitsGroups2(const std::list<std::string>& ids) {
    getOperationalLimitsHolder2().addSelectedOperationalLimitsGroups(ids);
}
void Branch::addSelectedOperationalLimitsGroupsByPredicate2(const stdcxx::Predicate<std::string>& operationalLimitsGroupIdPredicate) {
    getOperationalLimitsHolder2().addSelectedOperationalLimitsGroupsByPredicate(operationalLimitsGroupIdPredicate);
}
void Branch::removeOperationalLimitsGroup2(const std::string& id) {
    getOperationalLimitsHolder2().removeOperationalLimitsGroup(id);
}
void Branch::cancelSelectedOperationalLimitsGroup2() {
    getOperationalLimitsHolder2().cancelSelectedOperationalLimitsGroup();
}
void Branch::deselectOperationalLimitsGroups2(const std::list<std::string>& ids) {
    getOperationalLimitsHolder2().deselectOperationalLimitsGroups(ids);
}
OperationalLimitsGroup& Branch::getOrCreateSelectedOperationalLimitsGroup2() {
    return getOperationalLimitsHolder2().getOrCreateSelectedOperationalLimitsGroup();
}
OperationalLimitsGroup& Branch::getOrCreateSelectedOperationalLimitsGroup2(const std::string& id) {
    return getOperationalLimitsHolder2().getOrCreateSelectedOperationalLimitsGroup(id);
}


const std::list<std::string>& Branch::getAllSelectedOperationalLimitsGroupIds(const TwoSides& side) const {
    return getOperationalLimitsHolder(side).getAllSelectedOperationalLimitsGroupIds();
}
stdcxx::const_range<OperationalLimitsGroup> Branch::getAllSelectedOperationalLimitsGroups(const TwoSides& side) const {
    return getOperationalLimitsHolder(side).getAllSelectedOperationalLimitsGroups();
}
stdcxx::range<OperationalLimitsGroup> Branch::getAllSelectedOperationalLimitsGroups(const TwoSides& side) {
    return getOperationalLimitsHolder(side).getAllSelectedOperationalLimitsGroups();
}
void Branch::addSelectedOperationalLimitsGroups(const TwoSides& side, const std::list<std::string>& ids) {
    return getOperationalLimitsHolder(side).addSelectedOperationalLimitsGroups(ids);
}
void Branch::addSelectedOperationalLimitsGroupsByPredicate(const TwoSides& side, const stdcxx::Predicate<std::string>& operationalLimitsGroupIdPredicate) {
    return getOperationalLimitsHolder(side).addSelectedOperationalLimitsGroupsByPredicate(operationalLimitsGroupIdPredicate);
}
void Branch::deselectOperationalLimitsGroups(const TwoSides& side, const std::list<std::string>& ids) {
    return getOperationalLimitsHolder(side).deselectOperationalLimitsGroups(ids);
}


stdcxx::CReference<ActivePowerLimits> Branch::getActivePowerLimits(const TwoSides& side) const {
    return getOperationalLimitsHolder(side).getActivePowerLimits();
}

stdcxx::Reference<ActivePowerLimits> Branch::getActivePowerLimits(const TwoSides& side) {
    return getOperationalLimitsHolder(side).getActivePowerLimits();
}

stdcxx::CReference<ActivePowerLimits> Branch::getActivePowerLimits1() const {
    return getOperationalLimitsHolder1().getActivePowerLimits();
}
stdcxx::Reference<ActivePowerLimits> Branch::getActivePowerLimits1() {
    return getOperationalLimitsHolder1().getActivePowerLimits();
}

stdcxx::CReference<ActivePowerLimits> Branch::getActivePowerLimits2() const {
    return getOperationalLimitsHolder2().getActivePowerLimits();
}
stdcxx::Reference<ActivePowerLimits> Branch::getActivePowerLimits2() {
    return getOperationalLimitsHolder2().getActivePowerLimits();
}

stdcxx::const_range<ActivePowerLimits> Branch::getAllSelectedActivePowerLimits(const TwoSides& side) const {
    return getOperationalLimitsHolder(side).getAllSelectedActivePowerLimits();
}
stdcxx::range<ActivePowerLimits> Branch::getAllSelectedActivePowerLimits(const TwoSides& side) {
    return getOperationalLimitsHolder(side).getAllSelectedActivePowerLimits();
}
stdcxx::const_range<ActivePowerLimits> Branch::getAllSelectedActivePowerLimits1() const {
    return getOperationalLimitsHolder1().getAllSelectedActivePowerLimits();
}
stdcxx::range<ActivePowerLimits> Branch::getAllSelectedActivePowerLimits1() {
    return getOperationalLimitsHolder1().getAllSelectedActivePowerLimits();
}
stdcxx::const_range<ActivePowerLimits> Branch::getAllSelectedActivePowerLimits2() const {
    return getOperationalLimitsHolder2().getAllSelectedActivePowerLimits();
}
stdcxx::range<ActivePowerLimits> Branch::getAllSelectedActivePowerLimits2() {
    return getOperationalLimitsHolder2().getAllSelectedActivePowerLimits();
}

stdcxx::CReference<ApparentPowerLimits> Branch::getApparentPowerLimits(const TwoSides& side) const {
    return getOperationalLimitsHolder(side).getApparentPowerLimits();
}

stdcxx::Reference<ApparentPowerLimits> Branch::getApparentPowerLimits(const TwoSides& side) {
    return getOperationalLimitsHolder(side).getApparentPowerLimits();
}

stdcxx::CReference<ApparentPowerLimits> Branch::getApparentPowerLimits1() const {
    return getOperationalLimitsHolder1().getApparentPowerLimits();
}
stdcxx::Reference<ApparentPowerLimits> Branch::getApparentPowerLimits1() {
    return getOperationalLimitsHolder1().getApparentPowerLimits();
}

stdcxx::CReference<ApparentPowerLimits> Branch::getApparentPowerLimits2() const {
    return getOperationalLimitsHolder2().getApparentPowerLimits();
}
stdcxx::Reference<ApparentPowerLimits> Branch::getApparentPowerLimits2() {
    return getOperationalLimitsHolder2().getApparentPowerLimits();
}

stdcxx::const_range<ApparentPowerLimits> Branch::getAllSelectedApparentPowerLimits(const TwoSides& side) const {
    return getOperationalLimitsHolder(side).getAllSelectedApparentPowerLimits();
}
stdcxx::range<ApparentPowerLimits> Branch::getAllSelectedApparentPowerLimits(const TwoSides& side) {
    return getOperationalLimitsHolder(side).getAllSelectedApparentPowerLimits();
}
stdcxx::const_range<ApparentPowerLimits> Branch::getAllSelectedApparentPowerLimits1() const {
    return getOperationalLimitsHolder1().getAllSelectedApparentPowerLimits();
}
stdcxx::range<ApparentPowerLimits> Branch::getAllSelectedApparentPowerLimits1() {
    return getOperationalLimitsHolder1().getAllSelectedApparentPowerLimits();
}
stdcxx::const_range<ApparentPowerLimits> Branch::getAllSelectedApparentPowerLimits2() const {
    return getOperationalLimitsHolder2().getAllSelectedApparentPowerLimits();
}
stdcxx::range<ApparentPowerLimits> Branch::getAllSelectedApparentPowerLimits2() {
    return getOperationalLimitsHolder2().getAllSelectedApparentPowerLimits();
}

stdcxx::CReference<CurrentLimits> Branch::getCurrentLimits(const TwoSides& side) const {
    return getOperationalLimitsHolder(side).getCurrentLimits();
}

stdcxx::Reference<CurrentLimits> Branch::getCurrentLimits(const TwoSides& side) {
    return getOperationalLimitsHolder(side).getCurrentLimits();
}

stdcxx::CReference<CurrentLimits> Branch::getCurrentLimits1() const {
    return getOperationalLimitsHolder1().getCurrentLimits();
}
stdcxx::Reference<CurrentLimits> Branch::getCurrentLimits1() {
    return getOperationalLimitsHolder1().getCurrentLimits();
}

stdcxx::CReference<CurrentLimits> Branch::getCurrentLimits2() const {
    return getOperationalLimitsHolder2().getCurrentLimits();
}
stdcxx::Reference<CurrentLimits> Branch::getCurrentLimits2() {
    return getOperationalLimitsHolder2().getCurrentLimits();
}

stdcxx::const_range<CurrentLimits> Branch::getAllSelectedCurrentLimits(const TwoSides& side) const {
    return getOperationalLimitsHolder(side).getAllSelectedCurrentLimits();
}
stdcxx::range<CurrentLimits> Branch::getAllSelectedCurrentLimits(const TwoSides& side) {
    return getOperationalLimitsHolder(side).getAllSelectedCurrentLimits();
}
stdcxx::const_range<CurrentLimits> Branch::getAllSelectedCurrentLimits1() const {
    return getOperationalLimitsHolder1().getAllSelectedCurrentLimits();
}
stdcxx::range<CurrentLimits> Branch::getAllSelectedCurrentLimits1() {
    return getOperationalLimitsHolder1().getAllSelectedCurrentLimits();
}
stdcxx::const_range<CurrentLimits> Branch::getAllSelectedCurrentLimits2() const {
    return getOperationalLimitsHolder2().getAllSelectedCurrentLimits();
}
stdcxx::range<CurrentLimits> Branch::getAllSelectedCurrentLimits2() {
    return getOperationalLimitsHolder2().getAllSelectedCurrentLimits();
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

stdcxx::const_range<LoadingLimits> Branch::getAllSelectedLoadingLimits(const LimitType& type, const TwoSides& side) const {
    return getOperationalLimitsHolder(side).getAllSelectedLoadingLimits(type);
}
stdcxx::range<LoadingLimits> Branch::getAllSelectedLoadingLimits(const LimitType& type, const TwoSides& side) {
    return getOperationalLimitsHolder(side).getAllSelectedLoadingLimits(type);
}

unsigned long Branch::getOverloadDuration() const {
    std::vector<std::unique_ptr<Overload>> currentOverloads1 = checkAllTemporaryLimits(TwoSides::ONE, LimitType::CURRENT);
    std::vector<std::unique_ptr<Overload>> currentOverloads2 = checkAllTemporaryLimits(TwoSides::TWO, LimitType::CURRENT);

    unsigned long minDuration = std::numeric_limits<unsigned long>::max();
    for (const auto& overload : currentOverloads1) {
        minDuration = std::min(minDuration, overload->getTemporaryLimit().getAcceptableDuration());
    }
    for (const auto& overload : currentOverloads2) {
        minDuration = std::min(minDuration, overload->getTemporaryLimit().getAcceptableDuration());
    }
    return minDuration;
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


ActivePowerLimitsAdder Branch::newActivePowerLimits1(const ActivePowerLimits& activePowerLimits) {
    ActivePowerLimitsAdder adder = newActivePowerLimits1();
    LoadingLimitsUtil::initializeFromLoadingLimits(adder, activePowerLimits);
    return adder;
}

ActivePowerLimitsAdder Branch::newActivePowerLimits2(const ActivePowerLimits& activePowerLimits) {
    ActivePowerLimitsAdder adder = newActivePowerLimits2();
    LoadingLimitsUtil::initializeFromLoadingLimits(adder, activePowerLimits);
    return adder;
}

ApparentPowerLimitsAdder Branch::newApparentPowerLimits1(const ApparentPowerLimits& apparentPowerLimits) {
    ApparentPowerLimitsAdder adder = newApparentPowerLimits1();
    LoadingLimitsUtil::initializeFromLoadingLimits(adder, apparentPowerLimits);
    return adder;
}

ApparentPowerLimitsAdder Branch::newApparentPowerLimits2(const ApparentPowerLimits& apparentPowerLimits) {
    ApparentPowerLimitsAdder adder = newApparentPowerLimits2();
    LoadingLimitsUtil::initializeFromLoadingLimits(adder, apparentPowerLimits);
    return adder;
}

CurrentLimitsAdder Branch::newCurrentLimits1(const CurrentLimits& currentLimits) {
    CurrentLimitsAdder adder = newCurrentLimits1();
    LoadingLimitsUtil::initializeFromLoadingLimits(adder, currentLimits);
    return adder;
}

CurrentLimitsAdder Branch::newCurrentLimits2(const CurrentLimits& currentLimits) {
    CurrentLimitsAdder adder = newCurrentLimits2();
    LoadingLimitsUtil::initializeFromLoadingLimits(adder, currentLimits);
    return adder;
}


const FlowsLimitsHolder& Branch::getOperationalLimitsHolder(const TwoSides& side) const {
    switch (side) {
        case TwoSides::ONE:
            return getOperationalLimitsHolder1();
        case TwoSides::TWO:
            return getOperationalLimitsHolder2();
        case TwoSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected TwoSides value: %1%", side));
    }
}
FlowsLimitsHolder& Branch::getOperationalLimitsHolder(const TwoSides& side) {
    switch (side) {
        case TwoSides::ONE:
            return getOperationalLimitsHolder1();
        case TwoSides::TWO:
            return getOperationalLimitsHolder2();
        case TwoSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected TwoSides value: %1%", side));
    }
}

}  // namespace iidm

}  // namespace powsybl
