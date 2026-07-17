/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_LINECOMMUTATEDCONVERTERXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_LINECOMMUTATEDCONVERTERXML_HPP

#include <powsybl/iidm/LineCommutatedConverter.hpp>
#include <powsybl/iidm/LineCommutatedConverterAdder.hpp>

#include "AbstractAcDcConverterXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class LineCommutatedConverterXml : public AbstractAcDcConverterXml<LineCommutatedConverter, LineCommutatedConverterAdder> {
public:
    static const LineCommutatedConverterXml& getInstance();

protected:  // AbstractIdentifiableXml
    LineCommutatedConverterAdder createAdder(VoltageLevel& voltageLevel) const override;

    const char* getRootElementName() const override;

    LineCommutatedConverter& readRootElementAttributes(LineCommutatedConverterAdder& adder, VoltageLevel& voltageLevel, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const LineCommutatedConverter& lcc, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

    void readSubElements(LineCommutatedConverter& lcc, NetworkXmlReaderContext& context) const override;

private:
    LineCommutatedConverterXml() = default;

    ~LineCommutatedConverterXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_LINECOMMUTATEDCONVERTERXML_HPP

