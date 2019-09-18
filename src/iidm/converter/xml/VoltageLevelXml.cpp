/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "VoltageLevelXml.hpp"

#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/TopologyLevel.hpp>
#include <powsybl/iidm/VoltageLevelViews.hpp>
#include <powsybl/xml/XmlStreamException.hpp>

#include "iidm/converter/Constants.hpp"

#include "BatteryXml.hpp"
#include "BusBreakerViewSwitchXml.hpp"
#include "BusXml.hpp"
#include "BusbarSectionXml.hpp"
#include "DanglingLineXml.hpp"
#include "GeneratorXml.hpp"
#include "LccConverterStationXml.hpp"
#include "LoadXml.hpp"
#include "NodeBreakerViewInternalConnectionXml.hpp"
#include "NodeBreakerViewSwitchXml.hpp"
#include "ShuntCompensatorXml.hpp"
#include "StaticVarCompensatorXml.hpp"
#include "VscConverterStationXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

VoltageLevelAdder VoltageLevelXml::createAdder(Substation& substation) const {
    return substation.newVoltageLevel();
}

const VoltageLevelXml& VoltageLevelXml::getInstance() {
    static VoltageLevelXml s_instance;
    return s_instance;
}

const char* VoltageLevelXml::getRootElementName() const {
    return VOLTAGE_LEVEL;
}

VoltageLevel& VoltageLevelXml::readRootElementAttributes(VoltageLevelAdder& adder, NetworkXmlReaderContext& context) const {
    auto nominalVoltage = context.getReader().getAttributeValue<double>(NOMINAL_V);
    double lowVoltageLimit = context.getReader().getOptionalAttributeValue(LOW_VOLTAGE_LIMIT, stdcxx::nan());
    double highVoltageLimit = context.getReader().getOptionalAttributeValue(HIGH_VOLTAGE_LIMIT, stdcxx::nan());
    const auto& topologyKing = Enum::fromString<TopologyKind>(context.getReader().getAttributeValue(TOPOLOGY_KIND));
    return adder.setNominalVoltage(nominalVoltage)
                .setLowVoltageLimit(lowVoltageLimit)
                .setHighVoltageLimit(highVoltageLimit)
                .setTopologyKind(topologyKing)
                .add();
}

void VoltageLevelXml::readSubElements(VoltageLevel& voltageLevel, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(VOLTAGE_LEVEL, [this, &voltageLevel, &context]() {
        if (context.getReader().getLocalName() == NODE_BREAKER_TOPOLOGY) {
            const auto& nodeCount = context.getReader().getAttributeValue<unsigned long>(NODE_COUNT);
            voltageLevel.getNodeBreakerView().setNodeCount(nodeCount);
            context.getReader().readUntilEndElement(NODE_BREAKER_TOPOLOGY, [&voltageLevel, &context]() {
                if (context.getReader().getLocalName() == BUSBAR_SECTION) {
                    BusbarSectionXml::getInstance().read(voltageLevel, context);
                } else if (context.getReader().getLocalName() == SWITCH) {
                    NodeBreakerViewSwitchXml::getInstance().read(voltageLevel, context);
                } else if (context.getReader().getLocalName() == INTERNAL_CONNECTION) {
                    NodeBreakerViewInternalConnectionXml::getInstance().read(voltageLevel, context);
                } else {
                    throw powsybl::xml::XmlStreamException(logging::format("Unexpected element %1%", context.getReader().getLocalName()));
                }
            });
        } else if (context.getReader().getLocalName() == BUS_BREAKER_TOPOLOGY) {
            context.getReader().readUntilEndElement(BUS_BREAKER_TOPOLOGY, [&voltageLevel, &context]() {
                if (context.getReader().getLocalName() == BUS) {
                    BusXml::getInstance().read(voltageLevel, context);
                } else if (context.getReader().getLocalName() == SWITCH) {
                    BusBreakerViewSwitchXml::getInstance().read(voltageLevel, context);
                } else {
                    throw powsybl::xml::XmlStreamException(logging::format("Unsupported element %1%", context.getReader().getLocalName()));
                }
            });
        } else if (context.getReader().getLocalName() == BATTERY) {
            BatteryXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == GENERATOR) {
            GeneratorXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == LOAD) {
            LoadXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == SHUNT) {
            ShuntCompensatorXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == DANGLING_LINE) {
            DanglingLineXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == STATIC_VAR_COMPENSATOR) {
            StaticVarCompensatorXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == VSC_CONVERTER_STATION) {
            VscConverterStationXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == LCC_CONVERTER_STATION) {
            LccConverterStationXml::getInstance().read(voltageLevel, context);
        } else {
            AbstractIdentifiableXml::readSubElements(voltageLevel, context);
        }
    });
}

