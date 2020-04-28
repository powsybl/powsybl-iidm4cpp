/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_THREEWINDINGSTRANSFORMERXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_THREEWINDINGSTRANSFORMERXML_HPP

#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>

#include "AbstractTransformerXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class ThreeWindingsTransformerXml : public AbstractTransformerXml<ThreeWindingsTransformer, ThreeWindingsTransformerAdder> {
public:
    static const ThreeWindingsTransformerXml& getInstance();

protected:  // AbstractIdentifiableXml
    ThreeWindingsTransformerAdder createAdder(Substation& substation) const override;

    const char* getRootElementName() const override;

    ThreeWindingsTransformer& readRootElementAttributes(ThreeWindingsTransformerAdder& adder, NetworkXmlReaderContext& context) const override;

    void readSubElements(ThreeWindingsTransformer& twt, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const ThreeWindingsTransformer& twt, const Substation& substation, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const ThreeWindingsTransformer& twt, const Substation& substation, NetworkXmlWriterContext& context) const override;

private:
    static void writePhaseTapChanger(const stdcxx::CReference<PhaseTapChanger>& ptc, int index, NetworkXmlWriterContext& context);

    static void writeRatioTapChanger(const stdcxx::CReference<RatioTapChanger>& rtc, int index, NetworkXmlWriterContext& context);

private:
    ThreeWindingsTransformerXml() = default;

    ~ThreeWindingsTransformerXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_THREEWINDINGSTRANSFORMERXML_HPP

