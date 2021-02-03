/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_CURRENTLIMITSXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_CURRENTLIMITSXML_HPP

#include <string>

#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace xml {

class XmlStreamReader;
class XmlStreamWriter;

}  // namespace xml

namespace iidm {

namespace converter {

namespace xml {

class CurrentLimitsXml {
public:
    template <typename S, typename O>
    static void readCurrentLimits(CurrentLimitsAdder<S, O>& adder, const powsybl::xml::XmlStreamReader& reader, const stdcxx::optional<int>& index = stdcxx::optional<int>());

    static void writeCurrentLimits(const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const std::string& nsPrefix, const IidmXmlVersion& version, const stdcxx::optional<int>& index = stdcxx::optional<int>());
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/converter/xml/CurrentLimitsXml.hxx>

#endif  // POWSYBL_IIDM_CONVERTER_XML_CURRENTLIMITSXML_HPP
