/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_CURRENTLIMITSXML_HXX
#define POWSYBL_IIDM_CONVERTER_XML_CURRENTLIMITSXML_HXX

#include <powsybl/iidm/converter/xml/CurrentLimitsXml.hpp>

#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename S, typename O>
void CurrentLimitsXml::readCurrentLimits(CurrentLimitsAdder<S, O>& adder, const powsybl::xml::XmlStreamReader& reader, const boost::optional<int>& index) {
    const double& permanentLimit = reader.getOptionalAttributeValue(PERMANENT_LIMIT, stdcxx::nan());
    adder.setPermanentLimit(permanentLimit);
    reader.readUntilEndElement(toString(CURRENT_LIMITS, index), [&adder, &reader]() {
        if (reader.getLocalName() == TEMPORARY_LIMIT) {
            const std::string& name = reader.getAttributeValue(NAME);
            int acceptableDuration = reader.getOptionalAttributeValue(ACCEPTABLE_DURATION, std::numeric_limits<int>::max());
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
    adder.add();
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif // POWSYBL_IIDM_CONVERTER_XML_CURRENTLIMITSXML_HXX
