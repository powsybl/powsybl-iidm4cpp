/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "VoltageLevelXml.hpp"

#include <powsybl/iidm/TopologyLevel.hpp>

#include "iidm/converter/Constants.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

VoltageLevelAdder VoltageLevelXml::createAdder(Substation& substation) const {
    return substation.newVoltageLevel();
}

const char* VoltageLevelXml::getRootElementName() const {
    return VOLTAGE_LEVEL;
}

bool VoltageLevelXml::hasSubElements(const VoltageLevel& /*voltageLevel*/) const {
    return true;
}

VoltageLevelXml& VoltageLevelXml::instance() {
    static VoltageLevelXml instance;
    return instance;
}

VoltageLevel& VoltageLevelXml::readRootElementAttributes(VoltageLevelAdder& adder, const NetworkXmlReaderContext& context) const {
    auto nominalVoltage = context.getReader().getAttributeValue<double>(NOMINAL_V);
    double lowVoltageLimit = context.getReader().getOptionalAttributeValue(LOW_VOLTAGE_LIMIT, stdcxx::nan());
    double highVoltageLimit = context.getReader().getOptionalAttributeValue(HIGH_VOLTAGE_LIMIT, stdcxx::nan());
    const TopologyKind& topologyKing = getTopologyKind(context.getReader().getAttributeValue(TOPOLOGY_KIND));
    return adder.setNominalVoltage(nominalVoltage)
                .setLowVoltageLimit(lowVoltageLimit)
                .setHighVoltageLimit(highVoltageLimit)
                .setTopologyKind(topologyKing)
                .add();
}

void VoltageLevelXml::readSubElements(VoltageLevel& voltageLevel, const NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(VOLTAGE_LEVEL, [this, &voltageLevel, &context]() {
        AbstractIdentifiableXml::readSubElements(voltageLevel, context);
    });
}

void VoltageLevelXml::writeRootElementAttributes(const VoltageLevel& voltageLevel, const Substation& /*substation*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(NOMINAL_V, voltageLevel.getNominalVoltage());
    context.getWriter().writeOptionalAttribute(LOW_VOLTAGE_LIMIT, voltageLevel.getLowVoltageLimit(), stdcxx::nan());
    context.getWriter().writeOptionalAttribute(HIGH_VOLTAGE_LIMIT, voltageLevel.getHighVoltageLimit(), stdcxx::nan());
    const TopologyLevel& topologyLevel = getMinTopologyLevel(voltageLevel.getTopologyKind(), context.getOptions().getTopologyLevel());
    const TopologyKind& topologyKind = getTopologyKind(topologyLevel);
    context.getWriter().writeAttribute(TOPOLOGY_KIND, getTopologyKindName(topologyKind));
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

