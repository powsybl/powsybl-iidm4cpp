/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "OverloadManagementSystemXml.hpp"

#include <map>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/converter/Constants.hpp>

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include <powsybl/stdcxx/format.hpp>

#include <powsybl/xml/XmlStreamException.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

OverloadManagementSystemAdder OverloadManagementSystemXml::createAdder(Substation& substation) const {
    return substation.newOverloadManagementSystem();
}

const OverloadManagementSystemXml& OverloadManagementSystemXml::getInstance() {
    static OverloadManagementSystemXml s_instance;
    return s_instance;
}

const char* OverloadManagementSystemXml::getRootElementName() const {
    return OVERLOAD_MANAGEMENT_SYSTEM;
}

void OverloadManagementSystemXml::writeRootElementAttributes(const OverloadManagementSystem& oms, const Substation& /*substation*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(ENABLED, oms.isEnabled());
    std::string monitoredId = context.getAnonymizer().anonymizeString(oms.getMonitoredElementId());
    context.getWriter().writeAttribute(MONITORED_ELEMENT_ID, monitoredId);
    context.getWriter().writeAttribute(SIDE, Enum::toString(oms.getMonitoredSide()));
}

void OverloadManagementSystemXml::writeSubElements(const OverloadManagementSystem& oms, const Substation& /*substation*/, NetworkXmlWriterContext& context) const {
    for(const auto& tripping : oms.getTrippings()) {
        writeTripping(tripping, context);
    }
}

void OverloadManagementSystemXml::writeTripping(const OverloadManagementSystem::Tripping& tripping, NetworkXmlWriterContext& context) const {
    std::string monitoredId = "";
    if(tripping.getType() == OverloadManagementSystem::Tripping::Type::BRANCH_TRIPPING &&
            stdcxx::isInstanceOf<overload_management_system::BranchTripping>(tripping)) {
        const auto& branchTripping = dynamic_cast<const overload_management_system::BranchTripping&>(tripping);
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), BRANCH_TRIPPING);
        writeTrippingCommonAttributes(tripping, context);
        monitoredId = context.getAnonymizer().anonymizeString(branchTripping.getBranchToOperateId());
        context.getWriter().writeAttribute(BRANCH_ID, monitoredId);
        context.getWriter().writeAttribute(SIDE, Enum::toString(branchTripping.getSideToOperate()));
        context.getWriter().writeEndElement();
    } else if(tripping.getType() == OverloadManagementSystem::Tripping::Type::SWITCH_TRIPPING &&
            stdcxx::isInstanceOf<overload_management_system::SwitchTripping>(tripping)) {
        const auto& switchTripping = dynamic_cast<const overload_management_system::SwitchTripping&>(tripping);
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), SWITCH_TRIPPING);
        writeTrippingCommonAttributes(tripping, context);
        monitoredId = context.getAnonymizer().anonymizeString(switchTripping.getSwitchToOperateId());
        context.getWriter().writeAttribute(SWITCH_ID, monitoredId);
        context.getWriter().writeEndElement();
    } else if(tripping.getType() == OverloadManagementSystem::Tripping::Type::THREE_WINDINGS_TRANSFORMER_TRIPPING &&
            stdcxx::isInstanceOf<overload_management_system::ThreeWindingsTransformerTripping>(tripping)) {
        const auto& twtTripping = dynamic_cast<const overload_management_system::ThreeWindingsTransformerTripping&>(tripping);
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), THREE_WINDINGS_TRANSFORMER_TRIPPING);
        writeTrippingCommonAttributes(tripping, context);
        monitoredId = context.getAnonymizer().anonymizeString(twtTripping.getThreeWindingsTransformerToOperateId());
        context.getWriter().writeAttribute(THREE_WINDINGS_TRANSFORMER_ID, monitoredId);
        context.getWriter().writeAttribute(SIDE, Enum::toString(twtTripping.getSideToOperate()));
        context.getWriter().writeEndElement();
    } else {
        throw powsybl::xml::XmlStreamException(stdcxx::format("Unexpected tripping type: %1%", Enum::toString(tripping.getType())));
    }
}

void OverloadManagementSystemXml::writeTrippingCommonAttributes(const OverloadManagementSystem::Tripping& tripping, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(KEY, tripping.getKey());
    std::string nameOrKey = tripping.getNameOrKey();
    if(!nameOrKey.empty() && nameOrKey!=tripping.getKey()) {
        context.getWriter().writeAttribute(NAME, nameOrKey);
    }
    context.getWriter().writeAttribute(CURRENT_LIMIT, tripping.getCurrentLimit());
    context.getWriter().writeAttribute(OPEN_ACTION, tripping.isOpenAction());
}

