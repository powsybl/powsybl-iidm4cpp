/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "TwoWindingsTransformerXml.hpp"

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

TwoWindingsTransformer& TwoWindingsTransformerXml::readRootElementAttributes(TwoWindingsTransformerAdder& adder, NetworkXmlReaderContext& context) const {
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
    return twt;
}

void TwoWindingsTransformerXml::readSubElements(TwoWindingsTransformer& twt, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(TWO_WINDINGS_TRANSFORMER, [this, &twt, &context]() {
        if (context.getReader().getLocalName() == CURRENT_LIMITS1) {
            CurrentLimitsAdder<Branch::Side, Branch> adder = twt.newCurrentLimits1();
            readCurrentLimits(adder, context.getReader(), 1);
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS2) {
            CurrentLimitsAdder<Branch::Side, Branch> adder = twt.newCurrentLimits2();
            readCurrentLimits(adder, context.getReader(), 2);
        } else if (context.getReader().getLocalName() == RATIO_TAP_CHANGER) {
            readRatioTapChanger(twt, context);
        } else if (context.getReader().getLocalName() == PHASE_TAP_CHANGER) {
            readPhaseTapChanger(twt, context);
        } else {
            AbstractIdentifiableXml::readSubElements(twt, context);
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
}

void TwoWindingsTransformerXml::writeSubElements(const TwoWindingsTransformer& twt, const Substation& /*substation*/, NetworkXmlWriterContext& context) const {
    const auto& rtc = twt.getRatioTapChanger();
    if (rtc) {
        writeRatioTapChanger(RATIO_TAP_CHANGER, rtc, context);
    }
    const auto& ptc = twt.getPhaseTapChanger();
    if (ptc) {
        writePhaseTapChanger(PHASE_TAP_CHANGER, ptc, context);
    }
    if (twt.getCurrentLimits1()) {
        writeCurrentLimits(twt.getCurrentLimits1(), context.getWriter(), context.getVersion(), 1);
    }
    if (twt.getCurrentLimits2()) {
        writeCurrentLimits(twt.getCurrentLimits2(), context.getWriter(), context.getVersion(), 2);
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

