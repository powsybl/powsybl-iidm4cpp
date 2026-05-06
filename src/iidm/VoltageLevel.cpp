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
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/DanglingLineFilter.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Ground.hpp>
#include <powsybl/iidm/GroundAdder.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineCommutatedConverter.hpp>
#include <powsybl/iidm/LineCommutatedConverterAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensatorAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TopologyVisitor.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VoltageSourceConverter.hpp>
#include <powsybl/iidm/VoltageSourceConverterAdder.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/iidm/util/VoltageLevels.hpp>

#include "BusBreakerTopologyModel.hpp"
#include "CalculatedBus.hpp"
#include "NodeBreakerTopologyModel.hpp"

namespace powsybl {

namespace iidm {

VoltageLevel::VoltageLevel(const std::string& id, const std::string& name, bool fictitious, const stdcxx::Reference<Substation>& substation,
                           Network& network, double nominalV, double lowVoltageLimit, double highVoltageLimit, const TopologyKind& topologyKind) :
    Container(id, name, fictitious, Container::Type::VOLTAGE_LEVEL),
    m_network(network),
    m_subnetworkRef(),
    m_substation(substation),
    m_highVoltageLimit(highVoltageLimit),
    m_lowVoltageLimit(lowVoltageLimit),
    m_nominalV(nominalV) {

        switch (topologyKind) {
            case TopologyKind::NODE_BREAKER:
                m_topologyModel = stdcxx::make_unique<NodeBreakerTopologyModel>(*this);
                break;
            case TopologyKind::BUS_BREAKER:
                m_topologyModel = stdcxx::make_unique<BusBreakerTopologyModel>(*this);
                break;
            default:
                throw AssertionError(stdcxx::format("Unexpected TopologyKind value: %1%", topologyKind));
        }

    checkNominalVoltage(*this, m_nominalV);
    checkVoltageLimits(*this, m_lowVoltageLimit, m_highVoltageLimit);
}

VoltageLevel::VoltageLevel(const std::string& id, const std::string& name, bool fictitious, const stdcxx::Reference<Substation>& substation,
                 Network& rootNetwork, Network& subnetwork, double nominalV, double lowVoltageLimit, double highVoltageLimit, const TopologyKind& topologyKind) : 
    VoltageLevel(id, name, fictitious, substation, rootNetwork, nominalV, lowVoltageLimit, highVoltageLimit, topologyKind) {
    m_subnetworkRef = subnetwork;
}

const VoltageLevel::BusBreakerView& VoltageLevel::getBusBreakerView() const {
    assertTopologyModel();
    return m_topologyModel->getBusBreakerView();
}

VoltageLevel::BusBreakerView& VoltageLevel::getBusBreakerView() {
    assertTopologyModel();
    return m_topologyModel->getBusBreakerView();
}

const VoltageLevel::BusView& VoltageLevel::getBusView() const {
    assertTopologyModel();
    return m_topologyModel->getBusView();
}

VoltageLevel::BusView& VoltageLevel::getBusView() {
    assertTopologyModel();
    return m_topologyModel->getBusView();
}

const VoltageLevel::NodeBreakerView& VoltageLevel::getNodeBreakerView() const {
    assertTopologyModel();
    return m_topologyModel->getNodeBreakerView();
}

VoltageLevel::NodeBreakerView& VoltageLevel::getNodeBreakerView() {
    assertTopologyModel();
    return m_topologyModel->getNodeBreakerView();
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

unsigned long VoltageLevel::getLineCommutatedConverterCount() const {
    return getConnectableCount<LineCommutatedConverter>();
}

stdcxx::const_range<LineCommutatedConverter> VoltageLevel::getLineCommutatedConverters() const {
    return getConnectables<LineCommutatedConverter>();
}

stdcxx::range<LineCommutatedConverter> VoltageLevel::getLineCommutatedConverters() {
    return getConnectables<LineCommutatedConverter>();
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

unsigned long VoltageLevel::getSwitchCount() const {
    assertTopologyModel();
    return m_topologyModel->getSwitchCount();
}

stdcxx::const_range<Switch> VoltageLevel::getSwitches() const {
    assertTopologyModel();
    return m_topologyModel->getSwitches();
}

stdcxx::range<Switch> VoltageLevel::getSwitches() {
    assertTopologyModel();
    return m_topologyModel->getSwitches();
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

const TopologyKind& VoltageLevel::getTopologyKind() const {
    assertTopologyModel();
    return m_topologyModel->getTopologyKind();
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

unsigned long VoltageLevel::getVoltageSourceConverterCount() const {
    return getConnectableCount<VoltageSourceConverter>();
}

stdcxx::const_range<VoltageSourceConverter> VoltageLevel::getVoltageSourceConverters() const {
    return getConnectables<VoltageSourceConverter>();
}

stdcxx::range<VoltageSourceConverter> VoltageLevel::getVoltageSourceConverters() {
    return getConnectables<VoltageSourceConverter>();
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

LineCommutatedConverterAdder VoltageLevel::newLineCommutatedConverter() {
    return LineCommutatedConverterAdder(*this);
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

VoltageSourceConverterAdder VoltageLevel::newVoltageSourceConverter() {
    return VoltageSourceConverterAdder(*this);
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
    if(m_topologyModel){
        m_topologyModel->removeTopology();
    }

    for (auto &area : m_areas){
        if(static_cast<bool>(area)) {
            area.get().removeVoltageLevel(*this);
        }
    }

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
    assertTopologyModel();
    TopologyVisitor::visitEquipments(m_topologyModel->getTerminals(), visitor);
}

void VoltageLevel::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Identifiable::allocateVariantArrayElement(indexes, sourceIndex);
    if(m_topologyModel) {
        m_topologyModel->allocateVariantArrayElement(indexes, sourceIndex);
    }
}

void VoltageLevel::deleteVariantArrayElement(unsigned long index) {
    Identifiable::deleteVariantArrayElement(index);
    if(m_topologyModel) {
        m_topologyModel->deleteVariantArrayElement(index);
    }
}

void VoltageLevel::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Identifiable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    if(m_topologyModel) {
        m_topologyModel->extendVariantArraySize(initVariantArraySize, number, sourceIndex);
    }
}

void VoltageLevel::reduceVariantArraySize(unsigned long number) {
    Identifiable::reduceVariantArraySize(number);
    if(m_topologyModel) {
        m_topologyModel->reduceVariantArraySize(number);
    }
}

void VoltageLevel::assertTopologyModel() const {
    if(!static_cast<bool>(m_topologyModel)) {
        throw PowsyblException(stdcxx::format("TopologyModel missing from VoltageLevel %1%", getId())); 
    }
}

void VoltageLevel::convertToTopology(const TopologyKind& newTopologyKind) {
    assertTopologyModel();
    if(newTopologyKind == getTopologyKind()) {
        return;
    }

    switch (newTopologyKind) {
        case TopologyKind::NODE_BREAKER:
            throw PowsyblException(stdcxx::format("Topology model conversion from %1% to %2% not yet supported", getTopologyKind(), newTopologyKind));
        case TopologyKind::BUS_BREAKER:
            convertToBusBreakerTopology();
            break;
        default:
            throw AssertionError(stdcxx::format("Unexpected TopologyKind value: %1%", newTopologyKind));
    }
}

void VoltageLevel::convertToBusBreakerTopology() {
    assertTopologyModel();
    auto& nodeBreakerTopology = dynamic_cast<NodeBreakerTopologyModel&>(*m_topologyModel);

    std::unique_ptr<TopologyModel> newTopologyModel = stdcxx::make_unique<BusBreakerTopologyModel>(*this);
    auto& busBreakerTopologyModel = dynamic_cast<BusBreakerTopologyModel&>(*newTopologyModel);

    // first store all bus/breaker topological infos associated to terminals because we will start moving
    // them from old mode to new one, it will modify the old topology model so we can then reconnect them
    struct TopologyModelTerminalInfos{
        Terminal& m_terminal;
        std::string m_connectableBusId;
        bool m_connected;
    };
    std::vector<TopologyModelTerminalInfos> oldTopologyModelInfos;
    for (Terminal& oldTerminal : m_topologyModel->getTerminals()) {
        if(oldTerminal.getConnectable() && oldTerminal.getConnectable().get().getType() != IdentifiableType::BUSBAR_SECTION) {
            stdcxx::Reference<Bus> connectableBus = oldTerminal.getBusBreakerView().getConnectableBus();
            std::string connectableBusId = static_cast<bool>(connectableBus) ? connectableBus.get().getId() : "";
            bool connected = oldTerminal.isConnected();
            oldTopologyModelInfos.emplace_back(TopologyModelTerminalInfos{oldTerminal, connectableBusId, connected});
        }
    }

    //Convert calculated buses
    for (Bus& bus : m_topologyModel->getBusBreakerView().getBuses()) {
        BusAdder(*this)
            .setId(bus.getId())
            .setName(bus.getOptionalName())
            .setFictitious(bus.isFictitious())
            .addOnTopology(busBreakerTopologyModel);
    }

    //Transfer retained switches
    for(Switch& sw : m_topologyModel->getBusBreakerView().getSwitches()) {
        auto bus1 = m_topologyModel->getBusBreakerView().getBus1(sw.getId());
        auto bus2 = m_topologyModel->getBusBreakerView().getBus2(sw.getId());
        std::string busId1 = (static_cast<bool>(bus1)) ? bus1.get().getId() : "";
        std::string busId2 = (static_cast<bool>(bus2)) ? bus2.get().getId() : "";
        nodeBreakerTopology.removeSwitchFromTopology(sw.getId());
        busBreakerTopologyModel.addSwitchToTopology(sw, busId1, busId2);
    }

    //reconnect connectables in new topology
    for (auto& infos : oldTopologyModelInfos ) {
        stdcxx::Reference<Connectable> connectable = infos.m_terminal.getConnectable();
        if(!connectable){
            continue;
        }

        // if there is no way to find a connectable bus, remove the connectable
        // an alternative would be to connect them all to a new trash configured bus
        if(infos.m_connectableBusId.empty()) {
            connectable.get().remove();
            continue;
        }

        //Create new Terminal
        std::unique_ptr<Terminal> newTerminalPtr = TerminalBuilder(*this, *this, infos.m_terminal.getSide())
                                                        .setBus((infos.m_connected) ? infos.m_connectableBusId : "")
                                                        .setConnectableBus(infos.m_connectableBusId)
                                                        .build();

        //Replace terminal in connectable
        connectable.get().replaceTerminal(infos.m_terminal, std::move(newTerminalPtr), busBreakerTopologyModel);
    }

    // remove busbar sections because not needed in a bus/breaker topology
    for (BusbarSection& bbs : m_topologyModel->getNodeBreakerView().getBusbarSections()) {
        bbs.remove();
    }

    // also here keep the notification for remaining switches removal
    m_topologyModel->removeTopology();
    m_topologyModel = std::move(newTopologyModel);

    return;
}

}  // namespace iidm

}  // namespace powsybl
