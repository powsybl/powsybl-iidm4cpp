/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>

#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/OperationalLimitsGroup.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/PropertiesXml.hpp>
#include <powsybl/iidm/converter/xml/TopologyLevelXmlUtil.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamException.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

void AbstractConnectableXml::readActivePowerLimits(ActivePowerLimitsAdder&& adder, const NetworkXmlReaderContext& context, const stdcxx::optional<int>& index) {
    readLoadingLimits(ACTIVE_POWER_LIMITS, std::move(adder), context, index);
}

void AbstractConnectableXml::readApparentPowerLimits(ApparentPowerLimitsAdder&& adder, const NetworkXmlReaderContext& context, const stdcxx::optional<int>& index) {
    readLoadingLimits(APPARENT_POWER_LIMITS, std::move(adder), context, index);
}

void AbstractConnectableXml::readCurrentLimits(CurrentLimitsAdder&& adder, const NetworkXmlReaderContext& context, const stdcxx::optional<int>& index) {
    readLoadingLimits(CURRENT_LIMITS, std::move(adder), context, index);
}

void AbstractConnectableXml::readNodeOrBus(int index, ThreeWindingsTransformerAdder::LegAdder& adder, const NetworkXmlReaderContext& context) {
    const auto& bus = context.getReader().getOptionalAttributeValue<std::string>(toString(BUS, index));
    const auto& connectableBus = context.getReader().getOptionalAttributeValue<std::string>(toString(CONNECTABLE_BUS, index));
    const auto& node = context.getReader().getOptionalAttributeValue<unsigned long>(toString(NODE, index));
    const std::string& voltageLevelId = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(toString(VOLTAGE_LEVEL_ID, index)));
    if (bus) {
        adder.setBus(context.getAnonymizer().deanonymizeString(*bus));
    }
    if (connectableBus) {
        adder.setConnectableBus(context.getAnonymizer().deanonymizeString(*connectableBus));
    }
    if (node) {
        adder.setNode(*node);
    }
    adder.setVoltageLevel(voltageLevelId);
}

void AbstractConnectableXml::readPQ(Terminal& terminal, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index) {
    const double& p = reader.getOptionalAttributeValue(toString(P, index), stdcxx::nan());
    const double& q = reader.getOptionalAttributeValue(toString(Q, index), stdcxx::nan());
    terminal.setP(p).setQ(q);
}

void AbstractConnectableXml::readPI(DcTerminal& dcTerminal, const powsybl::xml::XmlStreamReader& reader) {
    std::string suffix = getDcTerminalSuffix(dcTerminal);
    const double& dcP = reader.getOptionalAttributeValue(std::string(DC_P) + suffix, stdcxx::nan());
    const double& dcI = reader.getOptionalAttributeValue(std::string(DC_I) + suffix, stdcxx::nan());
    dcTerminal.setP(dcP).setI(dcI);
}

void AbstractConnectableXml::writeBus(const stdcxx::CReference<Bus>& bus, const stdcxx::CReference<Bus>& connectableBus, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index) {
    if (bus) {
        context.getWriter().writeAttribute(toString(BUS, index), context.getAnonymizer().anonymizeString(bus.get().getId()));
    }
    if (connectableBus) {
        context.getWriter().writeAttribute(toString(CONNECTABLE_BUS, index), context.getAnonymizer().anonymizeString(connectableBus.get().getId()));
    }
}

void AbstractConnectableXml::writeActivePowerLimits(const ActivePowerLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index) {
    writeLoadingLimits(limits, writer, version.getPrefix(), version, ACTIVE_POWER_LIMITS, index);
}


void AbstractConnectableXml::writeApparentPowerLimits(const ApparentPowerLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index) {
    writeLoadingLimits(limits, writer, version.getPrefix(), version, APPARENT_POWER_LIMITS, index);
}


void AbstractConnectableXml::writeCurrentLimits(const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index) {
    writeLoadingLimits(limits, writer, version.getPrefix(), version, CURRENT_LIMITS, index);
}


void AbstractConnectableXml::writeCurrentLimits(const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const std::string& nsPrefix, const IidmXmlVersion& version, const stdcxx::optional<int>& index) {
    writeLoadingLimits(limits, writer, nsPrefix, version, CURRENT_LIMITS, index);
}

