/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_DCNODEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_DCNODEXML_HPP

#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/DcNodeAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/xml/AbstractSimpleIdentifiableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class DcNodeXml : public AbstractSimpleIdentifiableXml<DcNode, DcNodeAdder, Network> {
public:
    static const DcNodeXml& getInstance();

protected:  // AbstractIdentifiableXml
    DcNodeAdder createAdder(Network& network) const override;

    const char* getRootElementName() const override;

    DcNode& readRootElementAttributes(DcNodeAdder& adder, Network& network, NetworkXmlReaderContext& context) const override;

    void readSubElements(DcNode& dcNode, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const DcNode& dcNode, const Network& network, NetworkXmlWriterContext& context) const override;

private:
    DcNodeXml() = default;

    ~DcNodeXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_DCNODEXML_HPP

