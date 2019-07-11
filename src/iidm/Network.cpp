/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <unordered_set>

#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/stdcxx/hash.hpp>

#include "converter/xml/NetworkXml.hpp"

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

Network Network::readXml(std::istream& istream) {
    return readXml(istream, converter::ImportOptions(), stdcxx::CReference<converter::Anonymizer>());
}

Network Network::readXml(std::istream& istream, const converter::ImportOptions& options, const stdcxx::CReference<converter::Anonymizer>& anonymizer) {
    return converter::xml::NetworkXml::read(istream, options, anonymizer);
}

std::unique_ptr<converter::Anonymizer> Network::writeXml(std::ostream& ostream, const Network& network) {
    converter::ExportOptions options;
    return writeXml(ostream, network, options);
}

std::unique_ptr<converter::Anonymizer> Network::writeXml(std::ostream& ostream, const Network& network, const converter::ExportOptions& options) {
    return converter::xml::NetworkXml::write(ostream, network, options);
}

Network::Network(const std::string& id, const std::string& sourceFormat) :
    Container(id, id, Container::Type::NETWORK),
    m_forecastDistance(0),
    m_sourceFormat(checkNotEmpty(*this, sourceFormat, "Source format is empty")),
    m_variantManager(*this) {
}

const Battery& Network::getBattery(const std::string& id) const {
    return get<Battery>(id);
}

Battery& Network::getBattery(const std::string& id) {
    return get<Battery>(id);
}

unsigned long Network::getBatteryCount() const {
    return getObjectCount<Battery>();
}

Network::const_range<Battery> Network::getBatteries() const {
    return m_networkIndex.getAll<Battery>();
}

Network::range<Battery> Network::getBatteries() {
    return m_networkIndex.getAll<Battery>();
}

const Branch& Network::getBranch(const std::string& id) const {
    return get<Branch>(id);
}

Branch& Network::getBranch(const std::string& id) {
    return get<Branch>(id);
}

unsigned long Network::getBranchCount() const {
    return getLineCount() + getTwoWindingsTransformerCount();
}

Network::const_range<Branch> Network::getBranches() const {
    return boost::range::join(
        boost::range::join(
            m_networkIndex.getAll<Line, Branch>(),
            m_networkIndex.getAll<TieLine, Branch>()),
        m_networkIndex.getAll<TwoWindingsTransformer, Branch>());
}

Network::range<Branch> Network::getBranches() {
    return boost::range::join(
        boost::range::join(
            m_networkIndex.getAll<Line, Branch>(),
            m_networkIndex.getAll<TieLine, Branch>()),
        m_networkIndex.getAll<TwoWindingsTransformer, Branch>());
}

const BusbarSection& Network::getBusbarSection(const std::string& id) const {
    return get<BusbarSection>(id);
}

BusbarSection& Network::getBusbarSection(const std::string& id) {
    return get<BusbarSection>(id);
}

unsigned long Network::getBusbarSectionCount() const {
    return getObjectCount<BusbarSection>();
}

Network::const_range<BusbarSection> Network::getBusbarSections() const {
    return m_networkIndex.getAll<BusbarSection>();
}

Network::range<BusbarSection> Network::getBusbarSections() {
    return m_networkIndex.getAll<BusbarSection>();
}

const stdcxx::DateTime& Network::getCaseDate() const {
    return m_caseDate;
}

unsigned long Network::getCountryCount() const {
    std::unordered_set<Country, stdcxx::hash<Country>> countries;
    for (const auto& substation : getSubstations()) {
        const stdcxx::optional<Country>& country = substation.get().getCountry();
        if (country) {
            countries.emplace(*country);
        }
    }

    return countries.size();
}

const DanglingLine& Network::getDanglingLine(const std::string& id) const {
    return get<DanglingLine>(id);
}

DanglingLine& Network::getDanglingLine(const std::string& id) {
    return get<DanglingLine>(id);
}

unsigned long Network::getDanglingLineCount() const {
    return getObjectCount<DanglingLine>();
}

Network::const_range<DanglingLine> Network::getDanglingLines() const {
    return m_networkIndex.getAll<DanglingLine>();
}

Network::range<DanglingLine> Network::getDanglingLines() {
    return m_networkIndex.getAll<DanglingLine>();
}

int Network::getForecastDistance() const {
    return m_forecastDistance;
}

const Generator& Network::getGenerator(const std::string& id) const {
    return get<Generator>(id);
}

Generator& Network::getGenerator(const std::string& id) {
    return get<Generator>(id);
}

unsigned long Network::getGeneratorCount() const {
    return getObjectCount<Generator>();
}

Network::const_range<Generator> Network::getGenerators() const {
    return m_networkIndex.getAll<Generator>();
}

Network::range<Generator> Network::getGenerators() {
    return m_networkIndex.getAll<Generator>();
}

const HvdcConverterStation& Network::getHvdcConverterStation(const std::string& id) const {
    return get<HvdcConverterStation>(id);
}

