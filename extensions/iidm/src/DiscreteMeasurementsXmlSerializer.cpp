/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/DiscreteMeasurementsXmlSerializer.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurement.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurementAdder.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurements.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurementsAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

DiscreteMeasurementsXmlSerializer::DiscreteMeasurementsXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("discreteMeasurements", "network", "dm", "http://www.powsybl.org/schema/iidm/ext/discrete_measurements/1_0") {
}

Extension& DiscreteMeasurementsXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    extendable.newExtension<DiscreteMeasurementsAdder>().add();
    auto& discreteMeasurements = extendable.getExtension<DiscreteMeasurements>();
    const xml::XmlStreamReader& reader = context.getReader();
    context.getReader().readUntilEndElement("discreteMeasurements", [&reader, &discreteMeasurements](){
        if (reader.getLocalName() == "discreteMeasurement") {
            readDiscreteMeasurement(discreteMeasurements, reader);
        } else {
            throw PowsyblException(stdcxx::format("Unexpected element: %1%", reader.getLocalName()));
        }
    });
    return discreteMeasurements;
}

void DiscreteMeasurementsXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    auto& discreteMeasurements = safeCast<DiscreteMeasurements>(extension);
    xml::XmlStreamWriter& writer = context.getWriter();
    for (const DiscreteMeasurement& discreteMeasurement : discreteMeasurements.getDiscreteMeasurements()) {
        writer.writeStartElement(getNamespacePrefix(), "discreteMeasurement");
        if (!discreteMeasurement.getId().empty()) {
            writer.writeAttribute(converter::ID, discreteMeasurement.getId());
        }
        writer.writeAttribute(converter::TYPE, Enum::toString(discreteMeasurement.getType()));
        if (discreteMeasurement.getTapChanger()) {
            writer.writeAttribute("tapChanger", Enum::toString(*discreteMeasurement.getTapChanger()));
        }
        writer.writeAttribute("valueType", Enum::toString(discreteMeasurement.getValueType()));
        switch (discreteMeasurement.getValueType()) {
            case DiscreteMeasurement::ValueType::BOOLEAN:
                writer.writeAttribute(converter::VALUE, discreteMeasurement.getValueAsBoolean());
                break;

            case DiscreteMeasurement::ValueType::INT:
                writer.writeAttribute(converter::VALUE, discreteMeasurement.getValueAsInt());
                break;

            case DiscreteMeasurement::ValueType::STRING:
                if (!discreteMeasurement.getValueAsString().empty()) {
                    writer.writeAttribute(converter::VALUE, discreteMeasurement.getValueAsString());
                }
                break;

            default:
                throw PowsyblException(stdcxx::format("Unsupported serialization for value type: %1%", discreteMeasurement.getValueType()));
        }
        writer.writeAttribute("valid", discreteMeasurement.isValid());
        for (const std::string& name : discreteMeasurement.getPropertyNames()) {
            writer.writeStartElement(getNamespacePrefix(), converter::PROPERTY);
            writer.writeAttribute(converter::NAME, name);
            writer.writeAttribute(converter::VALUE, discreteMeasurement.getProperty(name));
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
}

void DiscreteMeasurementsXmlSerializer::readDiscreteMeasurement(DiscreteMeasurements& discreteMeasurements, const xml::XmlStreamReader& reader) {
    DiscreteMeasurementAdder adder = discreteMeasurements.newDiscreteMeasurement()
        .setId(reader.getAttributeValue(converter::ID))
        .setType(Enum::fromString<DiscreteMeasurement::Type>(reader.getAttributeValue(converter::TYPE)))
        .setValid(reader.getAttributeValue<bool>("valid"));
    const std::string& tapChanger = reader.getOptionalAttributeValue("tapChanger", "");
    if (!tapChanger.empty()) {
        adder.setTapChanger(Enum::fromString<DiscreteMeasurement::TapChanger>(tapChanger));
    }
    auto valueType = Enum::fromString<DiscreteMeasurement::ValueType>(reader.getAttributeValue("valueType"));
    if (!reader.getOptionalAttributeValue(converter::VALUE, "").empty()) {
        switch (valueType) {
            case DiscreteMeasurement::ValueType::BOOLEAN:
                adder.setValue(reader.getAttributeValue<bool>(converter::VALUE));
                break;

            case DiscreteMeasurement::ValueType::INT:
                adder.setValue(reader.getAttributeValue<int>(converter::VALUE));
                break;

            case DiscreteMeasurement::ValueType::STRING:
                adder.setValue(reader.getAttributeValue(converter::VALUE));
                break;

            default:
                throw PowsyblException(stdcxx::format("Unsupported value type: %1%", valueType));
        }
    }
    reader.readUntilEndElement("discreteMeasurement", [&reader, &adder]() {
        if (reader.getLocalName() == converter::PROPERTY) {
            adder.putProperty(reader.getAttributeValue(converter::NAME), reader.getAttributeValue(converter::VALUE));
        } else {
            throw PowsyblException(stdcxx::format("Unexpected element: %1%", reader.getLocalName()));
        }
    });
    adder.add();
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
