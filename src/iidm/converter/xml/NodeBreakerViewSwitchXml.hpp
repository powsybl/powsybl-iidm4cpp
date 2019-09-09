/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_NODEBREAKERVIEWSWITCHXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_NODEBREAKERVIEWSWITCHXML_HPP

#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelViews.hpp>

#include "AbstractSwitchXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class NodeBreakerViewSwitchXml : public AbstractSwitchXml<VoltageLevel::NodeBreakerView::SwitchAdder> {
public:
    static const NodeBreakerViewSwitchXml& getInstance();

protected:  // AbstractIdentifiableXml
    VoltageLevel::NodeBreakerView::SwitchAdder createAdder(VoltageLevel& voltageLevel) const override;

    Switch& readRootElementAttributes(VoltageLevel::NodeBreakerView::SwitchAdder& adder, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const Switch& sw, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_NODEBREAKERVIEWSWITCHXML_HPP

