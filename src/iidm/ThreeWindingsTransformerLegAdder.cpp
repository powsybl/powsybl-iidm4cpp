/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ThreeWindingsTransformerLegAdder.hpp>

#include <array>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TerminalBuilder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

namespace three_windings_transformer {

LegAdder::LegAdder(ThreeWindingsTransformerAdder& parent, unsigned long legNumber) :
    m_parent(parent),
    m_legNumber(legNumber) {
}

LegAdder& LegAdder::operator=(const LegAdder& adder) {
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

ThreeWindingsTransformerAdder& LegAdder::add() {
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

ThreeWindingsTransformer::Leg LegAdder::build() const {
    return ThreeWindingsTransformer::Leg(m_legNumber, m_r, m_x, m_g, m_b, m_ratedU, m_ratedS);
}

std::unique_ptr<Terminal> LegAdder::checkAndGetTerminal(VoltageLevel& voltageLevel) {
    return TerminalBuilder(voltageLevel, *this)
        .setNode(m_node)
        .setBus(m_bus)
        .setConnectableBus(m_connectableBus)
        .build();
}

VoltageLevel& LegAdder::checkAndGetVoltageLevel() {
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

std::string LegAdder::getMessageHeader() const {
    static std::array<std::string, 3> s_messageHeaders {{
        u8"3 windings transformer leg1: ",
        u8"3 windings transformer leg2: ",
        u8"3 windings transformer leg3: "
    }};

    return s_messageHeaders[m_legNumber - 1];
}

LegAdder& LegAdder::setB(double b) {
    m_b = b;
    return *this;
}

LegAdder& LegAdder::setBus(const std::string& bus) {
    m_bus = bus;
    return *this;
}

LegAdder& LegAdder::setConnectableBus(const std::string& connectableBus) {
    m_connectableBus = connectableBus;
    return *this;
}

LegAdder& LegAdder::setG(double g) {
    m_g = g;
    return *this;
}

LegAdder& LegAdder::setNode(unsigned long node) {
    m_node = node;
    return *this;
}

LegAdder& LegAdder::setR(double r) {
    m_r = r;
    return *this;
}

LegAdder& LegAdder::setRatedS(double ratedS) {
    m_ratedS = ratedS;
    return *this;
}

LegAdder& LegAdder::setRatedU(double ratedU) {
    m_ratedU = ratedU;
    return *this;
}

LegAdder& LegAdder::setVoltageLevel(const std::string& voltageLevelId) {
    m_voltageLevelId = voltageLevelId;
    return *this;
}

LegAdder& LegAdder::setX(double x) {
    m_x = x;
    return *this;
}

}  // namespace three_windings_transformer

}  // namespace iidm

}  // namespace powsybl
