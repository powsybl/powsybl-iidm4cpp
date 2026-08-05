/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTBOUNDARYLINEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTBOUNDARYLINEXML_HPP

#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/BoundaryLineAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>
#include <powsybl/iidm/converter/xml/AbstractSimpleIdentifiableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class AbstractBoundaryLineXml : public AbstractSimpleIdentifiableXml<BoundaryLine, BoundaryLineAdder, VoltageLevel>, public AbstractConnectableXml {

protected:  // AbstractIdentifiableXml
    BoundaryLineAdder createAdder(VoltageLevel& voltageLevel) const override;

    BoundaryLine& readRootElementAttributes(BoundaryLineAdder& adder, VoltageLevel& voltageLevel, NetworkXmlReaderContext& context) const override;

    void readSubElements(BoundaryLine& bl, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const BoundaryLine& bl, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const BoundaryLine& bl, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

    virtual void assertReadCompatibilityVersion(NetworkXmlReaderContext& context) const = 0;
    virtual void assertWriteCompatibilityVersion(NetworkXmlWriterContext& context) const = 0;

protected:
    AbstractBoundaryLineXml() = default;

    ~AbstractBoundaryLineXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTBOUNDARYLINEXML_HPP

