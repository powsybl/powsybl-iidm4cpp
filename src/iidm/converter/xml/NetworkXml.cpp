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
#include <powsybl/iidm/OverloadManagementSystem.hpp>
#include <powsybl/iidm/Subnetwork.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/SimpleAnonymizer.hpp>
#include <powsybl/iidm/converter/xml/AbstractVersionableExtensionXmlSerializer.hpp>
#include <powsybl/iidm/converter/xml/AliasesXml.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/PropertiesXml.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/xml/XmlEncoding.hpp>
#include <powsybl/xml/XmlStreamException.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

#include "AreaXml.hpp"
#include "DcGroundXml.hpp"
#include "DcLineXml.hpp"
#include "DcNodeXml.hpp"
#include "DcSwitchXml.hpp"
#include "HvdcLineXml.hpp"
#include "LineXml.hpp"
#include "SubstationXml.hpp"
#include "ThreeWindingsTransformerXml.hpp"
#include "TieLineXml.hpp"
#include "TwoWindingsTransformerXml.hpp"
#include "VoltageLevelXml.hpp"
#include "VoltageAngleLimitXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

void throwExceptionIfOption(bool throwExceptionOption, const std::string& message) {
    if(throwExceptionOption) {
        throw PowsyblException(message);
    } 
    logging::Logger& logger = logging::LoggerFactory::getLogger<NetworkXml>();
    logger.warn(message);
}

void checkExtensionsNotFound(const NetworkXmlReaderContext& context, const std::set<std::string>& extensionsNotFound) {
    if (!extensionsNotFound.empty()) {
        const std::string& message = stdcxx::format("Extensions %1% not found!", stdcxx::toString(extensionsNotFound));

        throwExceptionIfOption(context.getOptions().isThrowExceptionIfExtensionNotFound(), message);
    }
}

bool isExtensionIncluded(const stdcxx::CReference<ExtensionXmlSerializer>& extensionXmlSerializer, const ExportOptions& options) {
    if(!extensionXmlSerializer) {
        return false;
    }
    return options.withExtension(extensionXmlSerializer.get().getExtensionName());
}

bool canExtensionBeWritten(const Extension& extension, const stdcxx::CReference<ExtensionXmlSerializer>& extensionXmlSerializer, NetworkXmlWriterContext& context) {
    bool canBeWritten = false;
    
    if(!extensionXmlSerializer) {
        return canBeWritten;
    }

    bool versionSupported = true;
    if (stdcxx::isInstanceOf<AbstractVersionableExtensionXmlSerializer>(extensionXmlSerializer)) {
        const auto& serializer = dynamic_cast<const AbstractVersionableExtensionXmlSerializer&>(extensionXmlSerializer.get());
        versionSupported = serializer.isIIDMVersionSupported(context.getVersion());
    }
    if(!versionSupported) {
        const std::string& message = stdcxx::format("Version %1% does not support %2% extension", context.getVersion().toString("."), extensionXmlSerializer.get().getExtensionName());
        throwExceptionIfOption(context.getOptions().isThrowExceptionIfExtensionNotFound(),message);
    }

    canBeWritten = versionSupported && extensionXmlSerializer.get().isSerializable(extension, context);

    return canBeWritten;
}

stdcxx::CReference<ExtensionXmlSerializer> getExtensionSerializer(const Extension& extension, const ExportOptions& options) {
    powsybl::iidm::ExtensionProviders<ExtensionXmlSerializer>& extensionProviders = powsybl::iidm::ExtensionProviders<ExtensionXmlSerializer>::getInstance();

    stdcxx::CReference<ExtensionXmlSerializer> serializer;
    if (options.isThrowExceptionIfExtensionNotFound()) {
        serializer = stdcxx::cref(extensionProviders.findProviderOrThrowException(extension.getName()));
    } else {
        serializer = extensionProviders.findProvider(extension.getName());
        if (!serializer) {
            logging::Logger& logger = logging::LoggerFactory::getLogger<NetworkXml>();
            logger.warn("No extension XML serializer for %1%", extension.getName());
        }
    }

    return serializer;
}

std::set<std::string> getExtensionNames(const Network& network, NetworkXmlWriterContext& context) {
    std::set<std::string> names;

    if(context.getOptions().withNoExtension()) {
        return names;
    }

    for (const auto& identifiable : network.getIdentifiables()) {
        for (const auto& extension : identifiable.getExtensions()) {
            stdcxx::CReference<ExtensionXmlSerializer> serializer = getExtensionSerializer(extension, context.getOptions());
            if(static_cast<bool>(serializer) && canExtensionBeWritten(extension, serializer, context)) {
                names.insert(extension.getName());
            }
        }
    }

    return names;
}