void VoltageLevelXml::writeBatteries(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    for (const Battery& battery : voltageLevel.getBatteries()) {
        if (!context.getFilter().test(battery)) {
            continue;
        }
        BatteryXml::getInstance().write(battery, voltageLevel, context);
    }
}

void VoltageLevelXml::writeBusBranchTopology(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    context.getWriter().writeStartElement(IIDM_URI, BUS_BREAKER_TOPOLOGY);
    for (const Bus& bus : voltageLevel.getBusView().getBuses()) {
        // TODO(sebalaig) consider filtering
//        if (!context.getFilter().test(bus)) {
//            continue;
//        }
        BusXml::getInstance().write(bus, voltageLevel, context);
    }
    context.getWriter().writeEndElement();
}

void VoltageLevelXml::writeBusBreakerTopology(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    context.getWriter().writeStartElement(IIDM_PREFIX, BUS_BREAKER_TOPOLOGY);
    for (const Bus& bus : voltageLevel.getBusBreakerView().getBuses()) {
        if (!context.getFilter().test(bus)) {
            continue;
        }
        BusXml::getInstance().write(bus, voltageLevel, context);
    }
    for (const Switch& sw : voltageLevel.getBusBreakerView().getSwitches()) {
        const Bus& b1 = voltageLevel.getBusBreakerView().getBus1(context.getAnonymizer().anonymizeString(sw.getId()));
        const Bus& b2 = voltageLevel.getBusBreakerView().getBus2(context.getAnonymizer().anonymizeString(sw.getId()));
        if (!context.getFilter().test(b1) || !context.getFilter().test(b2)) {
            continue;
        }
        BusBreakerViewSwitchXml::getInstance().write(sw, voltageLevel, context);
    }
    context.getWriter().writeEndElement();
}

void VoltageLevelXml::writeDanglingLines(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    for (const auto& dl : voltageLevel.getDanglingLines()) {
        if (!context.getFilter().test(dl)) {
            continue;
        }
        DanglingLineXml::getInstance().write(dl, voltageLevel, context);
    }
}

void VoltageLevelXml::writeGenerators(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    for (const auto& generator : voltageLevel.getGenerators()) {
        if (!context.getFilter().test(generator)) {
            continue;
        }
        GeneratorXml::getInstance().write(generator, voltageLevel, context);
    }
}

void VoltageLevelXml::writeLccConverterStations(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    for (const auto& cs : voltageLevel.getLccConverterStations()) {
        if (!context.getFilter().test(cs)) {
            continue;
        }
        LccConverterStationXml::getInstance().write(cs, voltageLevel, context);
    }
}

void VoltageLevelXml::writeLoads(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    for (const auto& load : voltageLevel.getLoads()) {
        if (!context.getFilter().test(load)) {
            continue;
        }
        LoadXml::getInstance().write(load, voltageLevel, context);
    }
}

