/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Bus.hpp>

#include <boost/range/adaptor/filtered.hpp>

#include <powsybl/iidm/Battery.hpp>
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

template <typename T>
bool isInstanceOf(const std::reference_wrapper<Terminal>& terminal) {
    const auto& connectable = terminal.get().getConnectable();
    return static_cast<bool>(connectable) && stdcxx::isInstanceOf<T>(connectable.get());
}

template <typename T>
T& map(const std::reference_wrapper<Terminal>& terminal) {
    return dynamic_cast<T&>(terminal.get().getConnectable().get());
}

Bus::Bus(const std::string& id) :
    Identifiable(id, id) {
}

Bus::Bus(const std::string& id, const std::string& name) :
    Identifiable(id, name) {
}

template <typename T>
Bus::const_range<T> Bus::getAll() const {
    typename bus::range_traits<T>::filter filter = isInstanceOf<T>;
    typename bus::range_traits<T>::const_mapper mapper = map<const T>;

    return getConnectedTerminals() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

template <typename T>
Bus::range<T> Bus::getAll() {
    typename bus::range_traits<T>::filter filter = isInstanceOf<T>;
    typename bus::range_traits<T>::mapper mapper = map<T>;

    return getConnectedTerminals() | boost::adaptors::filtered(filter) | boost::adaptors::transformed(mapper);
}

Bus::const_range<Battery> Bus::getBatteries() const {
    return getAll<Battery>();
}

Bus::range<Battery> Bus::getBatteries() {
    return getAll<Battery>();
}

Bus::const_range<DanglingLine> Bus::getDanglingLines() const {
    return getAll<DanglingLine>();
}

Bus::range<DanglingLine> Bus::getDanglingLines() {
    return getAll<DanglingLine>();
}

Bus::const_range<Generator> Bus::getGenerators() const {
    return getAll<Generator>();
}

Bus::range<Generator> Bus::getGenerators() {
    return getAll<Generator>();
}

Bus::const_range<LccConverterStation> Bus::getLccConverterStations() const {
    return getAll<LccConverterStation>();
}

Bus::range<LccConverterStation> Bus::getLccConverterStations() {
    return getAll<LccConverterStation>();
}

Bus::const_range<Line> Bus::getLines() const {
    return getAll<Line>();
}

Bus::range<Line> Bus::getLines() {
    return getAll<Line>();
}

Bus::const_range<Load> Bus::getLoads() const {
    return getAll<Load>();
}

Bus::range<Load> Bus::getLoads() {
    return getAll<Load>();
}

Bus::const_range<ShuntCompensator> Bus::getShuntCompensators() const {
    return getAll<ShuntCompensator>();
}

Bus::range<ShuntCompensator> Bus::getShuntCompensators() {
    return getAll<ShuntCompensator>();
}

Bus::const_range<StaticVarCompensator> Bus::getStaticVarCompensators() const {
    return getAll<StaticVarCompensator>();
}

Bus::range<StaticVarCompensator> Bus::getStaticVarCompensators() {
    return getAll<StaticVarCompensator>();
}

Bus::const_range<ThreeWindingsTransformer> Bus::getThreeWindingsTransformers() const {
    return getAll<ThreeWindingsTransformer>();
}

Bus::range<ThreeWindingsTransformer> Bus::getThreeWindingsTransformers() {
    return getAll<ThreeWindingsTransformer>();
}

Bus::const_range<TwoWindingsTransformer> Bus::getTwoWindingsTransformers() const {
    return getAll<TwoWindingsTransformer>();
}

Bus::range<TwoWindingsTransformer> Bus::getTwoWindingsTransformers() {
    return getAll<TwoWindingsTransformer>();
}

Bus::const_range<VscConverterStation> Bus::getVscConverterStations() const {
    return getAll<VscConverterStation>();
}

Bus::range<VscConverterStation> Bus::getVscConverterStations() {
    return getAll<VscConverterStation>();
}

const std::string& Bus::getTypeDescription() const {
    static std::string s_typeDescription = "Bus";

    return s_typeDescription;
}

}  // namespace iidm

}  // namespace powsybl
