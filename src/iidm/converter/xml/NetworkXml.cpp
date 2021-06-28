/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NetworkXml.hpp"

#include <chrono>

#include <boost/filesystem/fstream.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/SimpleAnonymizer.hpp>
#include <powsybl/iidm/converter/xml/AbstractVersionableExtensionXmlSerializer.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/xml/XmlStreamException.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include "xml/XmlEncoding.hpp"

#include "AliasesXml.hpp"
#include "HvdcLineXml.hpp"
#include "LineXml.hpp"
#include "PropertiesXml.hpp"
#include "SubstationXml.hpp"
#include "TieLineXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

void checkExtensionsNotFound(const NetworkXmlReaderContext& context, const std::set<std::string>& extensionsNotFound) {
    if (!extensionsNotFound.empty()) {
        const std::string& message = stdcxx::format("Extensions %1% not found!", stdcxx::toString(extensionsNotFound));

        if (context.getOptions().isThrowExceptionIfExtensionNotFound()) {
            throw PowsyblException(message);
        }

        logging::Logger& logger = logging::LoggerFactory::getLogger<NetworkXml>();
        logger.warn(message);
    }
}

std::set<std::string> getExtensionNames(const Network& network) {
    std::set<std::string> names;
    for (const auto& identifiable : network.getIdentifiables()) {
        for (const auto& extension : identifiable.getExtensions()) {
            names.insert(extension.getName());
        }
    }
    return names;
}

stdcxx::CReference<ExtensionXmlSerializer> getExtensionSerializer(const ExportOptions& options, const std::string& extensionName) {
    powsybl::iidm::ExtensionProviders<ExtensionXmlSerializer>& extensionProviders = powsybl::iidm::ExtensionProviders<ExtensionXmlSerializer>::getInstance();

    stdcxx::CReference<ExtensionXmlSerializer> serializer;
    if (options.isThrowExceptionIfExtensionNotFound()) {
        serializer = stdcxx::cref(extensionProviders.findProviderOrThrowException(extensionName));
    } else {
        serializer = extensionProviders.findProvider(extensionName);
        if (!serializer) {
            logging::Logger& logger = logging::LoggerFactory::getLogger<NetworkXml>();
            logger.warn("No extension XML serializer for %1%", extensionName);
        }
    }

    return serializer;
}

const std::string& getNamespaceUri(const ExtensionXmlSerializer& extensionXmlSerializer, const ExportOptions& options, const IidmXmlVersion& networkVersion) {
    const auto& extensionVersion = options.getExtensionVersion(extensionXmlSerializer.getExtensionName());
    if (stdcxx::isInstanceOf<AbstractVersionableExtensionXmlSerializer>(extensionXmlSerializer)) {
        const auto& serializer = dynamic_cast<const AbstractVersionableExtensionXmlSerializer&>(extensionXmlSerializer);

        if (!extensionVersion.empty()) {
            serializer.checkWritingCompatibility(extensionVersion, networkVersion);
            return serializer.getNamespaceUri(extensionVersion);
        }

        return serializer.getNamespaceUri(serializer.getVersion(networkVersion));
    }

    if (!extensionVersion.empty()) {
        return extensionXmlSerializer.getNamespaceUri(extensionVersion);
    }

    return extensionXmlSerializer.getNamespaceUri();
}

const std::string& getNamespaceUri(const ExtensionXmlSerializer& extensionXmlSerializer, const ExportOptions& options) {
    const IidmXmlVersion& networkVersion = options.getVersion().empty() ? IidmXmlVersion::CURRENT_IIDM_XML_VERSION() : IidmXmlVersion::of(options.getVersion(), ".");
    return getNamespaceUri(extensionXmlSerializer, options, networkVersion);
}

void readExtensions(Identifiable& identifiable, NetworkXmlReaderContext& context, std::set<std::string>& extensionsNotFound) {
    powsybl::iidm::ExtensionProviders<ExtensionXmlSerializer>& extensionProviders = powsybl::iidm::ExtensionProviders<ExtensionXmlSerializer>::getInstance();

    context.getReader().readUntilEndElement(EXTENSION, [&identifiable, &context, &extensionsNotFound, &extensionProviders]() {
        const std::string& extensionName = context.getReader().getLocalName();
        if (!context.getOptions().withExtension(extensionName)) {
            context.getReader().readUntilEndElement(extensionName, []() {});
            return;
        }

        stdcxx::CReference<ExtensionXmlSerializer> serializer = extensionProviders.findProvider(extensionName);
        if (serializer) {
            serializer.get().read(identifiable, context);
        } else {
            extensionsNotFound.insert(extensionName);
            context.getReader().readUntilEndElement(extensionName, []() {});
        }
    });
}

