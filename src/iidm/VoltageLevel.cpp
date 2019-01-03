/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/StaticVarCompensatorAdder.hpp>
#include <powsybl/iidm/Substation.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

VoltageLevel::VoltageLevel(const std::string& id, const std::string& name, Substation& substation,
                           double nominalVoltage, double lowVoltageLimit, double highVoltageLimit) :
    Container(id, name, Container::Type::VOLTAGE_LEVEL),
    m_substation(substation),
    m_highVoltageLimit(highVoltageLimit),
    m_lowVoltageLimit(lowVoltageLimit),
    m_nominalVoltage(nominalVoltage) {

    checkNominalVoltage(*this, m_nominalVoltage);
    checkVoltageLimits(*this, m_lowVoltageLimit, m_highVoltageLimit);
}

double VoltageLevel::getHighVoltageLimit() const {
    return m_highVoltageLimit;
}

double VoltageLevel::getLowVoltageLimit() const {
    return m_lowVoltageLimit;
}

const Network& VoltageLevel::getNetwork() const {
    return getSubstation().getNetwork();
}

Network& VoltageLevel::getNetwork() {
    return getSubstation().getNetwork();
}

double VoltageLevel::getNominalVoltage() const {
    return m_nominalVoltage;
}

const Substation& VoltageLevel::getSubstation() const {
    return m_substation.get();
}

Substation& VoltageLevel::getSubstation() {
    return m_substation.get();
}

const std::string& VoltageLevel::getTypeDescription() const {
    static std::string s_typeDescription = "Voltage level";

    return s_typeDescription;
}

DanglingLineAdder VoltageLevel::newDanglingLine() {
    return DanglingLineAdder(*this);
}

GeneratorAdder VoltageLevel::newGenerator() {
    return GeneratorAdder(*this);
}

LoadAdder VoltageLevel::newLoad() {
    return LoadAdder(*this);
}

ShuntCompensatorAdder VoltageLevel::newShuntCompensator() {
    return ShuntCompensatorAdder(*this);
}

StaticVarCompensatorAdder VoltageLevel::newStaticVarCompensator() {
    return StaticVarCompensatorAdder(*this);
}

VoltageLevel& VoltageLevel::setHighVoltageLimit(double highVoltageLimit) {
    checkVoltageLimits(*this, m_lowVoltageLimit, highVoltageLimit);
    m_highVoltageLimit = highVoltageLimit;
    return *this;
}

VoltageLevel& VoltageLevel::setLowVoltageLimit(double lowVoltageLimit) {
    checkVoltageLimits(*this, lowVoltageLimit, m_highVoltageLimit);
    m_lowVoltageLimit = lowVoltageLimit;
    return *this;
}

VoltageLevel& VoltageLevel::setNominalVoltage(double nominalVoltage) {
    m_nominalVoltage = checkNominalVoltage(*this, nominalVoltage);
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
