/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "AreaBoundaryXml.hpp"

#include "BoundaryRefXml.hpp"
#include <powsybl/iidm/Area.hpp>
#include <powsybl/iidm/AreaBoundary.hpp>
#include <powsybl/iidm/AreaBoundaryAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {


const AreaBoundaryXml& AreaBoundaryXml::getInstance() {
    static AreaBoundaryXml s_instance;
    return s_instance;
}

void AreaBoundaryXml::read(Area& area, NetworkXmlReaderContext& context) const {
    auto ac = context.getReader().getAttributeValue<bool>(AC);
    auto ptrAdder = std::make_shared<AreaBoundaryAdder>(area.newAreaBoundary());
    AreaBoundaryAdder& adder = *ptrAdder.get();
    adder.setAc(ac);
    const std::string& type = context.getReader().getAttributeValue(TYPE);
    if(type == TERMINAL_REF) {
        TerminalRefXml::readTerminalRef(area.getNetwork(), context, [&adder](Terminal& terminal) {
                adder.setTerminal(terminal);
        });
    } else if(type == BOUNDARY_REF) {
        BoundaryRefXml::readBoundaryRef(area.getNetwork(), context, [&adder](Boundary& boundary) {
                adder.setBoundary(boundary);
        });
    } else {
        throw PowsyblException(stdcxx::format("Unexpected element for AreaBoundary: %1%. Should be %2% or %3%", type, BOUNDARY_REF, TERMINAL_REF));
    }
    context.addEndTask([ptrAdder]() {
        ptrAdder->add();
    });
}

void AreaBoundaryXml::write(const Area& area, NetworkXmlWriterContext& context) const {
    for(const auto& areaBoundary : area.getAreaBoundaries()) {
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), AREA_BOUNDARY);
        context.getWriter().writeAttribute(AC, areaBoundary.isAc());
        auto terminalRef = areaBoundary.getTerminal();
        if(static_cast<bool>(terminalRef)) {
            context.getWriter().writeAttribute(TYPE, TERMINAL_REF);
            TerminalRefXml::writeTerminalRefAttribute(terminalRef.get(), context);
        }
        auto boundaryRef = areaBoundary.getBoundary();
        if(static_cast<bool>(boundaryRef)) {
            context.getWriter().writeAttribute(TYPE, BOUNDARY_REF);
            BoundaryRefXml::writeBoundaryRefAttributes(boundaryRef.get(), context);
        }
        context.getWriter().writeEndElement();
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