HvdcConverterStation& Network::getHvdcConverterStation(const std::string& id) {
    return get<HvdcConverterStation>(id);
}

unsigned long Network::getHvdcConverterStationCount() const {
    return getLccConverterStationCount() + getVscConverterStationCount();
}

Network::const_range<HvdcConverterStation> Network::getHvdcConverterStations() const {
    return boost::range::join(
        m_networkIndex.getAll<LccConverterStation, HvdcConverterStation>(),
        m_networkIndex.getAll<VscConverterStation, HvdcConverterStation>());
}

Network::range<HvdcConverterStation> Network::getHvdcConverterStations() {
    return boost::range::join(
        m_networkIndex.getAll<LccConverterStation, HvdcConverterStation>(),
        m_networkIndex.getAll<VscConverterStation, HvdcConverterStation>());
}

const HvdcLine& Network::getHvdcLine(const std::string& id) const {
    return get<HvdcLine>(id);
}

HvdcLine& Network::getHvdcLine(const std::string& id) {
    return get<HvdcLine>(id);
}

unsigned long Network::getHvdcLineCount() const {
    return getObjectCount<HvdcLine>();
}

Network::const_range<HvdcLine> Network::getHvdcLines() const {
    return m_networkIndex.getAll<HvdcLine>();
}

Network::range<HvdcLine> Network::getHvdcLines() {
    return m_networkIndex.getAll<HvdcLine>();
}

Network::const_range<Identifiable> Network::getIdentifiables() const {
    return m_networkIndex.getAll<Identifiable>();
}

Network::range<Identifiable> Network::getIdentifiables() {
    return m_networkIndex.getAll<Identifiable>();
}

const LccConverterStation& Network::getLccConverterStation(const std::string& id) const {
    return get<LccConverterStation>(id);
}

LccConverterStation& Network::getLccConverterStation(const std::string& id) {
    return get<LccConverterStation>(id);
}

unsigned long Network::getLccConverterStationCount() const {
    return getObjectCount<LccConverterStation>();
}

Network::const_range<LccConverterStation> Network::getLccConverterStations() const {
    return m_networkIndex.getAll<LccConverterStation>();
}

Network::range<LccConverterStation> Network::getLccConverterStations() {
    return m_networkIndex.getAll<LccConverterStation>();
}

const Line& Network::getLine(const std::string& id) const {
    return get<Line>(id);
}

Line& Network::getLine(const std::string& id) {
    return get<Line>(id);
}

unsigned long Network::getLineCount() const {
    return getObjectCount<Line>() + getObjectCount<TieLine>();
}

Network::const_range<Line> Network::getLines() const {
    return boost::range::join(
        m_networkIndex.getAll<Line>(),
        m_networkIndex.getAll<TieLine, Line>());
}

Network::range<Line> Network::getLines() {
    return boost::range::join(
        m_networkIndex.getAll<Line>(),
        m_networkIndex.getAll<TieLine, Line>());
}

const Load& Network::getLoad(const std::string& id) const {
    return get<Load>(id);
}

Load& Network::getLoad(const std::string& id) {
    return get<Load>(id);
}

unsigned long Network::getLoadCount() const {
    return getObjectCount<Load>();
}

Network::const_range<Load> Network::getLoads() const {
    return m_networkIndex.getAll<Load>();
}

Network::range<Load> Network::getLoads() {
    return m_networkIndex.getAll<Load>();
}

const ShuntCompensator& Network::getShuntCompensator(const std::string& id) const {
    return get<ShuntCompensator>(id);
}

ShuntCompensator& Network::getShuntCompensator(const std::string& id) {
    return get<ShuntCompensator>(id);
}

unsigned long Network::getShuntCompensatorCount() const {
    return getObjectCount<ShuntCompensator>();
}

Network::const_range<ShuntCompensator> Network::getShuntCompensators() const {
    return m_networkIndex.getAll<ShuntCompensator>();
}

Network::range<ShuntCompensator> Network::getShuntCompensators() {
    return m_networkIndex.getAll<ShuntCompensator>();
}

const std::string& Network::getSourceFormat() const {
    return m_sourceFormat;
}

Network::const_range<MultiVariantObject> Network::getStatefulObjects() const {
    return m_networkIndex.getAll<Identifiable, MultiVariantObject>();
}

Network::range<MultiVariantObject> Network::getStatefulObjects() {
    return m_networkIndex.getAll<Identifiable, MultiVariantObject>();
}

const StaticVarCompensator& Network::getStaticVarCompensator(const std::string& id) const {
    return get<StaticVarCompensator>(id);
}

StaticVarCompensator& Network::getStaticVarCompensator(const std::string& id) {
    return get<StaticVarCompensator>(id);
}

unsigned long Network::getStaticVarCompensatorCount() const {
    return getObjectCount<StaticVarCompensator>();
}

Network::const_range<StaticVarCompensator> Network::getStaticVarCompensators() const {
    return m_networkIndex.getAll<StaticVarCompensator>();
}

