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
#include <powsybl/logging/Logger.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

ThreeWindingsTransformerAdder::LegAdder::LegAdder(ThreeWindingsTransformerAdder& parent, unsigned long legNumber) :
    m_parent(parent),
    m_legNumber(legNumber) {
}

ThreeWindingsTransformerAdder& ThreeWindingsTransformerAdder::LegAdder::add() {
    checkParams();
    return m_parent;
}

std::unique_ptr<ThreeWindingsTransformer::Leg> ThreeWindingsTransformerAdder::LegAdder::checkAndGetLeg() const {
    checkParams();

    return stdcxx::make_unique<ThreeWindingsTransformer::Leg>(m_legNumber, m_r, m_x, m_g, m_b, m_ratedU);
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

void ThreeWindingsTransformerAdder::LegAdder::checkParams() const {
    checkOptional(*this, m_r, "r is not set");
    checkOptional(*this, m_x, "x is not set");
    checkOptional(*this, m_g, "g is not set");
    checkOptional(*this, m_b, "b is not set");
    checkOptional(*this, m_ratedU, "rated U is not set");
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::LegAdder::clear() {
    m_voltageLevelId.clear();
    m_node.reset();
    m_bus.clear();
    m_connectableBus.clear();
    m_r = stdcxx::nan();
    m_x = stdcxx::nan();
    m_g = stdcxx::nan();
    m_b = stdcxx::nan();
    m_ratedU = stdcxx::nan();

    return *this;
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
    m_substation(substation),
    m_adder1(*this, 1),
    m_adder2(*this, 2),
    m_adder3(*this, 3) {
}

ThreeWindingsTransformer& ThreeWindingsTransformerAdder::add() {
    logging::Logger& logger = logging::LoggerFactory::getLogger<ThreeWindingsTransformer>();

    std::unique_ptr<ThreeWindingsTransformer::Leg> ptrLeg1 = m_adder1.checkAndGetLeg();
    VoltageLevel& voltageLevel1 = m_adder1.checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal1 = m_adder1.checkAndGetTerminal(voltageLevel1);

    std::unique_ptr<ThreeWindingsTransformer::Leg> ptrLeg2 = m_adder2.checkAndGetLeg();
    VoltageLevel& voltageLevel2 = m_adder2.checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal2 = m_adder2.checkAndGetTerminal(voltageLevel2);

    std::unique_ptr<ThreeWindingsTransformer::Leg> ptrLeg3 = m_adder3.checkAndGetLeg();
    VoltageLevel& voltageLevel3 = m_adder3.checkAndGetVoltageLevel();
    std::unique_ptr<Terminal> ptrTerminal3 = m_adder3.checkAndGetTerminal(voltageLevel3);

    // check that the 3 windings transformer is attachable on the 3 sides
    voltageLevel1.attach(*ptrTerminal1, true);
    voltageLevel2.attach(*ptrTerminal2, true);
    voltageLevel3.attach(*ptrTerminal3, true);

    // Define ratedU0 equal to ratedU1 if it has not been defined
    if (std::isnan(m_ratedU0)) {
        m_ratedU0 = ptrLeg1->getRatedU();
        logger.info(stdcxx::format("RatedU0 is not set. Fixed to leg1 ratedU: %1%", ptrLeg1->getRatedU()));
    }

    std::unique_ptr<ThreeWindingsTransformer> ptrTransformer = stdcxx::make_unique<ThreeWindingsTransformer>(checkAndGetUniqueId(), getName(), std::move(ptrLeg1), std::move(ptrLeg2), std::move(ptrLeg3), m_ratedU0);
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

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::newLeg1() {
    return m_adder1.clear();
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::newLeg2() {
    return m_adder2.clear().setB(0).setG(0);
}

ThreeWindingsTransformerAdder::LegAdder& ThreeWindingsTransformerAdder::newLeg3() {
    return m_adder3.clear().setB(0).setG(0);
}

ThreeWindingsTransformerAdder& ThreeWindingsTransformerAdder::setRatedU0(double ratedU0) {
    m_ratedU0 = ratedU0;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
