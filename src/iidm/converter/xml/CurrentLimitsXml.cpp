/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/CurrentLimitsXml.hpp>

#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

void CurrentLimitsXml::writeCurrentLimits(const CurrentLimits& limits, powsybl::xml::XmlStreamWriter& writer, const boost::optional<int>& index, const std::string& nsPrefix) {
    if (!std::isnan(limits.getPermanentLimit()) || !limits.getTemporaryLimits().empty()) {
        writer.writeStartElement(nsPrefix, toString(CURRENT_LIMITS, index));
        writer.writeAttribute(PERMANENT_LIMIT, limits.getPermanentLimit());

        constexpr unsigned long undefinedAcceptableDuration = std::numeric_limits<int>::max();

        for (const auto& tl : limits.getTemporaryLimits()) {
            writer.writeStartElement(nsPrefix, TEMPORARY_LIMIT);
            writer.writeAttribute(NAME, tl.get().getName());
            writer.writeOptionalAttribute(ACCEPTABLE_DURATION, tl.get().getAcceptableDuration(), undefinedAcceptableDuration);
            writer.writeOptionalAttribute(VALUE, tl.get().getValue(), std::numeric_limits<double>::max());
            writer.writeOptionalAttribute(FICTITIOUS, tl.get().isFictitious(), false);
            writer.writeEndElement();
        }

        writer.writeEndElement();
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
