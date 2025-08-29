/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_DANGLINGLINEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_DANGLINGLINEXML_HPP

#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/DanglingLineAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>
#include <powsybl/iidm/converter/xml/AbstractSimpleIdentifiableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class DanglingLineXml : public AbstractSimpleIdentifiableXml<DanglingLine, DanglingLineAdder, VoltageLevel>, public AbstractConnectableXml {
public:
    static const DanglingLineXml& getInstance();

protected:  // AbstractIdentifiableXml
    DanglingLineAdder createAdder(VoltageLevel& voltageLevel) const override;

    const char* getRootElementName() const override;

    DanglingLine& readRootElementAttributes(DanglingLineAdder& adder, VoltageLevel& voltageLevel, NetworkXmlReaderContext& context) const override;

    void readSubElements(DanglingLine& dl, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const DanglingLine& dl, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const DanglingLine& dl, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

private:
    DanglingLineXml() = default;

    ~DanglingLineXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_DANGLINGLINEXML_HPP

