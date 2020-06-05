/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ThreeWindingsTransformer.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

ThreeWindingsTransformer::Leg::Leg(unsigned long legNumber, double r, double x, double g, double b, double ratedU, double ratedS) :
    m_legNumber(legNumber),
    m_r(checkR(*this, r)),
    m_x(checkX(*this, x)),
    m_g(checkG(*this, g)),
    m_b(checkB(*this, b)),
    m_ratedU(checkRatedU(*this, ratedU, legNumber)),
    m_ratedS(checkRatedS(*this, ratedS)) {
}

double ThreeWindingsTransformer::Leg::getB() const {
    return m_b;
}

stdcxx::CReference<CurrentLimits> ThreeWindingsTransformer::Leg::getCurrentLimits() const {
    return stdcxx::cref(m_limits);
}

stdcxx::Reference<CurrentLimits> ThreeWindingsTransformer::Leg::getCurrentLimits() {
    return stdcxx::ref<CurrentLimits>(m_limits);
}

double ThreeWindingsTransformer::Leg::getG() const {
    return m_g;
}

std::string ThreeWindingsTransformer::Leg::getMessageHeader() const {
    return stdcxx::format("%1% '%2%': ", getTypeDescription(), m_transformer.get().getId());
}

const Network& ThreeWindingsTransformer::Leg::getNetwork() const {
    return m_transformer.get().getSubstation().getNetwork();
}

Network& ThreeWindingsTransformer::Leg::getNetwork() {
    return m_transformer.get().getSubstation().getNetwork();
}

stdcxx::CReference<PhaseTapChanger> ThreeWindingsTransformer::Leg::getPhaseTapChanger() const {
    return stdcxx::cref(m_phaseTapChanger);
}

stdcxx::Reference<PhaseTapChanger> ThreeWindingsTransformer::Leg::getPhaseTapChanger() {
    return stdcxx::ref<PhaseTapChanger>(m_phaseTapChanger);
}

double ThreeWindingsTransformer::Leg::getR() const {
    return m_r;
}

double ThreeWindingsTransformer::Leg::getRatedS() const {
    return m_ratedS;
}

double ThreeWindingsTransformer::Leg::getRatedU() const {
    return m_ratedU;
}

stdcxx::CReference<RatioTapChanger> ThreeWindingsTransformer::Leg::getRatioTapChanger() const {
    return stdcxx::cref(m_ratioTapChanger);
}

stdcxx::Reference<RatioTapChanger> ThreeWindingsTransformer::Leg::getRatioTapChanger() {
    return stdcxx::ref<RatioTapChanger>(m_ratioTapChanger);
}

unsigned long ThreeWindingsTransformer::Leg::getRegulatingTapChangerCount() const {
    unsigned long count = 0;

    const ThreeWindingsTransformer::Leg& leg1 = m_transformer.get().getLeg1();
    count += (leg1.hasPhaseTapChanger() && leg1.getPhaseTapChanger().get().isRegulating()) ? 1 : 0;
    count += (leg1.hasRatioTapChanger() && leg1.getRatioTapChanger().get().isRegulating()) ? 1 : 0;

    const ThreeWindingsTransformer::Leg& leg2 = m_transformer.get().getLeg2();
    count += (leg2.hasPhaseTapChanger() && leg2.getPhaseTapChanger().get().isRegulating()) ? 1 : 0;
    count += (leg2.hasRatioTapChanger() && leg2.getRatioTapChanger().get().isRegulating()) ? 1 : 0;

    const ThreeWindingsTransformer::Leg& leg3 = m_transformer.get().getLeg3();
    count += (leg3.hasPhaseTapChanger() && leg3.getPhaseTapChanger().get().isRegulating()) ? 1 : 0;
    count += (leg3.hasRatioTapChanger() && leg3.getRatioTapChanger().get().isRegulating()) ? 1 : 0;

    return count;
}

stdcxx::CReference<Terminal> ThreeWindingsTransformer::Leg::getTerminal() const {
    if (m_transformer) {
        return stdcxx::cref<Terminal>(m_transformer.get().Connectable::getTerminal(m_legNumber - 1));
    }
    return {};
}

stdcxx::Reference<Terminal> ThreeWindingsTransformer::Leg::getTerminal() {
    return stdcxx::ref(static_cast<const ThreeWindingsTransformer::Leg*>(this)->getTerminal());
}

