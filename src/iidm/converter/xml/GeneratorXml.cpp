/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "GeneratorXml.hpp"

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>

#include "ReactiveLimitsXml.hpp"

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

Generator& GeneratorXml::readRootElementAttributes(GeneratorAdder& generatorAdder, NetworkXmlReaderContext& context) const {
    const auto& energySource = Enum::fromString<EnergySource>(context.getReader().getAttributeValue(ENERGY_SOURCE));
    double minP = context.getReader().getOptionalAttributeValue(MIN_P, stdcxx::nan());
    double maxP = context.getReader().getOptionalAttributeValue(MAX_P, stdcxx::nan());
    double ratedS = context.getReader().getOptionalAttributeValue(RATED_S, stdcxx::nan());
    bool voltageRegulatorOn = context.getReader().getOptionalAttributeValue(VOLTAGE_REGULATOR_ON, false);
    double targetP = context.getReader().getOptionalAttributeValue(TARGET_P, stdcxx::nan());
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

    readPQ(generator.getTerminal(), context.getReader());
    return generator;
}

void GeneratorXml::readSubElements(Generator& generator, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(GENERATOR, [this, &generator, &context]() {
        if (context.getReader().getLocalName() == REGULATING_TERMINAL) {
            const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getOptionalAttributeValue(ID, ""));
            const std::string& side = context.getReader().getOptionalAttributeValue(SIDE, "");
            context.addEndTask([&generator, id, side]() {
                generator.setRegulatingTerminal(stdcxx::ref(TerminalRefXml::readTerminalRef(generator.getNetwork(), id, side)));
            });
        } else if (context.getReader().getLocalName() == REACTIVE_CAPABILITY_CURVE ||
                   context.getReader().getLocalName() == MIN_MAX_REACTIVE_LIMITS) {
            ReactiveLimitsXml::getInstance().read(generator, context);
        } else {
            AbstractConnectableXml::readSubElements(generator, context);
        }
    });
}

void GeneratorXml::writeRootElementAttributes(const Generator& generator, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(ENERGY_SOURCE, Enum::toString(generator.getEnergySource()));
    context.getWriter().writeAttribute(MIN_P, generator.getMinP());
    context.getWriter().writeAttribute(MAX_P, generator.getMaxP());
    context.getWriter().writeOptionalAttribute(RATED_S, generator.getRatedS());
    context.getWriter().writeAttribute(VOLTAGE_REGULATOR_ON, generator.isVoltageRegulatorOn());
    context.getWriter().writeAttribute(TARGET_P, generator.getTargetP());
    context.getWriter().writeOptionalAttribute(TARGET_V, generator.getTargetV());
    context.getWriter().writeOptionalAttribute(TARGET_Q, generator.getTargetQ());
    writeNodeOrBus(generator.getTerminal(), context);
    writePQ(generator.getTerminal(), context.getWriter());
}

void GeneratorXml::writeSubElements(const Generator& generator, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    if (!stdcxx::areSame(generator.getTerminal(), generator.getRegulatingTerminal())) {
        TerminalRefXml::writeTerminalRef(generator.getRegulatingTerminal(), context, REGULATING_TERMINAL);
    }
    ReactiveLimitsXml::getInstance().write(generator, context);
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