void writeExtensionNamespaces(const Network& network, NetworkXmlWriterContext& context) {
    powsybl::iidm::ExtensionProviders<ExtensionXmlSerializer>& extensionProviders = powsybl::iidm::ExtensionProviders<ExtensionXmlSerializer>::getInstance();

    std::set<std::string> extensionUris;
    std::set<std::string> extensionPrefixes;

    const auto& extensions = getExtensionNames(network);
    for (const auto& extension : extensions) {
        if (context.getOptions().withExtension(extension)) {
            stdcxx::CReference<ExtensionXmlSerializer> serializer = extensionProviders.findProvider(extension);
            if (!serializer) {
                continue;
            }

            const std::string& uri = getNamespaceUri(serializer.get(), context.getOptions());
            const std::string& prefix = serializer.get().getNamespacePrefix();

            if (extensionUris.find(uri) != extensionUris.end()) {
                throw PowsyblException(stdcxx::format("Extension namespace URI collision"));
            }

            if (extensionPrefixes.find(prefix) != extensionPrefixes.end()) {
                throw PowsyblException(stdcxx::format("Extension namespace prefix collision"));
            }

            extensionUris.insert(uri);
            extensionPrefixes.insert(prefix);
            context.getWriter().setPrefix(prefix, uri);
        }
    }
}

void writeExtension(const Extension& extension, NetworkXmlWriterContext& context) {
    powsybl::xml::XmlStreamWriter& writer = context.getWriter();
    stdcxx::CReference<ExtensionXmlSerializer> serializer = getExtensionSerializer(context.getOptions(), extension.getName());
    if (!serializer) {
        throw AssertionError(stdcxx::format("Extension XML Serializer of %1% should not be null", extension.getName()));
    }

    const std::string& version = context.getExtensionVersion(extension.getName());
    if (!version.empty()) {
        serializer.get().checkExtensionVersionSupported(version);
    }

    writer.writeStartElement(serializer.get().getNamespacePrefix(), extension.getName());
    serializer.get().write(extension, context);
    writer.writeEndElement();
}

void writeExtensions(const Network& network, NetworkXmlWriterContext& context) {
    for (const auto& identifiable : network.getIdentifiables()) {
        if (!context.isExportedEquipment(identifiable.getId()) || boost::empty(identifiable.getExtensions()) || !context.getOptions().hasAtLeastOneExtension(identifiable.getExtensions())) {
            continue;
        }
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), EXTENSION);
        context.getWriter().writeAttribute(ID, context.getAnonymizer().anonymizeString(identifiable.getId()));
        for (const auto& extension : identifiable.getExtensions()) {
            if (context.getOptions().withExtension(extension.getName())) {
                writeExtension(extension, context);
            }
        }
        context.getWriter().writeEndElement();
    }
}

