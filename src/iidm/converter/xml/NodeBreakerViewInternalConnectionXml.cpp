/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerViewInternalConnectionXml.hpp"

#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include "iidm/converter/Constants.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

const NodeBreakerViewInternalConnectionXml& NodeBreakerViewInternalConnectionXml::getInstance() {
    static NodeBreakerViewInternalConnectionXml s_instance;
    return s_instance;
}

void NodeBreakerViewInternalConnectionXml::read(VoltageLevel& voltageLevel, const NetworkXmlReaderContext& context) const {
    const auto& node1 = context.getReader().getAttributeValue<unsigned long>(NODE1);
    const auto& node2 = context.getReader().getAttributeValue<unsigned long>(NODE2);
    voltageLevel.getNodeBreakerView().newInternalConnection().setNode1(node1).setNode2(node2).add();
}

void NodeBreakerViewInternalConnectionXml::write(unsigned long node1, unsigned long node2, NetworkXmlWriterContext& context) const {
    context.getWriter().writeStartElement(IIDM_PREFIX, INTERNAL_CONNECTION);
    context.getWriter().writeAttribute(NODE1, node1);
    context.getWriter().writeAttribute(NODE2, node2);
    context.getWriter().writeEndElement();
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

