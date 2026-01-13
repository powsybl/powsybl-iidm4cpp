/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ThreeWindingsTransformer.hpp>

#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/util/LimitViolationUtils.hpp>

namespace powsybl {

namespace iidm {

ThreeWindingsTransformer::ThreeWindingsTransformer(const std::string& id, const std::string& name, bool fictitious, Leg&& leg1, Leg&& leg2, Leg&& leg3, double ratedU0) :
    Identifiable(id, name, fictitious),
    m_ratedU0(ratedU0) {
    m_legs.emplace_back(std::move(leg1.setTransformer(*this)));
    m_legs.emplace_back(std::move(leg2.setTransformer(*this)));
    m_legs.emplace_back(std::move(leg3.setTransformer(*this)));
}

void ThreeWindingsTransformer::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Connectable::allocateVariantArrayElement(indexes, sourceIndex);

    for (Leg& leg : m_legs) {
        if (leg.hasRatioTapChanger()) {
            leg.getRatioTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
        }
        if (leg.hasPhaseTapChanger()) {
            leg.getPhaseTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
        }
    }
}

void ThreeWindingsTransformer::deleteVariantArrayElement(unsigned long index) {
    Connectable::deleteVariantArrayElement(index);

    for (Leg& leg : m_legs) {
        if (leg.hasRatioTapChanger()) {
            leg.getRatioTapChanger().deleteVariantArrayElement(index);
        }
        if (leg.hasPhaseTapChanger()) {
            leg.getPhaseTapChanger().deleteVariantArrayElement(index);
        }
    }
}

void ThreeWindingsTransformer::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Connectable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    for (Leg& leg : m_legs) {
        if (leg.hasRatioTapChanger()) {
            leg.getRatioTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
        }
        if (leg.hasPhaseTapChanger()) {
            leg.getPhaseTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
        }
    }
}

const ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg(const ThreeSides& side) const {
    switch (side) {
        case ThreeSides::ONE:
            return getLeg1();
        case ThreeSides::TWO:
            return getLeg2();
        case ThreeSides::THREE:
            return getLeg3();
        case ThreeSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected ThreeSides value: %1%", side));
    }
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg(const ThreeSides& side) {
    const auto& leg = static_cast<const ThreeWindingsTransformer*>(this)->getLeg(side);
    return const_cast<ThreeWindingsTransformer::Leg&>(leg);
}

const ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg1() const {
    return m_legs[0];
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg1() {
    return m_legs[0];
}

const ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg2() const {
    return m_legs[1];
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg2() {
    return m_legs[1];
}

const ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg3() const {
    return m_legs[2];
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg3() {
    return m_legs[2];
}

stdcxx::const_range<ThreeWindingsTransformer::Leg> ThreeWindingsTransformer::getLegs() const {
    return m_legs;
}

stdcxx::range<ThreeWindingsTransformer::Leg> ThreeWindingsTransformer::getLegs() {
    return m_legs;
}

double ThreeWindingsTransformer::getRatedU0() const {
    return m_ratedU0;
}

ThreeWindingsTransformer& ThreeWindingsTransformer::setRatedU0(double ratedU0) {
    m_ratedU0 = checkRatedU(*this, ratedU0);
    return *this;
}

ThreeSides ThreeWindingsTransformer::getSide(const Terminal& terminal) const {
    if (stdcxx::areSame(m_legs[0].getTerminal(), terminal)) {
        return ThreeSides::ONE;
    }
    if (stdcxx::areSame(m_legs[1].getTerminal(), terminal)) {
        return ThreeSides::TWO;
    }
    if (stdcxx::areSame(m_legs[2].getTerminal(), terminal)) {
        return ThreeSides::THREE;
    }
    throw AssertionError("The terminal is not connected to this three windings transformer");
}

stdcxx::CReference<Substation> ThreeWindingsTransformer::getSubstation() const {
    for (const Leg& leg : m_legs) {
        if (leg.getTerminal().getVoltageLevel().getSubstation()) {
            return leg.getTerminal().getVoltageLevel().getSubstation();
        }
    }
    return stdcxx::cref<Substation>();
}

stdcxx::Reference<Substation> ThreeWindingsTransformer::getSubstation() {
    return stdcxx::ref(static_cast<const ThreeWindingsTransformer*>(this)->getSubstation());
}

const Terminal& ThreeWindingsTransformer::getTerminal(const ThreeSides& side) const {
    switch (side) {
        case ThreeSides::ONE:
            return m_legs[0].getTerminal();
        case ThreeSides::TWO:
            return m_legs[1].getTerminal();
        case ThreeSides::THREE:
            return m_legs[2].getTerminal();
        case ThreeSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected ThreeSides value: %1%", side));
    }
}

Terminal& ThreeWindingsTransformer::getTerminal(const ThreeSides& side) {
    const auto& terminal = static_cast<const ThreeWindingsTransformer*>(this)->getTerminal(side);

    return const_cast<Terminal&>(terminal);
}


bool ThreeWindingsTransformer::isOverloaded() const {
    return isOverloaded(1.0);
}

bool ThreeWindingsTransformer::isOverloaded(double limitReduction) const {
    return checkPermanentLimit1(limitReduction, LimitType::CURRENT)
        || checkPermanentLimit2(limitReduction, LimitType::CURRENT)
        || checkPermanentLimit3(limitReduction, LimitType::CURRENT);
}

unsigned long ThreeWindingsTransformer::getOverloadDuration() const {
    std::unique_ptr<Overload> o1 = checkTemporaryLimits1(LimitType::CURRENT);
    std::unique_ptr<Overload> o2 = checkTemporaryLimits2(LimitType::CURRENT);
    std::unique_ptr<Overload> o3 = checkTemporaryLimits3(LimitType::CURRENT);

    unsigned long duration1 = o1 ? o1->getTemporaryLimit().getAcceptableDuration() : std::numeric_limits<unsigned long>::max();
    unsigned long duration2 = o2 ? o2->getTemporaryLimit().getAcceptableDuration() : std::numeric_limits<unsigned long>::max();
    unsigned long duration3 = o3 ? o3->getTemporaryLimit().getAcceptableDuration() : std::numeric_limits<unsigned long>::max();

    return std::min(std::min(duration1, duration2), duration3);
}

bool ThreeWindingsTransformer::checkPermanentLimit(const ThreeSides& side, const LimitType& type) const {
    return checkPermanentLimit(side, 1.0, type);
}

bool ThreeWindingsTransformer::checkPermanentLimit(const ThreeSides& side, double limitReduction, const LimitType& type) const {
    double limitValue = LimitViolationUtils::getValueForLimit(getTerminal(side), type);
    return LimitViolationUtils::checkPermanentLimit(*this, side, limitReduction, limitValue, type);
}

bool ThreeWindingsTransformer::checkPermanentLimit1(const LimitType& type) const {
    return checkPermanentLimit1(1.0, type);
}

bool ThreeWindingsTransformer::checkPermanentLimit1(double limitReduction, const LimitType& type) const {
    return checkPermanentLimit(ThreeSides::ONE, limitReduction, type);
}

bool ThreeWindingsTransformer::checkPermanentLimit2(const LimitType& type) const {
    return checkPermanentLimit2(1.0, type);
}

bool ThreeWindingsTransformer::checkPermanentLimit2(double limitReduction, const LimitType& type) const {
    return checkPermanentLimit(ThreeSides::TWO, limitReduction, type);
}

bool ThreeWindingsTransformer::checkPermanentLimit3(const LimitType& type) const {
    return checkPermanentLimit3(1.0, type);
}

bool ThreeWindingsTransformer::checkPermanentLimit3(double limitReduction, const LimitType& type) const  {
    return checkPermanentLimit(ThreeSides::THREE, limitReduction, type);
}

std::unique_ptr<Overload> ThreeWindingsTransformer::checkTemporaryLimits(const ThreeSides& side, const LimitType& type) const {
    return checkTemporaryLimits(side, 1.0, type);
}

std::unique_ptr<Overload> ThreeWindingsTransformer::checkTemporaryLimits(const ThreeSides& side, double limitReduction, const LimitType& type) const {
    double limitValue = LimitViolationUtils::getValueForLimit(getTerminal(side), type);
    return LimitViolationUtils::checkTemporaryLimits(*this, side, limitReduction, limitValue, type);
}

std::unique_ptr<Overload> ThreeWindingsTransformer::checkTemporaryLimits1(const LimitType& type) const {
    return checkTemporaryLimits1(1.0, type);
}

std::unique_ptr<Overload> ThreeWindingsTransformer::checkTemporaryLimits1(double limitReduction, const LimitType& type) const {
    return checkTemporaryLimits(ThreeSides::ONE, limitReduction, type);
}

std::unique_ptr<Overload> ThreeWindingsTransformer::checkTemporaryLimits2(const LimitType& type) const {
    return checkTemporaryLimits2(1.0, type);
}

std::unique_ptr<Overload> ThreeWindingsTransformer::checkTemporaryLimits2(double limitReduction, const LimitType& type) const {
    return checkTemporaryLimits(ThreeSides::TWO, limitReduction, type);
}

std::unique_ptr<Overload> ThreeWindingsTransformer::checkTemporaryLimits3(const LimitType& type) const {
    return checkTemporaryLimits3(1.0, type);
}

std::unique_ptr<Overload> ThreeWindingsTransformer::checkTemporaryLimits3(double limitReduction, const LimitType& type) const {
    return checkTemporaryLimits(ThreeSides::THREE, limitReduction, type);
}

const IdentifiableType& ThreeWindingsTransformer::getType() const {
    static IdentifiableType s_type = IdentifiableType::THREE_WINDINGS_TRANSFORMER;
    return s_type;
}

const std::string& ThreeWindingsTransformer::getTypeDescription() const {
    static std::string s_typeDescription = "3 windings transformer";

    return s_typeDescription;
}

void ThreeWindingsTransformer::reduceVariantArraySize(unsigned long number) {
    Connectable::reduceVariantArraySize(number);

    for (Leg& leg : m_legs) {
        if (leg.hasRatioTapChanger()) {
            leg.getRatioTapChanger().reduceVariantArraySize(number);
        }
        if (leg.hasPhaseTapChanger()) {
            leg.getPhaseTapChanger().reduceVariantArraySize(number);
        }
    }
}

}  // namespace iidm

}  // namespace powsybl
