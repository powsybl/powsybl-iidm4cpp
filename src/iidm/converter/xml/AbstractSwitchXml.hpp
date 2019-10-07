/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTSWITCHXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTSWITCHXML_HPP

#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "AbstractIdentifiableXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename A>
class AbstractSwitchXml : public AbstractIdentifiableXml<Switch, A, VoltageLevel> {
public:  // AbstractIdentifiableXml
    const char* getRootElementName() const override;

    void readSubElements(Switch& sw, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const Switch& sw, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

protected:
    AbstractSwitchXml() = default;

    ~AbstractSwitchXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include "AbstractSwitchXml.hxx"

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTSWITCHXML_HPP

