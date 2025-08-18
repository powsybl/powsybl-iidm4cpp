/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/SecondaryVoltageControlXmlSerializer.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/iidm/extensions/iidm/ControlUnit.hpp>
#include <powsybl/iidm/extensions/iidm/ControlUnitAdder.hpp>
#include <powsybl/iidm/extensions/iidm/ControlZone.hpp>
#include <powsybl/iidm/extensions/iidm/ControlZoneAdder.hpp>
#include <powsybl/iidm/extensions/iidm/PilotPoint.hpp>
#include <powsybl/iidm/extensions/iidm/PilotPointAdder.hpp>
#include <powsybl/iidm/extensions/iidm/SecondaryVoltageControl.hpp>
#include <powsybl/iidm/extensions/iidm/SecondaryVoltageControlAdder.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

SecondaryVoltageControlXmlSerializer::SecondaryVoltageControlXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("secondaryVoltageControl", "network", "svc", "http://www.powsybl.org/schema/iidm/ext/secondary_voltage_control/1_0") {
}

void SecondaryVoltageControlXmlSerializer::readControlZone(converter::xml::NetworkXmlReaderContext& context, SecondaryVoltageControlAdder& svcAdder) const {
    std::string strName = context.getReader().getAttributeValue("name");
    ControlZoneAdder czAdder = svcAdder.newControlZone()
                                        .withName(strName);

    context.getReader().readUntilEndElement("controlZone", [this, &context, &czAdder](){
        if(context.getReader().getLocalName() == "pilotPoint") {
            readPilotPoint(context, czAdder);
        } else if(context.getReader().getLocalName() == "controlUnit") {
            readControlUnit(context, czAdder);
        }
        else {
            throw AssertionError(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });

    czAdder.add();

}
void SecondaryVoltageControlXmlSerializer::readControlUnit(converter::xml::NetworkXmlReaderContext& context, ControlZoneAdder& czAdder) const {
    bool participate = context.getReader().getAttributeValue<bool>("participate");
    std::string strId = context.getReader().readCharacters();

    czAdder.newControlUnit().withId(strId).withParticipate(participate).add();
}
void SecondaryVoltageControlXmlSerializer::readPilotPoint(converter::xml::NetworkXmlReaderContext& context, ControlZoneAdder& czAdder) const {
    double targetV = context.getReader().getAttributeValue<double>("targetV");

    std::vector<std::string> ids;
    context.getReader().readUntilEndElement("pilotPoint", [this, &context, &ids](){
        if(context.getReader().getLocalName() == "busbarSectionOrBusId") {
            ids.emplace_back(context.getReader().readCharacters());
        }
        else {
            throw AssertionError(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });

    czAdder.newPilotPoint().withTargetV(targetV).withBusbarSectionsOrBusesIds(ids).add();
}

Extension& SecondaryVoltageControlXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Network>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Network>()));
    }

    SecondaryVoltageControlAdder svcAdder = extendable.newExtension<SecondaryVoltageControlAdder>();

    context.getReader().readUntilEndElement(getExtensionName(), [this, &context, &svcAdder](){
        if(context.getReader().getLocalName() == "controlZone") {
            readControlZone(context, svcAdder);
        }
        else {
            throw AssertionError(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });

    svcAdder.add();

    return extendable.getExtension<SecondaryVoltageControl>();
}

void SecondaryVoltageControlXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& svcExtension = safeCast<SecondaryVoltageControl>(extension);

    for(const auto& controlZone : svcExtension.getControlZones()) {
        context.getWriter().writeStartElement(getNamespacePrefix(), "controlZone");
        context.getWriter().writeAttribute("name", controlZone.getName());
        writePilotPoint(controlZone.getPilotPoint(), context);
        writeControlUnits(controlZone.getControlUnits(), context);
        context.getWriter().writeEndElement();
    }

}

void SecondaryVoltageControlXmlSerializer::writePilotPoint(const PilotPoint& pilotPoint, converter::xml::NetworkXmlWriterContext& context) const {
    context.getWriter().writeStartElement(getNamespacePrefix(),"pilotPoint");
    context.getWriter().writeAttribute("targetV", pilotPoint.getTargetV());
    for(const auto& id : pilotPoint.getBusbarSectionsOrBusesIds()) {
        context.getWriter().writeStartElement(getNamespacePrefix(),"busbarSectionOrBusId");
        context.getWriter().writeCharacters(id);
        context.getWriter().writeEndElement();
    }
    context.getWriter().writeEndElement();
}
void SecondaryVoltageControlXmlSerializer::writeControlUnits(const std::vector<ControlUnit>& controlUnits, converter::xml::NetworkXmlWriterContext& context) const {
    for(const auto& controlUnit : controlUnits) {
        context.getWriter().writeStartElement(getNamespacePrefix(),"controlUnit");
        context.getWriter().writeAttribute("participate", controlUnit.isParticipate());
        context.getWriter().writeCharacters(controlUnit.getId());
        context.getWriter().writeEndElement();
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
