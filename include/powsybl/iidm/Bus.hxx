/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUS_HXX
#define POWSYBL_IIDM_BUS_HXX

#include <powsybl/iidm/Bus.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/HvdcConverterStation.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/TopologyVisitor.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

template <typename T>
void Bus::visitEquipments(const stdcxx::const_range<T>& terminals, TopologyVisitor& visitor) {
    for (const Terminal& terminal : terminals) {
        const Connectable& connectable = terminal.getConnectable();
        switch (connectable.getType()) {
            case ConnectableType::BUSBAR_SECTION:
                visitor.visitBusbarSection(dynamic_cast<const BusbarSection&>(connectable));
                break;

            case ConnectableType::LINE: {
                const auto& line = dynamic_cast<const Line&>(connectable);
                    visitor.visitLine(line, stdcxx::areSame(line.getTerminal1(), terminal) ? Branch::Side::ONE : Branch::Side::TWO);
                }
                break;

            case ConnectableType::GENERATOR:
                visitor.visitGenerator(dynamic_cast<const Generator&>(connectable));
                break;

            case ConnectableType::BATTERY:
                visitor.visitBattery(dynamic_cast<const Battery&>(connectable));
                break;

            case ConnectableType::SHUNT_COMPENSATOR:
                visitor.visitShuntCompensator(dynamic_cast<const ShuntCompensator&>(connectable));
                break;

            case ConnectableType::TWO_WINDINGS_TRANSFORMER: {
                const auto& twt = dynamic_cast<const TwoWindingsTransformer&>(connectable);
                    visitor.visitTwoWindingsTransformer(twt, stdcxx::areSame(twt.getTerminal1(), terminal) ? Branch::Side::ONE : Branch::Side::TWO);
                }
                break;

            case ConnectableType::THREE_WINDINGS_TRANSFORMER: {
                    const auto& thwt = dynamic_cast<const ThreeWindingsTransformer&>(connectable);
                    ThreeWindingsTransformer::Side side;
                    if (stdcxx::areSame(thwt.getLeg1().getTerminal(), terminal)) {
                        side = ThreeWindingsTransformer::Side::ONE;
                    } else if (stdcxx::areSame(thwt.getLeg2().getTerminal(), terminal)) {
                        side = ThreeWindingsTransformer::Side::TWO;
                    } else {
                        side = ThreeWindingsTransformer::Side::THREE;
                    }
                    visitor.visitThreeWindingsTransformer(thwt, side);
                }
                break;

            case ConnectableType::LOAD:
                visitor.visitLoad(dynamic_cast<const Load&>(connectable));
                break;

            case ConnectableType::DANGLING_LINE:
                visitor.visitDanglingLine(dynamic_cast<const DanglingLine&>(connectable));
                break;

            case ConnectableType::STATIC_VAR_COMPENSATOR:
                visitor.visitStaticVarCompensator(dynamic_cast<const StaticVarCompensator&>(connectable));
                break;

            case ConnectableType::HVDC_CONVERTER_STATION:
                visitor.visitHvdcConverterStation(dynamic_cast<const HvdcConverterStation&>(connectable));
                break;

            default:
                throw AssertionError(stdcxx::format("Unexpected connectable type %1%", connectable.getType()));
        }
    }
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUS_HXX
