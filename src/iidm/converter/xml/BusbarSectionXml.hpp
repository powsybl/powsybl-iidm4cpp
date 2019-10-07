/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_BUSBARSECTIONXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_BUSBARSECTIONXML_HPP

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/BusbarSectionAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "AbstractIdentifiableXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class BusbarSectionXml : public AbstractIdentifiableXml<BusbarSection, BusbarSectionAdder, VoltageLevel> {
public:
    static const BusbarSectionXml& getInstance();

protected:  // AbstractIdentifiableXml
    BusbarSectionAdder createAdder(VoltageLevel& voltageLevel) const override;

    const char* getRootElementName() const override;

    BusbarSection& readRootElementAttributes(BusbarSectionAdder& adder, NetworkXmlReaderContext& context) const override;

    void readSubElements(BusbarSection& busbarSection, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const BusbarSection& busbarSection, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_BUSBARSECTIONXML_HPP