void OverloadManagementSystemXml::readAndPostponeCreation(Substation& substation, NetworkXmlReaderContext& context) const {
    std::vector<std::function<void(Identifiable&)>> toApply;

    auto ptrAdder = std::make_shared<OverloadManagementSystemAdder>(substation);
    std::shared_ptr<Validable> storePtr = std::dynamic_pointer_cast<Validable>(ptrAdder);
    context.addEndAdder(storePtr);
    OverloadManagementSystemAdder& adder = *ptrAdder.get();
    const std::string id = readIdentifierAttributes(adder, context);
    readRootElementAttributes(adder, toApply, context);
    readSubElements(id, adder, toApply, context);

    context.addEndTask([&adder, toApply]() {
        OverloadManagementSystem& identifiable = adder.add();
        for(auto func : toApply) {
            func(identifiable);
        }
    });
}

void OverloadManagementSystemXml::readRootElementAttributes(OverloadManagementSystemAdder& adder, std::vector<std::function<void(Identifiable&)>>& /*toApply*/, NetworkXmlReaderContext& context) const {
    bool enabled;
    std::string monitoredElementId;
    ThreeSides side;
    getRootElementAttributes(context, enabled, monitoredElementId, side);
    adder.setEnabled(enabled)
        .setMonitoredElementId(monitoredElementId)
        .setMonitoredElementSide(side);
}
void OverloadManagementSystemXml::skipRootElementAttributes(NetworkXmlReaderContext& context) const {
    bool enabled;
    std::string monitoredElementId;
    ThreeSides side;
    getRootElementAttributes(context, enabled, monitoredElementId, side);
}
void OverloadManagementSystemXml::getRootElementAttributes(NetworkXmlReaderContext& context, bool& enabled, std::string& monitoredId, ThreeSides& side) const {
    enabled = context.getReader().getAttributeValue<bool>(ENABLED);
    monitoredId = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(MONITORED_ELEMENT_ID));
    side = Enum::fromString<ThreeSides>(context.getReader().getAttributeValue(SIDE));
}

void OverloadManagementSystemXml::readSubElements(const std::string& id, OverloadManagementSystemAdder& adder, std::vector<std::function<void(Identifiable&)>>& toApply, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(OVERLOAD_MANAGEMENT_SYSTEM, [this, &id, &adder, &toApply, &context]() {
        if(context.getReader().getLocalName() == BRANCH_TRIPPING) {
            readBranchTripping(adder, context);
        } else if(context.getReader().getLocalName() == SWITCH_TRIPPING) {
            readSwitchTripping(adder, context);
        } else if(context.getReader().getLocalName() == THREE_WINDINGS_TRANSFORMER_TRIPPING) {
            readThreeWindingsTransformerTripping(adder, context);
        } else {
            AbstractComplexIdentifiableXml::readSubElements(id, toApply, context);
        }
    });
}
void OverloadManagementSystemXml::skipSubElements(const std::string& id, NetworkXmlReaderContext& context) const {
    std::vector<std::function<void(Identifiable&)>> toApply;
    context.getReader().readUntilEndElement(OVERLOAD_MANAGEMENT_SYSTEM, [this, &id, &toApply, &context]() {
        if(context.getReader().getLocalName() == BRANCH_TRIPPING) {
            skipBranchTripping(context);
        } else if(context.getReader().getLocalName() == SWITCH_TRIPPING) {
            skipSwitchTripping(context);
        } else if(context.getReader().getLocalName() == THREE_WINDINGS_TRANSFORMER_TRIPPING) {
            skipThreeWindingsTransformerTripping(context);
        } else {
            AbstractComplexIdentifiableXml::readSubElements(id, toApply, context);
        }
    });
}

