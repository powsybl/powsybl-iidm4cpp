/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ThreeWindingsTransformer.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/Substation.hpp>

namespace powsybl {

namespace iidm {

ThreeWindingsTransformer::ThreeWindingsTransformer(const std::string& id, const std::string& name, bool fictitious, Leg&& leg1, Leg&& leg2, Leg&& leg3, double ratedU0) :
    Connectable(id, name, fictitious, ConnectableType::THREE_WINDINGS_TRANSFORMER),
    m_ratedU0(ratedU0) {
    m_legs.emplace_back(std::move(leg1));
    m_legs.emplace_back(std::move(leg2));
    m_legs.emplace_back(std::move(leg3));
    for (Leg& leg : m_legs) {
        leg.setTransformer(*this);
    }
}

void ThreeWindingsTransformer::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Connectable::allocateVariantArrayElement(indexes, sourceIndex);

    if (m_legs[0].hasRatioTapChanger()) {
        m_legs[0].getRatioTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_legs[0].hasPhaseTapChanger()) {
        m_legs[0].getPhaseTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_legs[1].hasRatioTapChanger()) {
        m_legs[1].getRatioTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_legs[1].hasPhaseTapChanger()) {
        m_legs[1].getPhaseTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_legs[2].hasRatioTapChanger()) {
        m_legs[2].getRatioTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
    }
    if (m_legs[2].hasPhaseTapChanger()) {
        m_legs[2].getPhaseTapChanger().allocateVariantArrayElement(indexes, sourceIndex);
    }
}

void ThreeWindingsTransformer::deleteVariantArrayElement(unsigned long index) {
    Connectable::deleteVariantArrayElement(index);

    if (m_legs[0].hasRatioTapChanger()) {
        m_legs[0].getRatioTapChanger().deleteVariantArrayElement(index);
    }
    if (m_legs[0].hasPhaseTapChanger()) {
        m_legs[0].getPhaseTapChanger().deleteVariantArrayElement(index);
    }
    if (m_legs[1].hasRatioTapChanger()) {
        m_legs[1].getRatioTapChanger().deleteVariantArrayElement(index);
    }
    if (m_legs[1].hasPhaseTapChanger()) {
        m_legs[1].getPhaseTapChanger().deleteVariantArrayElement(index);
    }
    if (m_legs[2].hasRatioTapChanger()) {
        m_legs[2].getRatioTapChanger().deleteVariantArrayElement(index);
    }
    if (m_legs[2].hasPhaseTapChanger()) {
        m_legs[2].getPhaseTapChanger().deleteVariantArrayElement(index);
    }
}

void ThreeWindingsTransformer::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Connectable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    if (m_legs[0].hasRatioTapChanger()) {
        m_legs[0].getRatioTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_legs[0].hasPhaseTapChanger()) {
        m_legs[0].getPhaseTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_legs[1].hasRatioTapChanger()) {
        m_legs[1].getRatioTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_legs[1].hasPhaseTapChanger()) {
        m_legs[1].getPhaseTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_legs[2].hasRatioTapChanger()) {
        m_legs[2].getRatioTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
    if (m_legs[2].hasPhaseTapChanger()) {
        m_legs[2].getPhaseTapChanger().extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
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

ThreeWindingsTransformer::Side ThreeWindingsTransformer::getSide(const Terminal& terminal) const {
    if (stdcxx::areSame(m_legs[0].getTerminal(), terminal)) {
        return Side::ONE;
    }
    if (stdcxx::areSame(m_legs[1].getTerminal(), terminal)) {
        return Side::TWO;
    }
    if (stdcxx::areSame(m_legs[2].getTerminal(), terminal)) {
        return Side::THREE;
    }
    throw AssertionError("The terminal is not connected to this three windings transformer");
}

const Substation& ThreeWindingsTransformer::getSubstation() const {
    return m_legs[0].getTerminal().getVoltageLevel().getSubstation();
}

Substation& ThreeWindingsTransformer::getSubstation() {
    return m_legs[0].getTerminal().getVoltageLevel().getSubstation();
}

const Terminal& ThreeWindingsTransformer::getTerminal(const Side& side) const {
    switch (side) {
        case Side::ONE:
            return m_legs[0].getTerminal();
        case Side::TWO:
            return m_legs[1].getTerminal();
        case Side::THREE:
            return m_legs[2].getTerminal();
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

    if (m_legs[0].hasRatioTapChanger()) {
        m_legs[0].getRatioTapChanger().reduceVariantArraySize(number);
    }
    if (m_legs[0].hasPhaseTapChanger()) {
        m_legs[0].getPhaseTapChanger().reduceVariantArraySize(number);
    }
    if (m_legs[1].hasRatioTapChanger()) {
        m_legs[1].getRatioTapChanger().reduceVariantArraySize(number);
    }
    if (m_legs[1].hasPhaseTapChanger()) {
        m_legs[1].getPhaseTapChanger().reduceVariantArraySize(number);
    }
    if (m_legs[2].hasRatioTapChanger()) {
        m_legs[2].getRatioTapChanger().reduceVariantArraySize(number);
    }
    if (m_legs[2].hasPhaseTapChanger()) {
        m_legs[2].getPhaseTapChanger().reduceVariantArraySize(number);
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
