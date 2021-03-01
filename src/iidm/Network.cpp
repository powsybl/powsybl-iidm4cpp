/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Network.hpp>

#include <sstream>
#include <unordered_set>

#include <boost/filesystem/fstream.hpp>
#include <boost/range/join.hpp>

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
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>

#include <powsybl/stdcxx/Properties.hpp>
#include <powsybl/stdcxx/hash.hpp>

#include "converter/xml/NetworkXml.hpp"

namespace powsybl {

namespace iidm {

Network Network::readXml(const boost::filesystem::path& path, const converter::ImportOptions& options) {
    boost::filesystem::ifstream is(path);
    if (!is.is_open()) {
        throw PowsyblException(stdcxx::format("Unable to open file '%1%' for reading", path.string()));
    }
    return converter::xml::NetworkXml::read(path.string(), is, options);
}

Network Network::readXml(const std::string& filename, std::istream& istream, const converter::ImportOptions& options) {
    return converter::xml::NetworkXml::read(filename, istream, options);
}

void Network::writeXml(const boost::filesystem::path& path, const Network& network, const converter::ExportOptions& options) {
    boost::filesystem::ofstream os(path);
    if (!os.is_open()) {
        throw PowsyblException(stdcxx::format("Unable to open file '%1%' for writing", path.string()));
    }
    converter::xml::NetworkXml::write(path.string(), os, network, options);
}

void Network::writeXml(const std::string& filename, std::ostream& ostream, const Network& network, const converter::ExportOptions& options) {
    converter::xml::NetworkXml::write(filename, ostream, network, options);
}

Network::Network(const std::string& id, const std::string& sourceFormat) :
    Container(id, id, false, Container::Type::NETWORK),
    m_sourceFormat(checkNotEmpty(*this, sourceFormat, "Source format is empty")),
    m_variantManager(*this),
    m_variants(*this, [this]() { return stdcxx::make_unique<network::VariantImpl>(*this); }),
    m_busBreakerView(*this),
    m_busView(*this) {
    checkAndAdd(std::unique_ptr<Network>(this));
}

Network::Network(Network&& network) noexcept :
    Container(std::move(network)),
    VariantManagerHolder(std::move(network)),
    m_caseDate(std::move(network.m_caseDate)),
    m_forecastDistance(network.m_forecastDistance),
    m_sourceFormat(std::move(network.m_sourceFormat)),
    m_networkIndex(*this, std::move(network.m_networkIndex)),
    m_variantManager(*this, std::move(network.m_variantManager)),
    m_variants(*this, std::move(network.m_variants)),
    m_busBreakerView(*this),
    m_busView(*this) {
}

void Network::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Container::allocateVariantArrayElement(indexes, sourceIndex);

