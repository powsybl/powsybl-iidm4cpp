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
void AbstractTransformerXml<Added, Adder>::readPhaseTapChanger(const std::string& elementName, const std::shared_ptr<PhaseTapChangerAdder>& adder, Terminal& terminal, NetworkXmlReaderContext& context) {
    const auto& lowTapPosition = context.getReader().getAttributeValue<long>(LOW_TAP_POSITION);
    const double& targetDeadband = readTargetDeadband(context);
    const double& regulationValue = context.getReader().getOptionalAttributeValue(REGULATION_VALUE, stdcxx::nan());
    adder->setLowTapPosition(lowTapPosition)
        .setTargetDeadband(targetDeadband)
        .setRegulationValue(regulationValue);
    const auto& tapPosition = context.getReader().getOptionalAttributeValue<long>(TAP_POSITION);
    const auto& regulating = context.getReader().getOptionalAttributeValue<bool>(REGULATING);
    const auto& regModeStr = context.getReader().getOptionalAttributeValue<std::string>(REGULATION_MODE);
    if(tapPosition) {
        adder->setTapPosition(*tapPosition);
    }
    if(regulating) {
        adder->setRegulating(*regulating);
    }
    if(regModeStr) {
        adder->setRegulationMode(Enum::fromString<PhaseTapChanger::RegulationMode>(*regModeStr));
    }
    bool hasTerminalRef = false;
    context.getReader().readUntilEndElement(elementName, [&adder, &context, &hasTerminalRef, &terminal]() {
        if (context.getReader().getLocalName() == TERMINAL_REF) {
            readTerminalRef(context, hasTerminalRef, [adder, &terminal](const std::string& id, const std::string& side) {
                adder->setRegulationTerminal(stdcxx::ref<Terminal>(TerminalRefXml::readTerminalRef(terminal.getVoltageLevel().getNetwork(), id, side)));
                adder->add();
            });
        } else if (context.getReader().getLocalName() == STEP) {
            PhaseTapChangerAdder::StepAdder stepAdder = adder->beginStep();
            readSteps(context, [&stepAdder](double r, double x, double g, double b, double rho) {
                stepAdder.setR(r)
                    .setX(x)
                    .setG(g)
                    .setB(b)
                    .setRho(rho);
            });
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
void AbstractTransformerXml<Added, Adder>::readRatioTapChanger(const std::string& elementName, const std::shared_ptr<RatioTapChangerAdder>& adder, Terminal& terminal, NetworkXmlReaderContext& context) {
    const auto& lowTapPosition = context.getReader().getAttributeValue<long>(LOW_TAP_POSITION);
    const double& targetDeadband = readTargetDeadband(context);
    const auto& loadTapChangingCapabilities = context.getReader().getAttributeValue<bool>(LOAD_TAP_CHANGING_CAPABILITIES);
    double targetV = context.getReader().getOptionalAttributeValue(TARGET_V, stdcxx::nan());
    adder->setLowTapPosition(lowTapPosition)
        .setTargetDeadband(targetDeadband)
        .setLoadTapChangingCapabilities(loadTapChangingCapabilities)
        .setTargetV(targetV);
    const auto& tapPosition = context.getReader().getOptionalAttributeValue<long>(TAP_POSITION);
    const auto& regulating = context.getReader().getOptionalAttributeValue<bool>(REGULATING);
    if(tapPosition) {
        adder->setTapPosition(*tapPosition);
    }
    if(regulating) {
        adder->setRegulating(*regulating);
    }
    bool hasTerminalRef = false;
    context.getReader().readUntilEndElement(elementName, [&adder, &context, &terminal, &hasTerminalRef]() {
        if (context.getReader().getLocalName() == TERMINAL_REF) {
            readTerminalRef(context, hasTerminalRef, [adder, &terminal](const std::string& id, const std::string& side) {
                adder->setRegulationTerminal(stdcxx::ref<Terminal>(TerminalRefXml::readTerminalRef(terminal.getVoltageLevel().getNetwork(), id, side)));
                adder->add();
            });
        } else if (context.getReader().getLocalName() == STEP) {
            readSteps(context, [&adder](double r, double x, double g, double b, double rho) {
                adder->beginStep()
                    .setR(r)
                    .setX(x)
                    .setG(g)
                    .setB(b)
                    .setRho(rho)
                    .endStep();
            });
        } else {
            throw PowsyblException(stdcxx::format("Unexpected XML element <%1%>", context.getReader().getLocalName()));
        }
    });
    if (!hasTerminalRef) {
        adder->add();
    }
}

template <typename Added, typename Adder>
template <typename StepConsumer>
void AbstractTransformerXml<Added, Adder>::readSteps(const NetworkXmlReaderContext& context, const StepConsumer& consumer) {
    const auto& r = context.getReader().getAttributeValue<double>(R);
    const auto& x = context.getReader().getAttributeValue<double>(X);
    const auto& g = context.getReader().getAttributeValue<double>(G);
    const auto& b = context.getReader().getAttributeValue<double>(B);
    const auto& rho = context.getReader().getAttributeValue<double>(RHO);
    consumer(r, x, g, b, rho);
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
template <typename TerminalRefConsumer>
void AbstractTransformerXml<Added, Adder>::readTerminalRef(NetworkXmlReaderContext& context, bool& hasTerminalRef, const TerminalRefConsumer& consumer) {
    const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));
    const std::string& side = context.getReader().getOptionalAttributeValue(SIDE, "");
    context.addEndTask([id, side, consumer]() {
        consumer(id, side);
    });
    hasTerminalRef = true;
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
    context.getWriter().writeAttribute(TARGET_V, rtc.getTargetV());
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
