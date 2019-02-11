/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <unordered_set>

#include <powsybl/iidm/Network.hpp>

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
#include <powsybl/stdcxx/hash.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

Network::Network(const std::string& id, const std::string& sourceFormat) :
    Container(id, id, Container::Type::NETWORK),
    m_sourceFormat(checkNotEmpty(*this, sourceFormat, "Source format is empty")),
    m_forecastDistance(0),
    m_variantManager(*this) {
}

Branch& Network::getBranch(const std::string& id) const {
    return get<Branch>(id);
}

unsigned long Network::getBranchCount() const {
    return getLineCount() + getTwoWindingsTransformerCount();
}

BusbarSection& Network::getBusbarSection(const std::string& id) const {
    return get<BusbarSection>(id);
}

unsigned long Network::getBusbarSectionCount() const {
    return getObjectCount<BusbarSection>();
}

stdcxx::CReference<Connectable> Network::getConnectable(const std::string& id) const {
    return stdcxx::cref<Connectable>(find<Connectable>(id));
}

stdcxx::Reference<Connectable> Network::getConnectable(const std::string& id) {
    return find<Connectable>(id);
}

unsigned long Network::getCountryCount() const {
    std::unordered_set<Country, stdcxx::hash<Country>> countries;
    for (auto it = cbegin<Substation>(); it != cend<Substation>(); ++it) {
        countries.emplace((*it).getCountry());
    }

    return countries.size();
}

DanglingLine& Network::getDanglingLine(const std::string& id) const {
    return get<DanglingLine>(id);
}

unsigned long Network::getDanglingLineCount() const {
    return getObjectCount<DanglingLine>();
}

int Network::getForecastDistance() const {
    return m_forecastDistance;
}

Generator& Network::getGenerator(const std::string& id) const {
    return get<Generator>(id);
}

unsigned long Network::getGeneratorCount() const {
    return getObjectCount<Generator>();
}

HvdcConverterStation& Network::getHvdcConverterStation(const std::string& id) const {
    return get<HvdcConverterStation>(id);
}

unsigned long Network::getHvdcConverterStationCount() const {
    return getLccConverterStationCount() + getVscConverterStationCount();
}

HvdcLine& Network::getHvdcLine(const std::string& id) const {
    return get<HvdcLine>(id);
}

unsigned long Network::getHvdcLineCount() const {
    return getObjectCount<HvdcLine>();
}

LccConverterStation& Network::getLccConverterStation(const std::string& id) const {
    return get<LccConverterStation>(id);
}

unsigned long Network::getLccConverterStationCount() const {
    return getObjectCount<LccConverterStation>();
}

Line& Network::getLine(const std::string& id) const {
    return get<Line>(id);
}

unsigned long Network::getLineCount() const {
    return getObjectCount<Line>() + getObjectCount<TieLine>();
}

Load& Network::getLoad(const std::string& id) const {
    return get<Load>(id);
}

unsigned long Network::getLoadCount() const {
    return getObjectCount<Load>();
}

ShuntCompensator& Network::getShuntCompensator(const std::string& id) const {
    return get<ShuntCompensator>(id);
}

unsigned long Network::getShuntCompensatorCount() const {
    return getObjectCount<ShuntCompensator>();
}

const std::string& Network::getSourceFormat() const {
    return m_sourceFormat;
}

StaticVarCompensator& Network::getStaticVarCompensator(const std::string& id) const {
    return get<StaticVarCompensator>(id);
}

unsigned long Network::getStaticVarCompensatorCount() const {
    return getObjectCount<StaticVarCompensator>();
}

Substation& Network::getSubstation(const std::string& id) const {
    return get<Substation>(id);
}

unsigned long Network::getSubstationCount() const {
    return getObjectCount<Substation>();
}

Switch& Network::getSwitch(const std::string& id) const {
    return get<Switch>(id);
}

unsigned long Network::getSwitchCount() const {
    return getObjectCount<Switch>();
}

ThreeWindingsTransformer& Network::getThreeWindingsTransformer(const std::string& id) const {
    return get<ThreeWindingsTransformer>(id);
}

unsigned long Network::getThreeWindingsTransformerCount() const {
    return getObjectCount<ThreeWindingsTransformer>();
}

TwoWindingsTransformer& Network::getTwoWindingsTransformer(const std::string& id) const {
    return get<TwoWindingsTransformer>(id);
}

unsigned long Network::getTwoWindingsTransformerCount() const {
    return getObjectCount<TwoWindingsTransformer>();
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

VoltageLevel& Network::getVoltageLevel(const std::string& id) const {
    return get<VoltageLevel>(id);
}

unsigned long Network::getVoltageLevelCount() const {
    return getObjectCount<VoltageLevel>();
}

VscConverterStation& Network::getVscConverterStation(const std::string& id) const {
    return get<VscConverterStation>(id);
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

Network& Network::setForecastDistance(int forecastDistance) {
    m_forecastDistance = checkForecastDistance(*this, forecastDistance);
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
