/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/AbstractTerminalTopologyVisitor.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Ground.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

void AbstractTerminalTopologyVisitor::visitBattery(const Battery& battery) {
    visitInjection(battery);
}

void AbstractTerminalTopologyVisitor::visitBranch(const Branch& branch, const TwoSides& side) {
    visitTerminal(branch.getTerminalFromSide(side));
}

void AbstractTerminalTopologyVisitor::visitBusbarSection(const BusbarSection& section) {
    visitInjection(section);
}

void AbstractTerminalTopologyVisitor::visitDanglingLine(const DanglingLine& danglingLine) {
    visitInjection(danglingLine);
}

void AbstractTerminalTopologyVisitor::visitGenerator(const Generator& generator) {
    visitInjection(generator);
}

void AbstractTerminalTopologyVisitor::visitGround(const Ground& ground) {
    visitInjection(ground);
}

void AbstractTerminalTopologyVisitor::visitHvdcConverterStation(const HvdcConverterStation& converterStation) {
    visitInjection(converterStation);
}

void AbstractTerminalTopologyVisitor::visitInjection(const Injection& injection) {
    visitTerminal(injection.getTerminal());
}

void AbstractTerminalTopologyVisitor::visitLine(const Line& line, const TwoSides& side) {
    visitBranch(line, side);
}

void AbstractTerminalTopologyVisitor::visitLoad(const Load& load) {
    visitInjection(load);
}

void AbstractTerminalTopologyVisitor::visitShuntCompensator(const ShuntCompensator& sc) {
    visitInjection(sc);
}

void AbstractTerminalTopologyVisitor::visitStaticVarCompensator(const StaticVarCompensator& staticVarCompensator) {
    visitInjection(staticVarCompensator);
}

void AbstractTerminalTopologyVisitor::visitThreeWindingsTransformer(const ThreeWindingsTransformer& transformer, const ThreeSides& side) {
    switch (side) {
        case ThreeSides::ONE:
            visitTerminal(transformer.getLeg1().getTerminal());
            break;

        case ThreeSides::TWO:
            visitTerminal(transformer.getLeg2().getTerminal());
            break;

        case ThreeSides::THREE:
            visitTerminal(transformer.getLeg3().getTerminal());
            break;

        case ThreeSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected 3WT side %1%", side));
    }
}

void AbstractTerminalTopologyVisitor::visitTwoWindingsTransformer(const TwoWindingsTransformer& transformer, const TwoSides& side) {
    visitBranch(transformer, side);
}

}  // namespace iidm

}  // namespace powsybl