std::string getExtensionVersion(const ExtensionXmlSerializer& extensionXmlSerializer, const ExportOptions& options) {
    const IidmXmlVersion& networkVersion = options.getVersion().empty() ? IidmXmlVersion::CURRENT_IIDM_XML_VERSION() : IidmXmlVersion::of(options.getVersion(), ".");
    const auto& specifiedExtensionVersion = options.getExtensionVersion(extensionXmlSerializer.getExtensionName());

    if (stdcxx::isInstanceOf<AbstractVersionableExtensionXmlSerializer>(extensionXmlSerializer)) {
        const auto& serializer = dynamic_cast<const AbstractVersionableExtensionXmlSerializer&>(extensionXmlSerializer);

        if (!specifiedExtensionVersion.empty()) {
            serializer.checkWritingCompatibility(specifiedExtensionVersion, networkVersion);
            return specifiedExtensionVersion;
        }
        return serializer.getVersion(networkVersion).toString();
    }

    if (!specifiedExtensionVersion.empty()) {
        return specifiedExtensionVersion;
    }
    return extensionXmlSerializer.getVersion();
}

void readExtensions(Network& network, NetworkXmlReaderContext& context, std::set<std::string>& extensionsNotFound) {
    context.executeEndTasks(XmlReaderEndTask::Step::BEFORE_EXTENSIONS);

    const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));

    powsybl::iidm::ExtensionProviders<ExtensionXmlSerializer>& extensionProviders = powsybl::iidm::ExtensionProviders<ExtensionXmlSerializer>::getInstance();

    context.getReader().readUntilEndElement(EXTENSION, [&network, &id, &context, &extensionsNotFound, &extensionProviders]() {
        const std::string& extensionSerializedName = context.getReader().getLocalName();

        if(context.isIgnoredEquipment(id)) {
            context.getReader().readUntilEndElement(extensionSerializedName, []() {});
            return;
        }

        stdcxx::CReference<ExtensionXmlSerializer> serializer = extensionProviders.findProvider(extensionSerializedName);
        std::string extensionName = (static_cast<bool>(serializer)) ? serializer.get().getExtensionName() : extensionSerializedName;

        if(context.getOptions().withExtension(extensionName) || context.getOptions().withExtension(extensionSerializedName)) {
            if(static_cast<bool>(serializer)) {
                Identifiable& identifiable = network.getIdentifiable(id);
                context.checkAndAddExtensionNamespaceUri();
                serializer.get().read(identifiable, context);
            } else {
                extensionsNotFound.insert(extensionName);
                context.getReader().readUntilEndElement(extensionSerializedName, []() {});
            }
        } else {
            context.getReader().readUntilEndElement(extensionSerializedName, []() {});
        }
    });
}

void writeExtensionNamespaces(const Network& network, NetworkXmlWriterContext& context) {
    powsybl::iidm::ExtensionProviders<ExtensionXmlSerializer>& extensionProviders = powsybl::iidm::ExtensionProviders<ExtensionXmlSerializer>::getInstance();

    std::set<std::string> extensionUris;
    std::set<std::string> extensionPrefixes;

    const auto& extensions = getExtensionNames(network, context);
    for (const auto& extension : extensions) {
        if (context.getOptions().withExtension(extension)) {
            stdcxx::CReference<ExtensionXmlSerializer> serializer = extensionProviders.findProvider(extension);
            if (!serializer) {
                continue;
            }

            std::string extensionVersionStr = getExtensionVersion(serializer.get(), context.getOptions());
            std::string uri = serializer.get().getNamespaceUri(extensionVersionStr);
            const std::string& prefix = serializer.get().getNamespacePrefix(extensionVersionStr);
            std::string fixedPrefix = prefix;

            if (extensionUris.find(uri) != extensionUris.end()) {
                throw PowsyblException(stdcxx::format("Extension namespace URI collision"));
            }

            //If prefix collision, compute another prefix
            int i = 1;
            while (i < MAX_NAMESPACE_PREFIX_NUM && extensionPrefixes.count(fixedPrefix)) {
                fixedPrefix = prefix + std::to_string(i++);
            }
            if(i>=MAX_NAMESPACE_PREFIX_NUM) {
                throw PowsyblException(stdcxx::format("Cannot compute a unique extension namespace prefix : %1%", prefix));
            }

            extensionUris.insert(uri);
            extensionPrefixes.insert(fixedPrefix);
            context.addExtensionFixedPrefix(extension, fixedPrefix);
            
            context.getWriter().setPrefix(fixedPrefix, uri);
        }
    }
}

