/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTACDCCONVERTERXML_HXX
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTACDCCONVERTERXML_HXX

#include "AbstractAcDcConverterXml.hpp"

#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>

#include "DroopCurveXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename Added, typename Adder>
void AbstractAcDcConverterXml<Added, Adder>::readRootElementPqiAttributes(Added& converter, NetworkXmlReaderContext& context) const {
    readPQ(converter.getTerminal1(), context.getReader(), 1);
    const auto& terminal2 = converter.getTerminal2();
    if(static_cast<bool>(terminal2)) {
        readPQ(terminal2.get(), context.getReader(), 2);
    }
    readPI(converter.getDcTerminal1(), context.getReader());
    readPI(converter.getDcTerminal2(), context.getReader());
}

template <typename Added, typename Adder>
void AbstractAcDcConverterXml<Added, Adder>::writeRootElementPqiAttributes(const Added& converter, NetworkXmlWriterContext& context) const {
    writePQ(converter.getTerminal1(), context.getWriter(), 1);
    const auto& terminal2 = converter.getTerminal2();
    if(static_cast<bool>(terminal2)) {
        writePQ(terminal2.get(), context.getWriter(), 2);
    }
    writePI(converter.getDcTerminal1(), context.getWriter());
    writePI(converter.getDcTerminal2(), context.getWriter());
}

template <typename Added, typename Adder>
void AbstractAcDcConverterXml<Added, Adder>::readRootElementCommonAttributes(Adder& adder, NetworkXmlReaderContext& context) const {
    auto& reader = context.getReader();

    const auto& dcNode1Id = reader.getAttributeValue(toString(DC_NODE, 1));
    const auto& dcConnected1 = reader.getAttributeValue<bool>(toString(DC_CONNECTED, 1));
    const auto& dcNode2Id = reader.getAttributeValue(toString(DC_NODE, 2));
    const auto& dcConnected2 = reader.getAttributeValue<bool>(toString(DC_CONNECTED, 2));

    const auto& idleLoss = reader.getAttributeValue<double>(IDLE_LOSS);
    const auto& switchingLoss = reader.getAttributeValue<double>(SWITCHING_LOSS);
    const auto& resistiveLoss = reader.getAttributeValue<double>(RESISTIVE_LOSS);

    const auto& controlMode = Enum::fromString<AcDcConverter::ControlMode>(reader.getAttributeValue(CONTROL_MODE));

    const double& targetP = reader.getOptionalAttributeValue(TARGET_P, stdcxx::nan());
    const double& targetVdc = reader.getOptionalAttributeValue(TARGET_VDC, stdcxx::nan());

    adder.setDcNode1(dcNode1Id)
         .setDcConnected1(dcConnected1)
         .setDcNode2(dcNode2Id)
         .setDcConnected2(dcConnected2)
         .setControlMode(controlMode)
         .setTargetP(targetP)
         .setTargetVdc(targetVdc)
         .setIdleLoss(idleLoss)
         .setSwitchingLoss(switchingLoss)
         .setResistiveLoss(resistiveLoss);

    readNodeOrBus(adder, context);
}

template <typename Added, typename Adder>
void AbstractAcDcConverterXml<Added, Adder>::readSubElements(Added& converter, NetworkXmlReaderContext& context) const {
    if (context.getReader().getLocalName() == PCC_TERMINAL) {
        TerminalRefXml::readTerminalRef(converter.getNetwork(), context, [&converter](Terminal& terminal){
            converter.setPccTerminal(stdcxx::ref<Terminal>(terminal));
        });
    } else if(context.getReader().getLocalName() == DROOP_CURVE) {
        DroopCurveXml::getInstance().read(converter, context);
    } else {
        AbstractSimpleIdentifiableXml<Added, Adder, VoltageLevel>::readSubElements(converter, context);
    }
}

template <typename Added, typename Adder>
void AbstractAcDcConverterXml<Added, Adder>::writeRootElementAttributes(const Added& converter, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    const auto& dcTerminal1 = converter.getDcTerminal1();
    const auto& dcTerminal2 = converter.getDcTerminal2();

    auto& writer = context.getWriter();
    writer.writeAttribute(toString(DC_NODE, 1), dcTerminal1.getDcNode().getId());
    writer.writeAttribute(toString(DC_CONNECTED, 1), dcTerminal1.isConnected());
    writer.writeAttribute(toString(DC_NODE, 2), dcTerminal2.getDcNode().getId());
    writer.writeAttribute(toString(DC_CONNECTED, 2), dcTerminal2.isConnected());

    writer.writeAttribute(IDLE_LOSS, converter.getIdleLoss());
    writer.writeAttribute(SWITCHING_LOSS, converter.getSwitchingLoss());
    writer.writeAttribute(RESISTIVE_LOSS, converter.getResistiveLoss());

    writer.writeAttribute(CONTROL_MODE, Enum::toString(converter.getControlMode()));
    writer.writeOptionalAttribute(TARGET_P, converter.getTargetP());
    writer.writeOptionalAttribute(TARGET_VDC, converter.getTargetVdc());

    writeNodeOrBus(converter, context);
}

template <typename Added, typename Adder>
void AbstractAcDcConverterXml<Added, Adder>::writeSubElements(const Added& converter, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {

    TerminalRefXml::writeTerminalRef(converter.getPccTerminal(), context, PCC_TERMINAL);
    DroopCurveXml::getInstance().write(converter, context);
    AbstractSimpleIdentifiableXml<Added, Adder, VoltageLevel>::writeSubElements(converter, voltageLevel, context); // nothing in there so we should remove this line

}


}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTACDCCONVERTERXML_HXX
