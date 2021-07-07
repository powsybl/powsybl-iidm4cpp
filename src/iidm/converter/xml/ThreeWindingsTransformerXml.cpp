/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ThreeWindingsTransformerXml.hpp"

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/stdcxx/format.hpp>

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
    ThreeWindingsTransformerAdder::LegAdder legAdder1 = adder.newLeg1()
        .setR(r1)
        .setX(x1)
        .setG(g1)
        .setB(b1)
        .setRatedU(ratedU1);
    ThreeWindingsTransformerAdder::LegAdder legAdder2 = adder.newLeg2()
        .setR(r2)
        .setX(x2)
        .setRatedU(ratedU2);
    ThreeWindingsTransformerAdder::LegAdder legAdder3 = adder.newLeg3()
        .setR(r3)
        .setX(x3)
        .setRatedU(ratedU3);
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_1(), context.getVersion(), [&adder, &legAdder2, &legAdder3, &context]() {
        const auto& ratedU0 = context.getReader().getAttributeValue<double>(RATED_U0);
        adder.setRatedU0(ratedU0);

        const auto& g2 = context.getReader().getAttributeValue<double>(G2);
        const auto& b2 = context.getReader().getAttributeValue<double>(B2);
        legAdder2.setG(g2).setB(b2);

        const auto& g3 = context.getReader().getAttributeValue<double>(G3);
        const auto& b3 = context.getReader().getAttributeValue<double>(B3);
        legAdder3.setG(g3).setB(b3);
    });
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&legAdder1, &legAdder2, &legAdder3, &context]() {
        readRatedS(RATED_S1, context, [&legAdder1](double ratedS) {
            legAdder1.setRatedS(ratedS);
        });
        readRatedS(RATED_S2, context, [&legAdder2](double ratedS) {
            legAdder2.setRatedS(ratedS);
        });
        readRatedS(RATED_S3, context, [&legAdder3](double ratedS) {
            legAdder3.setRatedS(ratedS);
        });
    });
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
            CurrentLimitsAdder<const std::nullptr_t, ThreeWindingsTransformer::Leg> adder = twt.getLeg1().newCurrentLimits();
            readCurrentLimits(adder, context.getReader(), 1);
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS2) {
            CurrentLimitsAdder<const std::nullptr_t, ThreeWindingsTransformer::Leg> adder = twt.getLeg2().newCurrentLimits();
            readCurrentLimits(adder, context.getReader(), 2);
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS3) {
            CurrentLimitsAdder<const std::nullptr_t, ThreeWindingsTransformer::Leg> adder = twt.getLeg3().newCurrentLimits();
            readCurrentLimits(adder, context.getReader(), 3);
        } else if (context.getReader().getLocalName() == RATIO_TAP_CHANGER1) {
            IidmXmlUtil::assertMinimumVersion(THREE_WINDINGS_TRANSFORMER, RATIO_TAP_CHANGER1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), context);
            readRatioTapChanger(1, twt.getLeg1(), context);
        } else if (context.getReader().getLocalName() == PHASE_TAP_CHANGER1) {
            IidmXmlUtil::assertMinimumVersion(THREE_WINDINGS_TRANSFORMER, PHASE_TAP_CHANGER1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), context);
            readPhaseTapChanger(1, twt.getLeg1(), context);
        } else if (context.getReader().getLocalName() == RATIO_TAP_CHANGER2) {
            readRatioTapChanger(2, twt.getLeg2(), context);
        } else if (context.getReader().getLocalName() == PHASE_TAP_CHANGER2) {
            IidmXmlUtil::assertMinimumVersion(THREE_WINDINGS_TRANSFORMER, PHASE_TAP_CHANGER2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), context);
            readPhaseTapChanger(2, twt.getLeg2(), context);
        } else if (context.getReader().getLocalName() == RATIO_TAP_CHANGER3) {
            readRatioTapChanger(3, twt.getLeg3(), context);
        } else if (context.getReader().getLocalName() == PHASE_TAP_CHANGER3) {
            IidmXmlUtil::assertMinimumVersion(THREE_WINDINGS_TRANSFORMER, PHASE_TAP_CHANGER3, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), context);
            readPhaseTapChanger(3, twt.getLeg3(), context);
        } else {
            AbstractTransformerXml::readSubElements(twt, context);
        }
    });
}

void ThreeWindingsTransformerXml::writePhaseTapChanger(const stdcxx::CReference<PhaseTapChanger>& ptc, int index, NetworkXmlWriterContext& context) {
    if (ptc) {
        AbstractTransformerXml<ThreeWindingsTransformer, ThreeWindingsTransformerAdder>::writePhaseTapChanger(toString(PHASE_TAP_CHANGER, index), ptc, context);
    }
}