Network NetworkXml::read(const std::string& filename, std::istream& is, const ImportOptions& options) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<NetworkXml>();

    auto startTime = std::chrono::high_resolution_clock::now();

    powsybl::xml::XmlStreamReader reader(is);

    reader.skipComments();

    const IidmXmlVersion& version = IidmXmlVersion::fromNamespaceURI(reader.getNamespaceOrDefault(IidmXmlVersion::getDefaultPrefix()));

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

    boost::filesystem::path csvPath = boost::filesystem::path(filename).replace_extension("csv");
    std::unique_ptr<Anonymizer> anonymizer;

    if (boost::filesystem::exists(csvPath)) {
        anonymizer = stdcxx::make_unique<SimpleAnonymizer>();
        boost::filesystem::ifstream csvStream(csvPath);
        if (!csvStream.is_open()) {
            throw PowsyblException(stdcxx::format("Unable to open file '%1%' for reading", csvPath.filename().string()));
        }
        anonymizer->read(csvStream);
    } else {
        anonymizer = stdcxx::make_unique<FakeAnonymizer>();
    }

    NetworkXmlReaderContext context(std::move(anonymizer), reader, options, version);

    const auto& extensionProviders = ExtensionProviders<ExtensionXmlSerializer>::getInstance();
    context.buildExtensionNamespaceUriList(extensionProviders.getProviders());

    std::set<std::string> extensionsNotFound;

    context.getReader().readUntilEndElement(NETWORK, [&network, &context, &extensionsNotFound]() {
        if (context.getReader().getLocalName() == ALIAS) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, ALIAS, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
            AliasesXml::read(network, context);
        } else if (context.getReader().getLocalName() == PROPERTY) {
            PropertiesXml::read(network, context);
        } else if (context.getReader().getLocalName() == SUBSTATION) {
            SubstationXml::getInstance().read(network, context);
        } else if (context.getReader().getLocalName() == LINE) {
            LineXml::getInstance().read(network, context);
        } else if (context.getReader().getLocalName() == TIE_LINE) {
            TieLineXml::getInstance().read(network, context);
        } else if (context.getReader().getLocalName() == HVDC_LINE) {
            HvdcLineXml::getInstance().read(network, context);
        } else if (context.getReader().getLocalName() == EXTENSION) {
            const std::string& id2 = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));
            Identifiable& identifiable = network.get(id2);
            readExtensions(identifiable, context, extensionsNotFound);
        } else {
            throw powsybl::xml::XmlStreamException(stdcxx::format("Unexpected element: %1%", context.getReader().getLocalName()));
        }
    });

    checkExtensionsNotFound(context, extensionsNotFound);

    for (const auto& task : context.getEndTasks()) {
        task();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = endTime - startTime;
    logger.debug("XIIDM import done in %1% ms", diff.count() * 1000.0);

    return network;
}

void NetworkXml::write(const std::string& filename, std::ostream& os, const Network& network, const ExportOptions& options) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<NetworkXml>();

    auto startTime = std::chrono::high_resolution_clock::now();

    powsybl::xml::XmlStreamWriter writer(os, options.isIndent());

    std::unique_ptr<Anonymizer> anonymizer = options.isAnonymized() ? stdcxx::make_unique<Anonymizer, SimpleAnonymizer>() : stdcxx::make_unique<Anonymizer, FakeAnonymizer>();

    const BusFilter& filter = BusFilter::create(network, options);

    const IidmXmlVersion& version = options.getVersion().empty() ? IidmXmlVersion::CURRENT_IIDM_XML_VERSION() : IidmXmlVersion::of(options.getVersion(), ".");
    NetworkXmlWriterContext context(std::move(anonymizer), writer, options, filter, version);

    writer.writeStartDocument(powsybl::xml::DEFAULT_ENCODING, "1.0");

    writer.writeStartElement(context.getVersion().getPrefix(), NETWORK);
    writer.setPrefix(context.getVersion().getPrefix(), version.getNamespaceUri());
    writeExtensionNamespaces(network, context);

    writer.writeAttribute(ID, network.getId());
    writer.writeAttribute(CASE_DATE, network.getCaseDate().toString());
    writer.writeAttribute(FORECAST_DISTANCE, network.getForecastDistance());
    writer.writeAttribute(SOURCE_FORMAT, network.getSourceFormat());

    AliasesXml::write(network, NETWORK, context);
    PropertiesXml::write(network, context);

    for (const auto& substation : network.getSubstations()) {
        SubstationXml::getInstance().write(substation, network, context);
    }

    for (const auto& line : network.getLines()) {
        if (!filter.test(line)) {
            continue;
        }
        if (line.isTieLine()) {
            TieLineXml::getInstance().write(dynamic_cast<const TieLine&>(line), network, context);
        } else {
            LineXml::getInstance().write(line, network, context);
        }
    }

    for (const auto& line : network.getHvdcLines()) {
        if (!filter.test(line.getConverterStation1()) || !filter.test(line.getConverterStation2())) {
            continue;
        }
        HvdcLineXml::getInstance().write(line, network, context);
    }

    writeExtensions(network, context);

    writer.writeEndElement();
    writer.writeEndDocument();

    if (options.isAnonymized()) {
        boost::filesystem::path csvPath = boost::filesystem::path(filename).replace_extension("csv");
        boost::filesystem::ofstream csvStream(csvPath);
        if (!csvStream.is_open()) {
            throw PowsyblException(stdcxx::format("Unable to open file '%1%' for writing", csvPath.string()));
        }
        context.getAnonymizer().write(csvStream);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = endTime - startTime;
    logger.debug("XIIDM export done in %1% ms", diff.count() * 1000.0);
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
