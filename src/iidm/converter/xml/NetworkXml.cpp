/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/NetworkXml.hpp>

#include <chrono>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>
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

#include "HvdcLineXml.hpp"
#include "LineXml.hpp"
#include "SubstationXml.hpp"
#include "TieLineXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

void checkExtensionsNotFound(const NetworkXmlReaderContext& context, const std::set<std::string>& extensionsNotFound) {
    if (!extensionsNotFound.empty()) {
        const std::string& message = logging::format("Extensions %1% not found!", logging::toString(extensionsNotFound));

        if (context.getOptions().isThrowExceptionIfExtensionNotFound()) {
            throw PowsyblException(message);
        }

        logging::Logger& logger = logging::LoggerFactory::getLogger<NetworkXml>();
        logger.error(message);
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
            std::unique_ptr<Extension> extension = serializer.get().read(identifiable, context);
            identifiable.addExtension(std::move(extension));
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
                throw PowsyblException(logging::format("Extension namespace URI collision"));
            }

            if (extensionPrefixes.find(prefix) != extensionPrefixes.end()) {
                throw PowsyblException(logging::format("Extension namespace prefix collision"));
            }

            extensionUris.insert(uri);
            extensionPrefixes.insert(prefix);
            context.getWriter().setPrefix(prefix, uri);
        }
    }
}

void writeExtension(const Extension& extension, NetworkXmlWriterContext& context) {
    powsybl::xml::XmlStreamWriter& writer = context.getExtensionsWriter();
    stdcxx::CReference<ExtensionXmlSerializer> serializer = getExtensionSerializer(context.getOptions(), extension.getName());

    if (serializer) {
        const std::string& version = context.getExtensionVersion(extension.getName());
        if (!version.empty()) {
            serializer.get().checkExtensionVersionSupported(version);
        }

        writer.writeStartElement(serializer.get().getNamespacePrefix(), extension.getName());
        serializer.get().write(extension, context);
        writer.writeEndElement();
    }
}

void writeExtensions(const Network& network, NetworkXmlWriterContext& context) {
    for (const auto& identifiable : network.getIdentifiables()) {
        const auto& extensions = identifiable.getExtensions();
        if (context.isExportedEquipment(identifiable.getId()) && boost::size(extensions) > 0) {

            context.getExtensionsWriter().writeStartElement(context.getVersion().getPrefix(), EXTENSION);
            context.getExtensionsWriter().writeAttribute(ID, context.getAnonymizer().anonymizeString(identifiable.getId()));
            for (const auto& extension : extensions) {
                if (context.getOptions().withExtension(extension.getName())) {
                    writeExtension(extension, context);
                }
            }
            context.getExtensionsWriter().writeEndElement();
        }
    }
}

Network NetworkXml::read(const std::string& data) {
    std::stringstream stream(data);
    return read(stream);
}

Network NetworkXml::read(std::istream& istream) {
    return read(istream, converter::ImportOptions(), converter::FakeAnonymizer());
}

Network NetworkXml::read(std::istream& is, const ImportOptions& options, const Anonymizer& anonymizer) {
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

    NetworkXmlReaderContext context(anonymizer, reader, options, version);

    const auto& extensionProviders = ExtensionProviders<ExtensionXmlSerializer>::getInstance();
    context.buildExtensionNamespaceUriList(extensionProviders.getProviders());

    std::set<std::string> extensionsNotFound;

    context.getReader().readUntilEndElement(NETWORK, [&network, &context, &extensionsNotFound]() {
        if (context.getReader().getLocalName() == PROPERTY) {
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
            throw powsybl::xml::XmlStreamException(logging::format("Unexpected element: %1%", context.getReader().getLocalName()));
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

std::unique_ptr<converter::Anonymizer> NetworkXml::write(std::ostream& ostream, const Network& network) {
    converter::ExportOptions options;
    return write(ostream, network, options);
}

std::unique_ptr<Anonymizer> NetworkXml::write(std::ostream& ostream, const Network& network, const ExportOptions& options) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<NetworkXml>();

    auto startTime = std::chrono::high_resolution_clock::now();

    powsybl::xml::XmlStreamWriter writer(ostream, options.isIndent());

    // FIXME(sebalaig): for now on, only one kind of anonymizer = FakeAnonymizer
    // later, a real anonymizer will be instantiated depending on options.isAnonymized()
    std::unique_ptr<Anonymizer> anonymizer(stdcxx::make_unique<FakeAnonymizer>());

    const BusFilter& filter = BusFilter::create(network, options);

    const IidmXmlVersion& version = options.getVersion().empty() ? IidmXmlVersion::CURRENT_IIDM_XML_VERSION() : IidmXmlVersion::of(options.getVersion(), ".");
    NetworkXmlWriterContext context(*anonymizer, writer, options, filter, version);

    writer.writeStartDocument(powsybl::xml::DEFAULT_ENCODING, "1.0");

    writer.writeStartElement(context.getVersion().getPrefix(), NETWORK);
    writer.setPrefix(context.getVersion().getPrefix(), version.getNamespaceUri());
    writeExtensionNamespaces(network, context);

    writer.writeAttribute(ID, network.getId());
    writer.writeAttribute(CASE_DATE, network.getCaseDate().toString());
    writer.writeAttribute(FORECAST_DISTANCE, network.getForecastDistance());
    writer.writeAttribute(SOURCE_FORMAT, network.getSourceFormat());

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

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = endTime - startTime;
    logger.debug("XIIDM export done in %1% ms", diff.count() * 1000.0);

    return anonymizer;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
