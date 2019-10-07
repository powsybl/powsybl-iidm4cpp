/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_SUBSTATIONXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_SUBSTATIONXML_HPP

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>

#include "AbstractIdentifiableXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class SubstationXml : public AbstractIdentifiableXml<Substation, SubstationAdder, Network> {
public:
    static const SubstationXml& getInstance();

protected:  // AbstractIdentifiableXml
    SubstationAdder createAdder(Network& network) const override;

    const char* getRootElementName() const override;

    Substation& readRootElementAttributes(SubstationAdder& adder, NetworkXmlReaderContext& context) const override;

    void readSubElements(Substation& substation, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const Substation& substation, const Network& network, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const Substation& substation, const Network& network, NetworkXmlWriterContext& context) const override;

private:
    SubstationXml() = default;

    ~SubstationXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_SUBSTATIONXML_HPP

