/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/BranchObservabilityXmlSerializer.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/BranchObservability.hpp>
#include <powsybl/iidm/extensions/iidm/BranchObservabilityAdder.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/iidm/extensions/iidm/ObservabilityQuality.hpp>

#include <powsybl/stdcxx/reference.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BranchObservabilityXmlSerializer::BranchObservabilityXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("branchObservability", "network", "bo", "http://www.itesla_project.eu/schema/iidm/ext/branch_observability/1_0") {
}

Extension& BranchObservabilityXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    bool observable = context.getReader().getOptionalAttributeValue(OBSERVABLE, false);

    BranchObservabilityAdder adder = extendable.newExtension<BranchObservabilityAdder>().withObservable(observable);

    context.getReader().readUntilEndElement(getExtensionName(), [&adder, &context]() {
        if (context.getReader().getLocalName() == QUALITY_P) {
            auto standardDeviation = context.getReader().getAttributeValue<double>(STANDARD_DEVIATION);
            auto redundant = context.getReader().getOptionalAttributeValue(REDUNDANT, false);
            const auto& side = Enum::fromString<Branch::Side>(context.getReader().getAttributeValue(SIDE));
            readQualityP(standardDeviation, redundant, side, adder);
        } else if (context.getReader().getLocalName() == QUALITY_Q) {
            auto standardDeviation = context.getReader().getAttributeValue<double>(STANDARD_DEVIATION);
            auto redundant = context.getReader().getOptionalAttributeValue(REDUNDANT, false);
            const auto& side = Enum::fromString<Branch::Side>(context.getReader().getAttributeValue(SIDE));
            readQualityQ(standardDeviation, redundant, side, adder);
        } else {
            throw PowsyblException(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });

    adder.add();
    return extendable.getExtension<BranchObservability>();
}

void BranchObservabilityXmlSerializer::readQualityP(double standardDeviation, bool redundant, const Branch::Side& side, BranchObservabilityAdder& adder) {
    if (side == Branch::Side::ONE) {
        adder.withStandardDeviationP1(standardDeviation);
        if (redundant) {
            adder.withRedundantP1(redundant);
        }
    } else if (side == Branch::Side::TWO) {
        adder.withStandardDeviationP2(standardDeviation);
        if (redundant) {
            adder.withRedundantP2(redundant);
        }
    }
}

void BranchObservabilityXmlSerializer::readQualityQ(double standardDeviation, bool redundant, const Branch::Side& side, BranchObservabilityAdder& adder) {
    if (side == Branch::Side::ONE) {
        adder.withStandardDeviationQ1(standardDeviation);
        if (redundant) {
            adder.withRedundantQ1(redundant);
        }
    } else if (side == Branch::Side::TWO) {
        adder.withStandardDeviationQ2(standardDeviation);
        if (redundant) {
            adder.withRedundantQ2(redundant);
        }
    }
}

void BranchObservabilityXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& branchObservability = safeCast<BranchObservability>(extension);
    context.getWriter().writeOptionalAttribute(OBSERVABLE, branchObservability.isObservable(), false);
    writeOptionalQuality(QUALITY_P, Branch::Side::ONE, branchObservability.getQualityP1(), context.getWriter());
    writeOptionalQuality(QUALITY_P, Branch::Side::TWO, branchObservability.getQualityP2(), context.getWriter());
    writeOptionalQuality(QUALITY_Q, Branch::Side::ONE, branchObservability.getQualityQ1(), context.getWriter());
    writeOptionalQuality(QUALITY_Q, Branch::Side::TWO, branchObservability.getQualityQ2(), context.getWriter());
}

void BranchObservabilityXmlSerializer::writeOptionalQuality(const std::string& elementName, const Branch::Side& side, const stdcxx::CReference<ObservabilityQuality>& quality, xml::XmlStreamWriter& writer) const {
    if (quality) {
        writer.writeStartElement(getNamespacePrefix(), elementName);
        writer.writeAttribute(SIDE, Enum::toString(side));
        writer.writeAttribute(STANDARD_DEVIATION, quality.get().getStandardDeviation());
        writer.writeOptionalAttribute(REDUNDANT, quality.get().isRedundant(), false);
        writer.writeEndElement();
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
