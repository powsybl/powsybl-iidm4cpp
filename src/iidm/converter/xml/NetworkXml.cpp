/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NetworkXml.hpp"

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/stdcxx/DateTime.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/xml/XmlStreamException.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include "xml/XmlEncoding.hpp"

#include "iidm/converter/Constants.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

Network NetworkXml::read(std::istream& is, const ImportOptions& /*options*/, const stdcxx::CReference<converter::Anonymizer>& /*anonymizer*/) {

    powsybl::xml::XmlStreamReader reader(is);

    reader.skipComments();

    const std::string& id = reader.getAttributeValue(ID);
    const std::string& sourceFormat = reader.getAttributeValue(SOURCE_FORMAT);
    int forecastDistance = reader.getOptionalAttributeValue(FORECAST_DISTANCE, 0);
    const std::string& caseDateStr = reader.getAttributeValue(CASE_DATE);

    Network network(id, sourceFormat);
    network.setForecastDistance(forecastDistance);

    try {
        network.setCaseDate(stdcxx::DateTime::parse(caseDateStr));
    } catch (const PowsyblException& err) {
        throw powsybl::xml::XmlStreamException(err.what());
    }

    return network;
}

std::unique_ptr<Anonymizer> NetworkXml::write(std::ostream& ostream, const Network& network, const ExportOptions& options) {
    powsybl::xml::XmlStreamWriter writer(ostream, options.isIndent());

    writer.writeStartDocument(powsybl::xml::DEFAULT_ENCODING, "1.0");

    writer.writeStartElement(IIDM_PREFIX, NETWORK);
    writer.setPrefix(IIDM_PREFIX, IIDM_URI);

    writer.writeAttribute(ID, network.getId());
    writer.writeAttribute(CASE_DATE, network.getCaseDate().toString());
    writer.writeAttribute(FORECAST_DISTANCE, network.getForecastDistance());
    writer.writeAttribute(SOURCE_FORMAT, network.getSourceFormat());

    writer.writeEndElement();
    writer.writeEndDocument();

    std::unique_ptr<Anonymizer> anonymizer(options.isAnonymized() ? stdcxx::make_unique<FakeAnonymizer>() : nullptr);
    return anonymizer;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
