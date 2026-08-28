/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "AbstractShuntCompensatorXml.hpp"

#include <map>

#include <powsybl/iidm/ShuntCompensatorLinearModel.hpp>
#include <powsybl/iidm/ShuntCompensatorNonLinearModel.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

ShuntCompensatorAdder AbstractShuntCompensatorXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newShuntCompensator();
}

void AbstractShuntCompensatorXml::readAndPostponeCreation(VoltageLevel& /*voltageLevel*/, NetworkXmlReaderContext& /*context*/) const {
    throw AssertionError("Not implemented");
}

void AbstractShuntCompensatorXml::readRootElementAttributes(ShuntCompensatorAdder& adder, std::vector<std::function<void(Identifiable&)>>& toApply ,NetworkXmlReaderContext& context) const {
    assertReadCompatibilityVersion(context);

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, &adder]() {
        bool voltageRegulatorOn = context.getReader().getOptionalAttributeValue(VOLTAGE_REGULATOR_ON, false);
        double targetV = context.getReader().getOptionalAttributeValue(TARGET_V, stdcxx::nan());
        double targetDeadband = context.getReader().getOptionalAttributeValue(TARGET_DEADBAND, stdcxx::nan());
        adder.setVoltageRegulatorOn(voltageRegulatorOn)
            .setTargetV(targetV)
            .setTargetDeadband(targetDeadband);
    });
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_1(), context.getVersion(), [&adder](){ adder.setVoltageRegulatorOn(false); });
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
        auto sectionCount = context.getReader().getOptionalAttributeValue<unsigned long>(SECTION_COUNT);
        if(sectionCount.has_value()) {
            adder.setSectionCount(*sectionCount);
        }
    });
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_14(), context.getVersion(), [&context, &adder]() {
        auto solvedSectionCount = context.getReader().getOptionalAttributeValue<unsigned long>(SOLVED_SECTION_COUNT);
        if(solvedSectionCount.has_value()) {
            adder.setSolvedSectionCount(*solvedSectionCount);
        }
    });
    readNodeOrBus(adder, context);
    double p = context.getReader().getOptionalAttributeValue(P, stdcxx::nan());
    double q = context.getReader().getOptionalAttributeValue(Q, stdcxx::nan());

    std::function<void(Identifiable&)> fun = [p, q](Identifiable &shuntCompensator) {
        ShuntCompensator& shunt = dynamic_cast<ShuntCompensator&>(shuntCompensator);
        shunt.getTerminal().setP(p).setQ(q);
    };

    toApply.emplace_back(fun);
}

void AbstractShuntCompensatorXml::readSubElements(const std::string& id, ShuntCompensatorAdder& adder, std::vector<std::function<void(Identifiable&)>>& toApply, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(getRootElementName(), [this, &toApply, &context, &adder, &id]() {
        if (context.getReader().getLocalName() == REGULATING_TERMINAL) {
            std::string regId = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));
            std::string regSide = context.getReader().getOptionalAttributeValue(SIDE, "");
            std::string regNumber = context.getReader().getOptionalAttributeValue(NUMBER, "");

            if (!regId.empty()) {
                std::function<void(Identifiable&)> fun = [&context, regId, regSide, regNumber](Identifiable &shuntCompensator) {
                    ShuntCompensator& sc = dynamic_cast<ShuntCompensator&>(shuntCompensator);
                    context.addEndTask(XmlReaderEndTask::Step::AFTER_EXTENSIONS, [&sc, regId, regSide, regNumber]() {
                        sc.setRegulatingTerminal(stdcxx::ref(TerminalRefXml::resolve(regId, regSide, regNumber, sc.getNetwork())));
                    });
                };
                toApply.emplace_back(fun);
            }
        } else if (context.getReader().getLocalName() == SHUNT_LINEAR_MODEL) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), SHUNT_LINEAR_MODEL, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
            readShuntLinearModel(id, adder, context);
        } else if (context.getReader().getLocalName() == SHUNT_NON_LINEAR_MODEL) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), SHUNT_NON_LINEAR_MODEL, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
            readNonLinearShuntModel(id, adder, context);
        } else {
            AbstractComplexIdentifiableXml::readSubElements(id, toApply, context);
        }
    });
}

