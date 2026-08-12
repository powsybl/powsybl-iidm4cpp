/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HPP

#include <powsybl/iidm/AcDcConverterAdder.hpp>
#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/BranchAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/InjectionAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/stdcxx/optional.hpp>


namespace powsybl {

namespace xml {

class XmlStreamReader;
class XmlStreamWriter;

}  // namespace xml

namespace iidm {

class AcDcConverter;
class ActivePowerLimits;
class ApparentPowerLimits;
class Bus;
class CurrentLimits;
class OperationalLimitsGroup;
class Terminal;

namespace converter {

namespace xml {

class AbstractConnectableXml {
public:
    static void readActivePowerLimits(ActivePowerLimitsAdder&& adder, const NetworkXmlReaderContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void readApparentPowerLimits(ApparentPowerLimitsAdder&& adder, const NetworkXmlReaderContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void readCurrentLimits(CurrentLimitsAdder&& adder, const NetworkXmlReaderContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeActivePowerLimits(const ActivePowerLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeApparentPowerLimits(const ApparentPowerLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeCurrentLimits(const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeCurrentLimits(const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const std::string& nsPrefix, const IidmXmlVersion& version, const stdcxx::optional<int>& index = stdcxx::optional<int>());

protected:
    template <typename Added, typename Adder>
    static void readNodeOrBus(BranchAdder<Added, Adder>& adder, const NetworkXmlReaderContext& context);

    template <typename Added, typename Adder>
    static void readNodeOrBus(InjectionAdder<Added, Adder>& adder, const NetworkXmlReaderContext& context);

    template <typename Added, typename Adder>
    static void readNodeOrBus(InjectionAdder<Added, Adder>& adder,const std::string& suffix, const NetworkXmlReaderContext& context);

    static void readNodeOrBus(int index, ThreeWindingsTransformerAdder::LegAdder& adder, const NetworkXmlReaderContext& context);

    template <typename Added, typename Adder>
    static void readNodeOrBus(AcDcConverterAdder<Added, Adder>& adder, const NetworkXmlReaderContext& context);

    static void readPQ(Terminal& terminal, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void readPI(DcTerminal& dcTerminal, const powsybl::xml::XmlStreamReader& reader);

    static void writeNodeOrBus(const Terminal& terminal, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeNodeOrBus(const AcDcConverter& converter, NetworkXmlWriterContext& context);

    static void writePQ(const Terminal& terminal, powsybl::xml::XmlStreamWriter& writer, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writePI(const DcTerminal& dcTerminal, powsybl::xml::XmlStreamWriter& writer);

    static void writeSelectedGroupId(const stdcxx::optional<std::string>& selectedGroupId, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeAllSelectedGroupIds(const std::list<std::string>& groupIds, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void readSelectedGroupId(NetworkXmlReaderContext& context, const std::function<void(const std::string&)>& endTaskConsumer, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void readAllSelectedGroupIds(NetworkXmlReaderContext& context, const std::function<void(const std::list<std::string>&)>& endTaskConsumer, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeLimits(NetworkXmlWriterContext& context, const char* rootName, const stdcxx::CReference<OperationalLimitsGroup>& selectedLimitsGroup, const stdcxx::const_range<OperationalLimitsGroup>& limitsGroups, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void readLoadingLimitsGroup(const NetworkXmlReaderContext& context, const char* groupElementName, const std::function<stdcxx::Reference<OperationalLimitsGroup>(const std::string&)>& groupBuilder);
    static void readLoadingLimitsGroup(const NetworkXmlReaderContext& context, const char* groupElementName, FlowsLimitsHolder& holder);

protected:
    AbstractConnectableXml() = default;

    ~AbstractConnectableXml() noexcept = default;

private:
    template <typename LimitsAdder>
    static void readLoadingLimits(const std::string& type, LimitsAdder&& adderValue, const NetworkXmlReaderContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeBus(const stdcxx::CReference<Bus>& bus, const stdcxx::CReference<Bus>& connectableBus, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    template <typename Limits>
    static void writeLoadingLimits(const Limits& limits, powsybl::xml::XmlStreamWriter& writer, const std::string& nsPrefix, const IidmXmlVersion& version, const std::string& type, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeNode(const Terminal& terminal, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeLoadingLimitsGroups(const stdcxx::const_range<OperationalLimitsGroup>& limitsGroups, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void readAllLoadingLimits(OperationalLimitsGroup& limitsGroup, const char* groupElementName, const NetworkXmlReaderContext& context);

    static std::string getDcTerminalSuffix(const DcTerminal& dcTerminal);

};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include "AbstractConnectableXml.hxx"

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HPP

