/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "DroopCurveXml.hpp"

#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

const DroopCurveXml& DroopCurveXml::getInstance() {
    static DroopCurveXml s_instance;
    return s_instance;
}

void DroopCurveXml::read(AcDcConverter& converter, const NetworkXmlReaderContext& context) const {
    if(context.getReader().getLocalName() == DROOP_CURVE) {
        DroopCurveAdder curveAdder = converter.newDroopCurve();
        read(curveAdder, context);
        curveAdder.add(); // Will create an "empty" DroopCurve even if no segment - Round trip will fail, since writing such a DroopCurve will write nothing at all.
    }
}

void DroopCurveXml::write(const AcDcConverter& converter, NetworkXmlWriterContext& context) const {
    const auto& droopCurve = converter.getDroopCurve();
    if(!droopCurve) { // no curve to write
        return;
    }

    write(droopCurve.get(), context);
}

void DroopCurveXml::read(DroopCurveAdder& curveAdder, const NetworkXmlReaderContext& context) const {

    context.getReader().readUntilEndElement(DROOP_CURVE, [&curveAdder, &context]() {
        if (context.getReader().getLocalName() == SEGMENT) {

            const auto& minV = context.getReader().getAttributeValue<double>(MIN_V);
            const auto& maxV = context.getReader().getAttributeValue<double>(MAX_V);
            const auto& k = context.getReader().getAttributeValue<double>(K);
            
            curveAdder.addSegment(minV,k)
                        .setMaxV(maxV); //segments are ordered, so the last segment's maxV will be the highest

        } else {
            throw PowsyblException(stdcxx::format("Unexpected element <%1%>", context.getReader().getLocalName()));
        }
    });

}

void DroopCurveXml::write(const DroopCurve& droopCurve, NetworkXmlWriterContext& context) const {
    const auto& segments = droopCurve.getSegments();
    if(segments.empty()) { //empty curve -> do not write !
        return;
    }

    context.getWriter().writeStartElement(context.getVersion().getPrefix(), DROOP_CURVE);
    for (const auto& segment : segments) {
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), SEGMENT);
        context.getWriter().writeAttribute(MIN_V, segment.getMinV());
        context.getWriter().writeAttribute(MAX_V, segment.getMaxV());
        context.getWriter().writeAttribute(K, segment.getK());
        context.getWriter().writeEndElement();
    }
    context.getWriter().writeEndElement();
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

