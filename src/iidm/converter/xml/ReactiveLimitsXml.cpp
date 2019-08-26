/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ReactiveLimitsXml.hpp"

#include <powsybl/iidm/MinMaxReactiveLimits.hpp>
#include <powsybl/iidm/ReactiveLimitsHolder.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include "iidm/converter/Constants.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

const ReactiveLimitsXml& ReactiveLimitsXml::getInstance() {
    static ReactiveLimitsXml s_instance;
    return s_instance;
}

void ReactiveLimitsXml::read(ReactiveLimitsHolder& holder, const NetworkXmlReaderContext& context) const {
    if (context.getReader().getLocalName() == REACTIVE_CAPABILITY_CURVE) {
        ReactiveCapabilityCurveAdder curveAdder = holder.newReactiveCapabilityCurve();
        context.getReader().readUntilEndElement(REACTIVE_CAPABILITY_CURVE, [&curveAdder, &context]() {
            if (context.getReader().getLocalName() == POINT) {
                const auto& p = context.getReader().getAttributeValue<double>(P);
                const auto& minQ = context.getReader().getAttributeValue<double>(MIN_Q);
                const auto& maxQ = context.getReader().getAttributeValue<double>(MAX_Q);
                curveAdder.beginPoint()
                    .setP(p)
                    .setMinQ(minQ)
                    .setMaxQ(maxQ)
                    .endPoint();
            } else {
                throw PowsyblException(logging::format("Unexpected element <%1%>", context.getReader().getLocalName()));
            }
        });
        curveAdder.add();
    } else if (context.getReader().getLocalName() == MIN_MAX_REACTIVE_LIMITS) {
        const auto& min = context.getReader().getAttributeValue<double>(MIN_Q);
        const auto& max = context.getReader().getAttributeValue<double>(MAX_Q);
        holder.newMinMaxReactiveLimits()
            .setMinQ(min)
            .setMaxQ(max)
            .add();
    } else {
        throw PowsyblException(logging::format("Unknown XML element <%1%>", context.getReader().getLocalName()));
    }
}

void ReactiveLimitsXml::write(const ReactiveLimitsHolder& holder, NetworkXmlWriterContext& context) const {
    switch (holder.getReactiveLimits<ReactiveLimits>().getKind()) {
        case ReactiveLimitsKind::CURVE: {
                const auto& curve = holder.getReactiveLimits<ReactiveCapabilityCurve>();
                context.getWriter().writeStartElement(IIDM_PREFIX, REACTIVE_CAPABILITY_CURVE);
                for (const auto& point : curve.getPoints()) {
                    context.getWriter().writeStartElement(IIDM_PREFIX, POINT);
                    context.getWriter().writeAttribute(P, point.getP());
                    context.getWriter().writeAttribute(MIN_Q, point.getMinQ());
                    context.getWriter().writeAttribute(MAX_Q, point.getMaxQ());
                    context.getWriter().writeEndElement();
                }
                context.getWriter().writeEndElement();
                break;
            }

        case ReactiveLimitsKind::MIN_MAX: {
                const auto& limits = holder.getReactiveLimits<MinMaxReactiveLimits>();
                context.getWriter().writeStartElement(IIDM_PREFIX, MIN_MAX_REACTIVE_LIMITS);
                context.getWriter().writeAttribute(MIN_Q, limits.getMinQ());
                context.getWriter().writeAttribute(MAX_Q, limits.getMaxQ());
                context.getWriter().writeEndElement();
                break;
            }

        default:
            throw PowsyblException(logging::format("Unknown reactive limit kind"));
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

