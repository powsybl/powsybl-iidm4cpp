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

template <typename T, typename A, typename P>
class AbstractConnectableXml : public AbstractIdentifiableXml<T, A, P> {
public:
    template <typename S, typename O>
    static void readCurrentLimits(const boost::optional<int>& index, CurrentLimitsAdder<S,O>& adder, const powsybl::xml::XmlStreamReader& reader);

    static void writeCurrentLimits(const boost::optional<int>& index, const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer);

    static void writeCurrentLimits(const boost::optional<int>& index, const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const std::string& nsPrefix);

protected:
    static void readNodeOrBus(BranchAdder<A>& adder, const NetworkXmlReaderContext& context);

    static void readNodeOrBus(InjectionAdder<A>& adder, const NetworkXmlReaderContext& context);

    static void readPQ(const boost::optional<int>& index, Terminal& terminal, const powsybl::xml::XmlStreamReader& reader);

    static std::string toString(const char* string, const boost::optional<int>& index);

    static void writeNodeOrBus(const boost::optional<int>& index, const Terminal& terminal, NetworkXmlWriterContext& context);

    static void writePQ(const boost::optional<int>& index, const Terminal& terminal, powsybl::xml::XmlStreamWriter& writer);

protected:
    AbstractConnectableXml() = default;

    ~AbstractConnectableXml() noexcept override = default;

private:
    static void writeBus(const boost::optional<int>& index, const stdcxx::CReference<Bus>& bus, const stdcxx::CReference<Bus>& connectableBus, NetworkXmlWriterContext& context);

    static void writeNode(const boost::optional<int>& index, const Terminal& terminal, NetworkXmlWriterContext& context);
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include "AbstractConnectableXml.hxx"

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTCONNECTABLEXML_HPP

