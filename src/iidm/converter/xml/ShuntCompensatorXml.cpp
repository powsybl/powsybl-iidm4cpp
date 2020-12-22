/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ShuntCompensatorXml.hpp"

#include <map>

#include <powsybl/iidm/ShuntCompensatorLinearModel.hpp>
#include <powsybl/iidm/ShuntCompensatorNonLinearModel.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

ShuntCompensatorAdder ShuntCompensatorXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newShuntCompensator();
}

const ShuntCompensatorXml& ShuntCompensatorXml::getInstance() {
    static ShuntCompensatorXml s_instance;
    return s_instance;
}

const char* ShuntCompensatorXml::getRootElementName() const {
    return SHUNT;
}

void ShuntCompensatorXml::readElement(const std::string& id, ShuntCompensatorAdder& adder, NetworkXmlReaderContext& context) const {
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, &adder]() {
        bool voltageRegulatorOn = context.getReader().getOptionalAttributeValue(VOLTAGE_REGULATOR_ON, false);
        double targetV = context.getReader().getOptionalAttributeValue(TARGET_V, stdcxx::nan());
        double targetDeadband = context.getReader().getOptionalAttributeValue(TARGET_DEADBAND, stdcxx::nan());
        adder.setVoltageRegulatorOn(voltageRegulatorOn)
            .setTargetV(targetV)
            .setTargetDeadband(targetDeadband);
    });
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, &adder]() {
        auto bPerSection = context.getReader().getAttributeValue<double>(B_PER_SECTION);
        auto maximumSectionCount = context.getReader().getAttributeValue<unsigned long>(MAXIMUM_SECTION_COUNT);
        auto sectionCount = context.getReader().getAttributeValue<unsigned long>(CURRENT_SECTION_COUNT);
        adder.setSectionCount(sectionCount);
        adder.newLinearModel()
            .setBPerSection(bPerSection)
            .setMaximumSectionCount(maximumSectionCount)
            .add();
    });
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &adder]() {
        auto sectionCount = context.getReader().getAttributeValue<unsigned long>(SECTION_COUNT);
        adder.setSectionCount(sectionCount);
    });
    readNodeOrBus(adder, context);
    double p = context.getReader().getOptionalAttributeValue(P, stdcxx::nan());
    double q = context.getReader().getOptionalAttributeValue(Q, stdcxx::nan());
    std::string regId;
    std::string regSide;
    stdcxx::Properties properties;
    std::map<std::string, std::string> aliases;
    context.getReader().readUntilEndElement(SHUNT, [&context, &adder, &regId, &regSide, &properties, &id, &aliases]() {
        if (context.getReader().getLocalName() == REGULATING_TERMINAL) {
            regId = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));
            regSide = context.getReader().getOptionalAttributeValue(SIDE, "");
        } else if (context.getReader().getLocalName() == PROPERTY) {
            const std::string& name = context.getReader().getAttributeValue(NAME);
            const std::string& value = context.getReader().getAttributeValue(VALUE);
            properties.set(name, value);
        } else if (context.getReader().getLocalName() == ALIAS) {
            IidmXmlUtil::assertMinimumVersion(SHUNT, ALIAS, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
            const auto& aliasType = context.getReader().getOptionalAttributeValue(TYPE, "");
            const auto& alias = context.getReader().readCharacters();
            aliases[alias] = aliasType;
        } else if (context.getReader().getLocalName() == SHUNT_LINEAR_MODEL) {
            IidmXmlUtil::assertMinimumVersion(SHUNT, SHUNT_LINEAR_MODEL, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
            auto bPerSection = context.getReader().getAttributeValue<double>(B_PER_SECTION);
            double gPerSection = context.getReader().getOptionalAttributeValue(G_PER_SECTION, stdcxx::nan());
            auto maximumSectionCount = context.getReader().getAttributeValue<unsigned long>(MAXIMUM_SECTION_COUNT);
            adder.newLinearModel()
                .setBPerSection(bPerSection)
                .setGPerSection(gPerSection)
                .setMaximumSectionCount(maximumSectionCount)
                .add();
        } else if (context.getReader().getLocalName() == SHUNT_NON_LINEAR_MODEL) {
            IidmXmlUtil::assertMinimumVersion(SHUNT, SHUNT_NON_LINEAR_MODEL, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
            ShuntCompensatorAdder::ShuntCompensatorNonLinearModelAdder modelAdder = adder.newNonLinearModel();
            context.getReader().readUntilEndElement(SHUNT_NON_LINEAR_MODEL, [&context, &modelAdder, &id]() {
                if (context.getReader().getLocalName() == SECTION) {
                    auto b = context.getReader().getAttributeValue<double>(B);
                    auto g = context.getReader().getAttributeValue<double>(G);
                    modelAdder.beginSection()
                        .setB(b)
                        .setG(g)
                        .endSection();
                } else {
                    throw PowsyblException(stdcxx::format("Unknown element name <%1%> in <%2%>", context.getReader().getLocalName(), id));
                }
            });
            modelAdder.add();
        } else {
            throw PowsyblException(stdcxx::format("Unknown element name <%1%> in <%2%>", context.getReader().getLocalName(), id));
        }
    });

    ShuntCompensator& sc = adder.add();
    if (!regId.empty()) {
        context.addEndTask([&sc, regId, regSide]() {
            sc.setRegulatingTerminal(stdcxx::ref(TerminalRefXml::readTerminalRef(sc.getNetwork(), regId, regSide)));
        });
    }

    for (const auto& it : properties) {
        sc.setProperty(it.first, it.second);
    }
    for (const auto& alias : aliases) {
        sc.addAlias(alias.first, alias.second);
    }
    sc.getTerminal().setP(p).setQ(q);
}

