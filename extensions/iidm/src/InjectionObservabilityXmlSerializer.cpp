/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/InjectionObservabilityXmlSerializer.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/InjectionObservability.hpp>
#include <powsybl/iidm/extensions/iidm/InjectionObservabilityAdder.hpp>
#include <powsybl/iidm/extensions/iidm/ObservabilityQuality.hpp>

#include <powsybl/stdcxx/reference.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

InjectionObservabilityXmlSerializer::InjectionObservabilityXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("injectionObservability", "network", "io", "http://www.itesla_project.eu/schema/iidm/ext/injection_observability/1_0") {
}

Extension& InjectionObservabilityXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    bool observable = context.getReader().getOptionalAttributeValue(OBSERVABLE, false);

    InjectionObservabilityAdder adder = extendable.newExtension<InjectionObservabilityAdder>().withObservable(observable);

    context.getReader().readUntilEndElement(getExtensionName(), [&adder, &context]() {
        if (context.getReader().getLocalName() == QUALITY_P) {
            auto standardDeviation = context.getReader().getAttributeValue<double>(STANDARD_DEVIATION);
            auto redundant = context.getReader().getOptionalAttributeValue(REDUNDANT, false);
            adder.withStandardDeviationP(standardDeviation)
                    .withRedundantP(redundant);
        } else if (context.getReader().getLocalName() == QUALITY_Q) {
            auto standardDeviation = context.getReader().getAttributeValue<double>(STANDARD_DEVIATION);
            auto redundant = context.getReader().getOptionalAttributeValue(REDUNDANT, false);
            adder.withStandardDeviationQ(standardDeviation)
                    .withRedundantQ(redundant);
        } else if (context.getReader().getLocalName() == QUALITY_V) {
            auto standardDeviation = context.getReader().getAttributeValue<double>(STANDARD_DEVIATION);
            auto redundant = context.getReader().getOptionalAttributeValue(REDUNDANT, false);
            adder.withStandardDeviationV(standardDeviation)
                    .withRedundantV(redundant);
        } else {
            throw PowsyblException(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });

    adder.add();
    return extendable.getExtension<InjectionObservability>();
}

void InjectionObservabilityXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& injectionObservability = safeCast<InjectionObservability>(extension);
    context.getWriter().writeOptionalAttribute(OBSERVABLE, injectionObservability.isObservable(), false);
    writeOptionalQuality(QUALITY_P, injectionObservability.getQualityP(), context.getWriter());
    writeOptionalQuality(QUALITY_Q, injectionObservability.getQualityQ(), context.getWriter());
    writeOptionalQuality(QUALITY_V, injectionObservability.getQualityV(), context.getWriter());
}

void InjectionObservabilityXmlSerializer::writeOptionalQuality(const std::string& elementName, const stdcxx::CReference<ObservabilityQuality>& quality, xml::XmlStreamWriter& writer) const {
    if (quality) {
        writer.writeStartElement(getNamespacePrefix(), elementName);
        writer.writeAttribute(STANDARD_DEVIATION, quality.get().getStandardDeviation());
        if(quality.get().getRedundant().has_value()) {
            writer.writeAttribute(REDUNDANT, quality.get().isRedundant());
        }
        writer.writeEndElement();
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
