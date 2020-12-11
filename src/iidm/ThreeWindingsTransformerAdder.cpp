/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>

#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TerminalBuilder.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

ThreeWindingsTransformerAdder::LegAdder::LegAdder(ThreeWindingsTransformerAdder& parent, unsigned long legNumber) :
    m_parent(parent),
    m_legNumber(legNumber) {
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::LegAdder::operator=(const LegAdder& adder) {
    m_legNumber = adder.m_legNumber;
    m_voltageLevelId = adder.m_voltageLevelId;
    m_node = adder.m_node;
    m_bus = adder.m_bus;
    m_connectableBus = adder.m_connectableBus;
    m_r = adder.m_r;
    m_x = adder.m_x;
    m_g = adder.m_g;
    m_b = adder.m_b;
    m_ratedU = adder.m_ratedU;
    m_ratedS = adder.m_ratedS;
    return *this;
}

ThreeWindingsTransformerAdder& ThreeWindingsTransformerAdder::LegAdder::add() {
    checkOptional(*this, m_r, "r is not set");
    checkOptional(*this, m_x, "x is not set");
    checkOptional(*this, m_g, "g is not set");
    checkOptional(*this, m_b, "b is not set");
    checkOptional(*this, m_ratedU, "rated U is not set");
    checkRatedS(*this, m_ratedS);

    switch (m_legNumber) {
        case 1:
            m_parent.setLegAdder1(*this);
            break;

        case 2:
            m_parent.setLegAdder2(*this);
            break;

        case 3:
            m_parent.setLegAdder3(*this);
            break;

        default:
            throw ValidationException(*this, "Unexpected leg number");
            break;
    }
    return m_parent;
}

std::unique_ptr<ThreeWindingsTransformer::Leg> ThreeWindingsTransformerAdder::LegAdder::build() const {
    return stdcxx::make_unique<ThreeWindingsTransformer::Leg>(m_legNumber, m_r, m_x, m_g, m_b, m_ratedU, m_ratedS);
}

std::unique_ptr<Terminal> ThreeWindingsTransformerAdder::LegAdder::checkAndGetTerminal(VoltageLevel& voltageLevel) {
    return TerminalBuilder(voltageLevel, *this)
        .setNode(m_node)
        .setBus(m_bus)
        .setConnectableBus(m_connectableBus)
        .build();
}

VoltageLevel& ThreeWindingsTransformerAdder::LegAdder::checkAndGetVoltageLevel() {
    checkNotEmpty(*this, m_voltageLevelId, "voltage level is not set");

    stdcxx::Reference<VoltageLevel> voltageLevel = m_parent.getNetwork().template find<VoltageLevel>(m_voltageLevelId);
    if (!voltageLevel) {
        throw ValidationException(*this, stdcxx::format("voltage level '%1%' not found", m_voltageLevelId));
    }
    if (!stdcxx::areSame(voltageLevel.get().getSubstation(), m_parent.getSubstation())) {
        throw ValidationException(*this, stdcxx::format("voltage level shall belong to the substation '%1%'", m_parent.getSubstation().getId()));
    }

    return voltageLevel.get();
}

std::string ThreeWindingsTransformerAdder::LegAdder::getMessageHeader() const {
    static std::array<std::string, 3> s_messageHeaders {{
        u8"3 windings transformer leg1: ",
        u8"3 windings transformer leg2: ",
        u8"3 windings transformer leg3: "
    }};

    return s_messageHeaders[m_legNumber - 1];
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::LegAdder::setB(double b) {
    m_b = b;
    return *this;
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::LegAdder::setBus(const std::string& bus) {
    m_bus = bus;
    return *this;
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::LegAdder::setConnectableBus(const std::string& connectableBus) {
    m_connectableBus = connectableBus;
    return *this;
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::LegAdder::setG(double g) {
    m_g = g;
    return *this;
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::LegAdder::setNode(unsigned long node) {
    m_node = node;
    return *this;
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::LegAdder::setR(double r) {
    m_r = r;
    return *this;
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::LegAdder::setRatedS(double ratedS) {
    m_ratedS = ratedS;
    return *this;
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::LegAdder::setRatedU(double ratedU) {
    m_ratedU = ratedU;
    return *this;
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::LegAdder::setVoltageLevel(const std::string& voltageLevelId) {
    m_voltageLevelId = voltageLevelId;
    return *this;
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::LegAdder::setX(double x) {
    m_x = x;
    return *this;
}

ThreeWindingsTransformerAdder::ThreeWindingsTransformerAdder(Substation& substation) :
    m_substation(substation) {
}

ThreeWindingsTransformer& ThreeWindingsTransformerAdder::add() {
    logging::Logger& logger = logging::LoggerFactory::getLogger<ThreeWindingsTransformer>();

    if (!m_adder1.is_initialized()) {
        throw ValidationException(newLeg1(), "leg 1 is not defined");
    }
    std::unique_ptr<ThreeWindingsTransformer::Leg> ptrLeg1 = m_adder1->build();
    VoltageLevel& voltageLevel1 = m_adder1->checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal1 = m_adder1->checkAndGetTerminal(voltageLevel1);

    if (!m_adder2.is_initialized()) {
        throw ValidationException(newLeg2(), "leg 2 is not defined");
    }
    std::unique_ptr<ThreeWindingsTransformer::Leg> ptrLeg2 = m_adder2->build();
    VoltageLevel& voltageLevel2 = m_adder2->checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal2 = m_adder2->checkAndGetTerminal(voltageLevel2);

    if (!m_adder3.is_initialized()) {
        throw ValidationException(newLeg3(), "leg 3 is not defined");
    }
    std::unique_ptr<ThreeWindingsTransformer::Leg> ptrLeg3 = m_adder3->build();
    VoltageLevel& voltageLevel3 = m_adder3->checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal3 = m_adder3->checkAndGetTerminal(voltageLevel3);

    // check that the 3 windings transformer is attachable on the 3 sides
    voltageLevel1.attach(*ptrTerminal1, true);
    voltageLevel2.attach(*ptrTerminal2, true);
    voltageLevel3.attach(*ptrTerminal3, true);

    // Define ratedU0 equal to ratedU1 if it has not been defined
    if (std::isnan(m_ratedU0)) {
        m_ratedU0 = ptrLeg1->getRatedU();
        logger.info(stdcxx::format("RatedU0 is not set. Fixed to leg1 ratedU: %1%", ptrLeg1->getRatedU()));
    }

    std::unique_ptr<ThreeWindingsTransformer> ptrTransformer = stdcxx::make_unique<ThreeWindingsTransformer>(checkAndGetUniqueId(), getName(), isFictitious(), std::move(ptrLeg1), std::move(ptrLeg2), std::move(ptrLeg3), m_ratedU0);
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

const Network& ThreeWindingsTransformerAdder::getNetwork() const {
    return m_substation.getNetwork();
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

ThreeWindingsTransformerAdder::LegAdder ThreeWindingsTransformerAdder::newLeg1() {
    return ThreeWindingsTransformerAdder::LegAdder(*this, 1);
}

ThreeWindingsTransformerAdder::LegAdder ThreeWindingsTransformerAdder::newLeg2() {
    return ThreeWindingsTransformerAdder::LegAdder(*this, 2).setB(0).setG(0);
}

ThreeWindingsTransformerAdder::LegAdder ThreeWindingsTransformerAdder::newLeg3() {
    return ThreeWindingsTransformerAdder::LegAdder(*this, 3).setB(0).setG(0);
}

void ThreeWindingsTransformerAdder::setLegAdder1(LegAdder& legAdder) {
    m_adder1.emplace(legAdder);
}

void ThreeWindingsTransformerAdder::setLegAdder2(LegAdder& legAdder) {
    m_adder2.emplace(legAdder);
}

void ThreeWindingsTransformerAdder::setLegAdder3(LegAdder& legAdder) {
    m_adder3.emplace(legAdder);
}

ThreeWindingsTransformerAdder& ThreeWindingsTransformerAdder::setRatedU0(double ratedU0) {
    m_ratedU0 = ratedU0;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
