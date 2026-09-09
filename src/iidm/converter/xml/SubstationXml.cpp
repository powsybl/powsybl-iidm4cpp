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

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include "OverloadManagementSystemXml.hpp"
#include "ThreeWindingsTransformerXml.hpp"
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

Substation& SubstationXml::readRootElementAttributes(SubstationAdder& adder, Network& /*network*/, NetworkXmlReaderContext& context) const {
    const std::string& countryCode = context.getReader().getOptionalAttributeValue(COUNTRY, "");
    if (!countryCode.empty()) {
        const auto& country = Enum::fromString<Country>(countryCode);
        adder.setCountry(context.getAnonymizer().deanonymizeCountry(country));
    }

    const std::string& tso = context.getReader().getOptionalAttributeValue(TSO, "");
    if (!tso.empty()) {
        adder.setTso(context.getAnonymizer().deanonymizeString(tso));
    }

    std::list<std::string> geographicalTags = context.getReader().getOptionalArrayAttributeValue(GEOGRAPHICAL_TAGS, "");
    if (!geographicalTags.empty()) {
        for (const auto& tag : geographicalTags) {
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
        } else if (context.getReader().getLocalName() == THREE_WINDINGS_TRANSFORMER) {
            ThreeWindingsTransformerXml::getInstance().read(substation, context);
        } else if (context.getReader().getLocalName() == OVERLOAD_MANAGEMENT_SYSTEM) {
            IidmXmlUtil::assertMinimumVersion(SUBSTATION, OVERLOAD_MANAGEMENT_SYSTEM, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_12(), context);
            if(context.getOptions().isWithAutomationSystems()) {
                OverloadManagementSystemXml::getInstance().read(substation, context);
            } else {
                OverloadManagementSystemXml::getInstance().skip(context);
            }
        }  else {
            AbstractSimpleIdentifiableXml::readSubElements(substation, context);
        }
    });
}

void SubstationXml::writeRootElementAttributes(const Substation& substation, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    const auto& country = substation.getCountry();
    if (country) {
        context.getWriter().writeAttribute(COUNTRY, Enum::toString(context.getAnonymizer().anonymizeCountry(*country)));
    }

    if (!substation.getTso().empty()) {
        context.getWriter().writeAttribute(TSO, context.getAnonymizer().anonymizeString(substation.getTso()));
    }

    if (!substation.getGeographicalTags().empty()) {
        std::list<std::string> tags;
        for (const auto& tag : substation.getGeographicalTags()) {
            tags.emplace_back(context.getAnonymizer().anonymizeString(tag));
        }

        context.getWriter().writeArrayAttribute(GEOGRAPHICAL_TAGS, tags);
    }
}

void SubstationXml::writeSubElements(const Substation& substation, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    for (const auto& voltageLevel : substation.getVoltageLevels()) {
        VoltageLevelXml::getInstance().write(voltageLevel, substation, context);
    }

    for (const auto& twt : substation.getTwoWindingsTransformers()) {
        if (!context.getFilter().test(twt)) {
            continue;
        }
        TwoWindingsTransformerXml::getInstance().write(twt, substation, context);
    }

    for (const auto& twt : substation.getThreeWindingsTransformers()) {
        if (!context.getFilter().test(twt)) {
            continue;
        }
        ThreeWindingsTransformerXml::getInstance().write(twt, substation, context);
    }

    if(context.getOptions().isWithAutomationSystems()) {
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_12(), context.getVersion(), [&context, &substation]() {
            stdcxx::const_range<OverloadManagementSystem> validOmsRange = substation.getOverloadManagementSystems() | boost::adaptors::filtered(SubstationXml::filterValidOverloadManagementSystems);
            for (const auto& oms : validOmsRange) {
                OverloadManagementSystemXml::getInstance().write(oms, substation, context);
            }
        });
    }

}

 bool SubstationXml::filterValidOverloadManagementSystems(const OverloadManagementSystem& oms) {
    const Network& n = oms.getSubstation().get().getNetwork();

    if(!static_cast<bool>(n.find(oms.getMonitoredElementId()))){
        logging::Logger& logger = logging::LoggerFactory::getLogger<SubstationXml>();
        logger.warn(stdcxx::format("Discard overload management system '%1%': monitored element '%2%' is unknown.", oms.getId(), oms.getMonitoredElementId()));
        return false;
    }

    for (const auto& tripping : oms.getTrippings()) {
        bool elementfound = false;
        std::string id = "";

        if (tripping.getType() == OverloadManagementSystem::Tripping::Type::BRANCH_TRIPPING &&
            stdcxx::isInstanceOf<overload_management_system::BranchTripping>(tripping)) {
            const auto &branchTripping = dynamic_cast<const overload_management_system::BranchTripping &>(tripping);
            id = branchTripping.getBranchToOperateId();
            elementfound = static_cast<bool>(n.find<Branch>(id));
        }
        else if (tripping.getType() == OverloadManagementSystem::Tripping::Type::SWITCH_TRIPPING &&
                 stdcxx::isInstanceOf<overload_management_system::SwitchTripping>(tripping))
        {
            const auto &switchTripping = dynamic_cast<const overload_management_system::SwitchTripping &>(tripping);
            id = switchTripping.getSwitchToOperateId();
            elementfound = static_cast<bool>(n.find<Switch>(id));
        }
        else if (tripping.getType() == OverloadManagementSystem::Tripping::Type::THREE_WINDINGS_TRANSFORMER_TRIPPING &&
                 stdcxx::isInstanceOf<overload_management_system::ThreeWindingsTransformerTripping>(tripping))
        {
            const auto &twtTripping = dynamic_cast<const overload_management_system::ThreeWindingsTransformerTripping &>(tripping);
            id = twtTripping.getThreeWindingsTransformerToOperateId();
            elementfound = static_cast<bool>(n.find<ThreeWindingsTransformer>(id));
        }

        if(!elementfound) {
            logging::Logger& logger = logging::LoggerFactory::getLogger<SubstationXml>();
            logger.warn(stdcxx::format("Discard overload management system '%1%': invalid %2% tripping. '%3%' is unknown.", oms.getNameOrId(), Enum::toString(oms.getType()), id));
            return false;
        }
    }

    return true;
 }

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
