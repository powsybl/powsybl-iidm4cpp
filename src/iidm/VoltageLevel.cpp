/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensatorAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>

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

unsigned long VoltageLevel::getBatteryCount() const {
    return getConnectableCount<Battery>();
}

VoltageLevel::const_range<Battery> VoltageLevel::getBatteries() const {
    return getConnectables<Battery>();
}

VoltageLevel::range<Battery> VoltageLevel::getBatteries() {
    return getConnectables<Battery>();
}

unsigned long VoltageLevel::getConnectableCount() const {
    const auto& terminals = getTerminals();
    return std::count_if(terminals.cbegin(), terminals.cend(), [](const std::reference_wrapper<Terminal>& terminal) {
        return terminal.get().getConnectable();
    });
}

unsigned long VoltageLevel::getDanglingLineCount() const {
    return getConnectableCount<DanglingLine>();
}

VoltageLevel::const_range<DanglingLine> VoltageLevel::getDanglingLines() const {
    return getConnectables<DanglingLine>();
}

VoltageLevel::range<DanglingLine> VoltageLevel::getDanglingLines() {
    return getConnectables<DanglingLine>();
}

unsigned long VoltageLevel::getGeneratorCount() const {
    return getConnectableCount<Generator>();
}

VoltageLevel::const_range<Generator> VoltageLevel::getGenerators() const {
    return getConnectables<Generator>();
}

VoltageLevel::range<Generator> VoltageLevel::getGenerators() {
    return getConnectables<Generator>();
}

double VoltageLevel::getHighVoltageLimit() const {
    return m_highVoltageLimit;
}

unsigned long VoltageLevel::getLccConverterStationCount() const {
    return getConnectableCount<LccConverterStation>();
}

VoltageLevel::const_range<LccConverterStation> VoltageLevel::getLccConverterStations() const {
    return getConnectables<LccConverterStation>();
}

VoltageLevel::range<LccConverterStation> VoltageLevel::getLccConverterStations() {
    return getConnectables<LccConverterStation>();
}

unsigned long VoltageLevel::getLoadCount() const {
    return getConnectableCount<Load>();
}

VoltageLevel::const_range<Load> VoltageLevel::getLoads() const {
    return getConnectables<Load>();
}

VoltageLevel::range<Load> VoltageLevel::getLoads() {
    return getConnectables<Load>();
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

unsigned long VoltageLevel::getShuntCompensatorCount() const {
    return getConnectableCount<ShuntCompensator>();
}

VoltageLevel::const_range<ShuntCompensator> VoltageLevel::getShuntCompensators() const {
    return getConnectables<ShuntCompensator>();
}

VoltageLevel::range<ShuntCompensator> VoltageLevel::getShuntCompensators() {
    return getConnectables<ShuntCompensator>();
}

unsigned long VoltageLevel::getStaticVarCompensatorCount() const {
    return getConnectableCount<StaticVarCompensator>();
}

VoltageLevel::const_range<StaticVarCompensator> VoltageLevel::getStaticVarCompensators() const {
    return getConnectables<StaticVarCompensator>();
}

VoltageLevel::range<StaticVarCompensator> VoltageLevel::getStaticVarCompensators() {
    return getConnectables<StaticVarCompensator>();
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

unsigned long VoltageLevel::getVscConverterStationCount() const {
    return getConnectableCount<VscConverterStation>();
}

VoltageLevel::const_range<VscConverterStation> VoltageLevel::getVscConverterStations() const {
    return getConnectables<VscConverterStation>();
}

VoltageLevel::range<VscConverterStation> VoltageLevel::getVscConverterStations() {
    return getConnectables<VscConverterStation>();
}

BatteryAdder VoltageLevel::newBattery() {
    return BatteryAdder(*this);
}

DanglingLineAdder VoltageLevel::newDanglingLine() {
    return DanglingLineAdder(*this);
}

GeneratorAdder VoltageLevel::newGenerator() {
    return GeneratorAdder(*this);
}

LccConverterStationAdder VoltageLevel::newLccConverterStation() {
    return LccConverterStationAdder(*this);
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

VscConverterStationAdder VoltageLevel::newVscConverterStation() {
    return VscConverterStationAdder(*this);
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
