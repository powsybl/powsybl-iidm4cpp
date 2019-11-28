/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "iidm/extensions/ExtensionRoundTrip.hpp"

#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include "iidm/converter/RoundTrip.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

std::unique_ptr<Extension> ExtensionRoundTrip::runXml(Extendable& extendable, const Extension& extension, const converter::xml::ExtensionXmlSerializer& serializer, const std::string& ref) {
    const auto& writer = [&serializer](const Extension& e, std::ostream& stream) {
        converter::ExportOptions exportOptions;
        xml::XmlStreamWriter writer(stream, exportOptions.isIndent());
        converter::FakeAnonymizer anonymizer;
        converter::xml::NetworkXmlWriterContext context(anonymizer, writer, exportOptions);
        context.setExtensionsWriter(writer);

        writer.writeStartDocument("UTF-8", "1.0");
        writer.writeStartElement(serializer.getNamespacePrefix(), e.getName());
        writer.setPrefix(serializer.getNamespacePrefix(), serializer.getNamespaceUri());
        serializer.write(e, context);
        writer.writeEndElement();
        writer.writeEndDocument();
    };

    const auto& reader = [&extendable, &serializer](const std::string& xml) {
        converter::ImportOptions importOptions;
        std::istringstream stream(xml);
        xml::XmlStreamReader reader(stream);
        converter::FakeAnonymizer anonymizer;
        converter::xml::NetworkXmlReaderContext context(anonymizer, reader, importOptions);

        return serializer.read(extendable, context);
    };

    const auto& compare = converter::RoundTrip::compareXml;

    const std::string& output1 = write(extension, writer, compare, ref);
    std::unique_ptr<Extension> extension2 = reader(output1);
    write(*extension2, writer, compare, ref);
    return extension2;
}

std::string ExtensionRoundTrip::write(const Extension& extension, const Writer& out, const Comparator& compare, const std::string& ref) {
    std::stringstream buffer;
    out(extension, buffer);

    compare(ref, buffer.str());

    return buffer.str();
}

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
