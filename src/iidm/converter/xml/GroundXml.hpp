/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_GROUNDXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_GROUNDXML_HPP

#include <powsybl/iidm/Ground.hpp>
#include <powsybl/iidm/GroundAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>
#include <powsybl/iidm/converter/xml/AbstractSimpleIdentifiableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class GroundXml : public AbstractSimpleIdentifiableXml<Ground, GroundAdder, VoltageLevel>, public AbstractConnectableXml {
public:
    static const GroundXml& getInstance();

protected:  // AbstractIdentifiableXml
    GroundAdder createAdder(VoltageLevel& voltageLevel) const override;

    const char* getRootElementName() const override;

    void writeRootElementAttributes(const Ground& ground, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

    //AbstractSimpleIdentifiableXml
    void readSubElements(Ground& ground, NetworkXmlReaderContext& context) const override;

    Ground& readRootElementAttributes(GroundAdder& adder, VoltageLevel& voltageLevel, NetworkXmlReaderContext& context) const override;

private:
    GroundXml() = default;

    ~GroundXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_GROUNDXML_HPP
