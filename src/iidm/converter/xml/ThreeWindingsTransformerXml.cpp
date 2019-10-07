/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ThreeWindingsTransformerXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

ThreeWindingsTransformerAdder ThreeWindingsTransformerXml::createAdder(Substation& substation) const {
    return substation.newThreeWindingsTransformer();
}

const ThreeWindingsTransformerXml& ThreeWindingsTransformerXml::getInstance() {
    static ThreeWindingsTransformerXml s_instance;
    return s_instance;
}

const char* ThreeWindingsTransformerXml::getRootElementName() const {
    return THREE_WINDINGS_TRANSFORMER;
}

ThreeWindingsTransformer& ThreeWindingsTransformerXml::readRootElementAttributes(ThreeWindingsTransformerAdder& adder, NetworkXmlReaderContext& context) const {
    const auto& r1 = context.getReader().getAttributeValue<double>(R1);
    const auto& x1 = context.getReader().getAttributeValue<double>(X1);
    const auto& g1 = context.getReader().getAttributeValue<double>(G1);
    const auto& b1 = context.getReader().getAttributeValue<double>(B1);
    const auto& ratedU1 = context.getReader().getAttributeValue<double>(RATED_U1);
    const auto& r2 = context.getReader().getAttributeValue<double>(R2);
    const auto& x2 = context.getReader().getAttributeValue<double>(X2);
    const auto& ratedU2 = context.getReader().getAttributeValue<double>(RATED_U2);
    const auto& r3 = context.getReader().getAttributeValue<double>(R3);
    const auto& x3 = context.getReader().getAttributeValue<double>(X3);
    const auto& ratedU3 = context.getReader().getAttributeValue<double>(RATED_U3);
    ThreeWindingsTransformerAdder::Leg1Adder& legAdder1 = adder.newLeg1()
        .setR(r1)
        .setX(x1)
        .setG(g1)
        .setB(b1)
        .setRatedU(ratedU1);
    ThreeWindingsTransformerAdder::Leg2Adder& legAdder2 = adder.newLeg2()
        .setR(r2)
        .setX(x2)
        .setRatedU(ratedU2);
    ThreeWindingsTransformerAdder::Leg3Adder& legAdder3 = adder.newLeg3()
        .setR(r3)
        .setX(x3)
        .setRatedU(ratedU3);
    readNodeOrBus(1, legAdder1, context);
    readNodeOrBus(2, legAdder2, context);
    readNodeOrBus(3, legAdder3, context);
    legAdder1.add();
    legAdder2.add();
    legAdder3.add();
    ThreeWindingsTransformer& twt = adder.add();
    readPQ(twt.getLeg1().getTerminal(), context.getReader(), 1);
    readPQ(twt.getLeg2().getTerminal(), context.getReader(), 2);
    readPQ(twt.getLeg3().getTerminal(), context.getReader(), 3);
    return twt;
}

void ThreeWindingsTransformerXml::readSubElements(ThreeWindingsTransformer& twt, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(THREE_WINDINGS_TRANSFORMER, [this, &twt, &context]() {
        if (context.getReader().getLocalName() == CURRENT_LIMITS1) {
            CurrentLimitsAdder<const std::nullptr_t, ThreeWindingsTransformer::LegBase<ThreeWindingsTransformer::Leg1> > adder = twt.getLeg1().newCurrentLimits();
            readCurrentLimits(adder, context.getReader(), 1);
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS2) {
            CurrentLimitsAdder<const std::nullptr_t, ThreeWindingsTransformer::LegBase<ThreeWindingsTransformer::Leg2or3> > adder = twt.getLeg2().newCurrentLimits();
            readCurrentLimits(adder, context.getReader(), 2);
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS3) {
            CurrentLimitsAdder<const std::nullptr_t, ThreeWindingsTransformer::LegBase<ThreeWindingsTransformer::Leg2or3> > adder = twt.getLeg3().newCurrentLimits();
            readCurrentLimits(adder, context.getReader(), 3);
        } else if (context.getReader().getLocalName() == RATIO_TAP_CHANGER2) {
            readRatioTapChanger(2, twt.getLeg2(), context);
        } else if (context.getReader().getLocalName() == RATIO_TAP_CHANGER3) {
            readRatioTapChanger(3, twt.getLeg3(), context);
        } else {
            AbstractIdentifiableXml::readSubElements(twt, context);
        }
    });
}

void ThreeWindingsTransformerXml::writeRootElementAttributes(const ThreeWindingsTransformer& twt, const Substation& /*substation*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(R1, twt.getLeg1().getR());
    context.getWriter().writeAttribute(X1, twt.getLeg1().getX());
    context.getWriter().writeAttribute(G1, twt.getLeg1().getG());
    context.getWriter().writeAttribute(B1, twt.getLeg1().getB());
    context.getWriter().writeAttribute(RATED_U1, twt.getLeg1().getRatedU());
    context.getWriter().writeAttribute(R2, twt.getLeg2().getR());
    context.getWriter().writeAttribute(X2, twt.getLeg2().getX());
    context.getWriter().writeAttribute(RATED_U2, twt.getLeg2().getRatedU());
    context.getWriter().writeAttribute(R3, twt.getLeg3().getR());
    context.getWriter().writeAttribute(X3, twt.getLeg3().getX());
    context.getWriter().writeAttribute(RATED_U3, twt.getLeg3().getRatedU());
    writeNodeOrBus(twt.getLeg1().getTerminal(), context, 1);
    writeNodeOrBus(twt.getLeg2().getTerminal(), context, 2);
    writeNodeOrBus(twt.getLeg3().getTerminal(), context, 3);
    if (context.getOptions().isWithBranchSV()) {
        writePQ(twt.getLeg1().getTerminal(), context.getWriter(), 1);
        writePQ(twt.getLeg2().getTerminal(), context.getWriter(), 2);
        writePQ(twt.getLeg3().getTerminal(), context.getWriter(), 3);
    }
}

void ThreeWindingsTransformerXml::writeSubElements(const ThreeWindingsTransformer& twt, const Substation& /*substation*/, NetworkXmlWriterContext& context) const {
    const stdcxx::CReference<RatioTapChanger>& rtc2 = twt.getLeg2().getRatioTapChanger();
    if (rtc2) {
        writeRatioTapChanger(RATIO_TAP_CHANGER2, rtc2, context);
    }
    const stdcxx::CReference<RatioTapChanger>& rtc3 = twt.getLeg3().getRatioTapChanger();
    if (rtc3) {
        writeRatioTapChanger(RATIO_TAP_CHANGER3, rtc3, context);
    }
    if (twt.getLeg1().getCurrentLimits()) {
        writeCurrentLimits(twt.getLeg1().getCurrentLimits(), context.getWriter(), 1);
    }
    if (twt.getLeg2().getCurrentLimits()) {
        writeCurrentLimits(twt.getLeg2().getCurrentLimits(), context.getWriter(), 2);
    }
    if (twt.getLeg3().getCurrentLimits()) {
        writeCurrentLimits(twt.getLeg3().getCurrentLimits(), context.getWriter(), 3);
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