Network::range<StaticVarCompensator> Network::getStaticVarCompensators() {
    return m_networkIndex.getAll<StaticVarCompensator>();
}

const Substation& Network::getSubstation(const std::string& id) const {
    return get<Substation>(id);
}

Substation& Network::getSubstation(const std::string& id) {
    return get<Substation>(id);
}

unsigned long Network::getSubstationCount() const {
    return getObjectCount<Substation>();
}

Network::const_range<Substation> Network::getSubstations() const {
    return m_networkIndex.getAll<Substation>();
}

Network::range<Substation> Network::getSubstations() {
    return m_networkIndex.getAll<Substation>();
}

const Switch& Network::getSwitch(const std::string& id) const {
    return get<Switch>(id);
}

Switch& Network::getSwitch(const std::string& id) {
    return get<Switch>(id);
}

unsigned long Network::getSwitchCount() const {
    return getObjectCount<Switch>();
}

Network::const_range<Switch> Network::getSwitches() const {
    return m_networkIndex.getAll<Switch>();
}

Network::range<Switch> Network::getSwitches() {
    return m_networkIndex.getAll<Switch>();
}

const ThreeWindingsTransformer& Network::getThreeWindingsTransformer(const std::string& id) const {
    return get<ThreeWindingsTransformer>(id);
}

ThreeWindingsTransformer& Network::getThreeWindingsTransformer(const std::string& id) {
    return get<ThreeWindingsTransformer>(id);
}

unsigned long Network::getThreeWindingsTransformerCount() const {
    return getObjectCount<ThreeWindingsTransformer>();
}

Network::const_range<ThreeWindingsTransformer> Network::getThreeWindingsTransformers() const {
    return m_networkIndex.getAll<ThreeWindingsTransformer>();
}

Network::range<ThreeWindingsTransformer> Network::getThreeWindingsTransformers() {
    return m_networkIndex.getAll<ThreeWindingsTransformer>();
}

const TwoWindingsTransformer& Network::getTwoWindingsTransformer(const std::string& id) const {
    return get<TwoWindingsTransformer>(id);
}

TwoWindingsTransformer& Network::getTwoWindingsTransformer(const std::string& id) {
    return get<TwoWindingsTransformer>(id);
}

unsigned long Network::getTwoWindingsTransformerCount() const {
    return getObjectCount<TwoWindingsTransformer>();
}

Network::const_range<TwoWindingsTransformer> Network::getTwoWindingsTransformers() const {
    return m_networkIndex.getAll<TwoWindingsTransformer>();
}

Network::range<TwoWindingsTransformer> Network::getTwoWindingsTransformers() {
    return m_networkIndex.getAll<TwoWindingsTransformer>();
}

const std::string& Network::getTypeDescription() const {
    static std::string s_typeDescription = "Network";

    return s_typeDescription;
}

unsigned long Network::getVariantIndex() const {
    return m_variantManager.getVariantIndex();
}

const VariantManager& Network::getVariantManager() const {
    return m_variantManager;
}

VariantManager& Network::getVariantManager() {
    return m_variantManager;
}

const VoltageLevel& Network::getVoltageLevel(const std::string& id) const {
    return get<VoltageLevel>(id);
}

VoltageLevel& Network::getVoltageLevel(const std::string& id) {
    return get<VoltageLevel>(id);
}

unsigned long Network::getVoltageLevelCount() const {
    return getObjectCount<VoltageLevel>();
}

Network::const_range<VoltageLevel> Network::getVoltageLevels() const {
    return m_networkIndex.getAll<VoltageLevel>();
}

Network::range<VoltageLevel> Network::getVoltageLevels() {
    return m_networkIndex.getAll<VoltageLevel>();
}

const VscConverterStation& Network::getVscConverterStation(const std::string& id) const {
    return get<VscConverterStation>(id);
}

VscConverterStation& Network::getVscConverterStation(const std::string& id) {
    return get<VscConverterStation>(id);
}

Network::const_range<VscConverterStation> Network::getVscConverterStations() const {
    return m_networkIndex.getAll<VscConverterStation>();
}

Network::range<VscConverterStation> Network::getVscConverterStations() {
    return m_networkIndex.getAll<VscConverterStation>();
}

unsigned long Network::getVscConverterStationCount() const {
    return getObjectCount<VscConverterStation>();
}

HvdcLineAdder Network::newHvdcLine() {
    return HvdcLineAdder(*this);
}

LineAdder Network::newLine() {
    return LineAdder(*this);
}

SubstationAdder Network::newSubstation() {
    return SubstationAdder(*this);
}

TieLineAdder Network::newTieLine() {
    return TieLineAdder(*this);
}

void Network::remove(Identifiable& identifiable) {
    m_networkIndex.remove(identifiable);
}


Network& Network::setCaseDate(const stdcxx::DateTime& caseDate) {
    m_caseDate = caseDate;
    return *this;
}

Network& Network::setForecastDistance(int forecastDistance) {
    m_forecastDistance = checkForecastDistance(*this, forecastDistance);
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