void AbstractShuntCompensatorXml::readShuntLinearModel(const std::string& id, ShuntCompensatorAdder& adder, NetworkXmlReaderContext& context) const {
    auto bPerSection = context.getReader().getAttributeValue<double>(B_PER_SECTION);
    double gPerSection = context.getReader().getOptionalAttributeValue(G_PER_SECTION, stdcxx::nan());
    auto maximumSectionCount = context.getReader().getAttributeValue<unsigned long>(MAXIMUM_SECTION_COUNT);
    ShuntCompensatorAdder::ShuntCompensatorLinearModelAdder linearAdder = adder.newLinearModel();
    linearAdder.setBPerSection(bPerSection)
            .setGPerSection(gPerSection)
            .setMaximumSectionCount(maximumSectionCount);
    context.getReader().readUntilEndElement(SHUNT_LINEAR_MODEL, [&context, &linearAdder, &id]() {
        if(context.getReader().getLocalName() == PROPERTY) {
            PropertiesXml::read(linearAdder, context);
        } else {
            throw PowsyblException(stdcxx::format("Unknown element name <%1%> in <%2%>/<%3%>", context.getReader().getLocalName(), id, SHUNT_LINEAR_MODEL));
        }
    });
    linearAdder.add();
}
void AbstractShuntCompensatorXml::readNonLinearShuntModel(const std::string& id, ShuntCompensatorAdder& adder, NetworkXmlReaderContext& context) const {
    ShuntCompensatorAdder::ShuntCompensatorNonLinearModelAdder modelAdder = adder.newNonLinearModel();
    context.getReader().readUntilEndElement(SHUNT_NON_LINEAR_MODEL, [&context, &modelAdder, &id]() {
        if (context.getReader().getLocalName() == SECTION) {
            auto b = context.getReader().getAttributeValue<double>(B);
            auto g = context.getReader().getAttributeValue<double>(G);
            ShuntCompensatorAdder::ShuntCompensatorNonLinearModelAdder::SectionAdder sectionAdder = modelAdder.beginSection();
            sectionAdder.setB(b)
                        .setG(g);
            context.getReader().readUntilEndElement(SECTION, [&context, &sectionAdder, &id]() {
                if(context.getReader().getLocalName() == PROPERTY) {
                    PropertiesXml::read(sectionAdder, context);
                } else {
                    throw PowsyblException(stdcxx::format("Unknown element name <%1%> in <%2%>/<%3%>/<%4%>", context.getReader().getLocalName(), id, SHUNT_NON_LINEAR_MODEL, SECTION));
                }
            });
            sectionAdder.endSection();
        } else if(context.getReader().getLocalName() == PROPERTY) {
            PropertiesXml::read(modelAdder, context);
        } else {
            throw PowsyblException(stdcxx::format("Unknown element name <%1%> in <%2%>/<%3%>", context.getReader().getLocalName(), id, SHUNT_NON_LINEAR_MODEL));
        }
    });
    modelAdder.add();
}

void AbstractShuntCompensatorXml::writeModel(const ShuntCompensator& sc, NetworkXmlWriterContext& context) {
    if (sc.getModelType() == ShuntCompensatorModelType::LINEAR) {
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), SHUNT_LINEAR_MODEL);
        double bPerSection = sc.getModel<ShuntCompensatorLinearModel>().getBPerSection();
        if(bPerSection == 0.0 && context.getVersion() <= IidmXmlVersion::V1_4()) {
            logging::Logger& logger = logging::LoggerFactory::getLogger<AbstractShuntCompensatorXml>();
            logger.warn(stdcxx::format("bPerSection of %1% is 0. It is set as %2% since XIIDM version < 1.5 (%3%)", sc.getId(), 
                std::numeric_limits<double>::min(), context.getVersion().toString(".")));
            bPerSection = std::numeric_limits<double>::min();
        }
        context.getWriter().writeAttribute(B_PER_SECTION, bPerSection);
        context.getWriter().writeAttribute(G_PER_SECTION, sc.getModel<ShuntCompensatorLinearModel>().getGPerSection());
        context.getWriter().writeAttribute(MAXIMUM_SECTION_COUNT, sc.getMaximumSectionCount());
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_16(), context.getVersion(), [&context, &sc](){
            PropertiesXml::write(sc.getModel<ShuntCompensatorLinearModel>(), context);
        });
        context.getWriter().writeEndElement();
    } else if (sc.getModelType() == ShuntCompensatorModelType::NON_LINEAR) {
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &sc]() {
            context.getWriter().writeStartElement(context.getVersion().getPrefix(), SHUNT_NON_LINEAR_MODEL);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_16(), context.getVersion(), [&context, &sc](){
                PropertiesXml::write(sc.getModel<ShuntCompensatorNonLinearModel>(), context);
            });
            for (const ShuntCompensatorNonLinearModel::Section& s : sc.getModel<ShuntCompensatorNonLinearModel>().getAllSections()) {
                context.getWriter().writeStartElement(context.getVersion().getPrefix(), SECTION);
                context.getWriter().writeAttribute(B, s.getB());
                context.getWriter().writeAttribute(G, s.getG());
                IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_16(), context.getVersion(), [&context, &s](){
                    PropertiesXml::write(s, context);
                });
                context.getWriter().writeEndElement();
            }
            context.getWriter().writeEndElement();
        });
    } else {
        throw PowsyblException(stdcxx::format("Unexpected shunt type %1% for shunt %2%", sc.getModelType(), sc.getId()));
    }
}

