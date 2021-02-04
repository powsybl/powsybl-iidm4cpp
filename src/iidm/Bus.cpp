/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Bus.hpp>

#include <boost/range/adaptor/filtered.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/Component.hpp>
#include <powsybl/iidm/ComponentConstants.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

Bus::Bus(const std::string& id, const std::string& name, bool fictitious) :
    Identifiable(id, name, fictitious) {
}

template <typename T>
stdcxx::const_range<T> Bus::getAll() const {
    return getConnectedTerminals() | boost::adaptors::filtered(Terminal::isInstanceOf<T>) | boost::adaptors::transformed(Terminal::map<const T>);
}

template <typename T>
stdcxx::range<T> Bus::getAll() {
    return getConnectedTerminals() | boost::adaptors::filtered(Terminal::isInstanceOf<T>) | boost::adaptors::transformed(Terminal::map<T>);
}

stdcxx::const_range<Battery> Bus::getBatteries() const {
    return getAll<Battery>();
}

stdcxx::range<Battery> Bus::getBatteries() {
    return getAll<Battery>();
}

stdcxx::const_range<DanglingLine> Bus::getDanglingLines() const {
    return getAll<DanglingLine>();
}

stdcxx::range<DanglingLine> Bus::getDanglingLines() {
    return getAll<DanglingLine>();
}

stdcxx::const_range<Generator> Bus::getGenerators() const {
    return getAll<Generator>();
}

stdcxx::range<Generator> Bus::getGenerators() {
    return getAll<Generator>();
}

stdcxx::const_range<LccConverterStation> Bus::getLccConverterStations() const {
    return getAll<LccConverterStation>();
}

stdcxx::range<LccConverterStation> Bus::getLccConverterStations() {
    return getAll<LccConverterStation>();
}

stdcxx::const_range<Line> Bus::getLines() const {
    return getAll<Line>();
}

stdcxx::range<Line> Bus::getLines() {
    return getAll<Line>();
}

stdcxx::const_range<Load> Bus::getLoads() const {
    return getAll<Load>();
}

stdcxx::range<Load> Bus::getLoads() {
    return getAll<Load>();
}

const Network& Bus::getNetwork() const {
    return getVoltageLevel().getNetwork();
}

Network& Bus::getNetwork() {
    return getVoltageLevel().getNetwork();
}

stdcxx::const_range<ShuntCompensator> Bus::getShuntCompensators() const {
    return getAll<ShuntCompensator>();
}

stdcxx::range<ShuntCompensator> Bus::getShuntCompensators() {
    return getAll<ShuntCompensator>();
}

stdcxx::const_range<StaticVarCompensator> Bus::getStaticVarCompensators() const {
    return getAll<StaticVarCompensator>();
}

stdcxx::range<StaticVarCompensator> Bus::getStaticVarCompensators() {
    return getAll<StaticVarCompensator>();
}

stdcxx::const_range<ThreeWindingsTransformer> Bus::getThreeWindingsTransformers() const {
    return getAll<ThreeWindingsTransformer>();
}

stdcxx::range<ThreeWindingsTransformer> Bus::getThreeWindingsTransformers() {
    return getAll<ThreeWindingsTransformer>();
}

stdcxx::const_range<TwoWindingsTransformer> Bus::getTwoWindingsTransformers() const {
    return getAll<TwoWindingsTransformer>();
}

stdcxx::range<TwoWindingsTransformer> Bus::getTwoWindingsTransformers() {
    return getAll<TwoWindingsTransformer>();
}

stdcxx::const_range<VscConverterStation> Bus::getVscConverterStations() const {
    return getAll<VscConverterStation>();
}

stdcxx::range<VscConverterStation> Bus::getVscConverterStations() {
    return getAll<VscConverterStation>();
}

const std::string& Bus::getTypeDescription() const {
    static std::string s_typeDescription = "Bus";

    return s_typeDescription;
}

bool Bus::isInMainConnectedComponent() const {
    const auto& cc = getConnectedComponent();
    return static_cast<bool>(cc) && (cc.get().getNum() == ComponentConstants::MAIN_NUM);
}

bool Bus::isInMainSynchronousComponent() const {
    const auto& sc = getSynchronousComponent();
    return static_cast<bool>(sc) && (sc.get().getNum() == ComponentConstants::MAIN_NUM);
}

void Bus::visitConnectedEquipments(TopologyVisitor& visitor) {
    visitEquipments<Terminal>(getConnectedTerminals(), visitor);
}

}  // namespace iidm

}  // namespace powsybl
