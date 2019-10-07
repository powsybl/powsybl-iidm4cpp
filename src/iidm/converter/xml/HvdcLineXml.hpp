/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_HVDCLINEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_HVDCLINEXML_HPP

#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/HvdcLineAdder.hpp>
#include <powsybl/iidm/Network.hpp>

#include "AbstractIdentifiableXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class HvdcLineXml : public AbstractIdentifiableXml<HvdcLine, HvdcLineAdder, Network> {
public:
    static const HvdcLineXml& getInstance();

protected:  // AbstractIdentifiableXml
    HvdcLineAdder createAdder(Network& network) const override;

    const char* getRootElementName() const override;

    HvdcLine& readRootElementAttributes(HvdcLineAdder& adder, NetworkXmlReaderContext& context) const override;

    void readSubElements(HvdcLine& line, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const HvdcLine& line, const Network& network, NetworkXmlWriterContext& context) const override;

private:
    HvdcLineXml() = default;

    ~HvdcLineXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_HVDCLINEXML_HPP