ShuntCompensator& ShuntCompensatorXml::readRootElementAttributes(ShuntCompensatorAdder& /*adder*/, NetworkXmlReaderContext& /*context*/) const {
    throw PowsyblException("Unsupported operation");
}

void ShuntCompensatorXml::writeModel(const ShuntCompensator& sc, NetworkXmlWriterContext& context) {
    if (sc.getModelType() == ShuntCompensatorModelType::LINEAR) {
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), SHUNT_LINEAR_MODEL);
        context.getWriter().writeAttribute(B_PER_SECTION, sc.getModel<ShuntCompensatorLinearModel>().getBPerSection());
        context.getWriter().writeAttribute(G_PER_SECTION, sc.getModel<ShuntCompensatorLinearModel>().getGPerSection());
        context.getWriter().writeAttribute(MAXIMUM_SECTION_COUNT, sc.getMaximumSectionCount());
        context.getWriter().writeEndElement();
    } else if (sc.getModelType() == ShuntCompensatorModelType::NON_LINEAR) {
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &sc]() {
            context.getWriter().writeStartElement(context.getVersion().getPrefix(), SHUNT_NON_LINEAR_MODEL);
            for (const ShuntCompensatorNonLinearModel::Section& s : sc.getModel<ShuntCompensatorNonLinearModel>().getAllSections()) {
                context.getWriter().writeStartElement(context.getVersion().getPrefix(), SECTION);
                context.getWriter().writeAttribute(B, s.getB());
                context.getWriter().writeAttribute(G, s.getG());
                context.getWriter().writeEndElement();
            }
            context.getWriter().writeEndElement();
        });
    } else {
        throw PowsyblException(stdcxx::format("Unexpected shunt type %1% for shunt %2%", sc.getModelType(), sc.getId()));
    }
}

void ShuntCompensatorXml::writeRootElementAttributes(const ShuntCompensator& shuntCompensator, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    if (ShuntCompensatorModelType::NON_LINEAR == shuntCompensator.getModelType()) {
        IidmXmlUtil::assertMinimumVersion(getRootElementName(), SHUNT_NON_LINEAR_MODEL, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
    }
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, &shuntCompensator]() {
        const ShuntCompensatorModel& model = shuntCompensator.getModel();
        double bPerSection = stdcxx::isInstanceOf<ShuntCompensatorLinearModel>(model) ? shuntCompensator.getModel<ShuntCompensatorLinearModel>().getBPerSection() : shuntCompensator.getB();
        context.getWriter().writeAttribute(B_PER_SECTION, bPerSection);
        unsigned long maximumSectionCount = stdcxx::isInstanceOf<ShuntCompensatorLinearModel>(model) ? shuntCompensator.getMaximumSectionCount() : 1;
        context.getWriter().writeAttribute(MAXIMUM_SECTION_COUNT, maximumSectionCount);
        unsigned long currentSectionCount = stdcxx::isInstanceOf<ShuntCompensatorLinearModel>(model) ? shuntCompensator.getSectionCount() : 1;
        context.getWriter().writeAttribute(CURRENT_SECTION_COUNT, currentSectionCount);
    });
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &shuntCompensator]() { context.getWriter().writeAttribute(SECTION_COUNT, shuntCompensator.getSectionCount()); });
    IidmXmlUtil::writeBooleanAttributeFromMinimumVersion(SHUNT, VOLTAGE_REGULATOR_ON, shuntCompensator.isVoltageRegulatorOn(), false, ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_2(), context);
    IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(SHUNT, TARGET_V, shuntCompensator.getTargetV(), ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_2(), context);
    IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(SHUNT, TARGET_DEADBAND, shuntCompensator.getTargetDeadband(), ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_2(), context);
    writeNodeOrBus(shuntCompensator.getTerminal(), context);
    writePQ(shuntCompensator.getTerminal(), context.getWriter());
}

void ShuntCompensatorXml::writeSubElements(const ShuntCompensator& sc, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &sc]() { writeModel(sc, context); });
    if (!stdcxx::areSame(sc, sc.getRegulatingTerminal().getConnectable().get())) {
        IidmXmlUtil::assertMinimumVersion(SHUNT, REGULATING_TERMINAL, ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_2(), context);
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, &sc]() { TerminalRefXml::writeTerminalRef(sc.getRegulatingTerminal(), context, REGULATING_TERMINAL); });
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

