/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_CURRENTLIMITSXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_CURRENTLIMITSXML_HPP

#include <string>

#include <boost/optional.hpp>

#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/converter/Constants.hpp>

namespace powsybl {

namespace xml {

class XmlStreamReader;
class XmlStreamWriter;

}

namespace iidm {

namespace converter {

namespace xml {

class CurrentLimitsXml {
public:
    template <typename S, typename O>
    static void readCurrentLimits(CurrentLimitsAdder<S, O>& adder, const powsybl::xml::XmlStreamReader& reader, const boost::optional<int>& index = boost::optional<int>());

    static void writeCurrentLimits(const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const boost::optional<int>& index = boost::optional<int>(), const std::string& nsPrefix = IIDM_PREFIX);
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/converter/xml/CurrentLimitsXml.hxx>

#endif  // POWSYBL_IIDM_CONVERTER_XML_CURRENTLIMITSXML_HPP
