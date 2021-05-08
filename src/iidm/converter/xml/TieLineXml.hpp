/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_TIELINEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_TIELINEXML_HPP

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/TieLineAdder.hpp>

#include "AbstractConnectableXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class TieLineXml : public AbstractConnectableXml<TieLine, TieLineAdder, Network> {
public:
    static const TieLineXml& getInstance();

protected:  // AbstractIdentifiableXml
    TieLineAdder createAdder(Network& network) const override;

    const char* getRootElementName() const override;

    TieLine& readRootElementAttributes(TieLineAdder& adder, NetworkXmlReaderContext& context) const override;

    void readSubElements(TieLine& line, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const TieLine& line, const Network& network, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const TieLine& line, const Network& network, NetworkXmlWriterContext& context) const override;

private:
    static void checkBoundaryValue(double imported, double calculated, const std::string& name, const std::string& tlId);

    static void readHalf(TieLineAdder::HalfLineAdder adder, const NetworkXmlReaderContext& context, int side);

    static void writeHalf(const TieLine::HalfLine& halfLine, NetworkXmlWriterContext& context, int side);

private:
    TieLineXml() = default;

    ~TieLineXml() override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_TIELINEXML_HPP

