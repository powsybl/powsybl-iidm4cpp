/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TopologyVisitor.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>

namespace powsybl {

namespace iidm {

void TopologyVisitor::visitBattery(const Battery& /*battery*/) {
    // nothing to do
}

void TopologyVisitor::visitBusbarSection(const BusbarSection& /*section*/) {
    // nothing to do
}

void TopologyVisitor::visitDanglingLine(const DanglingLine& /*danglingLine*/) {
    // nothing to do
}

void TopologyVisitor::visitEquipments(const stdcxx::const_range<Terminal>& terminals, TopologyVisitor& visitor) {
    for (const Terminal& terminal : terminals) {
        const Connectable& connectable = terminal.getConnectable();
        switch (connectable.getType()) {
            case IdentifiableType::BUSBAR_SECTION:
                visitor.visitBusbarSection(dynamic_cast<const BusbarSection&>(connectable));
                break;

            case IdentifiableType::LINE: {
                const auto& line = dynamic_cast<const Line&>(connectable);
                visitor.visitLine(line, line.getSide(terminal));
                break;
            }

            case IdentifiableType::GENERATOR:
                visitor.visitGenerator(dynamic_cast<const Generator&>(connectable));
                break;

            case IdentifiableType::BATTERY:
                visitor.visitBattery(dynamic_cast<const Battery&>(connectable));
                break;

            case IdentifiableType::SHUNT_COMPENSATOR:
                visitor.visitShuntCompensator(dynamic_cast<const ShuntCompensator&>(connectable));
                break;

            case IdentifiableType::TWO_WINDINGS_TRANSFORMER: {
                const auto& twt = dynamic_cast<const TwoWindingsTransformer&>(connectable);
                visitor.visitTwoWindingsTransformer(twt, twt.getSide(terminal));
                break;
            }

            case IdentifiableType::THREE_WINDINGS_TRANSFORMER: {
                const auto& twt = dynamic_cast<const ThreeWindingsTransformer&>(connectable);
                visitor.visitThreeWindingsTransformer(twt, twt.getSide(terminal));
                break;
            }

            case IdentifiableType::LOAD:
                visitor.visitLoad(dynamic_cast<const Load&>(connectable));
                break;

            case IdentifiableType::DANGLING_LINE:
                visitor.visitDanglingLine(dynamic_cast<const DanglingLine&>(connectable));
                break;

            case IdentifiableType::STATIC_VAR_COMPENSATOR:
                visitor.visitStaticVarCompensator(dynamic_cast<const StaticVarCompensator&>(connectable));
                break;

            case IdentifiableType::HVDC_CONVERTER_STATION:
                visitor.visitHvdcConverterStation(dynamic_cast<const HvdcConverterStation&>(connectable));
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
}

void TopologyVisitor::visitGenerator(const Generator& /*generator*/) {
    // nothing to do
}

void TopologyVisitor::visitHvdcConverterStation(const HvdcConverterStation& /*converterStation*/) {
    // nothing to do
}

void TopologyVisitor::visitLine(const Line& /*line*/, const Branch::Side& /*side*/) {
    // nothing to do
}

void TopologyVisitor::visitLoad(const Load& /*load*/) {
    // nothing to do
}

void TopologyVisitor::visitShuntCompensator(const ShuntCompensator& /*sc*/) {
    // nothing to do
}

void TopologyVisitor::visitStaticVarCompensator(const StaticVarCompensator& /*staticVarCompensator*/) {
    // nothing to do
}

void TopologyVisitor::visitThreeWindingsTransformer(const ThreeWindingsTransformer& /*transformer*/, const ThreeWindingsTransformer::Side& /*side*/) {
    // nothing to do
}

void TopologyVisitor::visitTwoWindingsTransformer(const TwoWindingsTransformer& /*transformer*/, const Branch::Side& /*side*/) {
    // nothing to do
}

}  // namespace iidm

}  // namespace powsybl
