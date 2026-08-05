/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_TIELINEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_TIELINEXML_HPP

#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/BoundaryLineAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>
#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>
#include <powsybl/iidm/converter/xml/AbstractSimpleIdentifiableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class TieLineXml : public AbstractSimpleIdentifiableXml<TieLine, TieLineAdder, Network>, public AbstractConnectableXml {
public:
    static const TieLineXml& getInstance();

protected:  // AbstractIdentifiableXml
    TieLineAdder createAdder(Network& network) const override;

    const char* getRootElementName() const override;

    TieLine& readRootElementAttributes(TieLineAdder& adder, Network& network, NetworkXmlReaderContext& context) const override;

    void readSubElements(TieLine& line, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const TieLine& line, const Network& network, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const TieLine& line, const Network& network, NetworkXmlWriterContext& context) const override;

private:
    static void checkBoundaryValue(double imported, double calculated, const std::string& name, const std::string& tlId);

    static BoundaryLine& readBoundaryLine(BoundaryLineAdder& adder, const NetworkXmlReaderContext& context, int side);
    static BoundaryLineAdder readVlAndNodeOrBus(const NetworkXmlReaderContext& context, Network& network, int side);

    static void writeBoundaryLine(const BoundaryLine& bl, NetworkXmlWriterContext& context, int side);

private:
    TieLineXml() = default;

    ~TieLineXml() override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_TIELINEXML_HPP

