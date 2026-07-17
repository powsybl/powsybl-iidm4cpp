/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_DCGROUNDXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_DCGROUNDXML_HPP

#include <powsybl/iidm/DcGround.hpp>
#include <powsybl/iidm/DcGroundAdder.hpp>
#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>
#include <powsybl/iidm/converter/xml/AbstractSimpleIdentifiableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class DcGroundXml : public AbstractSimpleIdentifiableXml<DcGround, DcGroundAdder, Network>, public AbstractConnectableXml {
public:
    static const DcGroundXml& getInstance();

protected:  // AbstractIdentifiableXml
    DcGroundAdder createAdder(Network& network) const override;

    const char* getRootElementName() const override;

    DcGround& readRootElementAttributes(DcGroundAdder& adder, Network& network, NetworkXmlReaderContext& context) const override;

    void readSubElements(DcGround& dcGround, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const DcGround& dcGround, const Network& network, NetworkXmlWriterContext& context) const override;

private:
    DcGroundXml() = default;

    ~DcGroundXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_DCGROUNDXML_HPP

