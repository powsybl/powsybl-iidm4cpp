/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Subnetwork.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineFilter.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/VoltageAngleLimit.hpp>
#include <powsybl/iidm/VoltageAngleLimitAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>

#include <boost/range/adaptor/map.hpp>
#include <boost/range/join.hpp>

namespace powsybl {

namespace iidm {

Subnetwork::Subnetwork(Network& parentNetwork, const std::string& id, const std::string& sourceFormat) :
    Network(id, sourceFormat),
    m_subBusBreakerView(*this),
    m_subBusView(*this) {
    m_parentNetworkRef = parentNetwork;
    m_filterIdentifiable = [this](const Identifiable& ide){
        return contains(ide);
    };
    m_filterVoltageAngleLimit = [this](const VoltageAngleLimit& val){
        return contains(val.getTerminalFrom().getVoltageLevel()) && contains(val.getTerminalTo().getVoltageLevel());
    };
}

Subnetwork::Subnetwork(Subnetwork&& network) noexcept :
    Network(std::move(network)),
    m_subBusBreakerView(*this),
    m_subBusView(*this) {
    m_filterIdentifiable = [this](const Identifiable& ide){
        return contains(ide);
    };
    m_filterVoltageAngleLimit = [this](const VoltageAngleLimit& val){
        return contains(val.getTerminalFrom().getVoltageLevel()) && contains(val.getTerminalTo().getVoltageLevel());
    };
}

unsigned long Subnetwork::getVariantIndex() const {
    return getRootNetwork().m_variantManager.getVariantIndex();
}
const VariantManager& Subnetwork::getVariantManager() const {
    return getRootNetwork().m_variantManager;
}
VariantManager& Subnetwork::getVariantManager() {
    return getRootNetwork().m_variantManager;
}


stdcxx::CReference<HvdcLine> Subnetwork::findHvdcLine(const HvdcConverterStation& station) const {
    if(contains(station)){
        const auto& filterHdvcStation = [&station](const HvdcLine& hvdcLine) {
            return stdcxx::areSame(hvdcLine.getConverterStation1().get(), station) || stdcxx::areSame(hvdcLine.getConverterStation2().get(), station);
        };
        auto hvdcLines = getHvdcLines() | boost::adaptors::filtered(filterHdvcStation);
        return hvdcLines.empty() ? stdcxx::cref<HvdcLine>() : stdcxx::cref(*hvdcLines.begin());
    }
    return stdcxx::cref<HvdcLine>();
}
stdcxx::Reference<HvdcLine> Subnetwork::findHvdcLine(const HvdcConverterStation& station){
    return stdcxx::ref(const_cast<const Subnetwork*>(this)->findHvdcLine(station));
}

const Battery& Subnetwork::getBattery(const std::string& id) const {
    const Battery& battery = Network::getBattery(id);
    if(!contains(battery)) {
        throw PowsyblException(stdcxx::format("Battery '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return battery;
}
Battery& Subnetwork::getBattery(const std::string& id) {
    return const_cast<Battery&>(static_cast<const Subnetwork*>(this)->getBattery(id));
}
unsigned long Subnetwork::getBatteryCount() const {
    return boost::size(getBatteries());
}
stdcxx::const_range<Battery> Subnetwork::getBatteries() const {
    return getRootNetwork().Network::getBatteries() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<Battery> Subnetwork::getBatteries() {
    return getRootNetwork().Network::getBatteries() | boost::adaptors::filtered(m_filterIdentifiable);
}

const Branch& Subnetwork::getBranch(const std::string& id) const {
    const Branch& branch = Network::getBranch(id);
    if(!contains(branch)) {
        throw PowsyblException(stdcxx::format("Branch '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return branch;
}
Branch& Subnetwork::getBranch(const std::string& id) {
    return const_cast<Branch&>(static_cast<const Subnetwork*>(this)->getBranch(id));
}
unsigned long Subnetwork::getBranchCount() const {
    return boost::size(getBranches());
}
stdcxx::const_range<Branch> Subnetwork::getBranches() const {
    return getRootNetwork().Network::getBranches() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<Branch> Subnetwork::getBranches() {
    return getRootNetwork().Network::getBranches() | boost::adaptors::filtered(m_filterIdentifiable);
}

const BusbarSection& Subnetwork::getBusbarSection(const std::string& id) const {
    const BusbarSection& bbs = Network::getBusbarSection(id);
    if(!contains(bbs)) {
        throw PowsyblException(stdcxx::format("Busbar section '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return bbs;
}
BusbarSection& Subnetwork::getBusbarSection(const std::string& id) {
    return const_cast<BusbarSection&>(static_cast<const Subnetwork*>(this)->getBusbarSection(id));
}
unsigned long Subnetwork::getBusbarSectionCount() const {
    return boost::size(getBusbarSections());
}
stdcxx::const_range<BusbarSection> Subnetwork::getBusbarSections() const {
    return getRootNetwork().Network::getBusbarSections() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<BusbarSection> Subnetwork::getBusbarSections() {
    return getRootNetwork().Network::getBusbarSections() | boost::adaptors::filtered(m_filterIdentifiable);
}

const Network::BusBreakerView& Subnetwork::getBusBreakerView() const {
    return m_subBusBreakerView;
}
Network::BusBreakerView& Subnetwork::getBusBreakerView() {
    return m_subBusBreakerView;
}

const Network::BusView& Subnetwork::getBusView() const {
    return m_subBusView;
}
Network::BusView& Subnetwork::getBusView() {
    return m_subBusView;
}

const DanglingLine& Subnetwork::getDanglingLine(const std::string& id) const {
    const DanglingLine& line = Network::getDanglingLine(id);
    if(!contains(line)) {
        throw PowsyblException(stdcxx::format("Dangling line '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return line;
}
DanglingLine& Subnetwork::getDanglingLine(const std::string& id) {
    return const_cast<DanglingLine&>(static_cast<const Subnetwork*>(this)->getDanglingLine(id));
}
unsigned long Subnetwork::getDanglingLineCount() const {
    return boost::size(getDanglingLines());
}
stdcxx::const_range<DanglingLine> Subnetwork::getDanglingLines(const DanglingLineFilter& filter) const {
    return getDanglingLines() | boost::adaptors::filtered(filter.getPredicate());
}
stdcxx::range<DanglingLine> Subnetwork::getDanglingLines(const DanglingLineFilter& filter) {
    return getDanglingLines() | boost::adaptors::filtered(filter.getPredicate());
}
stdcxx::const_range<DanglingLine> Subnetwork::getDanglingLines() const {
    return getRootNetwork().Network::getDanglingLines() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<DanglingLine> Subnetwork::getDanglingLines() {
    return getRootNetwork().Network::getDanglingLines() | boost::adaptors::filtered(m_filterIdentifiable);
}

const Generator& Subnetwork::getGenerator(const std::string& id) const {
    const Generator& gen = Network::getGenerator(id);
    if(!contains(gen)) {
        throw PowsyblException(stdcxx::format("Generator '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return gen;
}
Generator& Subnetwork::getGenerator(const std::string& id) {
    return const_cast<Generator&>(static_cast<const Subnetwork*>(this)->getGenerator(id));
}
unsigned long Subnetwork::getGeneratorCount() const {
    return boost::size(getGenerators());
}
stdcxx::const_range<Generator> Subnetwork::getGenerators() const {
    return getRootNetwork().Network::getGenerators() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<Generator> Subnetwork::getGenerators() {
    return getRootNetwork().Network::getGenerators() | boost::adaptors::filtered(m_filterIdentifiable);
}

const HvdcConverterStation& Subnetwork::getHvdcConverterStation(const std::string& id) const  {
    const HvdcConverterStation& station = Network::getHvdcConverterStation(id);
    if(!contains(station)) {
        throw PowsyblException(stdcxx::format("HVDC converter station '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return station;
}
HvdcConverterStation& Subnetwork::getHvdcConverterStation(const std::string& id) {
    return const_cast<HvdcConverterStation&>(static_cast<const Subnetwork*>(this)->getHvdcConverterStation(id));
}
unsigned long Subnetwork::getHvdcConverterStationCount() const {
    return boost::size(getHvdcConverterStations());
}
stdcxx::const_range<HvdcConverterStation> Subnetwork::getHvdcConverterStations() const {
    return getRootNetwork().Network::getHvdcConverterStations() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<HvdcConverterStation> Subnetwork::getHvdcConverterStations() {
    return getRootNetwork().Network::getHvdcConverterStations() | boost::adaptors::filtered(m_filterIdentifiable);
}

const HvdcLine& Subnetwork::getHvdcLine(const std::string& id) const {
    const HvdcLine& line = Network::getHvdcLine(id);
    if(!contains(line)) {
        throw PowsyblException(stdcxx::format("HVDC line '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return line;
}
HvdcLine& Subnetwork::getHvdcLine(const std::string& id) {
    return const_cast<HvdcLine&>(static_cast<const Subnetwork*>(this)->getHvdcLine(id));
}
const HvdcLine& Subnetwork::getHvdcLine(const HvdcConverterStation& station) const {
    auto hvdcLine = findHvdcLine(station);
    if (!hvdcLine) {
        throw PowsyblException(stdcxx::format("Unable to find to the HVDC line for station '%1%' in subnetwork '%2%'", station.getId(), getId()));
    }
    return hvdcLine.get();
}
HvdcLine& Subnetwork::getHvdcLine(const HvdcConverterStation& station) {
    return const_cast<HvdcLine&>(static_cast<const Subnetwork*>(this)->getHvdcLine(station));
}
unsigned long Subnetwork::getHvdcLineCount() const {
    return boost::size(getHvdcLines());
}
stdcxx::const_range<HvdcLine> Subnetwork::getHvdcLines() const {
    return getRootNetwork().Network::getHvdcLines() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<HvdcLine> Subnetwork::getHvdcLines() {
    return getRootNetwork().Network::getHvdcLines() | boost::adaptors::filtered(m_filterIdentifiable);
}

const Identifiable& Subnetwork::getIdentifiable(const std::string& id) const {
    const Identifiable& identifiable = Network::getIdentifiable(id);
    if(!contains(identifiable)) {
        throw PowsyblException(stdcxx::format("Identifiable '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return identifiable;
}
Identifiable& Subnetwork::getIdentifiable(const std::string& id) {
    return const_cast<Identifiable&>(static_cast<const Subnetwork*>(this)->getIdentifiable(id));
}
stdcxx::const_range<Identifiable> Subnetwork::getIdentifiables() const {
    return getRootNetwork().Network::getIdentifiables() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<Identifiable> Subnetwork::getIdentifiables() {
    return getRootNetwork().Network::getIdentifiables() | boost::adaptors::filtered(m_filterIdentifiable);
}

const LccConverterStation& Subnetwork::getLccConverterStation(const std::string& id) const {
    const LccConverterStation& lcc = Network::getLccConverterStation(id);
    if(!contains(lcc)) {
        throw PowsyblException(stdcxx::format("LccConverterStation '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return lcc;
}
LccConverterStation& Subnetwork::getLccConverterStation(const std::string& id)  {
    return const_cast<LccConverterStation&>(static_cast<const Subnetwork*>(this)->getLccConverterStation(id));
}
unsigned long Subnetwork::getLccConverterStationCount() const {
    return boost::size(getLccConverterStations());
}
stdcxx::const_range<LccConverterStation> Subnetwork::getLccConverterStations() const {
    return getRootNetwork().Network::getLccConverterStations() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<LccConverterStation> Subnetwork::getLccConverterStations() {
    return getRootNetwork().Network::getLccConverterStations() | boost::adaptors::filtered(m_filterIdentifiable);
}

const Line& Subnetwork::getLine(const std::string& id) const {
    const Line& line = Network::getLine(id);
    if(!contains(line)) {
        throw PowsyblException(stdcxx::format("Line '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return line;
}
Line& Subnetwork::getLine(const std::string& id)  {
    return const_cast<Line&>(static_cast<const Subnetwork*>(this)->getLine(id));
}
unsigned long Subnetwork::getLineCount() const {
    return boost::size(getLines());
}
stdcxx::const_range<Line> Subnetwork::getLines() const {
    return getRootNetwork().Network::getLines() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<Line> Subnetwork::getLines() {
    return getRootNetwork().Network::getLines() | boost::adaptors::filtered(m_filterIdentifiable);
}

const TieLine& Subnetwork::getTieLine(const std::string& id) const {
    const TieLine& line = Network::getTieLine(id);
    if(!contains(line)) {
        throw PowsyblException(stdcxx::format("Tie line '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return line;
}
TieLine& Subnetwork::getTieLine(const std::string& id)  {
    return const_cast<TieLine&>(static_cast<const Subnetwork*>(this)->getTieLine(id));
}
unsigned long Subnetwork::getTieLineCount() const {
    return boost::size(getTieLines());
}
stdcxx::const_range<TieLine> Subnetwork::getTieLines() const {
    return getRootNetwork().Network::getTieLines() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<TieLine> Subnetwork::getTieLines() {
    return getRootNetwork().Network::getTieLines() | boost::adaptors::filtered(m_filterIdentifiable);
}

const Load& Subnetwork::getLoad(const std::string& id) const {
    const Load& load = Network::getLoad(id);
    if(!contains(load)) {
        throw PowsyblException(stdcxx::format("Load '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return load;
}
Load& Subnetwork::getLoad(const std::string& id)  {
    return const_cast<Load&>(static_cast<const Subnetwork*>(this)->getLoad(id));
}
unsigned long Subnetwork::getLoadCount() const {
    return boost::size(getLoads());
}
stdcxx::const_range<Load> Subnetwork::getLoads() const {
    return getRootNetwork().Network::getLoads() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<Load> Subnetwork::getLoads() {
    return getRootNetwork().Network::getLoads() | boost::adaptors::filtered(m_filterIdentifiable);
}

const ShuntCompensator& Subnetwork::getShuntCompensator(const std::string& id) const {
    const ShuntCompensator& shunt = Network::getShuntCompensator(id);
    if(!contains(shunt)) {
        throw PowsyblException(stdcxx::format("Shunt compensator '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return shunt;
}
ShuntCompensator& Subnetwork::getShuntCompensator(const std::string& id)  {
    return const_cast<ShuntCompensator&>(static_cast<const Subnetwork*>(this)->getShuntCompensator(id));
}
unsigned long Subnetwork::getShuntCompensatorCount() const {
    return boost::size(getShuntCompensators());
}
stdcxx::const_range<ShuntCompensator> Subnetwork::getShuntCompensators() const {
    return getRootNetwork().Network::getShuntCompensators() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<ShuntCompensator> Subnetwork::getShuntCompensators() {
    return getRootNetwork().Network::getShuntCompensators() | boost::adaptors::filtered(m_filterIdentifiable);
}

const StaticVarCompensator& Subnetwork::getStaticVarCompensator(const std::string& id) const {
    const StaticVarCompensator& svc = Network::getStaticVarCompensator(id);
    if(!contains(svc)) {
        throw PowsyblException(stdcxx::format("StaticVarCompensator '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return svc;
}
StaticVarCompensator& Subnetwork::getStaticVarCompensator(const std::string& id)  {
    return const_cast<StaticVarCompensator&>(static_cast<const Subnetwork*>(this)->getStaticVarCompensator(id));
}
unsigned long Subnetwork::getStaticVarCompensatorCount() const {
    return boost::size(getStaticVarCompensators());
}
stdcxx::const_range<StaticVarCompensator> Subnetwork::getStaticVarCompensators() const {
    return getRootNetwork().Network::getStaticVarCompensators() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<StaticVarCompensator> Subnetwork::getStaticVarCompensators() {
    return getRootNetwork().Network::getStaticVarCompensators() | boost::adaptors::filtered(m_filterIdentifiable);
}

stdcxx::CReference<Network> Subnetwork::getSubNetwork(const std::string& /*id*/) const {
    return stdcxx::CReference<Network>();
}
stdcxx::Reference<Network> Subnetwork::getSubNetwork(const std::string& /*id*/) {
    return stdcxx::Reference<Network>();
}
unsigned long Subnetwork::getSubNetworksCount() const {
    return 0;
}
stdcxx::const_range<Network> Subnetwork::getSubNetworks() const {
    return stdcxx::const_range<Network>();
}
stdcxx::range<Network> Subnetwork::getSubNetworks() {
    return stdcxx::range<Network>();
}

const Substation& Subnetwork::getSubstation(const std::string& id) const {
    const Substation& sub = Network::getSubstation(id);
    if(!contains(sub)) {
        throw PowsyblException(stdcxx::format("Substation '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return sub;
}
Substation& Subnetwork::getSubstation(const std::string& id) {
    return const_cast<Substation&>(static_cast<const Subnetwork*>(this)->getSubstation(id));
}
unsigned long Subnetwork::getSubstationCount() const {
    return boost::size(getSubstations());
}
stdcxx::const_range<Substation> Subnetwork::getSubstations() const {
    return getRootNetwork().Network::getSubstations() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<Substation> Subnetwork::getSubstations() {
    return getRootNetwork().Network::getSubstations() | boost::adaptors::filtered(m_filterIdentifiable);
}

const Switch& Subnetwork::getSwitch(const std::string& id) const {
    const Switch& sw = Network::getSwitch(id);
    if(!contains(sw)) {
        throw PowsyblException(stdcxx::format("Switch '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return sw;
}
Switch& Subnetwork::getSwitch(const std::string& id) {
    return const_cast<Switch&>(static_cast<const Subnetwork*>(this)->getSwitch(id));
}
unsigned long Subnetwork::getSwitchCount() const {
    return boost::size(getSwitches());
}
stdcxx::const_range<Switch> Subnetwork::getSwitches() const {
    return getRootNetwork().Network::getSwitches() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<Switch> Subnetwork::getSwitches() {
    return getRootNetwork().Network::getSwitches() | boost::adaptors::filtered(m_filterIdentifiable);
}

const ThreeWindingsTransformer& Subnetwork::getThreeWindingsTransformer(const std::string& id) const {
    const ThreeWindingsTransformer& twt = Network::getThreeWindingsTransformer(id);
    if(!contains(twt)) {
        throw PowsyblException(stdcxx::format("Three windings transformer '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return twt;
}
ThreeWindingsTransformer& Subnetwork::getThreeWindingsTransformer(const std::string& id) {
    return const_cast<ThreeWindingsTransformer&>(static_cast<const Subnetwork*>(this)->getThreeWindingsTransformer(id));
}
unsigned long Subnetwork::getThreeWindingsTransformerCount() const {
    return boost::size(getThreeWindingsTransformers());
}
stdcxx::const_range<ThreeWindingsTransformer> Subnetwork::getThreeWindingsTransformers() const {
    return getRootNetwork().Network::getThreeWindingsTransformers() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<ThreeWindingsTransformer> Subnetwork::getThreeWindingsTransformers() {
    return getRootNetwork().Network::getThreeWindingsTransformers() | boost::adaptors::filtered(m_filterIdentifiable);
}

const TwoWindingsTransformer& Subnetwork::getTwoWindingsTransformer(const std::string& id) const {
    const TwoWindingsTransformer& twt = Network::getTwoWindingsTransformer(id);
    if(!contains(twt)) {
        throw PowsyblException(stdcxx::format("Two windings transformer '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return twt;
}
TwoWindingsTransformer& Subnetwork::getTwoWindingsTransformer(const std::string& id) {
    return const_cast<TwoWindingsTransformer&>(static_cast<const Subnetwork*>(this)->getTwoWindingsTransformer(id));
}
unsigned long Subnetwork::getTwoWindingsTransformerCount() const {
    return boost::size(getTwoWindingsTransformers());
}
stdcxx::const_range<TwoWindingsTransformer> Subnetwork::getTwoWindingsTransformers() const {
    return getRootNetwork().Network::getTwoWindingsTransformers() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<TwoWindingsTransformer> Subnetwork::getTwoWindingsTransformers() {
    return getRootNetwork().Network::getTwoWindingsTransformers() | boost::adaptors::filtered(m_filterIdentifiable);
}

const VoltageAngleLimit& Subnetwork::getVoltageAngleLimit(const std::string& id) const {
    const VoltageAngleLimit& val = getRootNetwork().Network::getVoltageAngleLimit(id);
    if(!m_filterVoltageAngleLimit(val)) {
        throw PowsyblException(stdcxx::format("Voltage angle limit '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return val;
}
VoltageAngleLimit& Subnetwork::getVoltageAngleLimit(const std::string& id) {
    return const_cast<VoltageAngleLimit&>(static_cast<const Subnetwork*>(this)->getVoltageAngleLimit(id));
}
stdcxx::const_range<VoltageAngleLimit> Subnetwork::getVoltageAngleLimits() const {
    return getRootNetwork().Network::getVoltageAngleLimits() | boost::adaptors::filtered(m_filterVoltageAngleLimit);
}
stdcxx::range<VoltageAngleLimit> Subnetwork::getVoltageAngleLimits() {
    return getRootNetwork().Network::getVoltageAngleLimits() | boost::adaptors::filtered(m_filterVoltageAngleLimit);
}
std::map<std::string, VoltageAngleLimit>& Subnetwork::getVoltageAngleLimitsIndex() {
    return getRootNetwork().m_voltageAngleLimitsIndex;
}

const VoltageLevel& Subnetwork::getVoltageLevel(const std::string& id) const {
    const VoltageLevel& vl = Network::getVoltageLevel(id);
    if(!contains(vl)) {
        throw PowsyblException(stdcxx::format("Voltage level '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return vl;
}
VoltageLevel& Subnetwork::getVoltageLevel(const std::string& id) {
    return const_cast<VoltageLevel&>(static_cast<const Subnetwork*>(this)->getVoltageLevel(id));
}
unsigned long Subnetwork::getVoltageLevelCount() const {
    return boost::size(getVoltageLevels());
}
stdcxx::const_range<VoltageLevel> Subnetwork::getVoltageLevels() const {
    return getRootNetwork().Network::getVoltageLevels() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<VoltageLevel> Subnetwork::getVoltageLevels() {
    return getRootNetwork().Network::getVoltageLevels() | boost::adaptors::filtered(m_filterIdentifiable);
}

const VscConverterStation& Subnetwork::getVscConverterStation(const std::string& id) const {
    const VscConverterStation& vsc = Network::getVscConverterStation(id);
    if(!contains(vsc)) {
        throw PowsyblException(stdcxx::format("VscConverterStation '%1%' does not belong to the subnetwork '%2%'", id, getId()));
    }
    return vsc;
}
VscConverterStation& Subnetwork::getVscConverterStation(const std::string& id) {
    return const_cast<VscConverterStation&>(static_cast<const Subnetwork*>(this)->getVscConverterStation(id));
}
unsigned long Subnetwork::getVscConverterStationCount() const {
    return boost::size(getVscConverterStations());
}
stdcxx::const_range<VscConverterStation> Subnetwork::getVscConverterStations() const {
    return getRootNetwork().Network::getVscConverterStations() | boost::adaptors::filtered(m_filterIdentifiable);
}
stdcxx::range<VscConverterStation> Subnetwork::getVscConverterStations() {
    return getRootNetwork().Network::getVscConverterStations() | boost::adaptors::filtered(m_filterIdentifiable);
}

HvdcLineAdder Subnetwork::newHvdcLine() {
    return HvdcLineAdder(getRootNetwork(), getId());
}
HvdcLineAdder Subnetwork::newHvdcLine(const std::string& subNetworkId) {
    return HvdcLineAdder(getRootNetwork(), subNetworkId);
}
LineAdder Subnetwork::newLine() {
    return LineAdder(getRootNetwork(), getId());
}
LineAdder Subnetwork::newLine(const std::string& subNetworkId) {
    return LineAdder(getRootNetwork(), subNetworkId);
}
Network& Subnetwork::newSubnetwork(const std::string& /*id*/, const std::string& /*sourceFormat*/) {
    throw PowsyblException(stdcxx::format("Unsupported operation, Inner subnetworks are not supported"));
}
SubstationAdder Subnetwork::newSubstation() {
    return SubstationAdder(getRootNetwork(), *this);
}
TieLineAdder Subnetwork::newTieLine() {
    return TieLineAdder(getRootNetwork(), getId());
}
TieLineAdder Subnetwork::newTieLine(const std::string& subNetworkId) {
    return TieLineAdder(getRootNetwork(), subNetworkId);
}
VoltageAngleLimitAdder Subnetwork::newVoltageAngleLimit() {
    return VoltageAngleLimitAdder(getRootNetwork(), getId());
}
VoltageAngleLimitAdder Subnetwork::newVoltageAngleLimit(const std::string& subNetworkId) {
    return VoltageAngleLimitAdder(getRootNetwork(), subNetworkId);
}
VoltageLevelAdder Subnetwork::newVoltageLevel() {
    return VoltageLevelAdder(getRootNetwork(), *this);
}

ValidationLevel Subnetwork::runValidationChecks() {
    return getRootNetwork().Network::runValidationChecks();
}
ValidationLevel Subnetwork::runValidationChecks(const ValidationLevel& vl) {
    return getRootNetwork().Network::runValidationChecks(vl);
}
const ValidationLevel& Subnetwork::validate() {
    return getRootNetwork().Network::validate();
}
ValidationLevel Subnetwork::getValidationLevel() const {
    return getRootNetwork().Network::getValidationLevel();
}
Network& Subnetwork::setMinimumAcceptableValidationLevel(const ValidationLevel& minimumValidationLevel) {
    return getRootNetwork().Network::setMinimumAcceptableValidationLevel(minimumValidationLevel);
}
const ValidationLevel& Subnetwork::getMinimumValidationLevel() const {
    return getRootNetwork().Network::getMinimumValidationLevel();
}
Network& Subnetwork::setValidationLevelIfGreaterThan(const ValidationLevel& vl) {
    return getRootNetwork().Network::setValidationLevelIfGreaterThan(vl);
}
Network& Subnetwork::invalidateValidationLevel() {
    getRootNetwork().Network::invalidateValidationLevel();
    return *this;
}

const network::VariantArray& Subnetwork::getVariants() const {
    return getRootNetwork().m_variants;
}
network::VariantArray& Subnetwork::getVariants() {
    return getRootNetwork().m_variants;
}

const NetworkIndex& Subnetwork::getIndex() const {
    return getRootNetwork().m_networkIndex;
}
NetworkIndex& Subnetwork::getIndex() {
    return getRootNetwork().m_networkIndex;
}


}  // namespace iidm

}  // namespace powsybl