void writeExtension(const Extension& extension, NetworkXmlWriterContext& context) {
    powsybl::xml::XmlStreamWriter& writer = context.getWriter();
    stdcxx::CReference<ExtensionXmlSerializer> serializer = getExtensionSerializer(extension, context.getOptions());
    if (!serializer) {
        throw AssertionError(stdcxx::format("Extension XML Serializer of %1% should not be null", extension.getName()));
    }

    const std::string& version = context.getExtensionVersion(extension.getName());
    if (!version.empty()) {
        serializer.get().checkExtensionVersionSupported(version);
    }

    std::string prefix = context.getExtensionFixedPrefix(extension.getName());
    if(prefix.empty()) { //Retrieve default one from extension provider
        prefix = serializer.get().getNamespacePrefix(version);
    }

    std::string serializationName = serializer.get().getSerializationName(version);

    writer.writeStartElement(prefix, serializationName);
    serializer.get().write(extension, context);
    writer.writeEndElement();
}

void NetworkXml::writeExtensions(const Network& network, NetworkXmlWriterContext& context) {
    for (const auto& identifiable : network.getIdentifiables()) {
        if (ignoreEquipmentAtExport(identifiable, context) || boost::empty(identifiable.getExtensions()) 
        || !isElementWrittenInsideNetwork(identifiable, network, context)
        || !context.getOptions().hasAtLeastOneExtension(identifiable.getExtensions())) {
            continue;
        }

        bool atLeastOneExtensionToWrite = false;
        for (const auto& extension : identifiable.getExtensions()) {
            auto extensionSerializer = getExtensionSerializer(extension, context.getOptions());
            if(static_cast<bool>(extensionSerializer) && 
               isExtensionIncluded(extensionSerializer, context.getOptions()) && 
               canExtensionBeWritten(extension, extensionSerializer, context)) {
                atLeastOneExtensionToWrite = true;
                break;
            }
        }

        if(atLeastOneExtensionToWrite) {
            context.getWriter().writeStartElement(context.getVersion().getPrefix(), EXTENSION);
            context.getWriter().writeAttribute(ID, context.getAnonymizer().anonymizeString(identifiable.getId()));

            for (const auto& extension : identifiable.getExtensions()) {
                bool canBeWritten = canExtensionBeWritten(extension, getExtensionSerializer(extension, context.getOptions()), context);
                if (canBeWritten && context.getOptions().withExtension(extension.getName())) {
                    writeExtension(extension, context);
                }
            }
            context.getWriter().writeEndElement();
        }
        
    }
}