const std::string& ThreeWindingsTransformer::Leg::getTypeDescription() const {
    static std::array<std::string, 3> s_typeDescriptions {{
        u8"3 windings transformer leg1",
        u8"3 windings transformer leg2",
        u8"3 windings transformer leg3"
    }};

    return s_typeDescriptions[m_legNumber - 1];
}

double ThreeWindingsTransformer::Leg::getX() const {
    return m_x;
}

bool ThreeWindingsTransformer::Leg::hasPhaseTapChanger() const {
    return static_cast<bool>(m_phaseTapChanger);
}

bool ThreeWindingsTransformer::Leg::hasRatioTapChanger() const {
    return static_cast<bool>(m_ratioTapChanger);
}

CurrentLimitsAdder<const std::nullptr_t, ThreeWindingsTransformer::Leg> ThreeWindingsTransformer::Leg::newCurrentLimits() {
    return CurrentLimitsAdder<const std::nullptr_t, Leg>(nullptr, *this);
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::Leg::setB(double b) {
    m_b = checkB(*this, b);
    return *this;
}

void ThreeWindingsTransformer::Leg::setCurrentLimits(std::nullptr_t /*side*/, std::unique_ptr<CurrentLimits> limits) {
    m_limits = std::move(limits);
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::Leg::setG(double g) {
    m_g = checkG(*this, g);
    return *this;
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::Leg::setR(double r) {
    m_r = checkR(*this, r);
    return *this;
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::Leg::setRatedS(double ratedS) {
    m_ratedS = checkRatedS(*this, ratedS);
    return *this;
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::Leg::setRatedU(double ratedU) {
    m_ratedU = checkRatedU(*this, ratedU);
    return *this;
}

PhaseTapChangerAdder ThreeWindingsTransformer::Leg::newPhaseTapChanger() {
    return PhaseTapChangerAdder(*this);
}

RatioTapChangerAdder ThreeWindingsTransformer::Leg::newRatioTapChanger() {
    return RatioTapChangerAdder(*this);
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::Leg::setX(double x) {
    m_x = checkX(*this, x);
    return *this;
}

void ThreeWindingsTransformer::Leg::setPhaseTapChanger(std::unique_ptr<PhaseTapChanger> phaseTapChanger) {
    m_phaseTapChanger = std::move(phaseTapChanger);
}

void ThreeWindingsTransformer::Leg::setRatioTapChanger(std::unique_ptr<RatioTapChanger> ratioTapChanger) {
    m_ratioTapChanger = std::move(ratioTapChanger);
}

void ThreeWindingsTransformer::Leg::setTransformer(ThreeWindingsTransformer& transformer) {
    m_transformer = stdcxx::ref(transformer);
}

std::string ThreeWindingsTransformer::Leg::toString() const {
    return stdcxx::format("%1% leg%2%", m_transformer.get().getId(), m_legNumber);
}

ThreeWindingsTransformer::ThreeWindingsTransformer(const std::string& id, const std::string& name, std::unique_ptr<Leg> leg1, std::unique_ptr<Leg> leg2, std::unique_ptr<Leg> leg3, double ratedU0) :
    Connectable(id, name, ConnectableType::THREE_WINDINGS_TRANSFORMER),
    m_leg1(std::move(leg1)),
    m_leg2(std::move(leg2)),
    m_leg3(std::move(leg3)),
    m_ratedU0(ratedU0) {
}

void ThreeWindingsTransformer::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Connectable::allocateVariantArrayElement(indexes, sourceIndex);

    if (m_leg1->getRatioTapChanger()) {
        m_leg1->getRatioTapChanger().get().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_leg1->getPhaseTapChanger()) {
        m_leg1->getPhaseTapChanger().get().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_leg2->getRatioTapChanger()) {
        m_leg2->getRatioTapChanger().get().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_leg2->getPhaseTapChanger()) {
        m_leg2->getPhaseTapChanger().get().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_leg3->getRatioTapChanger()) {
        m_leg3->getRatioTapChanger().get().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_leg3->getPhaseTapChanger()) {
        m_leg3->getPhaseTapChanger().get().allocateVariantArrayElement(indexes, sourceIndex);
    }
}

void ThreeWindingsTransformer::deleteVariantArrayElement(unsigned long index) {
    Connectable::deleteVariantArrayElement(index);

    if (m_leg1->getRatioTapChanger()) {
        m_leg1->getRatioTapChanger().get().deleteVariantArrayElement(index);
    }
    if (m_leg1->getPhaseTapChanger()) {
        m_leg1->getPhaseTapChanger().get().deleteVariantArrayElement(index);
    }
    if (m_leg2->getRatioTapChanger()) {
        m_leg2->getRatioTapChanger().get().deleteVariantArrayElement(index);
    }
    if (m_leg2->getPhaseTapChanger()) {
        m_leg2->getPhaseTapChanger().get().deleteVariantArrayElement(index);
    }
    if (m_leg3->getRatioTapChanger()) {
        m_leg3->getRatioTapChanger().get().deleteVariantArrayElement(index);
    }
    if (m_leg3->getPhaseTapChanger()) {
        m_leg3->getPhaseTapChanger().get().deleteVariantArrayElement(index);
    }
}

void ThreeWindingsTransformer::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Connectable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    if (m_leg1->getRatioTapChanger()) {
        m_leg1->getRatioTapChanger().get().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_leg1->getPhaseTapChanger()) {
        m_leg1->getPhaseTapChanger().get().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_leg2->getRatioTapChanger()) {
        m_leg2->getRatioTapChanger().get().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_leg2->getPhaseTapChanger()) {
        m_leg2->getPhaseTapChanger().get().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_leg3->getRatioTapChanger()) {
        m_leg3->getRatioTapChanger().get().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_leg3->getPhaseTapChanger()) {
        m_leg3->getPhaseTapChanger().get().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
}

const ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg1() const {
    return *m_leg1;
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg1() {
    return *m_leg1;
}

const ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg2() const {
    return *m_leg2;
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg2() {
    return *m_leg2;
}

const ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg3() const {
    return *m_leg3;
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg3() {
    return *m_leg3;
}

double ThreeWindingsTransformer::getRatedU0() const {
    return m_ratedU0;
}

ThreeWindingsTransformer::Side ThreeWindingsTransformer::getSide(const Terminal& terminal) const {
    if (stdcxx::areSame(m_leg1->getTerminal().get(), terminal)) {
        return Side::ONE;
    }
    if (stdcxx::areSame(m_leg2->getTerminal().get(), terminal)) {
        return Side::TWO;
    }
    if (stdcxx::areSame(m_leg3->getTerminal().get(), terminal)) {
        return Side::THREE;
    }
    throw AssertionError("The terminal is not connected to this three windings transformer");
}

const Substation& ThreeWindingsTransformer::getSubstation() const {
    return m_leg1->getTerminal().get().getVoltageLevel().getSubstation();
}

Substation& ThreeWindingsTransformer::getSubstation() {
    return m_leg1->getTerminal().get().getVoltageLevel().getSubstation();
}

const Terminal& ThreeWindingsTransformer::getTerminal(const Side& side) const {
    switch (side) {
        case Side::ONE:
            return m_leg1->getTerminal().get();
        case Side::TWO:
            return m_leg2->getTerminal().get();
        case Side::THREE:
            return m_leg3->getTerminal().get();
        default:
            throw AssertionError(stdcxx::format("Unexpected side value: %1%", side));
    }
}

Terminal& ThreeWindingsTransformer::getTerminal(const Side& side) {
    const auto& terminal = static_cast<const ThreeWindingsTransformer*>(this)->getTerminal(side);

    return const_cast<Terminal&>(terminal);
}

const std::string& ThreeWindingsTransformer::getTypeDescription() const {
    static std::string s_typeDescription = "3 windings transformer";

    return s_typeDescription;
}

void ThreeWindingsTransformer::reduceVariantArraySize(unsigned long number) {
    Connectable::reduceVariantArraySize(number);

    if (m_leg1->getRatioTapChanger()) {
        m_leg1->getRatioTapChanger().get().reduceVariantArraySize(number);
    }
    if (m_leg1->getPhaseTapChanger()) {
        m_leg1->getPhaseTapChanger().get().reduceVariantArraySize(number);
    }
    if (m_leg2->getRatioTapChanger()) {
        m_leg2->getRatioTapChanger().get().reduceVariantArraySize(number);
    }
    if (m_leg2->getPhaseTapChanger()) {
        m_leg2->getPhaseTapChanger().get().reduceVariantArraySize(number);
    }
    if (m_leg3->getRatioTapChanger()) {
        m_leg3->getRatioTapChanger().get().reduceVariantArraySize(number);
    }
    if (m_leg3->getPhaseTapChanger()) {
        m_leg3->getPhaseTapChanger().get().reduceVariantArraySize(number);
    }
}

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<ThreeWindingsTransformer::Side>() {
    static std::initializer_list<std::string> s_threeWindingsTransformerSideNames {
        "ONE",
        "TWO",
        "THREE"
    };
    return s_threeWindingsTransformerSideNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
