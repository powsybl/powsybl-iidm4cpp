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
#include <powsybl/iidm/DanglingLineFilter.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Ground.hpp>
#include <powsybl/iidm/GroundAdder.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/Line.hpp>
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

VoltageLevel::VoltageLevel(const std::string& id, const std::string& name, bool fictitious, const stdcxx::Reference<Substation>& substation,
                           Network& network, double nominalV, double lowVoltageLimit, double highVoltageLimit) :
    Container(id, name, fictitious, Container::Type::VOLTAGE_LEVEL),
    m_network(network),
    m_subnetworkRef(),
    m_substation(substation),
    m_highVoltageLimit(highVoltageLimit),
    m_lowVoltageLimit(lowVoltageLimit),
    m_nominalV(nominalV) {

    checkNominalVoltage(*this, m_nominalV);
    checkVoltageLimits(*this, m_lowVoltageLimit, m_highVoltageLimit);
}

VoltageLevel::VoltageLevel(const std::string& id, const std::string& name, bool fictitious, const stdcxx::Reference<Substation>& substation,
                 Network& rootNetwork, Network& subnetwork, double nominalV, double lowVoltageLimit, double highVoltageLimit) : 
    VoltageLevel(id, name, fictitious, substation, rootNetwork, nominalV, lowVoltageLimit, highVoltageLimit) {
    m_subnetworkRef = subnetwork;
}

void VoltageLevel::addNextTerminals(Terminal& otherTerminal, TerminalSet& nextTerminals) {
    Connectable& otherConnectable = otherTerminal.getConnectable();
    if (stdcxx::isInstanceOf<Branch>(otherConnectable)) {
        auto& branch = dynamic_cast<Branch&>(otherConnectable);
        if (stdcxx::areSame(branch.getTerminal1(), otherTerminal)) {
            nextTerminals.emplace(branch.getTerminal2());
        } else if (stdcxx::areSame(branch.getTerminal2(), otherTerminal)) {
            nextTerminals.emplace(branch.getTerminal1());
        } else {
            throw AssertionError("Terminal is not one the branch terminals");
        }
    } else if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(otherConnectable)) {
        auto& ttc = dynamic_cast<ThreeWindingsTransformer&>(otherConnectable);
        if (stdcxx::areSame(ttc.getLeg1().getTerminal(), otherTerminal)) {
            nextTerminals.emplace(ttc.getLeg2().getTerminal());
            nextTerminals.emplace(ttc.getLeg3().getTerminal());
        } else if (stdcxx::areSame(ttc.getLeg2().getTerminal(), otherTerminal)) {
            nextTerminals.emplace(ttc.getLeg1().getTerminal());
            nextTerminals.emplace(ttc.getLeg3().getTerminal());
        } else if (stdcxx::areSame(ttc.getLeg3().getTerminal(), otherTerminal)) {
            nextTerminals.emplace(ttc.getLeg1().getTerminal());
            nextTerminals.emplace(ttc.getLeg2().getTerminal());
        } else {
            throw AssertionError("Terminal is not one the 3 legs terminals");
        }
    }
}

unsigned long VoltageLevel::getAreaCount() const {
    return boost::size(getAreas());
}
stdcxx::const_range<Area> VoltageLevel::getAreas() const {
    return m_areas;
}
stdcxx::range<Area> VoltageLevel::getAreas() {
    return m_areas;
}
stdcxx::CReference<Area> VoltageLevel::getArea(const std::string& areaType) const {
    for (const auto& area : getAreas()) {
        if (area.getAreaType()==areaType) {
            return stdcxx::cref(area);
        }
    }
    return stdcxx::CReference<Area>();
}
stdcxx::Reference<Area> VoltageLevel::getArea(const std::string& areaType) {
    return stdcxx::ref(const_cast<const VoltageLevel*>(this)->getArea(areaType));
}