Network NetworkXml::read(const std::string& filename, std::istream& is, const ImportOptions& options) {
    logging::Logger& logger = logging::LoggerFactory::getLogger<NetworkXml>();

    auto startTime = std::chrono::high_resolution_clock::now();

    powsybl::xml::XmlStreamReader reader(is);

    reader.skipComments();

    const IidmXmlVersion& version = IidmXmlVersion::fromNamespaceURI(reader.getNamespaceOrDefault(IidmXmlVersion::getDefaultPrefix()));

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

    const std::string& id = context.getAnonymizer().deanonymizeString(reader.getAttributeValue(ID));
    const std::string& sourceFormat = reader.getAttributeValue(SOURCE_FORMAT);
    Network network(id, sourceFormat);
    initNetwork(network, context);

    std::set<std::string> extensionsNotFound;

    readNetworkElements(network, context, extensionsNotFound);

    context.executeEndTasks(XmlReaderEndTask::Step::AFTER_EXTENSIONS);

    checkExtensionsNotFound(context, extensionsNotFound);

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
    
    ValidationLevel networkValidationLevel = network.getValidationLevel();
    NetworkXmlWriterContext context(std::move(anonymizer), writer, options, filter, version, networkValidationLevel == ValidationLevel::STEADY_STATE_HYPOTHESIS);
    IidmXmlUtil::assertMinimumVersionIfNotDefault(networkValidationLevel != ValidationLevel::STEADY_STATE_HYPOTHESIS, NETWORK, MINIMUM_VALIDATION_LEVEL, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_7(), context);
    
    writer.writeStartDocument(options.getXmlEncoding(), "1.0");

    writeNetwork(network, context);

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

void NetworkXml::writeNetwork(const Network& network, NetworkXmlWriterContext& context) {
    powsybl::xml::XmlStreamWriter& writer = context.getWriter();
    const BusFilter& filter = context.getFilter();
    const IidmXmlVersion& version = context.getVersion();
    ValidationLevel networkValidationLevel = network.getValidationLevel();

    writer.writeStartElement(context.getVersion().getPrefix(), NETWORK);

    if(stdcxx::areSame(network.getParentNetwork(), network)) { //root network
        writer.setPrefix(context.getVersion().getPrefix(), version.getNamespaceUri(networkValidationLevel == ValidationLevel::STEADY_STATE_HYPOTHESIS));

        writeExtensionNamespaces(network, context);
    }

    //write main attributes
    writer.writeAttribute(ID, context.getAnonymizer().anonymizeString(network.getId()));
    writer.writeAttribute(CASE_DATE, network.getCaseDate().toString());
    writer.writeAttribute(FORECAST_DISTANCE, network.getForecastDistance());
    writer.writeAttribute(SOURCE_FORMAT, network.getSourceFormat());
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_7(), version, [&writer, &networkValidationLevel] { writer.writeAttribute(MINIMUM_VALIDATION_LEVEL, Enum::toString(networkValidationLevel)); });

    //consider the network as exported so its extensions will be written
    context.addExportedEquipment(network);

    //Write "base" network
    AliasesXml::write(network, NETWORK, context);
    PropertiesXml::write(network, context);

    if(supportSubnetworksExport(context) && !context.getOptions().isFlatten()) {
        writeSubnetworks(network, context);
    }

    writeDcDetailed(network, context);
    writeVoltageLevels(network, context);
    writeSubstations(network, context);
    writeLines(filter, network, context);
    writeTieLines(filter, network, context);
    writeHvdcLines(filter, network, context);

    if(supportAreasExport(context)){
        writeAreas(network, context);
    }

    writeVoltageAngleLimits(network, context);

    writeExtensions(network, context);

    writer.writeEndElement();
}

void NetworkXml::writeSubnetworks(const Network& network, NetworkXmlWriterContext& context) {
    for (const Network& subnetwork : network.getSubNetworks()) {
        IidmXmlUtil::assertMinimumVersion(NETWORK, NETWORK, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_11(), context);
        writeNetwork(subnetwork, context);
    }
}

void NetworkXml::writeHvdcLines(const BusFilter& filter, const Network& network, NetworkXmlWriterContext& context) {
    for (const HvdcLine& line : network.getHvdcLines()) {
        if (isElementWrittenInsideNetwork(line, network, context) && filter.test(line.getConverterStation1()) && filter.test(line.getConverterStation2())) {
            HvdcLineXml::getInstance().write(line, network, context);
        }
    }
}

void NetworkXml::writeLines(const BusFilter& filter, const Network& network, NetworkXmlWriterContext& context) {
    for (const Line& line : network.getLines()) {
        if (isElementWrittenInsideNetwork(line, network, context) && filter.test(line)) {
            LineXml::getInstance().write(line, network, context);
        }
    }
}

void NetworkXml::writeTieLines(const BusFilter& filter, const Network& network, NetworkXmlWriterContext& context) {
    for (const TieLine& tl : network.getTieLines()) {
        if (isElementWrittenInsideNetwork(tl, network, context) && filter.test(tl)) {
            TieLineXml::getInstance().write(tl, network, context);
        }
    }
}

void NetworkXml::writeSubstations(const Network& network, NetworkXmlWriterContext& context) {
    for (const Substation& substation : network.getSubstations()) {
        if(isElementWrittenInsideNetwork(substation, network, context)) {
            SubstationXml::getInstance().write(substation, network, context);
        }
    }
}

void NetworkXml::writeVoltageLevels(const Network& network, NetworkXmlWriterContext& context) {
    for (const VoltageLevel& voltageLevel : network.getVoltageLevels()) {
        if (isElementWrittenInsideNetwork(voltageLevel, network, context) && !voltageLevel.getSubstation()) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, VOLTAGE_LEVEL, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_6(), context);
            VoltageLevelXml::getInstance().write(voltageLevel, network, context);
        }
    }
}

