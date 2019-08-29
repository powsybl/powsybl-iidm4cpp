/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "AbstractTransformerXml.hpp"

#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>

#include "TerminalRefXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename T, typename A>
void AbstractTransformerXml<T,A>::readPhaseTapChanger(TwoWindingsTransformer& twt, NetworkXmlReaderContext& context) {
    const auto& lowTapPosition = context.getReader().getAttributeValue<long>(LOW_TAP_POSITION);
    const auto& tapPosition = context.getReader().getAttributeValue<long>(TAP_POSITION);
    const double& targetDeadband = context.getReader().getOptionalAttributeValue(TARGET_DEADBAND, stdcxx::nan());
    PhaseTapChanger::RegulationMode regulationMode = getRegulationMode(context.getReader().getAttributeValue(REGULATION_MODE));
    const double& regulationValue = context.getReader().getOptionalAttributeValue(REGULATION_VALUE, stdcxx::nan());
    bool regulating = context.getReader().getOptionalAttributeValue(REGULATING, false);
    PhaseTapChangerAdder adder = twt.newPhaseTapChanger()
        .setLowTapPosition(lowTapPosition)
        .setTapPosition(tapPosition)
        .setTargetDeadband(targetDeadband)
        .setRegulationMode(regulationMode)
        .setRegulationValue(regulationValue)
        .setRegulating(regulating);
    bool hasTerminalRef = false;
    context.getReader().readUntilEndElement(PHASE_TAP_CHANGER, [&adder, &context, &hasTerminalRef, &twt]() {
        if (context.getReader().getLocalName() == TERMINAL_REF) {
            const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));
            const std::string& side = context.getReader().getOptionalAttributeValue(SIDE, "");
            context.addEndTask([&adder, &twt, id, side]() {
                adder.setRegulationTerminal(stdcxx::ref(TerminalRefXml::readTerminalRef(twt.getTerminal1().getVoltageLevel().getSubstation().getNetwork(), id, side)));
            });
            hasTerminalRef = true;
        } else if (context.getReader().getLocalName() == STEP) {
            const auto& r = context.getReader().getAttributeValue<double>(R);
            const auto& x = context.getReader().getAttributeValue<double>(X);
            const auto& g = context.getReader().getAttributeValue<double>(G);
            const auto& b = context.getReader().getAttributeValue<double>(B);
            const auto& rho = context.getReader().getAttributeValue<double>(RHO);
            const auto& alpha = context.getReader().getAttributeValue<double>(ALPHA);
            adder.beginStep()
                .setR(r)
                .setX(x)
                .setG(g)
                .setB(b)
                .setRho(rho)
                .setAlpha(alpha)
                .endStep();
        } else {
            throw PowsyblException(logging::format("Unknown element <%1%>", context.getReader().getLocalName()));
        }
    });
    if (!hasTerminalRef) {
        adder.add();
    }
}

template <typename T, typename A>
void AbstractTransformerXml<T,A>::writePhaseTapChanger(const std::string& name, const PhaseTapChanger& ptc, NetworkXmlWriterContext& context) {
    context.getWriter().writeStartElement(IIDM_PREFIX, name);
    writeTapChanger<TwoWindingsTransformer, PhaseTapChanger, PhaseTapChangerStep>(ptc, context.getWriter());
    context.getWriter().writeAttribute(REGULATION_MODE, getRegulationModeName(ptc.getRegulationMode()));
    if (ptc.getRegulationMode() != PhaseTapChanger::RegulationMode::FIXED_TAP || !std::isnan(ptc.getRegulationValue())) {
        context.getWriter().writeAttribute(REGULATION_VALUE, ptc.getRegulationValue());
    }
    if (ptc.getRegulationMode() != PhaseTapChanger::RegulationMode::FIXED_TAP || ptc.isRegulating()) {
        context.getWriter().writeAttribute(REGULATING, ptc.isRegulating());
    }
    if (ptc.getRegulationTerminal()) {
        TerminalRefXml::writeTerminalRef(ptc.getRegulationTerminal(), context, TERMINAL_REF);
    }
    for (long p = ptc.getLowTapPosition(); p <= ptc.getHighTapPosition(); ++p) {
        const PhaseTapChangerStep& ptcs = ptc.getStep(p);
        context.getWriter().writeStartElement(IIDM_PREFIX, STEP);
        writeTapChangerStep(ptcs, context.getWriter());
        context.getWriter().writeOptionalAttribute(ALPHA, ptcs.getAlpha());
        context.getWriter().writeEndElement();
    }
    context.getWriter().writeEndElement();
}

template <typename T, typename A>
template <typename H, typename C, typename S>
void AbstractTransformerXml<T,A>::writeTapChanger(const TapChanger<H, C, S>& tc, powsybl::xml::XmlStreamWriter& writer) {
    writer.writeAttribute(LOW_TAP_POSITION, tc.getLowTapPosition());
    writer.writeAttribute(TAP_POSITION, tc.getTapPosition());
    writer.writeOptionalAttribute(TARGET_DEADBAND, tc.getTargetDeadband());
}

template <typename T, typename A>
template <typename S>
void AbstractTransformerXml<T,A>::writeTapChangerStep(const TapChangerStep<S>& tcs, powsybl::xml::XmlStreamWriter& writer) {
    writer.writeOptionalAttribute(R, tcs.getR());
    writer.writeOptionalAttribute(X, tcs.getX());
    writer.writeOptionalAttribute(G, tcs.getG());
    writer.writeOptionalAttribute(B, tcs.getB());
    writer.writeOptionalAttribute(RHO, tcs.getRho());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
