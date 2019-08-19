/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "GeneratorXml.hpp"

#include "iidm/converter/Constants.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

GeneratorAdder GeneratorXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newGenerator();
}

const GeneratorXml& GeneratorXml::getInstance() {
    static GeneratorXml s_instance;
    return s_instance;
}

const char* GeneratorXml::getRootElementName() const {
    return GENERATOR;
}

bool GeneratorXml::hasSubElements(const Generator& /*generator*/) const {
    return true;
}

Generator& GeneratorXml::readRootElementAttributes(GeneratorAdder& generatorAdder, const NetworkXmlReaderContext& context) const {
    EnergySource energySource = getEnergySource(context.getReader().getAttributeValue(ENERGY_SOURCE));
    const auto& minP = context.getReader().getAttributeValue<double>(MIN_P);
    const auto& maxP = context.getReader().getAttributeValue<double>(MAX_P);
    double ratedS = context.getReader().getOptionalAttributeValue(RATED_S, stdcxx::nan());
    const auto& voltageRegulatorOn = context.getReader().getAttributeValue<bool>(VOLTAGE_REGULATOR_ON);
    const auto& targetP = context.getReader().getAttributeValue<double>(TARGET_P);
    double targetV = context.getReader().getOptionalAttributeValue(TARGET_V, stdcxx::nan());
    double targetQ = context.getReader().getOptionalAttributeValue(TARGET_Q, stdcxx::nan());
    readNodeOrBus(generatorAdder, context);
    Generator& generator = generatorAdder.setEnergySource(energySource)
        .setMinP(minP)
        .setMaxP(maxP)
        .setRatedS(ratedS)
        .setVoltageRegulatorOn(voltageRegulatorOn)
        .setTargetP(targetP)
        .setTargetV(targetV)
        .setTargetQ(targetQ)
        .add();

    readPQ(boost::optional<int>(), generator.getTerminal(), context.getReader());
    return generator;
}

void GeneratorXml::readSubElements(Generator& generator, const NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(GENERATOR, [this, &generator, &context]() {
        if (context.getReader().getLocalName() == REGULATING_TERMINAL) {
            // TODO(sebalaig) implement "regulatingTerminal" read
        } else if (context.getReader().getLocalName() == REACTIVE_CAPABILITY_CURVE ||
                   context.getReader().getLocalName() == MIN_MAX_REACTIVE_LIMITS) {
            // TODO(sebalaig) implement ReactiveLimitsXml
        } else {
            AbstractIdentifiableXml::readSubElements(generator, context);
        }
    });
}

void GeneratorXml::writeRootElementAttributes(const Generator& generator, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(ENERGY_SOURCE, getEnergySourceName(generator.getEnergySource()));
    context.getWriter().writeAttribute(MIN_P, generator.getMinP());
    context.getWriter().writeAttribute(MAX_P, generator.getMaxP());
    context.getWriter().writeOptionalAttribute(RATED_S, generator.getRatedS(), stdcxx::nan());
    context.getWriter().writeAttribute(VOLTAGE_REGULATOR_ON, generator.isVoltageRegulatorOn() ? "true" : "false");
    context.getWriter().writeAttribute(TARGET_P, generator.getTargetP());
    context.getWriter().writeOptionalAttribute(TARGET_V, generator.getTargetV(), stdcxx::nan());
    context.getWriter().writeOptionalAttribute(TARGET_Q, generator.getTargetQ(), stdcxx::nan());
    writeNodeOrBus(boost::optional<int>(), generator.getTerminal(), context);
    writePQ(boost::optional<int>(), generator.getTerminal(), context.getWriter());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

