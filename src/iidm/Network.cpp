/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

Network::Network(const std::string& id, const std::string& sourceFormat) :
    Container(id, id, Container::Type::NETWORK),
    m_sourceFormat(checkNotEmpty(*this, sourceFormat, "Source format is empty")),
    m_forecastDistance(0),
    m_variantManager(*this) {
}

BusbarSection& Network::getBusbarSection(const std::string& id) const {
    return get<BusbarSection>(id);
}

unsigned long Network::getBusbarSectionCount() const {
    return getObjectCount<BusbarSection>();
}

Connectable& Network::getConnectable(const std::string& id) const {
    auto& identifiable = get<Identifiable>(id);
    assert(stdcxx::isInstanceOf<Connectable>(identifiable));

    return dynamic_cast<Connectable&>(identifiable);
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
    m_objectStore.remove(identifiable);
}

Network& Network::setForecastDistance(int forecastDistance) {
    m_forecastDistance = checkForecastDistance(*this, forecastDistance);
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