void AbstractConnectableXml::writeLimits(NetworkXmlWriterContext& context, const char* rootName, const stdcxx::CReference<OperationalLimitsGroup>& selectedLimitsGroup, const stdcxx::const_range<OperationalLimitsGroup>& limitsGroups, const stdcxx::optional<int>& index) {

    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_11(), context.getVersion(), [&context, &rootName, &selectedLimitsGroup, &index](){
        if(selectedLimitsGroup && selectedLimitsGroup.get().getActivePowerLimits()) {
            IidmXmlUtil::assertMinimumVersion(rootName, toString(ACTIVE_POWER_LIMITS, index), ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &selectedLimitsGroup, &index]() { 
                writeActivePowerLimits(selectedLimitsGroup.get().getActivePowerLimits(), context.getWriter(), context.getVersion(), index); 
            });
        }
        if(selectedLimitsGroup && selectedLimitsGroup.get().getApparentPowerLimits()) {
            IidmXmlUtil::assertMinimumVersion(rootName, toString(APPARENT_POWER_LIMITS, index), ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &selectedLimitsGroup, &index]() { 
                writeApparentPowerLimits(selectedLimitsGroup.get().getApparentPowerLimits(), context.getWriter(), context.getVersion(), index); 
            });
        }
        if(selectedLimitsGroup && selectedLimitsGroup.get().getCurrentLimits()) {
            writeCurrentLimits(selectedLimitsGroup.get().getCurrentLimits(), context.getWriter(), context.getVersion(), index); 
        }
    });

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_12(), context.getVersion(), [&limitsGroups, &context, &index](){
        writeLoadingLimitsGroups(limitsGroups, context, index);
    });

}

void AbstractConnectableXml::readLoadingLimitsGroup(const NetworkXmlReaderContext& context, const char* groupElementName, const std::function<stdcxx::Reference<OperationalLimitsGroup>(const std::string&)>& groupBuilder) {
    const std::string& id = context.getReader().getAttributeValue(ID);
    stdcxx::Reference<OperationalLimitsGroup> refLimitsGroup = groupBuilder(id);
    if(!refLimitsGroup) {
        throw PowsyblException(stdcxx::format("Could not retrieve OperationalLimitsGroup %1%", id));
    }
    readAllLoadingLimits(refLimitsGroup.get(), groupElementName, context);

}
void AbstractConnectableXml::readLoadingLimitsGroup(const NetworkXmlReaderContext& context, const char* groupElementName, FlowsLimitsHolder& holder) {
    const std::string& id = context.getReader().getAttributeValue(ID);
    OperationalLimitsGroup& limitsGroup = holder.newOperationalLimitsGroup(id);
    readAllLoadingLimits(limitsGroup, groupElementName, context);
}

void AbstractConnectableXml::writeLoadingLimitsGroups(const stdcxx::const_range<OperationalLimitsGroup>& limitsGroups, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index) {
    for( const auto& limitsGroup : limitsGroups) {
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), toString(LIMITS_GROUP, index));
        context.getWriter().writeAttribute(ID, limitsGroup.getId());

        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_14(), context.getVersion(), [&limitsGroup, &context](){
            PropertiesXml::write(limitsGroup, context);
        });

        if(limitsGroup.getActivePowerLimits()) {
            writeActivePowerLimits(limitsGroup.getActivePowerLimits(), context.getWriter(), context.getVersion());
        }
        if(limitsGroup.getApparentPowerLimits()) {
            writeApparentPowerLimits(limitsGroup.getApparentPowerLimits(), context.getWriter(), context.getVersion());
        }
        if(limitsGroup.getCurrentLimits()) {
            writeCurrentLimits(limitsGroup.getCurrentLimits(), context.getWriter(), context.getVersion());
        }
        context.getWriter().writeEndElement();
    }

}

void AbstractConnectableXml::readAllLoadingLimits(OperationalLimitsGroup& limitsGroup, const char* groupElementName, const NetworkXmlReaderContext& context) {
    context.getReader().readUntilEndElement(groupElementName, [&limitsGroup, &groupElementName, &context]() {
        std::string localName = context.getReader().getLocalName();
        if(localName == ACTIVE_POWER_LIMITS) {
            readActivePowerLimits(limitsGroup.newActivePowerLimits(), context);
        } else if(localName == APPARENT_POWER_LIMITS) {
            readApparentPowerLimits(limitsGroup.newApparentPowerLimits(), context);
        } else if(localName == CURRENT_LIMITS) {
            readCurrentLimits(limitsGroup.newCurrentLimits(), context);
        } else if(localName == PROPERTY) {
            PropertiesXml::read(limitsGroup, context);
        } else {
            throw PowsyblException(stdcxx::format("Unknown element name <%1%> in <%2%>", localName, groupElementName));
        }
    });
}

void AbstractConnectableXml::writeNode(const Terminal& terminal, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index) {
    context.getWriter().writeAttribute(toString(NODE, index), terminal.getNodeBreakerView().getNode());
}

