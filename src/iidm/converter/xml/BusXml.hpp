/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_BUSXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_BUSXML_HPP

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "AbstractIdentifiableXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class BusXml : public AbstractIdentifiableXml<Bus, BusAdder, VoltageLevel> {
public:
    static const BusXml& getInstance();

protected:  // AbstractIdentifiableXml
    BusAdder createAdder(VoltageLevel& voltageLevel) const override;

    const char* getRootElementName() const override;

    bool hasSubElements(const Bus& bus) const override;

    Bus& readRootElementAttributes(BusAdder& adder, const NetworkXmlReaderContext& context) const override;

    void readSubElements(Bus& bus, const NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const Bus& bus, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

private:
    BusXml() = default;

    ~BusXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_BUSXML_HPP