void VoltageLevel::addArea(Area& area) {
    auto it = std::find_if(m_areas.begin(), m_areas.end(), [&area](const stdcxx::Reference<Area>& ar) {
        return stdcxx::areSame(area, ar.get());
    });
    if (it!=m_areas.end()) {
        //Already present, nothing to do
        return;
    }

    // Check that the VoltageLevel belongs to the same network or subnetwork
    if (!stdcxx::areSame(area.getParentNetwork(), getParentNetwork())) {
        throw PowsyblException(stdcxx::format("VoltageLevel %1% cannot be added to Area %2%. It does not belong to the same network or subnetwork.", getId(), area.getId()));
    }
    // Check if the voltageLevel is already in another Area of the same type
    auto previousArea = getArea(area.getAreaType());
    if (static_cast<bool>(previousArea) && !stdcxx::areSame(previousArea.get(), area)) {
        // This instance already has a different area with the same AreaType
        throw PowsyblException("VoltageLevel " + getId() + " is already in Area of the same type=" + previousArea.get().getAreaType() + " with id=" + previousArea.get().getId());
    }

    //No conflict, add the given area to this voltageLevel and vice versa
    m_areas.emplace_back(stdcxx::ref(area));
    area.addVoltageLevel(*this);
}
void VoltageLevel::removeArea(Area& area) {
    //We ensure unicity in addArea() but we try to remove several just in case
    const auto removeIt = std::remove_if(m_areas.begin(), m_areas.end(), [&area](const stdcxx::Reference<Area>& ar) {
        return stdcxx::areSame(area, ar.get());
    });
    if(removeIt!=m_areas.end()) { //at least one element to erase
        m_areas.erase(removeIt, m_areas.end());
        area.removeVoltageLevel(*this);
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
    return getDanglingLines(DanglingLineFilter::ALL());
}

stdcxx::range<DanglingLine> VoltageLevel::getDanglingLines() {
    return getDanglingLines(DanglingLineFilter::ALL());
}

stdcxx::const_range<DanglingLine> VoltageLevel::getDanglingLines(const DanglingLineFilter& filter) const {
    return getConnectables<DanglingLine>() | boost::adaptors::filtered(filter.getPredicate());
}

stdcxx::range<DanglingLine> VoltageLevel::getDanglingLines(const DanglingLineFilter& filter) {
    return getConnectables<DanglingLine>() | boost::adaptors::filtered(filter.getPredicate());
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

unsigned long VoltageLevel::getGroundCount() const {
    return getConnectableCount<Ground>();
}

stdcxx::const_range<Ground> VoltageLevel::getGrounds() const {
    return getConnectables<Ground>();
}

stdcxx::range<Ground> VoltageLevel::getGrounds() {
    return getConnectables<Ground>();
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

unsigned long VoltageLevel::getLineCount() const {
    return getConnectableCount<Line>();
}

stdcxx::const_range<Line> VoltageLevel::getLines() const {
    return getConnectables<Line>();
}

stdcxx::range<Line> VoltageLevel::getLines() {
    return getConnectables<Line>();
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
    if (m_network) {
        return m_network.get();
    }
    if (m_substation) {
        return m_substation.get().getNetwork();
    }
    throw PowsyblException(stdcxx::format("Voltage level %1% has no container", getId()));
}

Network& VoltageLevel::getNetwork() {
    return const_cast<Network&>(static_cast<const VoltageLevel*>(this)->getNetwork());
}

const Network& VoltageLevel::getParentNetwork() const {
    if(static_cast<bool>(m_subnetworkRef)) {
        return m_subnetworkRef.get();
    }
    return getNetwork();
}

Network& VoltageLevel::getParentNetwork() {
    if(static_cast<bool>(m_subnetworkRef)) {
        return m_subnetworkRef.get();
    }
    return getNetwork();
}

std::string VoltageLevel::getSubnetworkId() const {
    if(static_cast<bool>(m_subnetworkRef)) {
        return m_subnetworkRef.get().getId();
    }
    return "";
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

stdcxx::CReference<Substation> VoltageLevel::getSubstation() const {
    return stdcxx::cref(m_substation);
}

stdcxx::Reference<Substation> VoltageLevel::getSubstation() {
    return m_substation;
}

unsigned long VoltageLevel::getThreeWindingsTransformerCount() const {
    return getConnectableCount<ThreeWindingsTransformer>();
}

stdcxx::const_range<ThreeWindingsTransformer> VoltageLevel::getThreeWindingsTransformers() const {
    return getConnectables<ThreeWindingsTransformer>();
}

stdcxx::range<ThreeWindingsTransformer> VoltageLevel::getThreeWindingsTransformers() {
    return getConnectables<ThreeWindingsTransformer>();
}

unsigned long VoltageLevel::getTwoWindingsTransformerCount() const {
    return getConnectableCount<TwoWindingsTransformer>();
}

stdcxx::const_range<TwoWindingsTransformer> VoltageLevel::getTwoWindingsTransformers() const {
    return getConnectables<TwoWindingsTransformer>();
}

stdcxx::range<TwoWindingsTransformer> VoltageLevel::getTwoWindingsTransformers() {
    return getConnectables<TwoWindingsTransformer>();
}

const IdentifiableType& VoltageLevel::getType() const {
    static IdentifiableType s_type = IdentifiableType::VOLTAGE_LEVEL;
    return s_type;
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

GroundAdder VoltageLevel::newGround() {
    return GroundAdder(*this);
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

    if (static_cast<bool>(m_substation)) {
        // Remove this voltage level from the network
        m_substation.get().remove(*this);
    }
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

void VoltageLevel::setNetworkRef(Network& network) {
    m_network.set(network);
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
