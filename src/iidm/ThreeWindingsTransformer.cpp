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
    return m_transformer.get().getNetwork();
}

Network& ThreeWindingsTransformer::Leg::getNetwork() {
    return m_transformer.get().getNetwork();
}

const PhaseTapChanger& ThreeWindingsTransformer::Leg::getPhaseTapChanger() const {
    if (!m_phaseTapChanger) {
        throw PowsyblException("Phase tap changer not set");
    }
    return *m_phaseTapChanger;
}

PhaseTapChanger& ThreeWindingsTransformer::Leg::getPhaseTapChanger() {
    if (!m_phaseTapChanger) {
        throw PowsyblException("Phase tap changer not set");
    }
    return *m_phaseTapChanger;
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

const RatioTapChanger& ThreeWindingsTransformer::Leg::getRatioTapChanger() const {
    if (!m_ratioTapChanger) {
        throw PowsyblException("Ratio tap changer not set");
    }
    return *m_ratioTapChanger;
}

RatioTapChanger& ThreeWindingsTransformer::Leg::getRatioTapChanger() {
    if (!m_ratioTapChanger) {
        throw PowsyblException("Ratio tap changer not set");
    }
    return *m_ratioTapChanger;
}

unsigned long ThreeWindingsTransformer::Leg::getRegulatingTapChangerCount() const {
    unsigned long count = 0;

    const ThreeWindingsTransformer::Leg& leg1 = m_transformer.get().getLeg1();
    count += (leg1.hasPhaseTapChanger() && leg1.getPhaseTapChanger().isRegulating()) ? 1 : 0;
    count += (leg1.hasRatioTapChanger() && leg1.getRatioTapChanger().isRegulating()) ? 1 : 0;

    const ThreeWindingsTransformer::Leg& leg2 = m_transformer.get().getLeg2();
    count += (leg2.hasPhaseTapChanger() && leg2.getPhaseTapChanger().isRegulating()) ? 1 : 0;
    count += (leg2.hasRatioTapChanger() && leg2.getRatioTapChanger().isRegulating()) ? 1 : 0;

    const ThreeWindingsTransformer::Leg& leg3 = m_transformer.get().getLeg3();
    count += (leg3.hasPhaseTapChanger() && leg3.getPhaseTapChanger().isRegulating()) ? 1 : 0;
    count += (leg3.hasRatioTapChanger() && leg3.getRatioTapChanger().isRegulating()) ? 1 : 0;

    return count;
}

const Terminal& ThreeWindingsTransformer::Leg::getTerminal() const {
    return m_transformer.get().Connectable::getTerminal(m_legNumber - 1);
}

Terminal& ThreeWindingsTransformer::Leg::getTerminal() {
    return m_transformer.get().Connectable::getTerminal(m_legNumber - 1);
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

ThreeWindingsTransformer::ThreeWindingsTransformer(const std::string& id, const std::string& name, bool fictitious, Leg&& leg1, Leg&& leg2, Leg&& leg3, double ratedU0) :
    Connectable(id, name, fictitious, ConnectableType::THREE_WINDINGS_TRANSFORMER),
    m_leg1(std::move(leg1)),
    m_leg2(std::move(leg2)),
    m_leg3(std::move(leg3)),
    m_ratedU0(ratedU0) {
}

void ThreeWindingsTransformer::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Connectable::allocateVariantArrayElement(indexes, sourceIndex);

    if (m_leg1.hasRatioTapChanger()) {
        m_leg1.getRatioTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_leg1.hasPhaseTapChanger()) {
        m_leg1.getPhaseTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_leg2.hasRatioTapChanger()) {
        m_leg2.getRatioTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_leg2.hasPhaseTapChanger()) {
        m_leg2.getPhaseTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_leg3.hasRatioTapChanger()) {
        m_leg3.getRatioTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_leg3.hasPhaseTapChanger()) {
        m_leg3.getPhaseTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
    }
}

void ThreeWindingsTransformer::deleteVariantArrayElement(unsigned long index) {
    Connectable::deleteVariantArrayElement(index);

    if (m_leg1.hasRatioTapChanger()) {
        m_leg1.getRatioTapChanger().deleteVariantArrayElement(index);
    }
    if (m_leg1.hasPhaseTapChanger()) {
        m_leg1.getPhaseTapChanger().deleteVariantArrayElement(index);
    }
    if (m_leg2.hasRatioTapChanger()) {
        m_leg2.getRatioTapChanger().deleteVariantArrayElement(index);
    }
    if (m_leg2.hasPhaseTapChanger()) {
        m_leg2.getPhaseTapChanger().deleteVariantArrayElement(index);
    }
    if (m_leg3.hasRatioTapChanger()) {
        m_leg3.getRatioTapChanger().deleteVariantArrayElement(index);
    }
    if (m_leg3.hasPhaseTapChanger()) {
        m_leg3.getPhaseTapChanger().deleteVariantArrayElement(index);
    }
}

void ThreeWindingsTransformer::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Connectable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    if (m_leg1.hasRatioTapChanger()) {
        m_leg1.getRatioTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_leg1.hasPhaseTapChanger()) {
        m_leg1.getPhaseTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_leg2.hasRatioTapChanger()) {
        m_leg2.getRatioTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_leg2.hasPhaseTapChanger()) {
        m_leg2.getPhaseTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_leg3.hasRatioTapChanger()) {
        m_leg3.getRatioTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_leg3.hasPhaseTapChanger()) {
        m_leg3.getPhaseTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
}

const ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg1() const {
    return m_leg1;
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg1() {
    return m_leg1;
}

const ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg2() const {
    return m_leg2;
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg2() {
    return m_leg2;
}

const ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg3() const {
    return m_leg3;
}

ThreeWindingsTransformer::Leg& ThreeWindingsTransformer::getLeg3() {
    return m_leg3;
}

double ThreeWindingsTransformer::getRatedU0() const {
    return m_ratedU0;
}

ThreeWindingsTransformer::Side ThreeWindingsTransformer::getSide(const Terminal& terminal) const {
    if (stdcxx::areSame(m_leg1.getTerminal(), terminal)) {
        return Side::ONE;
    }
    if (stdcxx::areSame(m_leg2.getTerminal(), terminal)) {
        return Side::TWO;
    }
    if (stdcxx::areSame(m_leg3.getTerminal(), terminal)) {
        return Side::THREE;
    }
    throw AssertionError("The terminal is not connected to this three windings transformer");
}

const Substation& ThreeWindingsTransformer::getSubstation() const {
    return m_leg1.getTerminal().getVoltageLevel().getSubstation();
}

Substation& ThreeWindingsTransformer::getSubstation() {
    return m_leg1.getTerminal().getVoltageLevel().getSubstation();
}

const Terminal& ThreeWindingsTransformer::getTerminal(const Side& side) const {
    switch (side) {
        case Side::ONE:
            return m_leg1.getTerminal();
        case Side::TWO:
            return m_leg2.getTerminal();
        case Side::THREE:
            return m_leg3.getTerminal();
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

    if (m_leg1.hasRatioTapChanger()) {
        m_leg1.getRatioTapChanger().reduceVariantArraySize(number);
    }
    if (m_leg1.hasPhaseTapChanger()) {
        m_leg1.getPhaseTapChanger().reduceVariantArraySize(number);
    }
    if (m_leg2.hasRatioTapChanger()) {
        m_leg2.getRatioTapChanger().reduceVariantArraySize(number);
    }
    if (m_leg2.hasPhaseTapChanger()) {
        m_leg2.getPhaseTapChanger().reduceVariantArraySize(number);
    }
    if (m_leg3.hasRatioTapChanger()) {
        m_leg3.getRatioTapChanger().reduceVariantArraySize(number);
    }
    if (m_leg3.hasPhaseTapChanger()) {
        m_leg3.getPhaseTapChanger().reduceVariantArraySize(number);
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
