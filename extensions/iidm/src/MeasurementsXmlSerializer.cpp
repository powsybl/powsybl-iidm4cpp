/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/MeasurementsXmlSerializer.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Measurement.hpp>
#include <powsybl/iidm/extensions/iidm/MeasurementAdder.hpp>
#include <powsybl/iidm/extensions/iidm/Measurements.hpp>
#include <powsybl/iidm/extensions/iidm/MeasurementsAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

MeasurementsXmlSerializer::MeasurementsXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("measurements", "network", "m", "http://www.powsybl.org/schema/iidm/ext/measurements/1_0") {
}

bool MeasurementsXmlSerializer::isSerializable(const Extension& extension) const {
    const auto& measurements = safeCast<Measurements>(extension);
    return !measurements.getMeasurements().empty();
}

Extension& MeasurementsXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Identifiable>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Identifiable>()));
    }
    extendable.newExtension<MeasurementsAdder>().add();
    auto& measurements = extendable.getExtension<Measurements>();
    context.getReader().readUntilEndElement("measurements", [&measurements, &context]() {
        if (context.getReader().getLocalName() == "measurement") {
            MeasurementAdder adder = measurements.newMeasurement()
                .setId(context.getReader().getAttributeValue(converter::ID))
                .setType(Enum::fromString<Measurement::Type>(context.getReader().getAttributeValue(converter::TYPE)))
                .setValue(context.getReader().getOptionalAttributeValue(converter::VALUE, stdcxx::nan()))
                .setStandardDeviation(context.getReader().getOptionalAttributeValue("standardDeviation", stdcxx::nan()))
                .setValid(context.getReader().getAttributeValue<bool>("valid"));
            const std::string& side = context.getReader().getOptionalAttributeValue(converter::SIDE, "");
            if (!side.empty()) {
                adder.setSide(Enum::fromString<Measurement::Side>(side));
            }
            context.getReader().readUntilEndElement("measurement", [&context, &adder]() {
                if (context.getReader().getLocalName() == converter::PROPERTY) {
                    adder.putProperty(context.getReader().getAttributeValue(converter::NAME), context.getReader().getAttributeValue(converter::VALUE));
                } else {
                    throw PowsyblException(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
                }
            });
            adder.add();
        } else {
            throw PowsyblException(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });
    return measurements;
}

void MeasurementsXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& measurements = safeCast<Measurements>(extension);
    xml::XmlStreamWriter& writer = context.getWriter();
    for (const Measurement& measurement : measurements.getMeasurements()) {
        writer.writeStartElement(getNamespacePrefix(), "measurement");
        if (!measurement.getId().empty()) {
            writer.writeAttribute(converter::ID, measurement.getId());
        }
        writer.writeAttribute(converter::TYPE, Enum::toString(measurement.getType()));
        if (measurement.getSide()) {
            writer.writeAttribute(converter::SIDE, Enum::toString(*measurement.getSide()));
        }
        writer.writeAttribute(converter::VALUE, measurement.getValue());
        writer.writeAttribute("standardDeviation", measurement.getStandardDeviation());
        writer.writeAttribute("valid", measurement.isValid());
        for (const std::string& name : measurement.getPropertyNames()) {
            writer.writeStartElement(getNamespacePrefix(), converter::PROPERTY);
            writer.writeAttribute(converter::NAME, name);
            writer.writeAttribute(converter::VALUE, measurement.getProperty(name));
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
