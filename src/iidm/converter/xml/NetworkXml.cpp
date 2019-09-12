/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NetworkXml.hpp"

#include <chrono>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/xml/XmlStreamException.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>

#include "iidm/converter/Constants.hpp"
#include "xml/XmlEncoding.hpp"

#include "HvdcLineXml.hpp"
#include "LineXml.hpp"
#include "SubstationXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

Network NetworkXml::read(std::istream& is, const ImportOptions& options, const Anonymizer& anonymizer) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<NetworkXml>();

    auto startTime = std::chrono::high_resolution_clock::now();

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

    NetworkXmlReaderContext context(anonymizer, reader, options);

    context.getReader().readUntilEndElement(NETWORK, [&network, &context]() {
        if (context.getReader().getLocalName() == SUBSTATION) {
            SubstationXml::getInstance().read(network, context);
        } else if (context.getReader().getLocalName() == LINE) {
            LineXml::getInstance().read(network, context);
        } else if (context.getReader().getLocalName() == HVDC_LINE) {
            HvdcLineXml::getInstance().read(network, context);
        } else {
            throw powsybl::xml::XmlStreamException(logging::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });

    for (const auto& task : context.getEndTasks()) {
        task();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = endTime - startTime;
    logger.debug("XIIDM import done in %1% ms", diff.count() * 1000.0);

    return network;
}

std::unique_ptr<Anonymizer> NetworkXml::write(std::ostream& ostream, const Network& network, const ExportOptions& options) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<NetworkXml>();

    auto startTime = std::chrono::high_resolution_clock::now();

    powsybl::xml::XmlStreamWriter writer(ostream, options.isIndent());

    // FIXME(sebalaig): for now on, only one kind of anonymizer = FakeAnonymizer
    // later, a real anonymizer will be instantiated depending on options.isAnonymized()
    std::unique_ptr<Anonymizer> anonymizer(stdcxx::make_unique<FakeAnonymizer>());

    NetworkXmlWriterContext context(*anonymizer, writer, options);

    writer.writeStartDocument(powsybl::xml::DEFAULT_ENCODING, "1.0");

    writer.writeStartElement(IIDM_PREFIX, NETWORK);
    writer.setPrefix(IIDM_PREFIX, IIDM_URI);

    writer.writeAttribute(ID, network.getId());
    writer.writeAttribute(CASE_DATE, network.getCaseDate().toString());
    writer.writeAttribute(FORECAST_DISTANCE, network.getForecastDistance());
    writer.writeAttribute(SOURCE_FORMAT, network.getSourceFormat());

    for (const auto& substation : network.getSubstations()) {
        SubstationXml::getInstance().write(substation, network, context);
    }

    for (const auto& line : network.getLines()) {
        // TODO(sebalaig) consider filtering
        if (line.isTieLine()) {
            // TODO(sebalaig) implement TieLineXml
        } else {
            LineXml::getInstance().write(line, network, context);
        }
    }

    for (const auto& line : network.getHvdcLines()) {
        // TODO(sebalaig) consider filtering
//        if (!filter.test(line.getConverterStation1()) || !filter.test(line.getConverterStation2())) {
//            continue;
//        }
        HvdcLineXml::getInstance().write(line, network, context);
    }

    writer.writeEndElement();
    writer.writeEndDocument();

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = endTime - startTime;
    logger.debug("XIIDM export done in %1% ms", diff.count() * 1000.0);

    return anonymizer;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
