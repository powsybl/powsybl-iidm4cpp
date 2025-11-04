/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HXX
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HXX

#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamException.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename LimitsAdder>
void AbstractConnectableXml::readLoadingLimits(const std::string& type, LimitsAdder&& adderValue, const NetworkXmlReaderContext& context, const stdcxx::optional<int>& index) {
    auto&& adder = std::forward<LimitsAdder>(adderValue);
    const powsybl::xml::XmlStreamReader& reader = context.getReader();
    double permanentLimit = reader.getOptionalAttributeValue(PERMANENT_LIMIT, stdcxx::nan());
    if(std::isnan(permanentLimit) && context.getVersion() >= IidmXmlVersion::V1_12()) {
        throw PowsyblException(stdcxx::format("permanentLimit is absent in '%1%'", toString(type.c_str(), index)));
    }
    adder.setPermanentLimit(permanentLimit);
    //Read and add temporaryLimits
    reader.readUntilEndElement(toString(type.c_str(), index), [&reader, &adder]() {
        if (reader.getLocalName() == TEMPORARY_LIMIT) {
            const std::string& name = reader.getAttributeValue(NAME);
            unsigned long acceptableDuration = reader.getOptionalAttributeValue(ACCEPTABLE_DURATION, std::numeric_limits<unsigned long>::max());
            double value = reader.getOptionalAttributeValue(VALUE, std::numeric_limits<double>::max());
            bool fictitious = reader.getOptionalAttributeValue(FICTITIOUS, false);
            adder.beginTemporaryLimit()
                .setName(name)
                .setAcceptableDuration(acceptableDuration)
                .setValue(value)
                .setFictitious(fictitious)
                .endTemporaryLimit();
        }
    });
    adder.fixLimits(context.getOptions().getMissingPermanentLimitPercentage());
    adder.add();
}

template <typename Added, typename Adder>
void AbstractConnectableXml::readNodeOrBus(BranchAdder<Added, Adder>& adder, const NetworkXmlReaderContext& context) {
    const auto& bus1 = context.getReader().getOptionalAttributeValue<std::string>(BUS1);
    const auto& connectableBus1 = context.getReader().getOptionalAttributeValue<std::string>(CONNECTABLE_BUS1);
    const auto& node1 = context.getReader().getOptionalAttributeValue<int>(NODE1);
    const std::string& voltageLevelId1 = context.getReader().getAttributeValue(VOLTAGE_LEVEL_ID1);
    const auto& bus2 = context.getReader().getOptionalAttributeValue<std::string>(BUS2);
    const auto& connectableBus2 = context.getReader().getOptionalAttributeValue<std::string>(CONNECTABLE_BUS2);
    const auto& node2 = context.getReader().getOptionalAttributeValue<int>(NODE2);
    const std::string& voltageLevelId2 = context.getReader().getAttributeValue(VOLTAGE_LEVEL_ID2);
    if (bus1) {
        adder.setBus1(context.getAnonymizer().deanonymizeString(*bus1));
    }
    if (connectableBus1) {
        adder.setConnectableBus1(context.getAnonymizer().deanonymizeString(*connectableBus1));
    }
    if (node1) {
        adder.setNode1(*node1);
    }
    adder.setVoltageLevel1(context.getAnonymizer().deanonymizeString(voltageLevelId1));
    if (bus2) {
        adder.setBus2(context.getAnonymizer().deanonymizeString(*bus2));
    }
    if (connectableBus2) {
        adder.setConnectableBus2(context.getAnonymizer().deanonymizeString(*connectableBus2));
    }
    if (node2) {
        adder.setNode2(*node2);
    }
    adder.setVoltageLevel2(context.getAnonymizer().deanonymizeString(voltageLevelId2));
}

template <typename Added, typename Adder>
void AbstractConnectableXml::readNodeOrBus(InjectionAdder<Added, Adder>& adder, const NetworkXmlReaderContext& context) {
    readNodeOrBus(adder, "", context);
}

template <typename Added, typename Adder>
void AbstractConnectableXml::readNodeOrBus(InjectionAdder<Added, Adder>& adder,const std::string& suffix, const NetworkXmlReaderContext& context) {
    const auto& bus = context.getReader().getOptionalAttributeValue<std::string>(BUS + suffix);
    const auto& connectableBus = context.getReader().getOptionalAttributeValue<std::string>(CONNECTABLE_BUS + suffix);
    const auto& node = context.getReader().getOptionalAttributeValue<unsigned long>(NODE + suffix);

    if (bus) {
        adder.setBus(context.getAnonymizer().deanonymizeString(*bus));
    }
    if (connectableBus) {
        adder.setConnectableBus(context.getAnonymizer().deanonymizeString(*connectableBus));
    }
    if (node) {
        adder.setNode(*node);
    }
}

template <typename Limits>
void AbstractConnectableXml::writeLoadingLimits(const Limits& limits, powsybl::xml::XmlStreamWriter& writer, const std::string& nsPrefix, const IidmXmlVersion& version, const std::string& type, const stdcxx::optional<int>& index) {
    if (!std::isnan(limits.getPermanentLimit()) || !boost::empty(limits.getTemporaryLimits()) || !boost::empty(limits.getFictitiousLimits())) {
        writer.writeStartElement(nsPrefix, toString(type.c_str(), index));
        writer.writeAttribute(PERMANENT_LIMIT, limits.getPermanentLimit());

        for (const auto& fl : limits.getFictitiousLimits()) {
            writer.writeStartElement(version.getPrefix(), TEMPORARY_LIMIT);
            writer.writeAttribute(NAME, fl.getName());
            writer.writeOptionalAttribute(ACCEPTABLE_DURATION, fl.getAcceptableDuration(), std::numeric_limits<unsigned long>::max());
            writer.writeOptionalAttribute(VALUE, fl.getValue(), std::numeric_limits<double>::max());
            writer.writeOptionalAttribute(FICTITIOUS, fl.isFictitious(), false); // for fictitious limits that attribute is supposed to be true.
            writer.writeEndElement();
        }
        for (const auto& tl : limits.getTemporaryLimits()) {
            writer.writeStartElement(version.getPrefix(), TEMPORARY_LIMIT);
            writer.writeAttribute(NAME, tl.getName());
            writer.writeOptionalAttribute(ACCEPTABLE_DURATION, tl.getAcceptableDuration(), std::numeric_limits<unsigned long>::max());
            writer.writeOptionalAttribute(VALUE, tl.getValue(), std::numeric_limits<double>::max());
            writer.writeOptionalAttribute(FICTITIOUS, tl.isFictitious(), false);
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HXX
