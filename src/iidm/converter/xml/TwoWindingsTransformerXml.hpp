/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_TWOWINDINGSTRANSFORMERXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_TWOWINDINGSTRANSFORMERXML_HPP

#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformerAdder.hpp>

#include "AbstractTransformerXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class TwoWindingsTransformerXml : public AbstractTransformerXml<TwoWindingsTransformer, TwoWindingsTransformerAdder> {
public:
    static const TwoWindingsTransformerXml& getInstance();

protected:  // AbstractIdentifiableXml
    TwoWindingsTransformerAdder createAdder(Substation& substation) const override;

    const char* getRootElementName() const override;

    bool hasSubElements(const TwoWindingsTransformer& twt) const override;

    TwoWindingsTransformer& readRootElementAttributes(TwoWindingsTransformerAdder& adder, const NetworkXmlReaderContext& context) const override;

    void readSubElements(TwoWindingsTransformer& twt, const NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const TwoWindingsTransformer& twt, const Substation& substation, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const TwoWindingsTransformer& twt, const Substation& substation, NetworkXmlWriterContext& context) const override;

private:
    TwoWindingsTransformerXml() = default;

    ~TwoWindingsTransformerXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_TWOWINDINGSTRANSFORMERXML_HPP