    m_variants.allocateVariantArrayElement(indexes, [this, sourceIndex]() { return m_variants.copy(sourceIndex); });
}

void Network::deleteVariantArrayElement(unsigned long index) {
    Container::deleteVariantArrayElement(index);

    m_variants.deleteVariantArrayElement(index);
}

void Network::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Container::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_variants.extendVariantArraySize(initVariantArraySize, number, [this, sourceIndex]() { return m_variants.copy(sourceIndex); });
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

stdcxx::const_range<Battery> Network::getBatteries() const {
    return m_networkIndex.getAll<Battery>();
}

stdcxx::range<Battery> Network::getBatteries() {
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

stdcxx::const_range<Branch> Network::getBranches() const {
    return boost::range::join(
        boost::range::join(
            m_networkIndex.getAll<Line, Branch>(),
            m_networkIndex.getAll<TieLine, Branch>()),
        m_networkIndex.getAll<TwoWindingsTransformer, Branch>());
}

stdcxx::range<Branch> Network::getBranches() {
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

stdcxx::const_range<BusbarSection> Network::getBusbarSections() const {
    return m_networkIndex.getAll<BusbarSection>();
}

stdcxx::range<BusbarSection> Network::getBusbarSections() {
    return m_networkIndex.getAll<BusbarSection>();
}

const Network::BusBreakerView& Network::getBusBreakerView() const {
    return m_busBreakerView;
}

Network::BusBreakerView& Network::getBusBreakerView() {
    return m_busBreakerView;
}

const Network::BusView& Network::getBusView() const {
    return m_busView;
}

Network::BusView& Network::getBusView() {
    return m_busView;
}

const stdcxx::DateTime& Network::getCaseDate() const {
    return m_caseDate;
}

const ConnectedComponentsManager& Network::getConnectedComponentsManager() const {
    return m_variants.get().getConnectedComponentsManager();
}

ConnectedComponentsManager& Network::getConnectedComponentsManager() {
    return m_variants.get().getConnectedComponentsManager();
}

unsigned long Network::getCountryCount() const {
    std::unordered_set<Country, stdcxx::hash<Country>> countries;
    for (const auto& substation : getSubstations()) {
        const stdcxx::optional<Country>& country = substation.getCountry();
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

stdcxx::const_range<DanglingLine> Network::getDanglingLines() const {
    return m_networkIndex.getAll<DanglingLine>();
}

stdcxx::range<DanglingLine> Network::getDanglingLines() {
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

stdcxx::const_range<Generator> Network::getGenerators() const {
    return m_networkIndex.getAll<Generator>();
}

stdcxx::range<Generator> Network::getGenerators() {
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

stdcxx::const_range<HvdcConverterStation> Network::getHvdcConverterStations() const {
    return boost::range::join(
        m_networkIndex.getAll<LccConverterStation, HvdcConverterStation>(),
        m_networkIndex.getAll<VscConverterStation, HvdcConverterStation>());
}

stdcxx::range<HvdcConverterStation> Network::getHvdcConverterStations() {
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

stdcxx::const_range<HvdcLine> Network::getHvdcLines() const {
    return m_networkIndex.getAll<HvdcLine>();
}

stdcxx::range<HvdcLine> Network::getHvdcLines() {
    return m_networkIndex.getAll<HvdcLine>();
}

const Identifiable& Network::getIdentifiable(const std::string& id) const {
    return get<Identifiable>(id);
}

Identifiable& Network::getIdentifiable(const std::string& id) {
    return get<Identifiable>(id);
}

stdcxx::const_range<Identifiable> Network::getIdentifiables() const {
    return m_networkIndex.getAll<Identifiable>();
}

stdcxx::range<Identifiable> Network::getIdentifiables() {
    return m_networkIndex.getAll<Identifiable>();
}

const NetworkIndex& Network::getIndex() const {
    return m_networkIndex;
}

NetworkIndex& Network::getIndex() {
    return m_networkIndex;
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

stdcxx::const_range<LccConverterStation> Network::getLccConverterStations() const {
    return m_networkIndex.getAll<LccConverterStation>();
}

stdcxx::range<LccConverterStation> Network::getLccConverterStations() {
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

stdcxx::const_range<Line> Network::getLines() const {
    return boost::range::join(
        m_networkIndex.getAll<Line>(),
        m_networkIndex.getAll<TieLine, Line>());
}

stdcxx::range<Line> Network::getLines() {
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

stdcxx::const_range<Load> Network::getLoads() const {
    return m_networkIndex.getAll<Load>();
}

stdcxx::range<Load> Network::getLoads() {
    return m_networkIndex.getAll<Load>();
}

const Network& Network::getNetwork() const {
    return *this;
}

Network& Network::getNetwork() {
    return *this;
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

stdcxx::const_range<ShuntCompensator> Network::getShuntCompensators() const {
    return m_networkIndex.getAll<ShuntCompensator>();
}

stdcxx::range<ShuntCompensator> Network::getShuntCompensators() {
    return m_networkIndex.getAll<ShuntCompensator>();
}

const std::string& Network::getSourceFormat() const {
    return m_sourceFormat;
}

stdcxx::const_range<MultiVariantObject> Network::getStatefulObjects() const {
    return m_networkIndex.getAll<Identifiable, MultiVariantObject>();
}

stdcxx::range<MultiVariantObject> Network::getStatefulObjects() {
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

stdcxx::const_range<StaticVarCompensator> Network::getStaticVarCompensators() const {
    return m_networkIndex.getAll<StaticVarCompensator>();
}

stdcxx::range<StaticVarCompensator> Network::getStaticVarCompensators() {
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

stdcxx::const_range<Substation> Network::getSubstations() const {
    return m_networkIndex.getAll<Substation>();
}

stdcxx::range<Substation> Network::getSubstations() {
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

stdcxx::const_range<Switch> Network::getSwitches() const {
    return m_networkIndex.getAll<Switch>();
}

stdcxx::range<Switch> Network::getSwitches() {
    return m_networkIndex.getAll<Switch>();
}

const SynchronousComponentsManager& Network::getSynchronousComponentsManager() const {
    return m_variants.get().getSynchronousComponentsManager();
}

SynchronousComponentsManager& Network::getSynchronousComponentsManager() {
    return m_variants.get().getSynchronousComponentsManager();
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

stdcxx::const_range<ThreeWindingsTransformer> Network::getThreeWindingsTransformers() const {
    return m_networkIndex.getAll<ThreeWindingsTransformer>();
}

stdcxx::range<ThreeWindingsTransformer> Network::getThreeWindingsTransformers() {
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

stdcxx::const_range<TwoWindingsTransformer> Network::getTwoWindingsTransformers() const {
    return m_networkIndex.getAll<TwoWindingsTransformer>();
}

stdcxx::range<TwoWindingsTransformer> Network::getTwoWindingsTransformers() {
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

stdcxx::const_range<VoltageLevel> Network::getVoltageLevels() const {
    return m_networkIndex.getAll<VoltageLevel>();
}

stdcxx::range<VoltageLevel> Network::getVoltageLevels() {
    return m_networkIndex.getAll<VoltageLevel>();
}

const VscConverterStation& Network::getVscConverterStation(const std::string& id) const {
    return get<VscConverterStation>(id);
}

VscConverterStation& Network::getVscConverterStation(const std::string& id) {
    return get<VscConverterStation>(id);
}

stdcxx::const_range<VscConverterStation> Network::getVscConverterStations() const {
    return m_networkIndex.getAll<VscConverterStation>();
}

stdcxx::range<VscConverterStation> Network::getVscConverterStations() {
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

void Network::reduceVariantArraySize(unsigned long number) {
    Container::reduceVariantArraySize(number);

    m_variants.reduceVariantArraySize(number);
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