void ThreeWindingsTransformerXml::writeRatioTapChanger(const stdcxx::CReference<RatioTapChanger>& rtc, int index, NetworkXmlWriterContext& context) {
    if (rtc) {
        AbstractTransformerXml<ThreeWindingsTransformer, ThreeWindingsTransformerAdder>::writeRatioTapChanger(toString(RATIO_TAP_CHANGER, index), rtc, context);
    }
}

void ThreeWindingsTransformerXml::writeRootElementAttributes(const ThreeWindingsTransformer& twt, const Substation& /*substation*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(R1, twt.getLeg1().getR());
    context.getWriter().writeAttribute(X1, twt.getLeg1().getX());
    context.getWriter().writeAttribute(G1, twt.getLeg1().getG());
    context.getWriter().writeAttribute(B1, twt.getLeg1().getB());
    context.getWriter().writeAttribute(RATED_U1, twt.getLeg1().getRatedU());
    writeRatedS(RATED_S1, twt.getLeg1().getRatedS(), context);
    context.getWriter().writeAttribute(R2, twt.getLeg2().getR());
    context.getWriter().writeAttribute(X2, twt.getLeg2().getX());
    IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(THREE_WINDINGS_TRANSFORMER, G2, twt.getLeg2().getG(), 0,
        ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_1(), context);
    IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(THREE_WINDINGS_TRANSFORMER, B2, twt.getLeg2().getB(), 0,
        ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_1(), context);
    context.getWriter().writeAttribute(RATED_U2, twt.getLeg2().getRatedU());
    writeRatedS(RATED_S2, twt.getLeg2().getRatedS(), context);
    context.getWriter().writeAttribute(R3, twt.getLeg3().getR());
    context.getWriter().writeAttribute(X3, twt.getLeg3().getX());
    IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(THREE_WINDINGS_TRANSFORMER, G3, twt.getLeg3().getG(), 0,
        ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_1(), context);
    IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(THREE_WINDINGS_TRANSFORMER, B3, twt.getLeg3().getB(), 0,
        ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_1(), context);
    context.getWriter().writeAttribute(RATED_U3, twt.getLeg3().getRatedU());
    writeRatedS(RATED_S3, twt.getLeg3().getRatedS(), context);
    IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(THREE_WINDINGS_TRANSFORMER, RATED_U0, twt.getRatedU0(), twt.getLeg1().getRatedU(),
        ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_1(), context);
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
    IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(twt.getLeg1().hasRatioTapChanger(), THREE_WINDINGS_TRANSFORMER, RATIO_TAP_CHANGER1, ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_1(), context, [&twt, &context]() {
        writeRatioTapChanger(twt.getLeg1().getOptionalRatioTapChanger(), 1, context);
    });
    IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(twt.getLeg1().hasPhaseTapChanger(), THREE_WINDINGS_TRANSFORMER, PHASE_TAP_CHANGER1, ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_1(), context, [&twt, &context]() {
        writePhaseTapChanger(twt.getLeg1().getOptionalPhaseTapChanger(), 1, context);
    });
    writeRatioTapChanger(twt.getLeg2().getOptionalRatioTapChanger(), 2, context);
    IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(twt.getLeg2().hasPhaseTapChanger(), THREE_WINDINGS_TRANSFORMER, PHASE_TAP_CHANGER2, ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_1(), context, [&twt, &context]() {
        writePhaseTapChanger(twt.getLeg2().getOptionalPhaseTapChanger(), 2, context);
    });
    writeRatioTapChanger(twt.getLeg3().getOptionalRatioTapChanger(), 3, context);
    IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(twt.getLeg3().hasPhaseTapChanger(), THREE_WINDINGS_TRANSFORMER, PHASE_TAP_CHANGER3, ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_1(), context, [&twt, &context]() {
        writePhaseTapChanger(twt.getLeg3().getOptionalPhaseTapChanger(), 3, context);
    });

    if (twt.getLeg1().getCurrentLimits()) {
        writeCurrentLimits(twt.getLeg1().getCurrentLimits(), context.getWriter(), context.getVersion(), 1);
    }
    if (twt.getLeg2().getCurrentLimits()) {
        writeCurrentLimits(twt.getLeg2().getCurrentLimits(), context.getWriter(), context.getVersion(), 2);
    }
    if (twt.getLeg3().getCurrentLimits()) {
        writeCurrentLimits(twt.getLeg3().getCurrentLimits(), context.getWriter(), context.getVersion(), 3);
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
