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
#include <powsybl/iidm/TapChangerStep.hpp>

#include "AbstractConnectableXml.hpp"

namespace powsybl {

namespace xml {

class XmlStreamWriter;

}  // namespace xml

namespace iidm {

class PhaseTapChanger;
class RatioTapChanger;
class Terminal;
class TwoWindingsTransformer;

namespace converter {

namespace xml {

template <typename T, typename A>
class AbstractTransformerXml : public AbstractConnectableXml<T, A, Substation> {
public:
    static void readPhaseTapChanger(TwoWindingsTransformer& twt, NetworkXmlReaderContext& context);

    static void readRatioTapChanger(TwoWindingsTransformer& twt, NetworkXmlReaderContext& context);

    static void readRatioTapChanger(int leg, ThreeWindingsTransformer::Leg2or3& twl, NetworkXmlReaderContext& context);

    static void readRatioTapChanger(const std::string& elementName, const std::shared_ptr<RatioTapChangerAdder>& adder, Terminal& terminal, NetworkXmlReaderContext& context);

    static void writePhaseTapChanger(const std::string& name, const PhaseTapChanger& ptc, NetworkXmlWriterContext& context);

    static void writeRatioTapChanger(const std::string& name, const RatioTapChanger& rtc, NetworkXmlWriterContext& context);

    template <typename H, typename C, typename S>
    static void writeTapChanger(const TapChanger<H, C, S>& tc, powsybl::xml::XmlStreamWriter& writer);

    template <typename S>
    static void writeTapChangerStep(const TapChangerStep<S>& tcs, powsybl::xml::XmlStreamWriter& writer);

protected:
    AbstractTransformerXml() = default;

    ~AbstractTransformerXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include "AbstractTransformerXml.hxx"

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTTRANSFORMERXML_HPP

