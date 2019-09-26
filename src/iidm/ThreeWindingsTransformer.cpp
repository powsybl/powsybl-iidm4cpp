/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ThreeWindingsTransformer.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/Substation.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

ThreeWindingsTransformer::Leg1::Leg1(double r, double x, double g, double b, double ratedU) :
    LegBase(r, x, ratedU),
    m_g(checkG(*this, g)),
    m_b(checkB(*this, b)) {
}

double ThreeWindingsTransformer::Leg1::getB() const {
    return m_b;
}

double ThreeWindingsTransformer::Leg1::getG() const {
    return m_g;
}

const std::string& ThreeWindingsTransformer::Leg1::getTypeDescription() const {
    static std::string s_typeDescription = "3 windings transformer leg 1";

    return s_typeDescription;
}

ThreeWindingsTransformer::Leg1& ThreeWindingsTransformer::Leg1::setB(double b) {
    m_b = checkB(*this, b);

    return *this;
}

ThreeWindingsTransformer::Leg1& ThreeWindingsTransformer::Leg1::setG(double g) {
    m_g = checkG(*this, g);

    return *this;
}

std::string ThreeWindingsTransformer::Leg1::toString() const {
    return logging::format("%1% leg 1", getRefTransformer().get().getId());
}

ThreeWindingsTransformer::Leg2or3::Leg2or3(double r, double x, double ratedU) :
    LegBase(r, x, ratedU) {
}

const Network& ThreeWindingsTransformer::Leg2or3::getNetwork() const {
    return getRefTransformer().get().getSubstation().getNetwork();
}

Network& ThreeWindingsTransformer::Leg2or3::getNetwork() {
    return getRefTransformer().get().getSubstation().getNetwork();
}

stdcxx::CReference<RatioTapChanger> ThreeWindingsTransformer::Leg2or3::getRatioTapChanger() const {
    return stdcxx::cref(m_ratioTapChanger);
}

stdcxx::Reference<RatioTapChanger> ThreeWindingsTransformer::Leg2or3::getRatioTapChanger() {
    return stdcxx::ref<RatioTapChanger>(m_ratioTapChanger);
}

const Identifiable& ThreeWindingsTransformer::Leg2or3::getTransformer() const {
    return getRefTransformer().get();
}

RatioTapChangerAdder ThreeWindingsTransformer::Leg2or3::newRatioTapChanger() {
    return RatioTapChangerAdder(*this);
}

void ThreeWindingsTransformer::Leg2or3::setRatioTapChanger(std::unique_ptr<RatioTapChanger> ratioTapChanger) {
    m_ratioTapChanger = std::move(ratioTapChanger);
}

ThreeWindingsTransformer::ThreeWindingsTransformer(const std::string& id, const std::string& name, std::unique_ptr<Leg1> leg1, std::unique_ptr<Leg2or3> leg2, std::unique_ptr<Leg2or3> leg3) :
    Connectable(id, name, ConnectableType::THREE_WINDINGS_TRANSFORMER),
    m_leg1(std::move(leg1)),
    m_leg2(std::move(leg2)),
    m_leg3(std::move(leg3)) {
}

void ThreeWindingsTransformer::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Connectable::allocateVariantArrayElement(indexes, sourceIndex);

    if (m_leg2->getRatioTapChanger()) {
        m_leg2->getRatioTapChanger().get().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_leg3->getRatioTapChanger()) {
        m_leg3->getRatioTapChanger().get().allocateVariantArrayElement(indexes, sourceIndex);
    }
}

void ThreeWindingsTransformer::deleteVariantArrayElement(unsigned long index) {
    Connectable::deleteVariantArrayElement(index);

    if (m_leg2->getRatioTapChanger()) {
        m_leg2->getRatioTapChanger().get().deleteVariantArrayElement(index);
    }
    if (m_leg3->getRatioTapChanger()) {
        m_leg3->getRatioTapChanger().get().deleteVariantArrayElement(index);
    }
}

void ThreeWindingsTransformer::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Connectable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    if (m_leg2->getRatioTapChanger()) {
        m_leg2->getRatioTapChanger().get().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_leg3->getRatioTapChanger()) {
        m_leg3->getRatioTapChanger().get().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
}

const ThreeWindingsTransformer::Leg1& ThreeWindingsTransformer::getLeg1() const {
    return *m_leg1;
}

ThreeWindingsTransformer::Leg1& ThreeWindingsTransformer::getLeg1() {
    return *m_leg1;
}

const ThreeWindingsTransformer::Leg2or3& ThreeWindingsTransformer::getLeg2() const {
    return *m_leg2;
}

ThreeWindingsTransformer::Leg2or3& ThreeWindingsTransformer::getLeg2() {
    return *m_leg2;
}

const ThreeWindingsTransformer::Leg2or3& ThreeWindingsTransformer::getLeg3() const {
    return *m_leg3;
}

ThreeWindingsTransformer::Leg2or3& ThreeWindingsTransformer::getLeg3() {
    return *m_leg3;
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
            throw AssertionError(logging::format("Unexpected side value: %1%", side));
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

    if (m_leg2->getRatioTapChanger()) {
        m_leg2->getRatioTapChanger().get().reduceVariantArraySize(number);
    }
    if (m_leg3->getRatioTapChanger()) {
        m_leg3->getRatioTapChanger().get().reduceVariantArraySize(number);
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
