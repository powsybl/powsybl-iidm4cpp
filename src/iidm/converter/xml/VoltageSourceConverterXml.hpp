/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_VOLTAGESOURCECONVERTERXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_VOLTAGESOURCECONVERTERXML_HPP

#include <powsybl/iidm/VoltageSourceConverter.hpp>
#include <powsybl/iidm/VoltageSourceConverterAdder.hpp>

#include "AbstractAcDcConverterXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class VoltageSourceConverterXml : public AbstractAcDcConverterXml<VoltageSourceConverter, VoltageSourceConverterAdder> {
public:
    static const VoltageSourceConverterXml& getInstance();

protected:  // AbstractIdentifiableXml
    VoltageSourceConverterAdder createAdder(VoltageLevel& voltageLevel) const override;

    const char* getRootElementName() const override;

    VoltageSourceConverter& readRootElementAttributes(VoltageSourceConverterAdder& adder, VoltageLevel& voltageLevel, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const VoltageSourceConverter& vsc, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;


    void readSubElements(VoltageSourceConverter& vsc, NetworkXmlReaderContext& context) const override;

    void writeSubElements(const VoltageSourceConverter& vsc, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

private:
    VoltageSourceConverterXml() = default;

    ~VoltageSourceConverterXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_VOLTAGESOURCECONVERTERXML_HPP

