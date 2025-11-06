/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTTRANSFORMERXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTTRANSFORMERXML_HPP

#include <memory>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/TapChanger.hpp>
#include <powsybl/iidm/TapChangerAdder.hpp>
#include <powsybl/iidm/TapChangerStep.hpp>
#include <powsybl/iidm/TapChangerStepAdder.hpp>
#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>
#include <powsybl/iidm/converter/xml/AbstractSimpleIdentifiableXml.hpp>

namespace powsybl {

namespace xml {

class XmlStreamWriter;

}  // namespace xml

namespace iidm {

class PhaseTapChanger;
class PhaseTapChangerAdder;
class RatioTapChangerAdder;
class RatioTapChanger;
class Terminal;
class TwoWindingsTransformer;

namespace converter {

namespace xml {

template <typename Added, typename Adder>
class AbstractTransformerXml : public AbstractSimpleIdentifiableXml<Added, Adder, Substation> , public AbstractConnectableXml {
protected:
    AbstractTransformerXml() = default;

    ~AbstractTransformerXml() noexcept override = default;

    static void readPhaseTapChanger(TwoWindingsTransformer& twt, NetworkXmlReaderContext& context);

    static void readPhaseTapChanger(int leg, ThreeWindingsTransformer::Leg& twl, NetworkXmlReaderContext& context);

    static void readPhaseTapChanger(const std::string& elementName, std::shared_ptr<PhaseTapChangerAdder>& adder, Terminal& terminal, NetworkXmlReaderContext& context);

    template <typename Consumer>
    static void readRatedS(const std::string& name, NetworkXmlReaderContext& context, const Consumer& consumer);

    static void readRatioTapChanger(TwoWindingsTransformer& twt, NetworkXmlReaderContext& context);

    static void readRatioTapChanger(int leg, ThreeWindingsTransformer::Leg& twl, NetworkXmlReaderContext& context);

    static void readRatioTapChanger(const std::string& elementName, std::shared_ptr<RatioTapChangerAdder>& adder, Terminal& terminal, NetworkXmlReaderContext& context);

    template <typename A, typename B>
    static void readSteps(const NetworkXmlReaderContext& context, TapChangerStepAdder<A, B>& tapChangerStepAdder);

    static double readTargetDeadband(NetworkXmlReaderContext& context);

    template <typename TC, typename TCAdder, typename TCStepAdder, typename TCHolder>
    static void readTapChangerTerminalRef(NetworkXmlReaderContext& context, std::shared_ptr<TapChangerAdder<TC, TCAdder, TCStepAdder, TCHolder>>& tapChangerAdder, Terminal& terminal);
    
    template <typename TC, typename TCAdder, typename TCStepAdder, typename TCHolder>
    static void readTapChangerAttributes(NetworkXmlReaderContext& context, std::shared_ptr<TapChangerAdder<TC, TCAdder, TCStepAdder, TCHolder>>& tapChangerAdder);

    static void writePhaseTapChanger(const std::string& name, const PhaseTapChanger& ptc, NetworkXmlWriterContext& context);

    static void writeRatedS(const std::string& name, double ratedS, NetworkXmlWriterContext& context);

    static void writeRatioTapChanger(const std::string& name, const RatioTapChanger& rtc, NetworkXmlWriterContext& context);

    template <typename H, typename C, typename S, typename R>
    static void writeTapChanger(const TapChanger<H, C, S, R>& tc, NetworkXmlWriterContext& context);

    template <typename S>
    static void writeTapChangerStep(const TapChangerStep<S>& tcs, powsybl::xml::XmlStreamWriter& writer);

    static void writeTargetDeadband(double targetDeadband, NetworkXmlWriterContext& context);
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include "AbstractTransformerXml.hxx"

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTTRANSFORMERXML_HPP

