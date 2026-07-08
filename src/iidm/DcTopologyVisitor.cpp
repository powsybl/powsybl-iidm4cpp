/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcTopologyVisitor.hpp>

#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/iidm/DcConnectable.hpp>
#include <powsybl/iidm/DcGround.hpp>
#include <powsybl/iidm/DcLine.hpp>
#include <powsybl/iidm/DcTerminal.hpp>

#include <powsybl/iidm/Enum.hpp>

namespace powsybl {

namespace iidm {

void DcTopologyVisitor::visitAcDcConverter(const AcDcConverter& /*converter*/, const TerminalNumber& /*terminalNumber*/) {
    // default empty
}

void DcTopologyVisitor::visitDcGround(const DcGround& /*dcGround*/) {
    // default empty
}

void DcTopologyVisitor::visitDcLine(const DcLine& /*dcLine*/, const TwoSides& /*side*/) {
    // default empty
}

void DcTopologyVisitor::visitDcEquipments(const stdcxx::const_range<DcTerminal>& dcTerminals, DcTopologyVisitor& visitor) {
    for (const DcTerminal& dcTerminal : dcTerminals) {
        const DcConnectable& dcConnectable = dcTerminal.getDcConnectable();
        switch (dcConnectable.getType()) {
            case IdentifiableType::DC_GROUND:
                visitor.visitDcGround(dynamic_cast<const DcGround&>(dcConnectable));
                break;
            case IdentifiableType::DC_LINE: {
                const auto& dcLine = dynamic_cast<const DcLine&>(dcConnectable);
                visitor.visitDcLine(dcLine, dcLine.getSide(dcTerminal));
                break;
            }
            case IdentifiableType::LINE_COMMUTATED_CONVERTER:
            case IdentifiableType::VOLTAGE_SOURCE_CONVERTER: {
                const auto& acDcConverter = dynamic_cast<const AcDcConverter&>(dcConnectable);
                visitor.visitAcDcConverter(acDcConverter, acDcConverter.getTerminalNumber(dcTerminal));
                break;
            }

            case IdentifiableType::BUSBAR_SECTION:
            case IdentifiableType::LINE:
            case IdentifiableType::GENERATOR:
            case IdentifiableType::BATTERY:
            case IdentifiableType::SHUNT_COMPENSATOR:
            case IdentifiableType::TWO_WINDINGS_TRANSFORMER:
            case IdentifiableType::THREE_WINDINGS_TRANSFORMER:
            case IdentifiableType::LOAD:
            case IdentifiableType::DANGLING_LINE:
            case IdentifiableType::STATIC_VAR_COMPENSATOR:
            case IdentifiableType::HVDC_CONVERTER_STATION:
            case IdentifiableType::GROUND:
            case IdentifiableType::NETWORK:
            case IdentifiableType::SUBSTATION:
            case IdentifiableType::VOLTAGE_LEVEL:
            case IdentifiableType::AREA:
            case IdentifiableType::OVERLOAD_MANAGEMENT_SYSTEM:
            case IdentifiableType::HVDC_LINE:
            case IdentifiableType::BUS:
            case IdentifiableType::SWITCH:
            case IdentifiableType::TIE_LINE:
            case IdentifiableType::DC_NODE:
            case IdentifiableType::DC_SWITCH:
            case IdentifiableType::DC_BUS:
            default:
                throw AssertionError(stdcxx::format("Unexpected IdentifiableType %1%", dcConnectable.getType()));
        }
    }
}

}  // namespace iidm

}  // namespace powsybl
