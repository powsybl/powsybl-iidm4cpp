/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_LCCCONVERTERSTATIONXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_LCCCONVERTERSTATIONXML_HPP

#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class LccConverterStationXml : public AbstractConnectableXml<LccConverterStation, LccConverterStationAdder, VoltageLevel> {
public:
    static const LccConverterStationXml& getInstance();

protected:  // AbstractIdentifiableXml
    LccConverterStationAdder createAdder(VoltageLevel& voltageLevel) const override;

    const char* getRootElementName() const override;

    LccConverterStation& readRootElementAttributes(LccConverterStationAdder& adder, NetworkXmlReaderContext& context) const override;

    void readSubElements(LccConverterStation& converterStation, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const LccConverterStation& converterStation, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

private:
    LccConverterStationXml() = default;

    ~LccConverterStationXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_LCCCONVERTERSTATIONXML_HPP

