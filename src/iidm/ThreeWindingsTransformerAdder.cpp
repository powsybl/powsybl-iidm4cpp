/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "ThreeWindingsTransformerLeg2.hpp"
#include "ThreeWindingsTransformerLeg3.hpp"
#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

ThreeWindingsTransformerAdder::Leg1Adder::Leg1Adder(ThreeWindingsTransformerAdder& parent) :
    LegAdder(parent),
    m_g(stdcxx::nan()),
    m_b(stdcxx::nan()) {
}

std::unique_ptr<ThreeWindingsTransformer::Leg1> ThreeWindingsTransformerAdder::Leg1Adder::checkAndGetLeg() const {
    checkParams();

    return stdcxx::make_unique<ThreeWindingsTransformer::Leg1>(getR(), getX(), m_g, m_b, getRatedU());
}

void ThreeWindingsTransformerAdder::Leg1Adder::checkParams() const {
    LegAdder::checkParams();

    checkOptional(*this, m_g, "g is not set");
    checkOptional(*this, m_b, "b is not set");
}

ThreeWindingsTransformerAdder::Leg1Adder& ThreeWindingsTransformerAdder::Leg1Adder::clear() {
    LegAdder::clear();

    m_g = stdcxx::nan();
    m_b = stdcxx::nan();

    return *this;
}

std::string ThreeWindingsTransformerAdder::Leg1Adder::getMessageHeader() const {
    static std::string s_messageHeader = "3 windings transformer leg 1: ";

    return s_messageHeader;
}

ThreeWindingsTransformerAdder::Leg1Adder& ThreeWindingsTransformerAdder::Leg1Adder::setB(double b) {
    m_b = b;
    return *this;
}

ThreeWindingsTransformerAdder::Leg1Adder& ThreeWindingsTransformerAdder::Leg1Adder::setG(double g) {
    m_g = g;
    return *this;
}

ThreeWindingsTransformerAdder::Leg2Adder::Leg2Adder(ThreeWindingsTransformerAdder& parent) :
    LegAdder(parent) {
}

std::unique_ptr<ThreeWindingsTransformer::Leg2or3> ThreeWindingsTransformerAdder::Leg2Adder::checkAndGetLeg() const {
    checkParams();

    return stdcxx::make_unique<ThreeWindingsTransformerLeg2>(getR(), getX(), getRatedU());
}

std::string ThreeWindingsTransformerAdder::Leg2Adder::getMessageHeader() const {
    static std::string s_messageHeader = "3 windings transformer leg 2: ";

    return s_messageHeader;
}

ThreeWindingsTransformerAdder::Leg3Adder::Leg3Adder(ThreeWindingsTransformerAdder& parent) :
    LegAdder(parent) {
}

std::unique_ptr<ThreeWindingsTransformer::Leg2or3> ThreeWindingsTransformerAdder::Leg3Adder::checkAndGetLeg() const {
    checkParams();

    return stdcxx::make_unique<ThreeWindingsTransformerLeg3>(getR(), getX(), getRatedU());
}

std::string ThreeWindingsTransformerAdder::Leg3Adder::getMessageHeader() const {
    static std::string s_messageHeader = "3 windings transformer leg 3: ";

    return s_messageHeader;
}

ThreeWindingsTransformerAdder::ThreeWindingsTransformerAdder(Substation& substation) :
    m_substation(substation),
    m_adder1(*this),
    m_adder2(*this),
    m_adder3(*this) {
}

ThreeWindingsTransformer& ThreeWindingsTransformerAdder::add() {
    std::unique_ptr<ThreeWindingsTransformer::Leg1> ptrLeg1 = m_adder1.checkAndGetLeg();
    VoltageLevel& voltageLevel1 = m_adder1.checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal1 = m_adder1.checkAndGetTerminal();

    std::unique_ptr<ThreeWindingsTransformer::Leg2or3> ptrLeg2 = m_adder2.checkAndGetLeg();
    VoltageLevel& voltageLevel2 = m_adder2.checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal2 = m_adder2.checkAndGetTerminal();

    std::unique_ptr<ThreeWindingsTransformer::Leg2or3> ptrLeg3 = m_adder3.checkAndGetLeg();
    VoltageLevel& voltageLevel3 = m_adder3.checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal3 = m_adder3.checkAndGetTerminal();

    // check that the 3 windings transformer is attachable on the 3 sides
    voltageLevel1.attach(*ptrTerminal1, true);
    voltageLevel2.attach(*ptrTerminal2, true);
    voltageLevel3.attach(*ptrTerminal3, true);

    std::unique_ptr<ThreeWindingsTransformer> ptrTransformer = stdcxx::make_unique<ThreeWindingsTransformer>(getId(), getName(), std::move(ptrLeg1), std::move(ptrLeg2), std::move(ptrLeg3));
    auto& transformer = getNetwork().checkAndAdd<ThreeWindingsTransformer>(std::move(ptrTransformer));
    transformer.getLeg1().setTransformer(transformer);
    transformer.getLeg2().setTransformer(transformer);
    transformer.getLeg3().setTransformer(transformer);

    Terminal& terminal1 = transformer.addTerminal(std::move(ptrTerminal1));
    Terminal& terminal2 = transformer.addTerminal(std::move(ptrTerminal2));
    Terminal& terminal3 = transformer.addTerminal(std::move(ptrTerminal3));

    voltageLevel1.attach(terminal1, false);
    voltageLevel2.attach(terminal2, false);
    voltageLevel3.attach(terminal3, false);

    return transformer;
}

Network& ThreeWindingsTransformerAdder::getNetwork() {
    return m_substation.getNetwork();
}

Substation& ThreeWindingsTransformerAdder::getSubstation() {
    return m_substation;
}

const std::string& ThreeWindingsTransformerAdder::getTypeDescription() const {
    static std::string s_typeDescription = "3 windings transformer";

    return s_typeDescription;
}

ThreeWindingsTransformerAdder::Leg1Adder& ThreeWindingsTransformerAdder::newLeg1() {
    return m_adder1.clear();
}

ThreeWindingsTransformerAdder::Leg2Adder& ThreeWindingsTransformerAdder::newLeg2() {
    return m_adder2.clear();
}

ThreeWindingsTransformerAdder::Leg3Adder& ThreeWindingsTransformerAdder::newLeg3() {
    return m_adder3.clear();
}

}  // namespace iidm

}  // namespace powsybl
