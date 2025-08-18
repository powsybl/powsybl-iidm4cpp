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
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/VersionsCompatibity.hpp>

#include <powsybl/stdcxx/map.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

ConnectablePositionXmlSerializer::ConnectablePositionXmlSerializer() :
    AbstractVersionableExtensionXmlSerializer("position", "network", "cp",
        converter::xml::VersionsCompatibilityBuilder()
            .put(converter::xml::IidmXmlVersion::V1_0(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_1(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_2(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_3(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_4(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_5(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_6(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_7(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_8(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_9(), {"1.0", "1.1"})
            .build(),
        stdcxx::MapBuilder<std::string, std::string>()
            .put("1.0", "http://www.itesla_project.eu/schema/iidm/ext/connectable_position/1_0")
            .put("1.1", "http://www.powsybl.org/schema/iidm/ext/connectable_position/1_1")
            .build()) {
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
    stdcxx::optional<std::string> name = context.getReader().getOptionalAttributeValue<std::string>("name");
    stdcxx::optional<unsigned long> order = context.getReader().getOptionalAttributeValue<unsigned long>("order");
    const auto& direction = Enum::fromString<ConnectablePosition::Direction>(context.getReader().getAttributeValue("direction"));
    if(name.has_value()) {
        feederAdder.withName(*name);
    } else {
        const std::string& extensionVersionStr = context.getExtensionVersion(*this);
        if (extensionVersionStr.empty()) {
            throw AssertionError("Extension version not found");
        }
        if(extensionVersionStr.compare("1.1") < 0) {
            throw PowsyblException("Feeder name is mandatory for version < 1.1");
        }
    }
    feederAdder.withDirection(direction);
    if(order.has_value()) {
        feederAdder.withOrder(*order);
    }
    feederAdder.add();
}

void ConnectablePositionXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& connectablePosition = safeCast<ConnectablePosition>(extension);
    const std::string& connectableId = connectablePosition.getExtendable<Connectable>().get().getId();
    if (connectablePosition.getFeeder()) {
        writePosition(connectableId, connectablePosition.getFeeder().get(), context);
    }
    if (connectablePosition.getFeeder1()) {
        writePosition(connectableId, connectablePosition.getFeeder1().get(), context, 1);
    }
    if (connectablePosition.getFeeder2()) {
        writePosition(connectableId, connectablePosition.getFeeder2().get(), context, 2);
    }
    if (connectablePosition.getFeeder3()) {
        writePosition(connectableId, connectablePosition.getFeeder3().get(), context, 3);
    }
}

void ConnectablePositionXmlSerializer::writePosition(const std::string& connectableId, const ConnectablePosition::Feeder& feeder, converter::xml::NetworkXmlWriterContext& context, const stdcxx::optional<int>& index) const {
    std::string elementName = "feeder";
    if (index) {
        elementName += std::to_string(*index);
    }

    context.getWriter().writeStartElement(getNamespacePrefix(), elementName);

    std::string extVersionStr = context.getExtensionVersion("position");
    if (extVersionStr.empty()) {
        extVersionStr = getVersion(context.getVersion());
    }
    stdcxx::optional<std::string> name = feeder.getName();
    if(extVersionStr == "1.0") {
        if(!name) {
            name = connectableId;
        }
        context.getWriter().writeAttribute("name", *name);
    } else if(extVersionStr == "1.1") {
        if(name.has_value()) {
            context.getWriter().writeAttribute("name", *name);
        }
    } else {
        throw PowsyblException(stdcxx::format("Unsupported version (%1%) for position", extVersionStr));
    }

    stdcxx::optional<unsigned long> order = feeder.getOrder();
    if(order.has_value()) {
        context.getWriter().writeAttribute("order", *order);
    }
    context.getWriter().writeAttribute("direction", Enum::toString(feeder.getDirection()));
    context.getWriter().writeEndElement();
}

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
