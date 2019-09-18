/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamException.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename T, typename A, typename P>
template <typename S, typename O>
void AbstractConnectableXml<T, A, P>::readCurrentLimits(const boost::optional<int>& index, CurrentLimitsAdder<S,O>& adder, const powsybl::xml::XmlStreamReader& reader) {
    const double& permanentLimit = reader.getOptionalAttributeValue(PERMANENT_LIMIT, stdcxx::nan());
    adder.setPermanentLimit(permanentLimit);
    reader.readUntilEndElement(toString(CURRENT_LIMITS, index), [&adder, &reader]() {
        if (reader.getLocalName() == TEMPORARY_LIMIT) {
            const std::string& name = reader.getAttributeValue(NAME);
            int acceptableDuration = reader.getOptionalAttributeValue(ACCEPTABLE_DURATION, std::numeric_limits<int>::max());
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

template <typename T, typename A, typename P>
void AbstractConnectableXml<T, A, P>::readNodeOrBus(BranchAdder<A>& adder, const NetworkXmlReaderContext& context) {
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
    adder.setVoltageLevel1(voltageLevelId1);
    if (bus2) {
        adder.setBus2(context.getAnonymizer().deanonymizeString(*bus2));
    }
    if (connectableBus2) {
        adder.setConnectableBus2(context.getAnonymizer().deanonymizeString(*connectableBus2));
    }
    if (node2) {
        adder.setNode2(*node2);
    }
    adder.setVoltageLevel2(voltageLevelId2);
}

template <typename T, typename A, typename P>
void AbstractConnectableXml<T, A, P>::readNodeOrBus(InjectionAdder<A>& adder, const NetworkXmlReaderContext& context) {
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

template <typename T, typename A, typename P>
template <typename L>
void AbstractConnectableXml<T, A, P>::readNodeOrBus(int index, ThreeWindingsTransformerAdder::LegAdder<L>& adder, const NetworkXmlReaderContext& context){
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

template <typename T, typename A, typename P>
void AbstractConnectableXml<T, A, P>::readPQ(const boost::optional<int>& index, Terminal& terminal, const powsybl::xml::XmlStreamReader& reader) {
    const double& p = reader.getOptionalAttributeValue(toString(converter::P, index), stdcxx::nan());
    const double& q = reader.getOptionalAttributeValue(toString(Q, index), stdcxx::nan());
    terminal.setP(p).setQ(q);
}

template <typename T, typename A, typename P>
std::string AbstractConnectableXml<T, A, P>::toString(const char* string, const boost::optional<int>& index) {
    std::string str = string;
    if (index) {
        str += std::to_string(*index);
    }
    return str;
}

template <typename T, typename A, typename P>
void AbstractConnectableXml<T, A, P>::writeBus(const boost::optional<int>& index, const stdcxx::CReference<Bus>& bus, const stdcxx::CReference<Bus>& connectableBus, NetworkXmlWriterContext& context) {
    if (bus) {
        context.getWriter().writeAttribute(toString(BUS, index), context.getAnonymizer().anonymizeString(bus.get().getId()));
    }
    if (connectableBus) {
        context.getWriter().writeAttribute(toString(CONNECTABLE_BUS, index), context.getAnonymizer().anonymizeString(connectableBus.get().getId()));
    }
}

template <typename T, typename A, typename P>
void AbstractConnectableXml<T, A, P>::writeCurrentLimits(const boost::optional<int>& index, const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer) {
    writeCurrentLimits(index, limits, writer, IIDM_PREFIX);
}

template <typename T, typename A, typename P>
void AbstractConnectableXml<T, A, P>::writeCurrentLimits(const boost::optional<int>& index, const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const std::string& nsPrefix) {
    if (!std::isnan(limits.getPermanentLimit()) || !limits.getTemporaryLimits().empty()) {
        writer.writeStartElement(nsPrefix, toString(CURRENT_LIMITS, index));
        writer.writeAttribute(PERMANENT_LIMIT, limits.getPermanentLimit());

        for (const auto& tl : limits.getTemporaryLimits()) {
            writer.writeStartElement(nsPrefix, TEMPORARY_LIMIT);
            writer.writeAttribute(NAME, tl.get().getName());
            writer.writeOptionalAttribute(ACCEPTABLE_DURATION, tl.get().getAcceptableDuration(), std::numeric_limits<int>::max());
            writer.writeOptionalAttribute(VALUE, tl.get().getValue(), std::numeric_limits<double>::max());
            writer.writeOptionalAttribute(FICTITIOUS, tl.get().isFictitious(), false);
            writer.writeEndElement();
        }

        writer.writeEndElement();
    }
}

template <typename T, typename A, typename P>
void AbstractConnectableXml<T, A, P>::writeNode(const boost::optional<int>& index, const Terminal& terminal, NetworkXmlWriterContext& context) {
    context.getWriter().writeAttribute(toString(NODE, index), terminal.getNodeBreakerView().getNode());
}

template <typename T, typename A, typename P>
void AbstractConnectableXml<T, A, P>::writeNodeOrBus(const boost::optional<int>& index, const Terminal& terminal, NetworkXmlWriterContext& context) {
    const TopologyLevel& topologyLevel = getMinTopologyLevel(terminal.getVoltageLevel().getTopologyKind(), context.getOptions().getTopologyLevel());
    switch (topologyLevel) {
        case TopologyLevel::NODE_BREAKER:
            writeNode(index, terminal, context);
            break;
        case TopologyLevel::BUS_BREAKER:
            writeBus(index, terminal.getBusBreakerView().getBus(), terminal.getBusBreakerView().getConnectableBus(), context);
            break;
        case TopologyLevel::BUS_BRANCH:
            writeBus(index, terminal.getBusView().getBus(), terminal.getBusView().getConnectableBus(), context);
            break;
        default:
            throw powsybl::xml::XmlStreamException(logging::format("Unexpected TopologyLevel value: ", topologyLevel));
    }

    if (index.is_initialized()) {
        context.getWriter().writeAttribute(toString(VOLTAGE_LEVEL_ID, index), context.getAnonymizer().anonymizeString(terminal.getVoltageLevel().getId()));
    }
}

template <typename T, typename A, typename P>
void AbstractConnectableXml<T, A, P>::writePQ(const boost::optional<int>& index, const Terminal& terminal, powsybl::xml::XmlStreamWriter& writer) {
    writer.writeOptionalAttribute(toString(converter::P, index), terminal.getP());
    writer.writeOptionalAttribute(toString(Q, index), terminal.getQ());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

