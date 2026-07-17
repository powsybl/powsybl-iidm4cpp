/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "LineCommutatedConverterXml.hpp"

#include <powsybl/iidm/Enum.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

const LineCommutatedConverterXml& LineCommutatedConverterXml::getInstance() {
    static LineCommutatedConverterXml s_instance;
    return s_instance;
}

LineCommutatedConverterAdder LineCommutatedConverterXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newLineCommutatedConverter();
}

const char* LineCommutatedConverterXml::getRootElementName() const {
    return LINE_COMMUTATED_CONVERTER;
}

LineCommutatedConverter& LineCommutatedConverterXml::readRootElementAttributes(LineCommutatedConverterAdder& adder, VoltageLevel& /*voltageLevel*/, NetworkXmlReaderContext& context) const {
    readRootElementCommonAttributes(adder, context);
    const auto& reactiveModel = Enum::fromString<LineCommutatedConverter::ReactiveModel>(context.getReader().getAttributeValue(REACTIVE_MODEL));
    const auto& powerFactor = context.getReader().getAttributeValue<double>(POWER_FACTOR);
    LineCommutatedConverter& lcc = adder.setReactiveModel(reactiveModel)
                                        .setPowerFactor(powerFactor)
                                        .add();
    readRootElementPqiAttributes(lcc, context);
    return lcc;
}

void LineCommutatedConverterXml::readSubElements(LineCommutatedConverter& lcc, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(LINE_COMMUTATED_CONVERTER, [this, &lcc, &context]() {
        AbstractAcDcConverterXml<LineCommutatedConverter, LineCommutatedConverterAdder>::readSubElements(lcc, context);
    });
}

void LineCommutatedConverterXml::writeRootElementAttributes(const LineCommutatedConverter& lcc, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    AbstractAcDcConverterXml<LineCommutatedConverter,LineCommutatedConverterAdder>::writeRootElementAttributes(lcc, voltageLevel, context);
    context.getWriter().writeAttribute(REACTIVE_MODEL, Enum::toString(lcc.getReactiveModel()));
    context.getWriter().writeAttribute(POWER_FACTOR, lcc.getPowerFactor());
    writeRootElementPqiAttributes(lcc, context);
}


}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

