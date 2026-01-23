/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_AREAXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_AREAXML_HPP

#include <powsybl/iidm/Area.hpp>
#include <powsybl/iidm/AreaAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/xml/AbstractSimpleIdentifiableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class AreaXml : public AbstractSimpleIdentifiableXml<Area, AreaAdder, Network> {
public:
    static const AreaXml& getInstance();

protected:  // AbstractIdentifiableXml
    AreaAdder createAdder(Network& network) const override;

    const char* getRootElementName() const override;

    Area& readRootElementAttributes(AreaAdder& adder, Network& network, NetworkXmlReaderContext& context) const override;

    void readSubElements(Area& area, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const Area& area, const Network& network, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const Area& area, const Network& network, NetworkXmlWriterContext& context) const override;

private:
    AreaXml() = default;

    ~AreaXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_AREAXML_HPP

