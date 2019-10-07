/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_VSCCONVERTERSTATIONXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_VSCCONVERTERSTATIONXML_HPP

#include "AbstractConnectableXml.hpp"

#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class VscConverterStationXml : public AbstractConnectableXml<VscConverterStation, VscConverterStationAdder, VoltageLevel> {
public:
    static const VscConverterStationXml& getInstance();

protected:  // AbstractIdentifiableXml
    VscConverterStationAdder createAdder(VoltageLevel& voltageLevel) const override;

    const char* getRootElementName() const override;

    VscConverterStation& readRootElementAttributes(VscConverterStationAdder& adder, NetworkXmlReaderContext& context) const override;

    void readSubElements(VscConverterStation& converterStation, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const VscConverterStation& converterStation, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const VscConverterStation& converterStation, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

private:
    VscConverterStationXml() = default;

    ~VscConverterStationXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_VSCCONVERTERSTATIONXML_HPP

