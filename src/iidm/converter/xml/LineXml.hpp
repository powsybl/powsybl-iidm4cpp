/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_LINEXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_LINEXML_HPP

#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Network.hpp>

#include "AbstractConnectableXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class LineXml : public AbstractConnectableXml<Line, LineAdder, Network> {
public:
    static const LineXml& getInstance();

protected:  // AbstractIdentifiableXml
    LineAdder createAdder(Network& network) const override;

    const char* getRootElementName() const override;

    Line& readRootElementAttributes(LineAdder& adder, NetworkXmlReaderContext& context) const override;

    void readSubElements(Line& line, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const Line& line, const Network& network, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const Line& line, const Network& network, NetworkXmlWriterContext& context) const override;

private:
    LineXml() = default;

    ~LineXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_LINEXML_HPP

