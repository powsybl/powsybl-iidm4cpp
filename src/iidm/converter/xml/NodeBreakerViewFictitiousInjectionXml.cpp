/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerViewFictitiousInjectionXml.hpp"

#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

const NodeBreakerViewFictitiousInjectionXml& NodeBreakerViewFictitiousInjectionXml::getInstance() {
    static NodeBreakerViewFictitiousInjectionXml s_instance;
    return s_instance;
}

void NodeBreakerViewFictitiousInjectionXml::read(VoltageLevel& voltageLevel, const NetworkXmlReaderContext& context) const {
    const auto& node = context.getReader().getAttributeValue<unsigned long>(NODE);
    const double& fictP0 = context.getReader().getOptionalAttributeValue(FICTITIOUS_P0, stdcxx::nan());
    const double& fictQ0 = context.getReader().getOptionalAttributeValue(FICTITIOUS_Q0, stdcxx::nan());
    if (!std::isnan(fictP0)) {
        voltageLevel.getNodeBreakerView().setFictitiousP0(node, fictP0);
    }
    if (!std::isnan(fictQ0)) {
        voltageLevel.getNodeBreakerView().setFictitiousQ0(node, fictQ0);
    }
}

void NodeBreakerViewFictitiousInjectionXml::write(unsigned long node, double fictP0, double fictQ0, NetworkXmlWriterContext& context) const {
    context.getWriter().writeStartElement(context.getVersion().getPrefix(), FICTITIOUS_INJECTION);
    context.getWriter().writeAttribute(NODE, node);
    context.getWriter().writeOptionalAttribute(FICTITIOUS_P0, fictP0);
    context.getWriter().writeOptionalAttribute(FICTITIOUS_Q0, fictQ0);
    context.getWriter().writeEndElement();
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

