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
void AbstractConnectableXml<T, A, P>::readPQ(const boost::optional<int>& index, Terminal& terminal, const powsybl::xml::XmlStreamReader& reader) {
    const auto& p = reader.getOptionalAttributeValue(toString(converter::P, index), stdcxx::nan());
    const auto& q = reader.getOptionalAttributeValue(toString(Q, index), stdcxx::nan());
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
void AbstractConnectableXml<T, A, P>::writeBus(const boost::optional<int>& index, const stdcxx::Reference<Bus>& bus, const stdcxx::Reference<Bus>& connectableBus, NetworkXmlWriterContext& context) {
    if (bus) {
        context.getWriter().writeAttribute(toString(BUS, index), context.getAnonymizer().anonymizeString(bus.get().getId()));
    }
    if (connectableBus) {
        context.getWriter().writeAttribute(toString(CONNECTABLE_BUS, index), context.getAnonymizer().anonymizeString(connectableBus.get().getId()));
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
    writer.writeOptionalAttribute(toString(converter::P, index), terminal.getP(), stdcxx::nan());
    writer.writeOptionalAttribute(toString(Q, index), terminal.getQ(), stdcxx::nan());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

