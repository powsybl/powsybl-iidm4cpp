/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_DCLINEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_DCLINEXML_HPP

#include <powsybl/iidm/DcLine.hpp>
#include <powsybl/iidm/DcLineAdder.hpp>
#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>
#include <powsybl/iidm/converter/xml/AbstractSimpleIdentifiableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class DcLineXml : public AbstractSimpleIdentifiableXml<DcLine, DcLineAdder, Network>, public AbstractConnectableXml {
public:
    static const DcLineXml& getInstance();

protected:  // AbstractIdentifiableXml
    DcLineAdder createAdder(Network& network) const override;

    const char* getRootElementName() const override;

    DcLine& readRootElementAttributes(DcLineAdder& adder, Network& network, NetworkXmlReaderContext& context) const override;

    void readSubElements(DcLine& dcLine, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const DcLine& dcLine, const Network& network, NetworkXmlWriterContext& context) const override;

private:
    DcLineXml() = default;

    ~DcLineXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_DCLINEXML_HPP

