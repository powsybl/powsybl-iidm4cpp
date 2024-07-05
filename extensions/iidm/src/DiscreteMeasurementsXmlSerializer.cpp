/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/DiscreteMeasurementsXmlSerializer.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
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

bool DiscreteMeasurementsXmlSerializer::isSerializable(const Extension& extension) const {
    const auto& discreteMeasurements = safeCast<DiscreteMeasurements>(extension);
    return !discreteMeasurements.getDiscreteMeasurements().empty();
}

Extension& DiscreteMeasurementsXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    extendable.newExtension<DiscreteMeasurementsAdder>().add();
    auto& discreteMeasurements = extendable.getExtension<DiscreteMeasurements>();
    const xml::XmlStreamReader& reader = context.getReader();
    context.getReader().readUntilEndElement(DISCRETE_MEASUREMENTS, [&reader, &discreteMeasurements](){
        if (reader.getLocalName() == DISCRETE_MEASUREMENT) {
            readDiscreteMeasurement(discreteMeasurements, reader);
        } else {
            throw PowsyblException(stdcxx::format("Unexpected element: %1%", reader.getLocalName()));
        }
    });
    return discreteMeasurements;
}

void DiscreteMeasurementsXmlSerializer::readDiscreteMeasurement(DiscreteMeasurements& discreteMeasurements, const xml::XmlStreamReader& reader) {
    DiscreteMeasurementAdder adder = discreteMeasurements.newDiscreteMeasurement()
        .setType(Enum::fromString<DiscreteMeasurement::Type>(reader.getAttributeValue(TYPE)))
        .setValid(reader.getAttributeValue<bool>(VALID));
    const std::string& dmId = reader.getOptionalAttributeValue(ID, "");
    if (!dmId.empty()) {
        adder.setId(dmId);
    }
    const std::string& tapChanger = reader.getOptionalAttributeValue(TAP_CHANGER, "");
    if (!tapChanger.empty()) {
        adder.setTapChanger(Enum::fromString<DiscreteMeasurement::TapChanger>(tapChanger));
    }
    auto valueType = Enum::fromString<DiscreteMeasurement::ValueType>(reader.getAttributeValue(VALUE_TYPE));
    if (!reader.getOptionalAttributeValue(VALUE, "").empty()) {
        switch (valueType) {
            case DiscreteMeasurement::ValueType::BOOLEAN:
                adder.setValue(reader.getAttributeValue<bool>(VALUE));
                break;

            case DiscreteMeasurement::ValueType::INT:
                adder.setValue(reader.getAttributeValue<int>(VALUE));
                break;

            case DiscreteMeasurement::ValueType::STRING:
                adder.setValue(reader.getAttributeValue(VALUE));
                break;

            default:
                throw PowsyblException(stdcxx::format("Unsupported value type: %1%", valueType));
        }
    }
    reader.readUntilEndElement(DISCRETE_MEASUREMENT, [&reader, &adder]() {
        if (reader.getLocalName() == PROPERTY) {
            adder.putProperty(reader.getAttributeValue(NAME), reader.getAttributeValue(VALUE));
        } else {
            throw PowsyblException(stdcxx::format("Unexpected element: %1%", reader.getLocalName()));
        }
    });
    adder.add();
}

void DiscreteMeasurementsXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& discreteMeasurements = safeCast<DiscreteMeasurements>(extension);
    xml::XmlStreamWriter& writer = context.getWriter();
    for (const DiscreteMeasurement& discreteMeasurement : discreteMeasurements.getDiscreteMeasurements()) {
        writer.writeStartElement(getNamespacePrefix(), DISCRETE_MEASUREMENT);
        if (!discreteMeasurement.getId().empty()) {
            writer.writeAttribute(ID, discreteMeasurement.getId());
        }
        writer.writeAttribute(TYPE, Enum::toString(discreteMeasurement.getType()));
        if (discreteMeasurement.getTapChanger()) {
            writer.writeAttribute(TAP_CHANGER, Enum::toString(*discreteMeasurement.getTapChanger()));
        }
        writer.writeAttribute(VALUE_TYPE, Enum::toString(discreteMeasurement.getValueType()));
        switch (discreteMeasurement.getValueType()) {
            case DiscreteMeasurement::ValueType::BOOLEAN:
                writer.writeAttribute(VALUE, discreteMeasurement.getValueAsBoolean());
                break;

            case DiscreteMeasurement::ValueType::INT:
                writer.writeAttribute(VALUE, discreteMeasurement.getValueAsInt());
                break;

            case DiscreteMeasurement::ValueType::STRING:
                if (!discreteMeasurement.getValueAsString().empty()) {
                    writer.writeAttribute(VALUE, discreteMeasurement.getValueAsString());
                }
                break;

            default:
                throw PowsyblException(stdcxx::format("Unsupported serialization for value type: %1%", discreteMeasurement.getValueType()));
        }
        writer.writeAttribute(VALID, discreteMeasurement.isValid());
        for (const std::string& name : discreteMeasurement.getPropertyNames()) {
            writer.writeStartElement(getNamespacePrefix(), PROPERTY);
            writer.writeAttribute(NAME, name);
            writer.writeAttribute(VALUE, discreteMeasurement.getProperty(name));
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
