/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamException.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

void AbstractConnectableXml::readActivePowerLimits(ActivePowerLimitsAdder&& adder, const NetworkXmlReaderContext& context, const stdcxx::optional<int>& index) {
    readLoadingLimits(ACTIVE_POWER_LIMITS, std::move(adder), context, index);
}

void AbstractConnectableXml::readApparentPowerLimits(ApparentPowerLimitsAdder&& adder, const NetworkXmlReaderContext& context, const stdcxx::optional<int>& index) {
    readLoadingLimits(APPARENT_POWER_LIMITS, std::move(adder), context, index);
}

void AbstractConnectableXml::readCurrentLimits(CurrentLimitsAdder&& adder, const NetworkXmlReaderContext& context, const stdcxx::optional<int>& index) {
    readLoadingLimits(CURRENT_LIMITS, std::move(adder), context, index);
}

void AbstractConnectableXml::readNodeOrBus(int index, ThreeWindingsTransformerAdder::LegAdder& adder, const NetworkXmlReaderContext& context) {
    const auto& bus = context.getReader().getOptionalAttributeValue<std::string>(toString(BUS, index));
    const auto& connectableBus = context.getReader().getOptionalAttributeValue<std::string>(toString(CONNECTABLE_BUS, index));
    const auto& node = context.getReader().getOptionalAttributeValue<unsigned long>(toString(NODE, index));
    const std::string& voltageLevelId = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(toString(VOLTAGE_LEVEL_ID, index)));
    if (bus) {
        adder.setBus(context.getAnonymizer().deanonymizeString(*bus));
    }
    if (connectableBus) {
        adder.setConnectableBus(context.getAnonymizer().deanonymizeString(*connectableBus));
    }
    if (node) {
        adder.setNode(*node);
    }
    adder.setVoltageLevel(voltageLevelId);
}

void AbstractConnectableXml::readPQ(Terminal& terminal, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index) {
    const double& p = reader.getOptionalAttributeValue(toString(P, index), stdcxx::nan());
    const double& q = reader.getOptionalAttributeValue(toString(Q, index), stdcxx::nan());
    terminal.setP(p).setQ(q);
}

void AbstractConnectableXml::writeBus(const stdcxx::CReference<Bus>& bus, const stdcxx::CReference<Bus>& connectableBus, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index) {
    if (bus) {
        context.getWriter().writeAttribute(toString(BUS, index), context.getAnonymizer().anonymizeString(bus.get().getId()));
    }
    if (connectableBus) {
        context.getWriter().writeAttribute(toString(CONNECTABLE_BUS, index), context.getAnonymizer().anonymizeString(connectableBus.get().getId()));
    }
}

void AbstractConnectableXml::writeActivePowerLimits(const ActivePowerLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index) {
    writeLoadingLimits(limits, writer, version.getPrefix(), version, ACTIVE_POWER_LIMITS, index);
}


void AbstractConnectableXml::writeApparentPowerLimits(const ApparentPowerLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index) {
    writeLoadingLimits(limits, writer, version.getPrefix(), version, APPARENT_POWER_LIMITS, index);
}


void AbstractConnectableXml::writeCurrentLimits(const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index) {
    writeLoadingLimits(limits, writer, version.getPrefix(), version, CURRENT_LIMITS, index);
}


void AbstractConnectableXml::writeCurrentLimits(const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const std::string& nsPrefix, const IidmXmlVersion& version, const stdcxx::optional<int>& index) {
    writeLoadingLimits(limits, writer, nsPrefix, version, CURRENT_LIMITS, index);
}

void AbstractConnectableXml::writeNode(const Terminal& terminal, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index) {
    context.getWriter().writeAttribute(toString(NODE, index), terminal.getNodeBreakerView().getNode());
}

void AbstractConnectableXml::writeNodeOrBus(const Terminal& terminal, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index) {
    const TopologyLevel& topologyLevel = getMinTopologyLevel(terminal.getVoltageLevel().getTopologyKind(), context.getOptions().getTopologyLevel());
    switch (topologyLevel) {
        case TopologyLevel::NODE_BREAKER:
            writeNode(terminal, context, index);
            break;
        case TopologyLevel::BUS_BREAKER:
            writeBus(terminal.getBusBreakerView().getBus(), terminal.getBusBreakerView().getConnectableBus(), context, index);
            break;
        case TopologyLevel::BUS_BRANCH:
            writeBus(terminal.getBusView().getBus(), terminal.getBusView().getConnectableBus(), context, index);
            break;
        default:
            throw powsybl::xml::XmlStreamException(stdcxx::format("Unexpected TopologyLevel value: ", topologyLevel));
    }

    if (index) {
        context.getWriter().writeAttribute(toString(VOLTAGE_LEVEL_ID, index), context.getAnonymizer().anonymizeString(terminal.getVoltageLevel().getId()));
    }
}

void AbstractConnectableXml::writePQ(const Terminal& terminal, powsybl::xml::XmlStreamWriter& writer, const stdcxx::optional<int>& index) {
    writer.writeOptionalAttribute(toString(P, index), terminal.getP());
    writer.writeOptionalAttribute(toString(Q, index), terminal.getQ());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
