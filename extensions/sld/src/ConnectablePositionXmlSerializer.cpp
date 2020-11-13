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
    auto& connectable = dynamic_cast<Connectable&>(extendable);

    ConnectablePosition::OptionalFeeder feeder;
    ConnectablePosition::OptionalFeeder feeder1;
    ConnectablePosition::OptionalFeeder feeder2;
    ConnectablePosition::OptionalFeeder feeder3;

    context.getReader().readUntilEndElement(getExtensionName(), [this, &context, &feeder, &feeder1, &feeder2, &feeder3]() {
        if (context.getReader().getLocalName() == "feeder") {
            feeder = readPosition(context);
        } else if (context.getReader().getLocalName() == "feeder1") {
            feeder1 = readPosition(context);
        } else if (context.getReader().getLocalName() == "feeder2") {
            feeder2 = readPosition(context);
        } else if (context.getReader().getLocalName() == "feeder3") {
            feeder3 = readPosition(context);
        } else {
            throw AssertionError(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });

    extendable.addExtension(stdcxx::make_unique<ConnectablePosition>(connectable, feeder, feeder1, feeder2, feeder3));
    return extendable.getExtension<ConnectablePosition>();
}

ConnectablePosition::Feeder ConnectablePositionXmlSerializer::readPosition(converter::xml::NetworkXmlReaderContext& context) const {
    const std::string& name = context.getReader().getAttributeValue("name");
    const auto& order = context.getReader().getAttributeValue<unsigned long>("order");
    const auto& direction = Enum::fromString<ConnectablePosition::Direction>(context.getReader().getAttributeValue("direction"));
    return ConnectablePosition::Feeder(name, order, direction);
}

void ConnectablePositionXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& connectablePosition = safeCast<ConnectablePosition>(extension);
    if (connectablePosition.getFeeder()) {
        writePosition(*connectablePosition.getFeeder(), context);
    }
    if (connectablePosition.getFeeder1()) {
        writePosition(*connectablePosition.getFeeder1(), context, 1);
    }
    if (connectablePosition.getFeeder2()) {
        writePosition(*connectablePosition.getFeeder2(), context, 2);
    }
    if (connectablePosition.getFeeder3()) {
        writePosition(*connectablePosition.getFeeder3(), context, 3);
    }
}

void ConnectablePositionXmlSerializer::writePosition(const ConnectablePosition::Feeder& feeder, converter::xml::NetworkXmlWriterContext& context, const boost::optional<int>& index) const {
    std::string elementName = "feeder";
    if (index) {
        elementName += std::to_string(*index);
    }
    context.getExtensionsWriter().writeStartElement(getNamespacePrefix(), elementName);
    context.getExtensionsWriter().writeAttribute("name", feeder.getName());
    context.getExtensionsWriter().writeAttribute("order", feeder.getOrder());
    context.getExtensionsWriter().writeAttribute("direction", Enum::toString(feeder.getDirection()));
    context.getExtensionsWriter().writeEndElement();
}

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
