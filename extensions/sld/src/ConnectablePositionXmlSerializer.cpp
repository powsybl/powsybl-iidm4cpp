/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/sld/ConnectablePositionXmlSerializer.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

ConnectablePositionXmlSerializer::ConnectablePositionXmlSerializer() :
    AbstractExtensionXmlSerializer("position", "network", "cp", "http://www.itesla_project.eu/schema/iidm/ext/connectable_position/1_0") {
}

Extension& ConnectablePositionXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Connectable>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Connectable>()));
    }
    auto adder = extendable.newExtension<ConnectablePositionAdder>();

    context.getReader().readUntilEndElement(getExtensionName(), [this, &context, &adder]() {
        if (context.getReader().getLocalName() == "feeder") {
            ConnectablePositionAdder::FeederAdder feederAdder = adder.newFeeder();
            readPosition(context, feederAdder);
        } else if (context.getReader().getLocalName() == "feeder1") {
            ConnectablePositionAdder::FeederAdder feederAdder = adder.newFeeder1();
            readPosition(context, feederAdder);
        } else if (context.getReader().getLocalName() == "feeder2") {
            ConnectablePositionAdder::FeederAdder feederAdder = adder.newFeeder2();
            readPosition(context, feederAdder);
        } else if (context.getReader().getLocalName() == "feeder3") {
            ConnectablePositionAdder::FeederAdder feederAdder = adder.newFeeder3();
            readPosition(context, feederAdder);
        } else {
            throw AssertionError(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });
    adder.add();
    return extendable.getExtension<ConnectablePosition>();
}

void ConnectablePositionXmlSerializer::readPosition(const converter::xml::NetworkXmlReaderContext& context, ConnectablePositionAdder::FeederAdder& feederAdder) const {
    const auto& name = context.getReader().getAttributeValue("name");
    const auto& order = context.getReader().getAttributeValue<unsigned long>("order");
    const auto& direction = Enum::fromString<ConnectablePosition::Direction>(context.getReader().getAttributeValue("direction"));
    feederAdder.withName(name).withOrder(order).withDirection(direction).add();
}

void ConnectablePositionXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& connectablePosition = safeCast<ConnectablePosition>(extension);
    if (connectablePosition.getFeeder()) {
        writePosition(connectablePosition.getFeeder().get(), context);
    }
    if (connectablePosition.getFeeder1()) {
        writePosition(connectablePosition.getFeeder1().get(), context, 1);
    }
    if (connectablePosition.getFeeder2()) {
        writePosition(connectablePosition.getFeeder2().get(), context, 2);
    }
    if (connectablePosition.getFeeder3()) {
        writePosition(connectablePosition.getFeeder3().get(), context, 3);
    }
}

void ConnectablePositionXmlSerializer::writePosition(const ConnectablePosition::Feeder& feeder, converter::xml::NetworkXmlWriterContext& context, const stdcxx::optional<int>& index) const {
    std::string elementName = "feeder";
    if (index) {
        elementName += std::to_string(*index);
    }
    context.getWriter().writeStartElement(getNamespacePrefix(), elementName);
    context.getWriter().writeAttribute("name", feeder.getName());
    context.getWriter().writeAttribute("order", feeder.getOrder());
    context.getWriter().writeAttribute("direction", Enum::toString(feeder.getDirection()));
    context.getWriter().writeEndElement();
}

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
