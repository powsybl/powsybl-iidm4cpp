/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "AbstractSwitchXml.hpp"

#include <powsybl/iidm/SwitchKind.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename Adder>
const char* AbstractSwitchXml<Adder>::getRootElementName() const {
    return SWITCH;
}

template <typename Adder>
void AbstractSwitchXml<Adder>::readSubElements(Switch& sw, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(SWITCH, [this, &sw, &context]() {
        AbstractIdentifiableXml<Switch, Adder, VoltageLevel>::readSubElements(sw, context);
    });
}

template <typename Adder>
void AbstractSwitchXml<Adder>::writeRootElementAttributes(const Switch& sw, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(KIND, Enum::toString(sw.getKind()));
    context.getWriter().writeAttribute(RETAINED, sw.isRetained());
    context.getWriter().writeAttribute(OPEN, sw.isOpen());
    if (sw.isFictitious()) {
        context.getWriter().writeAttribute(FICTITIOUS, sw.isFictitious());
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

