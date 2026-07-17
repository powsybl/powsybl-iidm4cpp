/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_DCSWITCHXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_DCSWITCHXML_HPP

#include <powsybl/iidm/DcSwitch.hpp>
#include <powsybl/iidm/DcSwitchAdder.hpp>
#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/converter/xml/AbstractSimpleIdentifiableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class DcSwitchXml : public AbstractSimpleIdentifiableXml<DcSwitch, DcSwitchAdder, Network> {
public:
    static const DcSwitchXml& getInstance();

protected:  // AbstractIdentifiableXml
    DcSwitchAdder createAdder(Network& network) const override;

    const char* getRootElementName() const override;

    DcSwitch& readRootElementAttributes(DcSwitchAdder& adder, Network& network, NetworkXmlReaderContext& context) const override;

    void readSubElements(DcSwitch& dcSwitch, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const DcSwitch& dcSwitch, const Network& network, NetworkXmlWriterContext& context) const override;

private:
    DcSwitchXml() = default;

    ~DcSwitchXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_DCSWITCHXML_HPP