void AbstractShuntCompensatorXml::writeRootElementAttributes(const ShuntCompensator& shuntCompensator, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    assertWriteCompatibilityVersion(context);

    if (ShuntCompensatorModelType::NON_LINEAR == shuntCompensator.getModelType()) {
        IidmXmlUtil::assertMinimumVersion(getRootElementName(), SHUNT_NON_LINEAR_MODEL, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
    }
    stdcxx::optional<unsigned long> solvedSectionCount = shuntCompensator.getSolvedSectionCount();
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, &shuntCompensator, &solvedSectionCount]() {
        const ShuntCompensatorModel& model = shuntCompensator.getModel();
        double bPerSection = stdcxx::isInstanceOf<ShuntCompensatorLinearModel>(model) ? shuntCompensator.getModel<ShuntCompensatorLinearModel>().getBPerSection() : shuntCompensator.getB();
        if(bPerSection == 0.0) {
            logging::Logger& logger = logging::LoggerFactory::getLogger<AbstractShuntCompensatorXml>();
            logger.warn(stdcxx::format("bPerSection of %1% is 0. It is set as %2% since XIIDM version < 1.5 (%3%)", shuntCompensator.getId(), 
                std::numeric_limits<double>::min(), context.getVersion().toString(".")));
            bPerSection = std::numeric_limits<double>::min();
        }
        context.getWriter().writeAttribute(B_PER_SECTION, bPerSection);
        unsigned long maximumSectionCount = stdcxx::isInstanceOf<ShuntCompensatorLinearModel>(model) ? shuntCompensator.getMaximumSectionCount() : 1;
        context.getWriter().writeAttribute(MAXIMUM_SECTION_COUNT, maximumSectionCount);
        
        unsigned long currentSectionCount = 1;
        if(stdcxx::isInstanceOf<ShuntCompensatorLinearModel>(model)) {
            if(solvedSectionCount.has_value()) {
                currentSectionCount = solvedSectionCount.get();
            } else {
                currentSectionCount = shuntCompensator.getSectionCount();
            }
        }
        context.getWriter().writeAttribute(CURRENT_SECTION_COUNT, currentSectionCount);
    });
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &shuntCompensator, &solvedSectionCount]() {
        IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_13(), context.getVersion(), [&context, &shuntCompensator, &solvedSectionCount]() {
            unsigned long currentSectionCount = solvedSectionCount.has_value() ? solvedSectionCount.get() : shuntCompensator.getSectionCount();
            context.getWriter().writeAttribute(SECTION_COUNT, currentSectionCount);
        });
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_14(), context.getVersion(), [&context, &shuntCompensator, &solvedSectionCount]() {
            context.getWriter().writeAttribute(SECTION_COUNT, shuntCompensator.getSectionCount());
            if(solvedSectionCount.has_value()) {
                context.getWriter().writeAttribute(SOLVED_SECTION_COUNT, solvedSectionCount.get());
            }
        });
    });
    const std::string& rootElementName = getRootElementName();
    IidmXmlUtil::writeBooleanAttributeFromMinimumVersion(rootElementName, VOLTAGE_REGULATOR_ON, shuntCompensator.isVoltageRegulatorOn(), false, ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_2(), context);
    IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(rootElementName, TARGET_V, shuntCompensator.getTargetV(), ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_2(), context);
    IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(rootElementName, TARGET_DEADBAND, shuntCompensator.getTargetDeadband(), ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_2(), context);
    writeNodeOrBus(shuntCompensator.getTerminal(), context);
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_9(), context.getVersion(), [&context, &shuntCompensator]() { context.getWriter().writeOptionalAttribute(P, shuntCompensator.getTerminal().getP()); });
    context.getWriter().writeOptionalAttribute(Q, shuntCompensator.getTerminal().getQ());
}

void AbstractShuntCompensatorXml::writeSubElements(const ShuntCompensator& sc, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &sc]() { writeModel(sc, context); });
    if (!stdcxx::areSame(sc, sc.getRegulatingTerminal().getConnectable().get())) {
        IidmXmlUtil::assertMinimumVersion(getRootElementName(), REGULATING_TERMINAL, ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_2(), context);
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, &sc]() { TerminalRefXml::writeTerminalRef(sc.getRegulatingTerminal(), context, REGULATING_TERMINAL); });
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