void OverloadManagementSystemXml::readTrippingCommonAttributes(overload_management_system::TrippingAdder& trippingAdder, NetworkXmlReaderContext& context) const {
    std::string key;
    std::string name;
    double currentLimit;
    bool openAction;
    getTrippingCommonAttributes(context, key, name, currentLimit, openAction);

    trippingAdder.setKey(key)
                .setName(name)
                .setCurrentLimit(currentLimit)
                .setOpenAction(openAction);
}
void OverloadManagementSystemXml::skipTrippingCommonAttributes(NetworkXmlReaderContext& context) const {
    std::string key;
    std::string name;
    double currentLimit;
    bool openAction;
    getTrippingCommonAttributes(context, key, name, currentLimit, openAction);
}
void OverloadManagementSystemXml::getTrippingCommonAttributes(NetworkXmlReaderContext& context, std::string& key, std::string& name, double& currentLimit, bool& openAction) const {
    key = context.getReader().getAttributeValue(KEY);
    name = context.getReader().getAttributeValue(NAME);
    currentLimit = context.getReader().getAttributeValue<double>(CURRENT_LIMIT);
    openAction = context.getReader().getAttributeValue<bool>(OPEN_ACTION);
}

void OverloadManagementSystemXml::readSwitchTripping(OverloadManagementSystemAdder& adder, NetworkXmlReaderContext& context) const {
    std::string switchId;
    getSwitchTrippingAttributes(context, switchId);
    auto switchTrippingAdderPtr = adder.newSwitchTripping();
    auto& trippingAdder = switchTrippingAdderPtr->setSwitchToOperateId(switchId);
    readTrippingCommonAttributes(trippingAdder, context);
    trippingAdder.add();
}
void OverloadManagementSystemXml::skipSwitchTripping(NetworkXmlReaderContext& context) const {
    std::string switchId;
    getSwitchTrippingAttributes(context, switchId);
    skipTrippingCommonAttributes(context);
}
void OverloadManagementSystemXml::getSwitchTrippingAttributes(NetworkXmlReaderContext& context, std::string& switchId) const {
    switchId = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(SWITCH_ID));
}

void OverloadManagementSystemXml::readBranchTripping(OverloadManagementSystemAdder& adder, NetworkXmlReaderContext& context) const {
    std::string branchId;
    TwoSides twoSide; 
    getBranchTrippingAttributes(context, branchId, twoSide);
    auto branchTrippingAdderPtr = adder.newBranchTripping();
    auto& trippingAdder = branchTrippingAdderPtr->setBranchToOperateId(branchId)
                                                    .setSideToOperate(twoSide);
    readTrippingCommonAttributes(trippingAdder, context);
    trippingAdder.add();
}
void OverloadManagementSystemXml::skipBranchTripping(NetworkXmlReaderContext& context) const {
    std::string branchId;
    TwoSides twoSide; 
    getBranchTrippingAttributes(context, branchId, twoSide);
    skipTrippingCommonAttributes(context);
}
void OverloadManagementSystemXml::getBranchTrippingAttributes(NetworkXmlReaderContext& context, std::string& branchId, TwoSides& side) const {
    branchId = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(BRANCH_ID));
    const std::string& strSide = context.getReader().getAttributeValue(SIDE);
    side = Enum::fromString<TwoSides>(strSide);
}

void OverloadManagementSystemXml::readThreeWindingsTransformerTripping(OverloadManagementSystemAdder& adder, NetworkXmlReaderContext& context) const {
    std::string twtId;
    ThreeSides threeSide;
    getThreeWindingsTransformerTrippingAttributes(context, twtId, threeSide);
    auto twtTrippingAdderPtr = adder.newThreeWindingsTransformerTripping();
    auto& trippingAdder = twtTrippingAdderPtr->setThreeWindingsTransformerToOperateId(twtId)
                                                                        .setSideToOperate(threeSide);
    readTrippingCommonAttributes(trippingAdder, context);
    trippingAdder.add();
}
void OverloadManagementSystemXml::skipThreeWindingsTransformerTripping(NetworkXmlReaderContext& context) const {
    std::string twtId;
    ThreeSides threeSide;
    getThreeWindingsTransformerTrippingAttributes(context, twtId, threeSide);
    skipTrippingCommonAttributes(context);
}
void OverloadManagementSystemXml::getThreeWindingsTransformerTrippingAttributes(NetworkXmlReaderContext& context, std::string& twtId, ThreeSides& side) const {
    twtId = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(THREE_WINDINGS_TRANSFORMER_ID));
    const std::string& strSide = context.getReader().getAttributeValue(SIDE);
    side = Enum::fromString<ThreeSides>(strSide);
}

bool OverloadManagementSystemXml::postponeElementCreation() const {
    return true;
}

void OverloadManagementSystemXml::skip(NetworkXmlReaderContext& context) const {
    const std::string id = skipIdentifierAttributes(context);
    skipRootElementAttributes(context);
    skipSubElements(id, context);
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

