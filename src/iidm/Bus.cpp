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
#include <powsybl/iidm/TopologyVisitor.hpp>
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

double Bus::getFictitiousP0() const {
    return stdcxx::nan();
}

double Bus::getFictitiousQ0() const {
    return stdcxx::nan();
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

double Bus::getP() const {
    if (getConnectedTerminalCount() == 0) {
        return stdcxx::nan();
    }
    double p = 0;
    for (const Terminal& terminal : getConnectedTerminals()) {
        const Connectable& connectable = terminal.getConnectable();
        switch (connectable.getType()) {
            case IdentifiableType::BUSBAR_SECTION:
            case IdentifiableType::SHUNT_COMPENSATOR:
            case IdentifiableType::STATIC_VAR_COMPENSATOR:
            case IdentifiableType::LINE:
            case IdentifiableType::TWO_WINDINGS_TRANSFORMER:
            case IdentifiableType::THREE_WINDINGS_TRANSFORMER:
            case IdentifiableType::DANGLING_LINE:
                // skip
                break;
            case IdentifiableType::GENERATOR:
            case IdentifiableType::BATTERY:
            case IdentifiableType::LOAD:
            case IdentifiableType::HVDC_CONVERTER_STATION:
                if (!std::isnan(terminal.getP())) {
                    p += terminal.getP();
                }
                break;
            case IdentifiableType::NETWORK:
            case IdentifiableType::SUBSTATION:
            case IdentifiableType::VOLTAGE_LEVEL:
            case IdentifiableType::HVDC_LINE:
            case IdentifiableType::BUS:
            case IdentifiableType::SWITCH:
            default:
                throw AssertionError(stdcxx::format("Unexpected IdentifiableType %1%", connectable.getType()));
        }
    }
    return p;
}

double Bus::getQ() const {
    if (getConnectedTerminalCount() == 0) {
        return stdcxx::nan();
    }
    double q = 0;
    for (const Terminal& terminal : getConnectedTerminals()) {
        const Connectable& connectable = terminal.getConnectable();
        switch (connectable.getType()) {
            case IdentifiableType::BUSBAR_SECTION:
            case IdentifiableType::LINE:
            case IdentifiableType::TWO_WINDINGS_TRANSFORMER:
            case IdentifiableType::THREE_WINDINGS_TRANSFORMER:
            case IdentifiableType::DANGLING_LINE:
                // skip
                break;
            case IdentifiableType::GENERATOR:
            case IdentifiableType::BATTERY:
            case IdentifiableType::LOAD:
            case IdentifiableType::SHUNT_COMPENSATOR:
            case IdentifiableType::STATIC_VAR_COMPENSATOR:
            case IdentifiableType::HVDC_CONVERTER_STATION:
                if (!std::isnan(terminal.getQ())) {
                    q += terminal.getQ();
                }
                break;
            case IdentifiableType::NETWORK:
            case IdentifiableType::SUBSTATION:
            case IdentifiableType::VOLTAGE_LEVEL:
            case IdentifiableType::HVDC_LINE:
            case IdentifiableType::BUS:
            case IdentifiableType::SWITCH:
            default:
                throw AssertionError(stdcxx::format("Unexpected IdentifiableType %1%", connectable.getType()));
        }
    }
    return q;
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

const IdentifiableType& Bus::getType() const {
    static IdentifiableType s_type = IdentifiableType::BUS;
    return s_type;
}

const std::string& Bus::getTypeDescription() const {
    static std::string s_typeDescription = "Bus";

    return s_typeDescription;
}

stdcxx::const_range<VscConverterStation> Bus::getVscConverterStations() const {
    return getAll<VscConverterStation>();
}

stdcxx::range<VscConverterStation> Bus::getVscConverterStations() {
    return getAll<VscConverterStation>();
}

bool Bus::isInMainConnectedComponent() const {
    const auto& cc = getConnectedComponent();
    return static_cast<bool>(cc) && (cc.get().getNum() == ComponentConstants::MAIN_NUM);
}

bool Bus::isInMainSynchronousComponent() const {
    const auto& sc = getSynchronousComponent();
    return static_cast<bool>(sc) && (sc.get().getNum() == ComponentConstants::MAIN_NUM);
}

Bus& Bus::setFictitiousP0(double /*p0*/) {
    // do nothing
    return *this;
}

Bus& Bus::setFictitiousQ0(double /*q0*/) {
    // do nothing
    return *this;
}

void Bus::visitConnectedEquipments(TopologyVisitor& visitor) {
    TopologyVisitor::visitEquipments(getConnectedTerminals(), visitor);
}

}  // namespace iidm

}  // namespace powsybl
