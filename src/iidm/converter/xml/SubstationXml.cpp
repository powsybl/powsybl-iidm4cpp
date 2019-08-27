/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SubstationXml.hpp"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>

#include <powsybl/iidm/Country.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>

#include "TwoWindingsTransformerXml.hpp"
#include "VoltageLevelXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

SubstationAdder SubstationXml::createAdder(Network& network) const {
    return network.newSubstation();
}

const SubstationXml& SubstationXml::getInstance() {
    static SubstationXml s_instance;
    return s_instance;
}

const char* SubstationXml::getRootElementName() const {
    return SUBSTATION;
}

bool SubstationXml::hasSubElements(const Substation& /*substation*/) const {
    return true;
}

Substation& SubstationXml::readRootElementAttributes(SubstationAdder& adder, const NetworkXmlReaderContext& context) const {
    const std::string& countryCode = context.getReader().getOptionalAttributeValue(COUNTRY, "");
    if (!countryCode.empty()) {
        const Country& country = getCountryFromCode(context.getAnonymizer().deanonymizeString(countryCode));
        adder.setCountry(country);
    }

    const std::string& tso = context.getReader().getOptionalAttributeValue(TSO, "");
    if (!tso.empty()) {
        adder.setTso(context.getAnonymizer().deanonymizeString(tso));
    }

    const std::string& geographicalTags = context.getReader().getOptionalAttributeValue(GEOGRAPHICAL_TAGS, "");
    if (!geographicalTags.empty()) {
        std::vector<std::string> tags;
        boost::algorithm::split(tags, geographicalTags, boost::is_any_of(","));

        for (const auto& tag : tags) {
            adder.addGeographicalTag(context.getAnonymizer().deanonymizeString(tag));
        }
    }

    return adder.add();
}

void SubstationXml::readSubElements(Substation& substation, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(SUBSTATION, [this, &substation, &context]() {
        if (context.getReader().getLocalName() == VOLTAGE_LEVEL) {
            VoltageLevelXml::getInstance().read(substation, context);
        } else if (context.getReader().getLocalName() == TWO_WINDINGS_TRANSFORMER) {
            TwoWindingsTransformerXml::getInstance().read(substation, context);
        } else {
            AbstractIdentifiableXml::readSubElements(substation, context);
        }
    });
}

void SubstationXml::writeRootElementAttributes(const Substation& substation, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    const auto& country = substation.getCountry();
    if (country) {
        context.getWriter().writeAttribute(COUNTRY, context.getAnonymizer().anonymizeString(getCountryCode(*country)));
    }

    if (!substation.getTso().empty()) {
        context.getWriter().writeAttribute(TSO, context.getAnonymizer().anonymizeString(substation.getTso()));
    }

    if (!substation.getGeographicalTags().empty()) {
        std::vector<std::string> tags;
        for (const auto& tag : substation.getGeographicalTags()) {
            tags.emplace_back(context.getAnonymizer().anonymizeString(tag));
        }

        context.getWriter().writeAttribute(GEOGRAPHICAL_TAGS, boost::algorithm::join(tags, ","));
    }
}

void SubstationXml::writeSubElements(const Substation& substation, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    for (const auto& voltageLevel : substation.getVoltageLevels()) {
        VoltageLevelXml::getInstance().write(voltageLevel, substation, context);
    }

    for (const auto& twt : substation.getTwoWindingsTransformers()) {
        // TODO(sebalaig) consider filtering
        TwoWindingsTransformerXml::getInstance().write(twt, substation, context);
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
