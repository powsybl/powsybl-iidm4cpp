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

#include "AbstractIdentifiableXml.hpp"

namespace powsybl {

namespace xml {

class XmlStreamReader;
class XmlStreamWriter;

}  // namespace xml

namespace iidm {

class Bus;
class CurrentLimits;
class Terminal;

namespace converter {

namespace xml {

template <typename Added, typename Adder, typename Parent>
class AbstractConnectableXml : public AbstractIdentifiableXml<Added, Adder, Parent> {
public:
    template <typename S, typename O>
    static void readCurrentLimits(CurrentLimitsAdder<S, O>& adder, const powsybl::xml::XmlStreamReader& reader, const boost::optional<int>& index = boost::optional<int>());

    static void writeCurrentLimits(const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const boost::optional<int>& index = boost::optional<int>(), const std::string& nsPrefix = IIDM_PREFIX);

protected:
    static void readNodeOrBus(BranchAdder<Adder>& adder, const NetworkXmlReaderContext& context);

    static void readNodeOrBus(InjectionAdder<Adder>& adder, const NetworkXmlReaderContext& context);

    template <typename L>
    static void readNodeOrBus(int index, ThreeWindingsTransformerAdder::LegAdder<L>& adder, const NetworkXmlReaderContext& context);

    static void readPQ(Terminal& terminal, const powsybl::xml::XmlStreamReader& reader, const boost::optional<int>& index = boost::optional<int>());

    static std::string toString(const char* string, const boost::optional<int>& index);

    static void writeNodeOrBus(const Terminal& terminal, NetworkXmlWriterContext& context, const boost::optional<int>& index = boost::optional<int>());

    static void writePQ(const Terminal& terminal, powsybl::xml::XmlStreamWriter& writer, const boost::optional<int>& index = boost::optional<int>());

protected:
    AbstractConnectableXml() = default;

    ~AbstractConnectableXml() noexcept override = default;

private:
    static void writeBus(const stdcxx::CReference<Bus>& bus, const stdcxx::CReference<Bus>& connectableBus, NetworkXmlWriterContext& context, const boost::optional<int>& index = boost::optional<int>());

    static void writeNode(const Terminal& terminal, NetworkXmlWriterContext& context, const boost::optional<int>& index = boost::optional<int>());
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include "AbstractConnectableXml.hxx"

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HPP