void NetworkXml::writeAreas(const Network& network, NetworkXmlWriterContext& context) {
    for (const Area& area : network.getAreas()) {
        if (isElementWrittenInsideNetwork(area, network, context)) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, AREA, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_13(), context);
            AreaXml::getInstance().write(area, network, context);
        }
    }
}

void NetworkXml::writeVoltageAngleLimits(const Network& network, NetworkXmlWriterContext& context) {
    for (const VoltageAngleLimit& limit : network.getVoltageAngleLimits()) {
        VoltageAngleLimitXml::getInstance().write(limit, network, context);
    }
}

void NetworkXml::writeDcDetailed(const Network& network, NetworkXmlWriterContext& context) {
    //Introduced and supported only from IIDM V1.15
    //"DC-only" equipments and in particular DcNodes are written first in the network, so that AC/DC converters can refer to those DcNodes.
    writeDcNodes(network, context);
    writeDcSwitches(network, context);
    writeDcGrounds(network ,context);
    writeDcLines(network, context);
}
void NetworkXml::writeDcNodes(const Network& network, NetworkXmlWriterContext& context) {
    for (const DcNode& dcNode : network.getDcNodes()) {
        if (isElementWrittenInsideNetwork(dcNode, network, context)) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, DC_NODE, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_15(), context);
            DcNodeXml::getInstance().write(dcNode, network, context);
        }
    }
}
void NetworkXml::writeDcSwitches(const Network& network, NetworkXmlWriterContext& context) {
    for (const DcSwitch& dcSwitch : network.getDcSwitches()) {
        if (isElementWrittenInsideNetwork(dcSwitch, network, context)) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, DC_SWITCH, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_15(), context);
            DcSwitchXml::getInstance().write(dcSwitch, network, context);
        }
    }
}
void NetworkXml::writeDcGrounds(const Network& network, NetworkXmlWriterContext& context) {
    for (const DcGround& dcGround : network.getDcGrounds()) {
        if (isElementWrittenInsideNetwork(dcGround, network, context)) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, DC_GROUND, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_15(), context);
            DcGroundXml::getInstance().write(dcGround, network, context);
        }
    }
}
void NetworkXml::writeDcLines(const Network& network, NetworkXmlWriterContext& context) {
    for (const DcLine& dcLine : network.getDcLines()) {
        if (isElementWrittenInsideNetwork(dcLine, network, context)) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, DC_LINE, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_15(), context);
            DcLineXml::getInstance().write(dcLine, network, context);
        }
    }
}

bool NetworkXml::ignoreEquipmentAtExport(const Identifiable& identifiable, NetworkXmlWriterContext& context) {
    return ( !context.isExportedEquipment(identifiable.getId()) ||
        (stdcxx::isInstanceOf<OverloadManagementSystem>(identifiable) && !context.getOptions().isWithAutomationSystems()) );
}

bool NetworkXml::isElementWrittenInsideNetwork(const Identifiable& element, const Network &network, NetworkXmlWriterContext &context) {
    // if subnetworks not supported or flatten, all elements need to be written in the root network (in that case this is only called giving the root network)
    if (!supportSubnetworksExport(context) || context.getOptions().isFlatten()) {
        return true;
    }
    // corner case: if the element is the given network, it is considered as written within that network, as extensions have to be written within the network
    if (network.getId() == element.getId()) {
        return true;
    }
    // Main case: the element has to be written
    // - if the element is directly in the network (not in one of its subnetworks)
    // - and if it's not a network itself (linked to previous corner case)
    return stdcxx::areSame(element.getParentNetwork(), network) && element.getType() != IdentifiableType::NETWORK;
}

bool NetworkXml::supportSubnetworksExport(NetworkXmlWriterContext& context) {
    return context.getVersion() >= IidmXmlVersion::V1_11();
}

bool NetworkXml::supportAreasExport(NetworkXmlWriterContext& context) {
    return context.getVersion() >= IidmXmlVersion::V1_13();
}