void VoltageLevelXml::writeNodeBreakerTopology(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    context.getWriter().writeStartElement(IIDM_PREFIX, NODE_BREAKER_TOPOLOGY);
    context.getWriter().writeAttribute(NODE_COUNT, voltageLevel.getNodeBreakerView().getNodeCount());
    for (const BusbarSection& bs : voltageLevel.getNodeBreakerView().getBusbarSections()) {
        BusbarSectionXml::getInstance().write(bs, voltageLevel, context);
    }
    for (const Switch& sw : voltageLevel.getNodeBreakerView().getSwitches()) {
        NodeBreakerViewSwitchXml::getInstance().write(sw, voltageLevel, context);
    }
    writeNodeBreakerTopologyInternalConnections(voltageLevel, context);
    context.getWriter().writeEndElement();
}

void VoltageLevelXml::writeNodeBreakerTopologyInternalConnections(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    for (const auto& ic : voltageLevel.getNodeBreakerView().getInternalConnections()) {
        NodeBreakerViewInternalConnectionXml::getInstance().write(ic.getNode1(), ic.getNode2(), context);
    }
}

void VoltageLevelXml::writeRootElementAttributes(const VoltageLevel& voltageLevel, const Substation& /*substation*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(NOMINAL_V, voltageLevel.getNominalVoltage());
    context.getWriter().writeOptionalAttribute(LOW_VOLTAGE_LIMIT, voltageLevel.getLowVoltageLimit());
    context.getWriter().writeOptionalAttribute(HIGH_VOLTAGE_LIMIT, voltageLevel.getHighVoltageLimit());
    const TopologyLevel& topologyLevel = getMinTopologyLevel(voltageLevel.getTopologyKind(), context.getOptions().getTopologyLevel());
    const TopologyKind& topologyKind = getTopologyKind(topologyLevel);
    context.getWriter().writeAttribute(TOPOLOGY_KIND, Enum::toString(topologyKind));
}

void VoltageLevelXml::writeShuntCompensators(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    for (const auto& shuntCompensator : voltageLevel.getShuntCompensators()) {
        if (!context.getFilter().test(shuntCompensator)) {
            continue;
        }
        ShuntCompensatorXml::getInstance().write(shuntCompensator, voltageLevel, context);
    }
}

void VoltageLevelXml::writeStaticVarCompensators(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    for (const StaticVarCompensator& svc : voltageLevel.getStaticVarCompensators()) {
        if (!context.getFilter().test(svc)) {
            continue;
        }
        StaticVarCompensatorXml::getInstance().write(svc, voltageLevel, context);
    }
}

void VoltageLevelXml::writeSubElements(const VoltageLevel& voltageLevel, const Substation& /*substation*/, NetworkXmlWriterContext& context) const {
    TopologyLevel topologyLevel = getMinTopologyLevel(voltageLevel.getTopologyKind(), context.getOptions().getTopologyLevel());
    switch (topologyLevel) {
        case TopologyLevel::BUS_BREAKER:
            writeBusBreakerTopology(voltageLevel, context);
            break;

        case TopologyLevel::NODE_BREAKER:
            writeNodeBreakerTopology(voltageLevel, context);
            break;

        case TopologyLevel::BUS_BRANCH:
            writeBusBranchTopology(voltageLevel, context);
            break;

        default:
            throw powsybl::xml::XmlStreamException(logging::format("Unexpected TopologyLevel value: ", topologyLevel));
    }

    writeGenerators(voltageLevel, context);
    writeBatteries(voltageLevel, context);
    writeLoads(voltageLevel, context);
    writeShuntCompensators(voltageLevel, context);
    writeDanglingLines(voltageLevel, context);
    writeStaticVarCompensators(voltageLevel, context);
    writeVscConverterStations(voltageLevel, context);
    writeLccConverterStations(voltageLevel, context);
}

void VoltageLevelXml::writeVscConverterStations(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    for (const auto& converterStation : voltageLevel.getVscConverterStations()) {
        if (!context.getFilter().test(converterStation)) {
            continue;
        }
        VscConverterStationXml::getInstance().write(converterStation, voltageLevel, context);
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

