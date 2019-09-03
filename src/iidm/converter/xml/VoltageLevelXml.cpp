/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "VoltageLevelXml.hpp"

#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/TopologyLevel.hpp>
#include <powsybl/xml/XmlStreamException.hpp>

#include "iidm/converter/Constants.hpp"

#include "BusBreakerViewSwitchXml.hpp"
#include "BusXml.hpp"
#include "GeneratorXml.hpp"
#include "LoadXml.hpp"
#include "ShuntCompensatorXml.hpp"
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

bool VoltageLevelXml::hasSubElements(const VoltageLevel& /*voltageLevel*/) const {
    return true;
}

VoltageLevel& VoltageLevelXml::readRootElementAttributes(VoltageLevelAdder& adder, const NetworkXmlReaderContext& context) const {
    auto nominalVoltage = context.getReader().getAttributeValue<double>(NOMINAL_V);
    double lowVoltageLimit = context.getReader().getOptionalAttributeValue(LOW_VOLTAGE_LIMIT, stdcxx::nan());
    double highVoltageLimit = context.getReader().getOptionalAttributeValue(HIGH_VOLTAGE_LIMIT, stdcxx::nan());
    const TopologyKind& topologyKing = getTopologyKind(context.getReader().getAttributeValue(TOPOLOGY_KIND));
    return adder.setNominalVoltage(nominalVoltage)
                .setLowVoltageLimit(lowVoltageLimit)
                .setHighVoltageLimit(highVoltageLimit)
                .setTopologyKind(topologyKing)
                .add();
}

void VoltageLevelXml::readSubElements(VoltageLevel& voltageLevel, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(VOLTAGE_LEVEL, [this, &voltageLevel, &context]() {
        if (context.getReader().getLocalName() == BUS_BREAKER_TOPOLOGY) {
            context.getReader().readUntilEndElement(BUS_BREAKER_TOPOLOGY, [&voltageLevel, &context]() {
                if (context.getReader().getLocalName() == BUS) {
                    BusXml::getInstance().read(voltageLevel, context);
                } else if (context.getReader().getLocalName() == SWITCH) {
                    BusBreakerViewSwitchXml::getInstance().read(voltageLevel, context);
                } else {
                    throw powsybl::xml::XmlStreamException(logging::format("Unsupported element %1%", context.getReader().getLocalName()));
                }
            });
        } else if (context.getReader().getLocalName() == GENERATOR) {
            GeneratorXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == LOAD) {
            LoadXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == SHUNT) {
            ShuntCompensatorXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == VSC_CONVERTER_STATION) {
            VscConverterStationXml::getInstance().read(voltageLevel, context);
        } else {
            AbstractIdentifiableXml::readSubElements(voltageLevel, context);
        }
    });
}

void VoltageLevelXml::writeBusBreakerTopology(const VoltageLevel& voltageLevel, NetworkXmlWriterContext context) const {
    context.getWriter().writeStartElement(IIDM_PREFIX, BUS_BREAKER_TOPOLOGY);
    for (const Bus& bus : voltageLevel.getBusBreakerView().getBuses()) {
        // TODO(sebalaig) consider filtering
//        if (!context.getFilter().test(b)) {
//            continue;
//        }
        BusXml::getInstance().write(bus, voltageLevel, context);
    }
    for (const Switch& sw : voltageLevel.getBusBreakerView().getSwitches()) {
        // TODO(sebalaig) consider filtering
//        const Bus& b1 = voltageLevel.getBusBreakerView().getBus1(context.getAnonymizer().anonymizeString(sw.getId()));
//        const Bus& b2 = voltageLevel.getBusBreakerView().getBus2(context.getAnonymizer().anonymizeString(sw.getId()));
//        if (!context.getFilter().test(b1) || !context.getFilter().test(b2)) {
//            continue;
//        }
        BusBreakerViewSwitchXml::getInstance().write(sw, voltageLevel, context);
    }
    context.getWriter().writeEndElement();
}

void VoltageLevelXml::writeGenerators(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    for (const auto& generator : voltageLevel.getGenerators()) {
        // TODO(sebalaig) consider filtering
        GeneratorXml::getInstance().write(generator, voltageLevel, context);
    }
}

void VoltageLevelXml::writeLoads(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    for (const auto& load : voltageLevel.getLoads()) {
        // TODO(sebalaig) consider filtering
        LoadXml::getInstance().write(load, voltageLevel, context);
    }
}

void VoltageLevelXml::writeShuntCompensators(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    for (const auto& shuntCompensator : voltageLevel.getShuntCompensators()) {
        // TODO(sebalaig) consider filtering
        ShuntCompensatorXml::getInstance().write(shuntCompensator, voltageLevel, context);
    }
}

void VoltageLevelXml::writeRootElementAttributes(const VoltageLevel& voltageLevel, const Substation& /*substation*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(NOMINAL_V, voltageLevel.getNominalVoltage());
    context.getWriter().writeOptionalAttribute(LOW_VOLTAGE_LIMIT, voltageLevel.getLowVoltageLimit());
    context.getWriter().writeOptionalAttribute(HIGH_VOLTAGE_LIMIT, voltageLevel.getHighVoltageLimit());
    const TopologyLevel& topologyLevel = getMinTopologyLevel(voltageLevel.getTopologyKind(), context.getOptions().getTopologyLevel());
    const TopologyKind& topologyKind = getTopologyKind(topologyLevel);
    context.getWriter().writeAttribute(TOPOLOGY_KIND, getTopologyKindName(topologyKind));
}

void VoltageLevelXml::writeSubElements(const VoltageLevel& voltageLevel, const Substation& /*substation*/, NetworkXmlWriterContext& context) const {
    TopologyLevel topologyLevel = getMinTopologyLevel(voltageLevel.getTopologyKind(), context.getOptions().getTopologyLevel());
    switch (topologyLevel) {
        case TopologyLevel::BUS_BREAKER:
            writeBusBreakerTopology(voltageLevel, context);
            break;

        case TopologyLevel::NODE_BREAKER:
        case TopologyLevel::BUS_BRANCH:
            throw powsybl::xml::XmlStreamException(logging::format("Unsupported TopologyLevel value: ", topologyLevel));

        default:
            throw powsybl::xml::XmlStreamException(logging::format("Unexpected TopologyLevel value: ", topologyLevel));
    }

    writeGenerators(voltageLevel, context);
    writeLoads(voltageLevel, context);
    writeShuntCompensators(voltageLevel, context);
    writeVscConverterStations(voltageLevel, context);
}

void VoltageLevelXml::writeVscConverterStations(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    for (const auto& converterStation : voltageLevel.getVscConverterStations()) {
        // TODO(sebalaig) consider filtering
        VscConverterStationXml::getInstance().write(converterStation, voltageLevel, context);
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