void NetworkXml::initNetwork(Network& network, NetworkXmlReaderContext& context) {
    int forecastDistance = context.getReader().getOptionalAttributeValue(FORECAST_DISTANCE, 0);
    const std::string& caseDateStr = context.getReader().getAttributeValue(CASE_DATE);
    
    network.setForecastDistance(forecastDistance);

    try {
        network.setCaseDate(stdcxx::DateTime::parse(caseDateStr));
    } catch (const PowsyblException& err) {
        throw powsybl::xml::XmlStreamException(err.what());
    }

    ValidationLevel minValidationLevel;
    if (context.getOptions().getMinimalValidationLevel().has_value()) {
        minValidationLevel = context.getOptions().getMinimalValidationLevel().get();
        //Read min validation level from file, but don't use it
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_7(), context.getVersion(), [&context] { context.getReader().getAttributeValue(MINIMUM_VALIDATION_LEVEL); });
    } else {
        std::string minimumValidationLevel{STEADY_STATE_HYPOTHESIS};
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_7(), context.getVersion(), [&minimumValidationLevel, &context] { minimumValidationLevel = context.getReader().getAttributeValue(MINIMUM_VALIDATION_LEVEL); });
        minValidationLevel = minimumValidationLevel.empty() ? ValidationLevel::UNVALID : Enum::fromString<ValidationLevel>(minimumValidationLevel);
        IidmXmlUtil::assertMinimumVersionIfNotDefault(minValidationLevel != ValidationLevel::STEADY_STATE_HYPOTHESIS, NETWORK, MINIMUM_VALIDATION_LEVEL, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_7(), context);
        context.setNetworkValidationLevel(minValidationLevel);
    }
    network.setMinimumAcceptableValidationLevel(minValidationLevel);
}

void NetworkXml::readNetworkElements(Network& network, NetworkXmlReaderContext& context, std::set<std::string>& extensionsNotFound) {
    context.getReader().readUntilEndElement(NETWORK, [&network, &context, &extensionsNotFound]() {
        std::string localName = context.getReader().getLocalName();
        if (localName == ALIAS) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, ALIAS, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
            AliasesXml::read(network, context);
        } else if (localName == PROPERTY) {
            PropertiesXml::read(network, context);
        } else if (localName == NETWORK) {
            readSubnetwork(network, context, extensionsNotFound);
        } else if (localName == DC_NODE) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, DC_NODE, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_15(), context);
            DcNodeXml::getInstance().read(network, context);
        } else if (localName == DC_SWITCH) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, DC_SWITCH, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_15(), context);
            DcSwitchXml::getInstance().read(network, context);
        } else if (localName == DC_GROUND) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, DC_GROUND, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_15(), context);
            DcGroundXml::getInstance().read(network, context);
        } else if (localName == DC_LINE) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, DC_LINE, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_15(), context);
            DcLineXml::getInstance().read(network, context);
        } else if (localName == VOLTAGE_LEVEL) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, VOLTAGE_LEVEL, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_6(), context);
            VoltageLevelXml::getInstance().read(network, context);
        } else if (localName == SUBSTATION) {
            SubstationXml::getInstance().read(network, context);
        } else if (localName == LINE) {
            LineXml::getInstance().read(network, context);
        } else if (localName == TIE_LINE) {
            TieLineXml::getInstance().read(network, context);
        } else if (localName == HVDC_LINE) {
            HvdcLineXml::getInstance().read(network, context);
        } else if (localName == AREA) {
            IidmXmlUtil::assertMinimumVersion(NETWORK, AREA, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_13(), context);
            AreaXml::getInstance().read(network, context);
        } else if (localName == VOLTAGE_ANGLE_LIMIT) { 
            VoltageAngleLimitXml::getInstance().read(network, context);
        } else if (localName == EXTENSION) {
            readExtensions(network, context, extensionsNotFound);
        } else {
            throw powsybl::xml::XmlStreamException(stdcxx::format("Unexpected element: %1%", localName));
        }
    });
}

void NetworkXml::readSubnetwork(Network& parentnetwork, NetworkXmlReaderContext& context, std::set<std::string>& extensionsNotFound) {
    IidmXmlUtil::assertMinimumVersion(NETWORK, NETWORK, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_11(), context);
    if(static_cast<bool>(parentnetwork.getParentNetworkRef())) { //already on a subnetwork level
        throw powsybl::xml::XmlStreamException("Only one level of subnetwork is currently supported.");
    }
    
    //Create a new subnetwork
    const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));
    const std::string& sourceFormat = context.getReader().getAttributeValue(SOURCE_FORMAT);
    Network& subnetwork = parentnetwork.newSubnetwork(id, sourceFormat);
    initNetwork(subnetwork, context);

    readNetworkElements(subnetwork, context, extensionsNotFound);
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
