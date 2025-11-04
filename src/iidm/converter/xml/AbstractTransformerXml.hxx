/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTTRANSFORMERXML_HXX
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTTRANSFORMERXML_HXX

#include "AbstractTransformerXml.hpp"

#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename Added, typename Adder>
void AbstractTransformerXml<Added, Adder>::readPhaseTapChanger(TwoWindingsTransformer& twt, NetworkXmlReaderContext& context) {
    std::shared_ptr<PhaseTapChangerAdder> adder = std::make_shared<PhaseTapChangerAdder>(twt.newPhaseTapChanger());
    readPhaseTapChanger(PHASE_TAP_CHANGER, adder, twt.getTerminal1(), context);
}

template <typename Added, typename Adder>
void AbstractTransformerXml<Added, Adder>::readPhaseTapChanger(int leg, ThreeWindingsTransformer::Leg& twl, NetworkXmlReaderContext& context) {
    std::shared_ptr<PhaseTapChangerAdder> adder = std::make_shared<PhaseTapChangerAdder>(twl.newPhaseTapChanger());
    readPhaseTapChanger(toString(PHASE_TAP_CHANGER, leg), adder, twl.getTerminal(), context);
}

template <typename Added, typename Adder>
void AbstractTransformerXml<Added, Adder>::readPhaseTapChanger(const std::string& elementName, std::shared_ptr<PhaseTapChangerAdder>& adder, Terminal& terminal, NetworkXmlReaderContext& context) {
    auto tapChangerAdder = std::dynamic_pointer_cast<TapChangerAdder<PhaseTapChanger, PhaseTapChangerAdder, PhaseTapChangerStepAdder, PhaseTapChangerHolder>>(adder);
    readTapChangerAttributes<PhaseTapChanger, PhaseTapChangerAdder, PhaseTapChangerStepAdder, PhaseTapChangerHolder>(context, tapChangerAdder);
    const double& regulationValue = context.getReader().getOptionalAttributeValue(REGULATION_VALUE, stdcxx::nan());
    adder->setRegulationValue(regulationValue);
    const auto& regModeStr = context.getReader().getOptionalAttributeValue<std::string>(REGULATION_MODE);
    if(regModeStr.has_value()) {
        adder->setRegulationMode(Enum::fromString<PhaseTapChanger::RegulationMode>(*regModeStr));
    }
    bool hasTerminalRef = false;
    context.getReader().readUntilEndElement(elementName, [&adder, &tapChangerAdder, &context, &hasTerminalRef, &terminal]() {
        if (context.getReader().getLocalName() == TERMINAL_REF) {
            hasTerminalRef = true;
            readTapChangerTerminalRef<PhaseTapChanger, PhaseTapChangerAdder, PhaseTapChangerStepAdder, PhaseTapChangerHolder>(context, tapChangerAdder, terminal);
        } else if (context.getReader().getLocalName() == STEP) {
            PhaseTapChangerStepAdder stepAdder = adder->beginStep();
            readSteps(context, stepAdder);
            const auto& alpha = context.getReader().getAttributeValue<double>(ALPHA);
            stepAdder.setAlpha(alpha)
                .endStep();
        } else {
            throw PowsyblException(stdcxx::format("Unknown element <%1%>", context.getReader().getLocalName()));
        }
    });
    if (!hasTerminalRef) {
        adder->add();
    }
}

template <typename Added, typename Adder>
template <typename Consumer>
void AbstractTransformerXml<Added, Adder>::readRatedS(const std::string& name, NetworkXmlReaderContext& context, const Consumer& consumer) {
    const double& ratedS = context.getReader().getOptionalAttributeValue(name, stdcxx::nan());
    consumer(ratedS);
}

template <typename Added, typename Adder>
void AbstractTransformerXml<Added, Adder>::readRatioTapChanger(TwoWindingsTransformer& twt, NetworkXmlReaderContext& context) {
    std::shared_ptr<RatioTapChangerAdder> adder = std::make_shared<RatioTapChangerAdder>(twt.newRatioTapChanger());
    readRatioTapChanger(RATIO_TAP_CHANGER, adder, twt.getTerminal1(), context);
}

template <typename Added, typename Adder>
void AbstractTransformerXml<Added, Adder>::readRatioTapChanger(int leg, ThreeWindingsTransformer::Leg& twl, NetworkXmlReaderContext& context) {
    std::shared_ptr<RatioTapChangerAdder> adder = std::make_shared<RatioTapChangerAdder>(twl.newRatioTapChanger());
    readRatioTapChanger(toString(RATIO_TAP_CHANGER, leg), adder, twl.getTerminal(), context);
}

