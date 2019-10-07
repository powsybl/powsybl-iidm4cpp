/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_BATTERYXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_BATTERYXML_HPP

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "AbstractConnectableXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class BatteryXml : public AbstractConnectableXml<Battery, BatteryAdder, VoltageLevel> {
public:
    static const BatteryXml& getInstance();

protected:  // AbstractIdentifiableXml
    BatteryAdder createAdder(VoltageLevel& voltageLevel) const override;

    const char* getRootElementName() const override;

    Battery& readRootElementAttributes(BatteryAdder& adder, NetworkXmlReaderContext& context) const override;

    void readSubElements(Battery& battery, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const Battery& battery, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const Battery& battery, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

private:
    BatteryXml() = default;

    ~BatteryXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_BATTERYXML_HPP

