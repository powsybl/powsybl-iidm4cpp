/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HPP

#include <powsybl/iidm/BranchAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/InjectionAdder.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/stdcxx/optional.hpp>

#include "AbstractIdentifiableXml.hpp"

namespace powsybl {

namespace xml {

class XmlStreamReader;
class XmlStreamWriter;

}  // namespace xml

namespace iidm {

class ActivePowerLimits;
class ApparentPowerLimits;
class Bus;
class CurrentLimits;
class Terminal;

namespace converter {

namespace xml {

template <typename Added, typename Adder, typename Parent>
class AbstractConnectableXml : public AbstractIdentifiableXml<Added, Adder, Parent> {
public:
    static void readActivePowerLimits(ActivePowerLimitsAdder&& adder, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void readApparentPowerLimits(ApparentPowerLimitsAdder&& adder, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void readCurrentLimits(CurrentLimitsAdder&& adder, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeActivePowerLimits(const ActivePowerLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeApparentPowerLimits(const ApparentPowerLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeCurrentLimits(const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const IidmXmlVersion& version, const stdcxx::optional<int>& index = stdcxx::optional<int>());

protected:
    static void readNodeOrBus(BranchAdder<Adder>& adder, const NetworkXmlReaderContext& context);

    static void readNodeOrBus(InjectionAdder<Adder>& adder, const NetworkXmlReaderContext& context);

    static void readNodeOrBus(int index, ThreeWindingsTransformerAdder::LegAdder& adder, const NetworkXmlReaderContext& context);

    static void readPQ(Terminal& terminal, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeNodeOrBus(const Terminal& terminal, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writePQ(const Terminal& terminal, powsybl::xml::XmlStreamWriter& writer, const stdcxx::optional<int>& index = stdcxx::optional<int>());

protected:
    AbstractConnectableXml() = default;

    ~AbstractConnectableXml() noexcept override = default;

private:
    template <typename LimitsAdder>
    static void readLoadingLimits(const std::string& type, LimitsAdder&& adder, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeBus(const stdcxx::CReference<Bus>& bus, const stdcxx::CReference<Bus>& connectableBus, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    template <typename Limits>
    static void writeLoadingLimits(const Limits& limits, powsybl::xml::XmlStreamWriter& writer, const std::string& nsPrefix, const IidmXmlVersion& version, const std::string& type, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeNode(const Terminal& terminal, NetworkXmlWriterContext& context, const stdcxx::optional<int>& index = stdcxx::optional<int>());
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include "AbstractConnectableXml.hxx"

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HPP

