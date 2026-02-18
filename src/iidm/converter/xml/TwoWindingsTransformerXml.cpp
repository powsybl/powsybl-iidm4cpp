/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "TwoWindingsTransformerXml.hpp"

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

TwoWindingsTransformerAdder TwoWindingsTransformerXml::createAdder(Substation& substation) const {
    return substation.newTwoWindingsTransformer();
}

const TwoWindingsTransformerXml& TwoWindingsTransformerXml::getInstance() {
    static TwoWindingsTransformerXml s_instance;
    return s_instance;
}

const char* TwoWindingsTransformerXml::getRootElementName() const {
    return TWO_WINDINGS_TRANSFORMER;
}

TwoWindingsTransformer& TwoWindingsTransformerXml::readRootElementAttributes(TwoWindingsTransformerAdder& adder, Substation& /*substation*/, NetworkXmlReaderContext& context) const {
    const auto& r = context.getReader().getAttributeValue<double>(R);
    const auto& x = context.getReader().getAttributeValue<double>(X);
    const auto& g = context.getReader().getAttributeValue<double>(G);
    const auto& b = context.getReader().getAttributeValue<double>(B);
    const auto& ratedU1 = context.getReader().getAttributeValue<double>(RATED_U1);
    const auto& ratedU2 = context.getReader().getAttributeValue<double>(RATED_U2);
    adder.setR(r)
        .setX(x)
        .setG(g)
        .setB(b)
        .setRatedU1(ratedU1)
        .setRatedU2(ratedU2);
    readRatedS("ratedS", context, [&adder](double ratedS) {
        adder.setRatedS(ratedS);
    });
    readNodeOrBus(adder, context);
    TwoWindingsTransformer& twt = adder.add();
    readPQ(twt.getTerminal1(), context.getReader(), 1);
    readPQ(twt.getTerminal2(), context.getReader(), 2);

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_12(), context.getVersion(), [&context, &twt](){
        readSelectedGroupId(context, [&twt](const std::string& selectedId) {
            twt.setSelectedOperationalLimitsGroup1(selectedId);
        }, 1);
        readSelectedGroupId(context, [&twt](const std::string& selectedId) {
            twt.setSelectedOperationalLimitsGroup2(selectedId);
        }, 2);
    });

    return twt;
}

void TwoWindingsTransformerXml::readSubElements(TwoWindingsTransformer& twt, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(TWO_WINDINGS_TRANSFORMER, [this, &twt, &context]() {
        if (context.getReader().getLocalName() == LIMITS_GROUP_1) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), LIMITS_GROUP_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_12(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_12(), context.getVersion(), [&context, &twt]() {
                readLoadingLimitsGroup(context, LIMITS_GROUP_1, [&twt](const std::string& id){
                    return stdcxx::ref(twt.newOperationalLimitsGroup1(id));
                });
            });
        } else if (context.getReader().getLocalName() == ACTIVE_POWER_LIMITS_1) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), ACTIVE_POWER_LIMITS_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &twt]() { readActivePowerLimits(twt.getOrCreateSelectedOperationalLimitsGroup1().newActivePowerLimits(), context, 1); });
        } else if (context.getReader().getLocalName() == APPARENT_POWER_LIMITS_1) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), APPARENT_POWER_LIMITS_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &twt]() { readApparentPowerLimits(twt.getOrCreateSelectedOperationalLimitsGroup1().newApparentPowerLimits(), context, 1); });
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS1) {
            readCurrentLimits(twt.getOrCreateSelectedOperationalLimitsGroup1().newCurrentLimits(), context, 1);
        } else if(context.getReader().getLocalName() == LIMITS_GROUP_2) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), LIMITS_GROUP_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_12(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_12(), context.getVersion(), [&context, &twt]() {
                readLoadingLimitsGroup(context, LIMITS_GROUP_2, [&twt](const std::string& id){
                    return stdcxx::ref(twt.newOperationalLimitsGroup2(id));
                });
            });
        } else if (context.getReader().getLocalName() == ACTIVE_POWER_LIMITS_2) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), ACTIVE_POWER_LIMITS_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &twt]() { readActivePowerLimits(twt.getOrCreateSelectedOperationalLimitsGroup2().newActivePowerLimits(), context, 2);});
        } else if (context.getReader().getLocalName() == APPARENT_POWER_LIMITS_2) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), APPARENT_POWER_LIMITS_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &twt]() { readApparentPowerLimits(twt.getOrCreateSelectedOperationalLimitsGroup2().newApparentPowerLimits(), context, 2); });
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS2) {
            readCurrentLimits(twt.getOrCreateSelectedOperationalLimitsGroup2().newCurrentLimits(), context, 2);
        } else if (context.getReader().getLocalName() == RATIO_TAP_CHANGER) {
            readRatioTapChanger(twt, context);
        } else if (context.getReader().getLocalName() == PHASE_TAP_CHANGER) {
            readPhaseTapChanger(twt, context);
        } else {
            AbstractTransformerXml::readSubElements(twt, context);
        }
    });
}

void TwoWindingsTransformerXml::writeRootElementAttributes(const TwoWindingsTransformer& twt, const Substation& /*substation*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(R, twt.getR());
    context.getWriter().writeAttribute(X, twt.getX());
    context.getWriter().writeAttribute(G, twt.getG());
    context.getWriter().writeAttribute(B, twt.getB());
    context.getWriter().writeAttribute(RATED_U1, twt.getRatedU1());
    context.getWriter().writeAttribute(RATED_U2, twt.getRatedU2());
    writeRatedS(RATED_S, twt.getRatedS(), context);
    writeNodeOrBus(twt.getTerminal1(), context, 1);
    writeNodeOrBus(twt.getTerminal2(), context, 2);
    if (context.getOptions().isWithBranchSV()) {
        writePQ(twt.getTerminal1(), context.getWriter(), 1);
        writePQ(twt.getTerminal2(), context.getWriter(), 2);
    }
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_12(), context.getVersion(),[&twt, &context](){
        writeSelectedGroupId(twt.getSelectedOperationalLimitsGroupId1(), context, 1);
        writeSelectedGroupId(twt.getSelectedOperationalLimitsGroupId2(), context, 2);
    });
}

void TwoWindingsTransformerXml::writeSubElements(const TwoWindingsTransformer& twt, const Substation& /*substation*/, NetworkXmlWriterContext& context) const {
    if (twt.hasRatioTapChanger()) {
        writeRatioTapChanger(RATIO_TAP_CHANGER, twt.getRatioTapChanger(), context);
    }
    if (twt.hasPhaseTapChanger()) {
        writePhaseTapChanger(PHASE_TAP_CHANGER, twt.getPhaseTapChanger(), context);
    }
    writeLimits(context, getRootElementName(), twt.getSelectedOperationalLimitsGroup1(), twt.getOperationalLimitsGroups1(), 1);
    writeLimits(context, getRootElementName(), twt.getSelectedOperationalLimitsGroup2(), twt.getOperationalLimitsGroups2(), 2);
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

