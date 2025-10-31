/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "GroundXml.hpp"

#include <powsybl/iidm/converter/Constants.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

GroundAdder GroundXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newGround();
}

const GroundXml& GroundXml::getInstance() {
    static GroundXml s_instance;
    return s_instance;
}

const char* GroundXml::getRootElementName() const {
    return GROUND;
}

void GroundXml::writeRootElementAttributes(const Ground& ground, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    writeNodeOrBus(ground.getTerminal(), context);
}

void GroundXml::readSubElements(Ground& ground, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(GROUND, [this, &ground, &context]() {
        AbstractSimpleIdentifiableXml::readSubElements(ground, context);
    });
}

Ground& GroundXml::readRootElementAttributes(GroundAdder& adder, VoltageLevel& /*voltageLevel*/, NetworkXmlReaderContext& context) const {
    readNodeOrBus(adder, context);
    Ground& ground = adder.add();
    return ground;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

