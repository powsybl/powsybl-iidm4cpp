/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HXX
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HXX

#include "AbstractConnectableXml.hpp"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamException.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::readActivePowerLimits(ActivePowerLimitsAdder&& adder, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index) {
    readLoadingLimits(ACTIVE_POWER_LIMITS, adder, reader, index);
}

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::readApparentPowerLimits(ApparentPowerLimitsAdder&& adder, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index) {
    readLoadingLimits(APPARENT_POWER_LIMITS, adder, reader, index);
}

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::readCurrentLimits(CurrentLimitsAdder&& adder, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index) {
    readLoadingLimits(CURRENT_LIMITS, adder, reader, index);
}

template <typename Added, typename Adder, typename Parent>
template <typename LimitsAdder>
void AbstractConnectableXml<Added, Adder, Parent>::readLoadingLimits(const std::string& type, LimitsAdder&& adder, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index) {
    double permanentLimit = reader.getOptionalAttributeValue(PERMANENT_LIMIT, stdcxx::nan());
    adder.setPermanentLimit(permanentLimit);
    reader.readUntilEndElement(toString(type.c_str(), index), [&reader, &adder]() {
        if (reader.getLocalName() == TEMPORARY_LIMIT) {
            const std::string& name = reader.getAttributeValue(NAME);
            unsigned long acceptableDuration = reader.getOptionalAttributeValue(ACCEPTABLE_DURATION, std::numeric_limits<unsigned long>::max());
            double value = reader.getOptionalAttributeValue(VALUE, std::numeric_limits<double>::max());
            bool fictitious = reader.getOptionalAttributeValue(FICTITIOUS, false);
            adder.beginTemporaryLimit()
                .setName(name)
                .setAcceptableDuration(acceptableDuration)
                .setValue(value)
                .setFictitious(fictitious)
                .endTemporaryLimit();
        }
    });
    adder.add();
}

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::readNodeOrBus(BranchAdder<Adder>& adder, const NetworkXmlReaderContext& context) {
    const auto& bus1 = context.getReader().getOptionalAttributeValue<std::string>(BUS1);
    const auto& connectableBus1 = context.getReader().getOptionalAttributeValue<std::string>(CONNECTABLE_BUS1);
    const auto& node1 = context.getReader().getOptionalAttributeValue<int>(NODE1);
    const std::string& voltageLevelId1 = context.getReader().getAttributeValue(VOLTAGE_LEVEL_ID1);
    const auto& bus2 = context.getReader().getOptionalAttributeValue<std::string>(BUS2);
    const auto& connectableBus2 = context.getReader().getOptionalAttributeValue<std::string>(CONNECTABLE_BUS2);
    const auto& node2 = context.getReader().getOptionalAttributeValue<int>(NODE2);
    const std::string& voltageLevelId2 = context.getReader().getAttributeValue(VOLTAGE_LEVEL_ID2);
    if (bus1) {
        adder.setBus1(context.getAnonymizer().deanonymizeString(*bus1));
    }
    if (connectableBus1) {
        adder.setConnectableBus1(context.getAnonymizer().deanonymizeString(*connectableBus1));
    }
    if (node1) {
        adder.setNode1(*node1);
    }
    adder.setVoltageLevel1(context.getAnonymizer().deanonymizeString(voltageLevelId1));
    if (bus2) {
        adder.setBus2(context.getAnonymizer().deanonymizeString(*bus2));
    }
    if (connectableBus2) {
        adder.setConnectableBus2(context.getAnonymizer().deanonymizeString(*connectableBus2));
    }
    if (node2) {
        adder.setNode2(*node2);
    }
    adder.setVoltageLevel2(context.getAnonymizer().deanonymizeString(voltageLevelId2));
}

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::readNodeOrBus(InjectionAdder<Adder>& adder, const NetworkXmlReaderContext& context) {
    const auto& bus = context.getReader().getOptionalAttributeValue<std::string>(BUS);
    const auto& connectableBus = context.getReader().getOptionalAttributeValue<std::string>(CONNECTABLE_BUS);
    const auto& node = context.getReader().getOptionalAttributeValue<unsigned long>(NODE);

    if (bus) {
        adder.setBus(context.getAnonymizer().deanonymizeString(*bus));
    }
    if (connectableBus) {
        adder.setConnectableBus(context.getAnonymizer().deanonymizeString(*connectableBus));
    }
    if (node) {
        adder.setNode(*node);
    }
}

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::readNodeOrBus(int index, ThreeWindingsTransformerAdder::LegAdder& adder, const NetworkXmlReaderContext& context) {
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

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::readPQ(Terminal& terminal, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index) {
    const double& p = reader.getOptionalAttributeValue(toString(P, index), stdcxx::nan());
    const double& q = reader.getOptionalAttributeValue(toString(Q, index), stdcxx::nan());
    terminal.setP(p).setQ(q);
}

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::writeBus(const stdcxx::CReference<Bus>& bus, const stdcxx::CReference<Bus>& connectableBus, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index) {
    if (bus) {
        context.getWriter().writeAttribute(toString(BUS, index), context.getAnonymizer().anonymizeString(bus.get().getId()));
    }
    if (connectableBus) {
        context.getWriter().writeAttribute(toString(CONNECTABLE_BUS, index), context.getAnonymizer().anonymizeString(connectableBus.get().getId()));
    }
}

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::writeActivePowerLimits(const ActivePowerLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index) {
    writeLoadingLimits(limits, writer, version.getPrefix(), version, ACTIVE_POWER_LIMITS, index);
}

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::writeApparentPowerLimits(const ApparentPowerLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index) {
    writeLoadingLimits(limits, writer, version.getPrefix(), version, APPARENT_POWER_LIMITS, index);
}

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::writeCurrentLimits(const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index) {
    writeLoadingLimits(limits, writer, version.getPrefix(), version, CURRENT_LIMITS, index);
}

template <typename Added, typename Adder, typename Parent>
template <typename Limits>
void AbstractConnectableXml<Added, Adder, Parent>::writeLoadingLimits(const Limits& limits, powsybl::xml::XmlStreamWriter& writer, const std::string& nsPrefix, const IidmXmlVersion& version, const std::string& type, const stdcxx::optional<int>& index) {
    if (!std::isnan(limits.getPermanentLimit()) || !boost::empty(limits.getTemporaryLimits())) {
        writer.writeStartElement(nsPrefix, toString(type.c_str(), index));
        writer.writeAttribute(PERMANENT_LIMIT, limits.getPermanentLimit());

        for (const auto& tl : limits.getTemporaryLimits()) {
            writer.writeStartElement(version.getPrefix(), TEMPORARY_LIMIT);
            writer.writeAttribute(NAME, tl.getName());
            writer.writeOptionalAttribute(ACCEPTABLE_DURATION, tl.getAcceptableDuration(), std::numeric_limits<unsigned long>::max());
            writer.writeOptionalAttribute(VALUE, tl.getValue(), std::numeric_limits<double>::max());
            writer.writeOptionalAttribute(FICTITIOUS, tl.isFictitious(), false);
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
}

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::writeNode(const Terminal& terminal, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index) {
    context.getWriter().writeAttribute(toString(NODE, index), terminal.getNodeBreakerView().getNode());
}

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::writeNodeOrBus(const Terminal& terminal, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index) {
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

template <typename Added, typename Adder, typename Parent>
void AbstractConnectableXml<Added, Adder, Parent>::writePQ(const Terminal& terminal, powsybl::xml::XmlStreamWriter& writer, const stdcxx::optional<int>& index) {
    writer.writeOptionalAttribute(toString(P, index), terminal.getP());
    writer.writeOptionalAttribute(toString(Q, index), terminal.getQ());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HXX