template <typename Added, typename Adder>
void AbstractTransformerXml<Added, Adder>::readRatioTapChanger(const std::string& elementName, std::shared_ptr<RatioTapChangerAdder>& adder, Terminal& terminal, NetworkXmlReaderContext& context) {
    auto tapChangerAdder = std::dynamic_pointer_cast<TapChangerAdder<RatioTapChanger, RatioTapChangerAdder, RatioTapChangerStepAdder, RatioTapChangerHolder>>(adder);
    readTapChangerAttributes<RatioTapChanger, RatioTapChangerAdder, RatioTapChangerStepAdder, RatioTapChangerHolder>(context, tapChangerAdder);
    const auto& loadTapChangingCapabilities = context.getReader().getAttributeValue<bool>(LOAD_TAP_CHANGING_CAPABILITIES);
    adder->setLoadTapChangingCapabilities(loadTapChangingCapabilities);

    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_11(), context.getVersion(), [&context, &adder]() {
        double targetV = context.getReader().getOptionalAttributeValue(TARGET_V, stdcxx::nan());
        adder->setRegulationValue(targetV);
    });
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_12(), context.getVersion(), [&context, &adder]() {
        const double& regulationValue = context.getReader().getOptionalAttributeValue(REGULATION_VALUE, stdcxx::nan());
        const auto& regModeStr = context.getReader().getOptionalAttributeValue<std::string>(REGULATION_MODE);
        if(regModeStr.has_value()) {
            adder->setRegulationMode(Enum::fromString<RatioTapChanger::RegulationMode>(*regModeStr));
        }
        adder->setRegulationValue(regulationValue);
    });

    bool hasTerminalRef = false;
    context.getReader().readUntilEndElement(elementName, [&adder, &tapChangerAdder, &context, &terminal, &hasTerminalRef]() {
        if (context.getReader().getLocalName() == TERMINAL_REF) {
            hasTerminalRef = true;
            readTapChangerTerminalRef<RatioTapChanger, RatioTapChangerAdder, RatioTapChangerStepAdder, RatioTapChangerHolder>(context, tapChangerAdder, terminal);
        } else if (context.getReader().getLocalName() == STEP) {
            RatioTapChangerStepAdder stepAdder = adder->beginStep();
            readSteps(context, stepAdder);
            stepAdder.endStep();
        } else {
            throw PowsyblException(stdcxx::format("Unexpected XML element <%1%>", context.getReader().getLocalName()));
        }
    });
    if (!hasTerminalRef) {
        adder->add();
    }
}

template <typename Added, typename Adder>
template <typename TCStepAdder, typename TCAdder>
void AbstractTransformerXml<Added, Adder>::readSteps(const NetworkXmlReaderContext& context, TapChangerStepAdder<TCStepAdder, TCAdder>& tapChangerStepAdder) {
    const auto& r = context.getReader().getAttributeValue<double>(R);
    const auto& x = context.getReader().getAttributeValue<double>(X);
    const auto& g = context.getReader().getAttributeValue<double>(G);
    const auto& b = context.getReader().getAttributeValue<double>(B);
    const auto& rho = context.getReader().getAttributeValue<double>(RHO);
    tapChangerStepAdder.setR(r).setX(x).setG(g).setB(b).setRho(rho);
}

template <typename Added, typename Adder>
double AbstractTransformerXml<Added, Adder>::readTargetDeadband(NetworkXmlReaderContext& context) {
    double targetDeadband = stdcxx::nan();
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_1(), context.getVersion(), [&context, &targetDeadband]() {
        targetDeadband = context.getReader().getOptionalAttributeValue(TARGET_DEADBAND, stdcxx::nan());
        bool regulating = context.getReader().getOptionalAttributeValue(REGULATING, false);
        // in IIDM-XML version 1.0, NaN as targetDeadband when regulating is allowed.
        // in IIDM-XML version 1.1 and more recent, it is forbidden and throws an exception
        // to prevent issues, targetDeadband is set to 0 in this case
        if (regulating && std::isnan(targetDeadband)) {
            targetDeadband = 0.0;
        }
    });
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, &targetDeadband]() {
        targetDeadband = context.getReader().getOptionalAttributeValue(TARGET_DEADBAND, stdcxx::nan());
    });
    return targetDeadband;
}

template <typename Added, typename Adder>
template <typename TC, typename TCAdder, typename TCStepAdder, typename TCHolder>
void AbstractTransformerXml<Added, Adder>::readTapChangerTerminalRef(NetworkXmlReaderContext& context, std::shared_ptr<TapChangerAdder<TC, TCAdder, TCStepAdder, TCHolder>>& tapChangerAdder, Terminal& terminal) {
    Network& n = terminal.getVoltageLevel().getNetwork();
    TerminalRefXml::readTerminalRef(n, context, [tapChangerAdder](Terminal& regulationTerminal) {
                tapChangerAdder->setRegulationTerminal(stdcxx::ref<Terminal>(regulationTerminal));
                tapChangerAdder->add();
    });
}

template <typename Added, typename Adder>
template <typename TC, typename TCAdder, typename TCStepAdder, typename TCHolder>
void AbstractTransformerXml<Added, Adder>::readTapChangerAttributes(NetworkXmlReaderContext& context, std::shared_ptr<TapChangerAdder<TC, TCAdder, TCStepAdder, TCHolder>>& tapChangerAdder) {
    const auto& regulating = context.getReader().getOptionalAttributeValue<bool>(REGULATING);
    const auto& lowTapPosition = context.getReader().getAttributeValue<long>(LOW_TAP_POSITION);
    const auto& tapPosition = context.getReader().getOptionalAttributeValue<long>(TAP_POSITION);
    const double& targetDeadband = readTargetDeadband(context);
    
    tapChangerAdder->setLowTapPosition(lowTapPosition)
        .setTargetDeadband(targetDeadband);
    
    if(tapPosition.has_value()) {
        tapChangerAdder->setTapPosition(*tapPosition);
    }
    if(regulating.has_value()) {
        tapChangerAdder->setRegulating(*regulating);
    }
}

