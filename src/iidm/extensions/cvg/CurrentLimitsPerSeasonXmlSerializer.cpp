/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/CurrentLimitsPerSeasonXmlSerializer.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/cvg/CurrentLimitsPerSeason.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include "iidm/converter/xml/AbstractConnectableXml.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

CurrentLimitsPerSeasonXmlSerializer::CurrentLimitsPerSeasonXmlSerializer() :
    converter::xml::ExtensionXmlSerializer("currentLimitsPerSeason", "network", "http://www.itesla_project.eu/schema/iidm/ext/current-limits-per-season/1_0", "clps") {
}

std::unique_ptr<Extension> CurrentLimitsPerSeasonXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Connectable>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Connectable>()));
    }
    auto& connectable = dynamic_cast<Connectable&>(extendable);

    std::unique_ptr<Extension> currentLimitsPerSeason = stdcxx::make_unique<CurrentLimitsPerSeason>(connectable);
    CurrentLimitsPerSeason::CurrentLimitsTriple triple;
    context.getReader().readUntilEndElement(getExtensionName(), [context, &currentLimitsPerSeason, &triple]() {
        if (context.getReader().getLocalName() == "season") {
            const std::string& name = context.getReader().getAttributeValue("name");
            dynamic_cast<CurrentLimitsPerSeason&>(*currentLimitsPerSeason).getCurrentLimits()[name] = triple;
        } else if (context.getReader().getLocalName() == "currentLimits") {
            CurrentLimitsAdder<std::nullptr_t, CurrentLimitsPerSeason::CurrentLimitsTriple> adder = triple.newCurrentLimits();
            converter::xml::AbstractConnectableXml<Connectable, CurrentLimitsAdder<std::nullptr_t, CurrentLimitsPerSeason::CurrentLimitsTriple>, std::nullptr_t>::readCurrentLimits(adder, context.getReader());
        } else if (context.getReader().getLocalName() == "currentLimits1") {
            CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsPerSeason::CurrentLimitsTriple> adder = triple.newCurrentLimits1();
            converter::xml::AbstractConnectableXml<Connectable, CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsPerSeason::CurrentLimitsTriple>, std::nullptr_t>::readCurrentLimits(adder, context.getReader(), 1);
        } else if (context.getReader().getLocalName() == "currentLimits2") {
            CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsPerSeason::CurrentLimitsTriple> adder = triple.newCurrentLimits2();
            converter::xml::AbstractConnectableXml<Connectable, CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsPerSeason::CurrentLimitsTriple>, std::nullptr_t>::readCurrentLimits(adder, context.getReader(), 2);
        } else if (context.getReader().getLocalName() == "currentLimits3") {
            CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsPerSeason::CurrentLimitsTriple> adder = triple.newCurrentLimits3();
            converter::xml::AbstractConnectableXml<Connectable, CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsPerSeason::CurrentLimitsTriple>, std::nullptr_t>::readCurrentLimits(adder, context.getReader(), 3);
        } else {
            throw AssertionError(logging::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });
    return currentLimitsPerSeason;
}

void CurrentLimitsPerSeasonXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& currentLimitsPerSeason = safeCast<CurrentLimitsPerSeason>(extension);
    for (const auto& entry : currentLimitsPerSeason.getCurrentLimits()) {
        const std::string& name = entry.first;
        const CurrentLimitsPerSeason::CurrentLimitsTriple& triple = entry.second;
        context.getExtensionsWriter().writeStartElement(getNamespacePrefix(), "season");
        context.getExtensionsWriter().writeAttribute("name", name);
        if (triple.getCurrentLimits()) {
            converter::xml::AbstractConnectableXml<Connectable, CurrentLimitsAdder<std::nullptr_t, CurrentLimitsPerSeason::CurrentLimitsTriple>, std::nullptr_t>::writeCurrentLimits(triple.getCurrentLimits(), context.getExtensionsWriter(), boost::optional<int>(), getNamespacePrefix());
        }
        if (triple.getCurrentLimits1()) {
            converter::xml::AbstractConnectableXml<Connectable, CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsPerSeason::CurrentLimitsTriple>, std::nullptr_t>::writeCurrentLimits(triple.getCurrentLimits1(), context.getExtensionsWriter(), 1, getNamespacePrefix());
        }
        if (triple.getCurrentLimits2()) {
            converter::xml::AbstractConnectableXml<Connectable, CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsPerSeason::CurrentLimitsTriple>, std::nullptr_t>::writeCurrentLimits(triple.getCurrentLimits2(), context.getExtensionsWriter(), 2, getNamespacePrefix());
        }
        if (triple.getCurrentLimits3()) {
            converter::xml::AbstractConnectableXml<Connectable, CurrentLimitsAdder<ThreeWindingsTransformer::Side, CurrentLimitsPerSeason::CurrentLimitsTriple>, std::nullptr_t>::writeCurrentLimits(triple.getCurrentLimits3(), context.getExtensionsWriter(), 3, getNamespacePrefix());
        }
        context.getExtensionsWriter().writeEndElement();
    }
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