void AbstractConnectableXml::writeNodeOrBus(const Terminal& terminal, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index) {
    TopologyLevel topologyLevel = TopologyLevelXmlUtil::determineTopologyLevel(terminal.getVoltageLevel(), context);
    switch (topologyLevel) {
        case TopologyLevel::NODE_BREAKER:
            writeNode(terminal, context, index);
            break;
        case TopologyLevel::BUS_BREAKER:
            writeBus(terminal.getBusBreakerView().getBus(), terminal.getBusBreakerView().getConnectableBus(), context, index);
            break;
        case TopologyLevel::BUS_BRANCH:
            writeBus(terminal.getBusView().getBus(), terminal.getBusView().getConnectableBus(), context, index);
            break;
        default:
            throw powsybl::xml::XmlStreamException(stdcxx::format("Unexpected TopologyLevel value: ", topologyLevel));
    }

    if (index) {
        context.getWriter().writeAttribute(toString(VOLTAGE_LEVEL_ID, index), context.getAnonymizer().anonymizeString(terminal.getVoltageLevel().getId()));
    }
}

void AbstractConnectableXml::writeNodeOrBus(const AcDcConverter& converter, NetworkXmlWriterContext& context) {
    const auto& t1 = converter.getTerminal1();
    const auto& t2 = converter.getTerminal2();
    //t1 and t2 are on the same voltageLevel so they share the same topology level:
    TopologyLevel topologyLevel = TopologyLevelXmlUtil::determineTopologyLevel(t1.getVoltageLevel(), context);
    switch (topologyLevel) {
        case TopologyLevel::NODE_BREAKER:
            writeNode(t1, context, 1);
            if(static_cast<bool>(t2)){
                writeNode(t2.get(), context, 2);
            }
            break;
        case TopologyLevel::BUS_BREAKER:
            writeBus(t1.getBusBreakerView().getBus(), t1.getBusBreakerView().getConnectableBus(), context, 1);
            if(static_cast<bool>(t2)){
                writeBus(t2.get().getBusBreakerView().getBus(), t2.get().getBusBreakerView().getConnectableBus(), context, 2);
            }
            break;
        case TopologyLevel::BUS_BRANCH:
            writeBus(t1.getBusView().getBus(), t1.getBusView().getConnectableBus(), context, 1);
            if(static_cast<bool>(t2)){
                writeBus(t2.get().getBusView().getBus(), t2.get().getBusView().getConnectableBus(), context, 2);
            }
            break;
        default:
            throw powsybl::xml::XmlStreamException(stdcxx::format("Unexpected TopologyLevel value: ", topologyLevel));
    }
}

void AbstractConnectableXml::writePQ(const Terminal& terminal, powsybl::xml::XmlStreamWriter& writer, const stdcxx::optional<int>& index) {
    writer.writeOptionalAttribute(toString(P, index), terminal.getP());
    writer.writeOptionalAttribute(toString(Q, index), terminal.getQ());
}

void AbstractConnectableXml::writePI(const DcTerminal& dcTerminal, powsybl::xml::XmlStreamWriter& writer) {
    std::string suffix = getDcTerminalSuffix(dcTerminal);
    writer.writeOptionalAttribute(std::string(DC_P) + suffix, dcTerminal.getP());
    writer.writeOptionalAttribute(std::string(DC_I) + suffix, dcTerminal.getI());
}

void AbstractConnectableXml::writeSelectedGroupId(const stdcxx::optional<std::string>& selectedGroupId, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index) {
    if(selectedGroupId.has_value()) {
        context.getWriter().writeAttribute(toString(SELECTED_GROUP_ID, index), *selectedGroupId);
    }
}

void AbstractConnectableXml::readSelectedGroupId(NetworkXmlReaderContext& context, const std::function<void(const std::string&)>& endTaskConsumer, const stdcxx::optional<int>& index) {

    std::string selectedGroupId = context.getReader().getOptionalAttributeValue(toString(SELECTED_GROUP_ID, index), "");
    if(!selectedGroupId.empty()) {
        context.addEndTask(XmlReaderEndTask::Step::AFTER_EXTENSIONS, [selectedGroupId, endTaskConsumer](){
            endTaskConsumer(selectedGroupId);
        });
    }

}

std::string AbstractConnectableXml::getDcTerminalSuffix(const DcTerminal& dcTerminal) {
    TerminalNumber number = dcTerminal.getTerminalNumber();
    TwoSides side = dcTerminal.getSide();
    // note that they dcTerminal TerminalNumber and TwoSides are exclusive, meaning that only of those two can be set 

    return stdcxx::format("%1%%2%", 
        (number != TerminalNumber::UNDEFINED) ? std::to_string(static_cast<int>(number)) : "", 
        (side != TwoSides::UNDEFINED) ? std::to_string(static_cast<int>(side)) : "");
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
