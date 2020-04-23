/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "VoltageLevelXml.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/TopologyLevel.hpp>
#include <powsybl/iidm/VoltageLevelViews.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/util/Networks.hpp>
#include <powsybl/xml/XmlStreamException.hpp>

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

void VoltageLevelXml::readBusBreakerTopology(VoltageLevel& voltageLevel, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(BUS_BREAKER_TOPOLOGY, [&voltageLevel, &context]() {
        if (context.getReader().getLocalName() == BUS) {
            BusXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == SWITCH) {
            BusBreakerViewSwitchXml::getInstance().read(voltageLevel, context);
        } else {
            throw AssertionError(logging::format("Unsupported element %1%", context.getReader().getLocalName()));
        }
    });
}

void VoltageLevelXml::readCalculatedBus(VoltageLevel &voltageLevel, NetworkXmlReaderContext& context) const {
    IidmXmlUtil::assertMinimumVersion(VOLTAGE_LEVEL, BUS, xml::ErrorMessage::NOT_SUPPORTED, xml::IidmXmlVersion::V1_1(), context.getVersion());
    double v = context.getReader().getOptionalAttributeValue(V, stdcxx::nan());
    double angle = context.getReader().getOptionalAttributeValue(ANGLE, stdcxx::nan());
    std::shared_ptr<std::string> nodesString = std::make_shared<std::string>(context.getReader().getAttributeValue(NODES));
    context.addEndTask([v, angle, nodesString, &voltageLevel]() {
        std::vector<std::string> nodes;
        boost::algorithm::split(nodes, *nodesString, [](char c) { return c == ','; });
        for (const std::string& nodeStr : nodes) {
            unsigned long node = std::stoul(nodeStr);
            const auto& terminal = voltageLevel.getNodeBreakerView().getTerminal(node);
            if (terminal) {
                const auto& b = terminal.get().getBusView().getBus();
                if (b) {
                    b.get().setV(v).setAngle(angle);
                    break;
                }
            }
        }
    });
}

void VoltageLevelXml::readNodeBreakerTopology(VoltageLevel& voltageLevel, NetworkXmlReaderContext& context) const {
    const auto& nodeCount = context.getReader().getAttributeValue<unsigned long>(NODE_COUNT);
    voltageLevel.getNodeBreakerView().setNodeCount(nodeCount);
    context.getReader().readUntilEndElement(NODE_BREAKER_TOPOLOGY, [this, &voltageLevel, &context]() {
        if (context.getReader().getLocalName() == BUSBAR_SECTION) {
            BusbarSectionXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == SWITCH) {
            NodeBreakerViewSwitchXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == INTERNAL_CONNECTION) {
            NodeBreakerViewInternalConnectionXml::getInstance().read(voltageLevel, context);
        } else if (context.getReader().getLocalName() == BUS) {
            readCalculatedBus(voltageLevel, context);
        } else {
            throw AssertionError(logging::format("Unexpected element %1%", context.getReader().getLocalName()));
        }
    });
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
            readNodeBreakerTopology(voltageLevel, context);
        } else if (context.getReader().getLocalName() == BUS_BREAKER_TOPOLOGY) {
            readBusBreakerTopology(voltageLevel, context);
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
    context.getWriter().writeStartElement(IIDM_PREFIX, BUS_BREAKER_TOPOLOGY);
    for (const Bus& bus : voltageLevel.getBusView().getBuses()) {
        if (!context.getFilter().test(bus)) {
            continue;
        }
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

void VoltageLevelXml::writeCalculatedBus(const Bus& bus, const std::set<unsigned long>& nodes, NetworkXmlWriterContext& context) {
    context.getWriter().writeStartElement(IIDM_PREFIX, BUS);
    context.getWriter().writeAttribute(V, bus.getV());
    context.getWriter().writeAttribute(ANGLE, bus.getAngle());
    const auto& mapper = [](const unsigned long& i) { return std::to_string(i); };
    context.getWriter().writeAttribute(NODES, boost::algorithm::join(nodes | boost::adaptors::transformed(mapper), ","));
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

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_1(), context.getVersion(), [&context, &voltageLevel]() {
        const auto& nodesByBus = util::Networks::getNodesByBus(voltageLevel);
        for (const auto& pair : nodesByBus) {
            const stdcxx::CReference<Bus>& bus = voltageLevel.getBusView().getBus(pair.first);
            if (bus && (!std::isnan(bus.get().getV()) || !std::isnan(bus.get().getAngle()))) {
                writeCalculatedBus(bus.get(), pair.second, context);
            }
        }
    });

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
            throw AssertionError(logging::format("Unexpected TopologyLevel value: ", topologyLevel));
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

