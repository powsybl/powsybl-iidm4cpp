/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
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
#include <powsybl/iidm/TopologyVisitor.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/iidm/util/VoltageLevels.hpp>

namespace powsybl {

namespace iidm {

VoltageLevel::VoltageLevel(const std::string& id, const std::string& name, bool fictitious, Substation& substation,
                           double nominalV, double lowVoltageLimit, double highVoltageLimit) :
    Container(id, name, fictitious, Container::Type::VOLTAGE_LEVEL),
    m_substation(substation),
    m_highVoltageLimit(highVoltageLimit),
    m_lowVoltageLimit(lowVoltageLimit),
    m_nominalV(nominalV) {

    checkNominalVoltage(*this, m_nominalV);
    checkVoltageLimits(*this, m_lowVoltageLimit, m_highVoltageLimit);
}

void VoltageLevel::addNextTerminals(Terminal& otherTerminal, std::vector<std::reference_wrapper<Terminal>>& nextTerminals) {
    Connectable& otherConnectable = otherTerminal.getConnectable();
    if (stdcxx::isInstanceOf<Branch>(otherConnectable)) {
        auto& branch = dynamic_cast<Branch&>(otherConnectable);
        if (stdcxx::areSame(branch.getTerminal1(), otherTerminal)) {
            nextTerminals.emplace_back(branch.getTerminal2());
        } else if (stdcxx::areSame(branch.getTerminal2(), otherTerminal)) {
            nextTerminals.emplace_back(branch.getTerminal1());
        } else {
            throw AssertionError("Terminal is not one the branch terminals");
        }
    } else if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(otherConnectable)) {
        auto& ttc = dynamic_cast<ThreeWindingsTransformer&>(otherConnectable);
        if (stdcxx::areSame(ttc.getLeg1().getTerminal(), otherTerminal)) {
            nextTerminals.emplace_back(ttc.getLeg2().getTerminal());
            nextTerminals.emplace_back(ttc.getLeg3().getTerminal());
        } else if (stdcxx::areSame(ttc.getLeg2().getTerminal(), otherTerminal)) {
            nextTerminals.emplace_back(ttc.getLeg1().getTerminal());
            nextTerminals.emplace_back(ttc.getLeg3().getTerminal());
        } else if (stdcxx::areSame(ttc.getLeg3().getTerminal(), otherTerminal)) {
            nextTerminals.emplace_back(ttc.getLeg1().getTerminal());
            nextTerminals.emplace_back(ttc.getLeg2().getTerminal());
        } else {
            throw AssertionError("Terminal is not one the 3 legs terminals");
        }
    }
}

unsigned long VoltageLevel::getBatteryCount() const {
    return getConnectableCount<Battery>();
}

stdcxx::const_range<Battery> VoltageLevel::getBatteries() const {
    return getConnectables<Battery>();
}

stdcxx::range<Battery> VoltageLevel::getBatteries() {
    return getConnectables<Battery>();
}

unsigned long VoltageLevel::getDanglingLineCount() const {
    return getConnectableCount<DanglingLine>();
}

stdcxx::const_range<DanglingLine> VoltageLevel::getDanglingLines() const {
    return getConnectables<DanglingLine>();
}

stdcxx::range<DanglingLine> VoltageLevel::getDanglingLines() {
    return getConnectables<DanglingLine>();
}

unsigned long VoltageLevel::getGeneratorCount() const {
    return getConnectableCount<Generator>();
}

stdcxx::const_range<Generator> VoltageLevel::getGenerators() const {
    return getConnectables<Generator>();
}

stdcxx::range<Generator> VoltageLevel::getGenerators() {
    return getConnectables<Generator>();
}

double VoltageLevel::getHighVoltageLimit() const {
    return m_highVoltageLimit;
}

unsigned long VoltageLevel::getLccConverterStationCount() const {
    return getConnectableCount<LccConverterStation>();
}

stdcxx::const_range<LccConverterStation> VoltageLevel::getLccConverterStations() const {
    return getConnectables<LccConverterStation>();
}

stdcxx::range<LccConverterStation> VoltageLevel::getLccConverterStations() {
    return getConnectables<LccConverterStation>();
}

unsigned long VoltageLevel::getLoadCount() const {
    return getConnectableCount<Load>();
}

stdcxx::const_range<Load> VoltageLevel::getLoads() const {
    return getConnectables<Load>();
}

stdcxx::range<Load> VoltageLevel::getLoads() {
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

double VoltageLevel::getNominalV() const {
    return m_nominalV;
}

unsigned long VoltageLevel::getShuntCompensatorCount() const {
    return getConnectableCount<ShuntCompensator>();
}

stdcxx::const_range<ShuntCompensator> VoltageLevel::getShuntCompensators() const {
    return getConnectables<ShuntCompensator>();
}

stdcxx::range<ShuntCompensator> VoltageLevel::getShuntCompensators() {
    return getConnectables<ShuntCompensator>();
}

unsigned long VoltageLevel::getStaticVarCompensatorCount() const {
    return getConnectableCount<StaticVarCompensator>();
}

stdcxx::const_range<StaticVarCompensator> VoltageLevel::getStaticVarCompensators() const {
    return getConnectables<StaticVarCompensator>();
}

stdcxx::range<StaticVarCompensator> VoltageLevel::getStaticVarCompensators() {
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

stdcxx::const_range<VscConverterStation> VoltageLevel::getVscConverterStations() const {
    return getConnectables<VscConverterStation>();
}

stdcxx::range<VscConverterStation> VoltageLevel::getVscConverterStations() {
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

void VoltageLevel::remove() {
    VoltageLevels::checkRemovability(*this);

    // Remove all connectables
    for (Connectable& connectable : getConnectables()) {
        connectable.remove();
    }

    // Remove the topology
    removeTopology();

    // Remove this voltage level from the network
    getSubstation().remove(*this);
    getNetwork().getIndex().remove(*this);
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

VoltageLevel& VoltageLevel::setNominalV(double nominalV) {
    m_nominalV = checkNominalVoltage(*this, nominalV);
    return *this;
}

void VoltageLevel::visitEquipments(TopologyVisitor& visitor) const {
    TopologyVisitor::visitEquipments(getTerminals(), visitor);
}

}  // namespace iidm

}  // namespace powsybl