template <typename Added, typename Adder>
void AbstractTransformerXml<Added, Adder>::writePhaseTapChanger(const std::string& name, const PhaseTapChanger& ptc, NetworkXmlWriterContext& context) {
    context.getWriter().writeStartElement(context.getVersion().getPrefix(), name);
    writeTapChanger<PhaseTapChangerHolder, PhaseTapChanger, PhaseTapChangerStep>(ptc, context);
    context.getWriter().writeAttribute(REGULATION_MODE, Enum::toString(ptc.getRegulationMode()));
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
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), STEP);
        writeTapChangerStep(ptcs, context.getWriter());
        context.getWriter().writeAttribute(ALPHA, ptcs.getAlpha());
        context.getWriter().writeEndElement();
    }
    context.getWriter().writeEndElement();
}

template <typename Added, typename Adder>
void AbstractTransformerXml<Added, Adder>::writeRatedS(const std::string& name, double ratedS, NetworkXmlWriterContext& context) {
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, &name, &ratedS]() {
        context.getWriter().writeOptionalAttribute(name, ratedS);
    });
}

template <typename Added, typename Adder>
void AbstractTransformerXml<Added, Adder>::writeRatioTapChanger(const std::string& name, const RatioTapChanger& rtc, NetworkXmlWriterContext& context) {
    context.getWriter().writeStartElement(context.getVersion().getPrefix(), name);
    writeTapChanger<RatioTapChangerHolder, RatioTapChanger, RatioTapChangerStep>(rtc, context);
    context.getWriter().writeAttribute(LOAD_TAP_CHANGING_CAPABILITIES, rtc.hasLoadTapChangingCapabilities());
    if (rtc.hasLoadTapChangingCapabilities() || rtc.isRegulating()) {
        context.getWriter().writeAttribute(REGULATING, rtc.isRegulating());
    }

    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_11(), context.getVersion(), [&context, &rtc]() {
        context.getWriter().writeAttribute(TARGET_V, rtc.getTargetV());
    });
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_12(), context.getVersion(), [&context, &rtc]() {
        if(!std::isnan(rtc.getRegulationValue())) {
            context.getWriter().writeAttribute(REGULATION_MODE, Enum::toString(rtc.getRegulationMode()));
            context.getWriter().writeAttribute(REGULATION_VALUE, rtc.getRegulationValue());
        }
    });

    if (rtc.getRegulationTerminal()) {
        TerminalRefXml::writeTerminalRef(rtc.getRegulationTerminal(), context, TERMINAL_REF);
    }
    for (long p = rtc.getLowTapPosition(); p <= rtc.getHighTapPosition(); ++p) {
        const RatioTapChangerStep& rtcs = rtc.getStep(p);
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), STEP);
        writeTapChangerStep(rtcs, context.getWriter());
        context.getWriter().writeEndElement();
    }
    context.getWriter().writeEndElement();
}

template <typename Added, typename Adder>
template <typename H, typename C, typename S>
void AbstractTransformerXml<Added, Adder>::writeTapChanger(const TapChanger<H, C, S>& tc, NetworkXmlWriterContext& context) {
    context.getWriter().writeAttribute(LOW_TAP_POSITION, tc.getLowTapPosition());
    context.getWriter().writeAttribute(TAP_POSITION, tc.getTapPosition());
    writeTargetDeadband(tc.getTargetDeadband(), context);
}

template <typename Added, typename Adder>
template <typename S>
void AbstractTransformerXml<Added, Adder>::writeTapChangerStep(const TapChangerStep<S>& tcs, powsybl::xml::XmlStreamWriter& writer) {
    writer.writeOptionalAttribute(R, tcs.getR());
    writer.writeOptionalAttribute(X, tcs.getX());
    writer.writeOptionalAttribute(G, tcs.getG());
    writer.writeOptionalAttribute(B, tcs.getB());
    writer.writeOptionalAttribute(RHO, tcs.getRho());
}

template <typename Added, typename Adder>
void AbstractTransformerXml<Added, Adder>::writeTargetDeadband(double targetDeadband, NetworkXmlWriterContext& context) {
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_1(), context.getVersion(), [&context, targetDeadband]() {
        // in IIDM-XML version 1.0, 0 as targetDeadband is ignored for backwards compatibility
        // (i.e. ensuring round trips in IIDM-XML version 1.0)
        context.getWriter().writeOptionalAttribute(TARGET_DEADBAND, targetDeadband, 0.0);
    });
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, targetDeadband]() {
        context.getWriter().writeAttribute(TARGET_DEADBAND, targetDeadband);
    });
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTTRANSFORMERXML_HXX
