/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "AreaXml.hpp"

#include "AreaBoundaryXml.hpp"
#include "VoltageLevelRefXml.hpp"
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/Constants.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

AreaAdder AreaXml::createAdder(Network& network) const {
    return network.newArea();
}

const AreaXml& AreaXml::getInstance() {
    static AreaXml s_instance;
    return s_instance;
}

const char* AreaXml::getRootElementName() const {
    return AREA;
}

Area& AreaXml::readRootElementAttributes(AreaAdder& adder, Network& /*network*/, NetworkXmlReaderContext& context) const {
    const std::string& areaType = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(AREA_TYPE));
    adder.setAreaType(areaType);
    double interchangeTarget = context.getReader().getOptionalAttributeValue(INTERCHANGE_TARGET, stdcxx::nan());
    adder.setInterchangeTarget(interchangeTarget);

    return adder.add();
}

void AreaXml::readSubElements(Area& area, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(AREA, [this, &area, &context]() {
        if (context.getReader().getLocalName() == VOLTAGE_LEVEL_REF) {
            VoltageLevelRefXml::readVoltageLevelRef(area.getNetwork(), context, [&area](VoltageLevel& voltageLevel){
                area.addVoltageLevel(voltageLevel);
            });
        } else if(context.getReader().getLocalName() == AREA_BOUNDARY) {
            AreaBoundaryXml::getInstance().read(area, context);
        } else {
            AbstractSimpleIdentifiableXml::readSubElements(area, context);
        }
    });
}

void AreaXml::writeRootElementAttributes(const Area& area, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(AREA_TYPE, context.getAnonymizer().anonymizeString(area.getAreaType()));
    context.getWriter().writeAttribute(INTERCHANGE_TARGET,area.getInterchangeTarget());
}

void AreaXml::writeSubElements(const Area& area, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    std::vector<stdcxx::CReference<VoltageLevel>> voltageLevels;
    for(const auto& vl : area.getVoltageLevels()) {
        voltageLevels.push_back(stdcxx::cref(vl));
    }
    std::sort(voltageLevels.begin(), voltageLevels.end(), [](const VoltageLevel& vl1, const VoltageLevel& vl2) {
        return vl1.getId() < vl2.getId();
    });
    for (const auto& vl : voltageLevels) {
        VoltageLevelRefXml::writeVoltageLevelRef(vl.get(), context);
    }
    AreaBoundaryXml::getInstance().write(area, context);
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

