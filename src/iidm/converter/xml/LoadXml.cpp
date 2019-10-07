/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "LoadXml.hpp"

#include <string>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "iidm/converter/Constants.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

LoadAdder LoadXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newLoad();
}

const LoadXml& LoadXml::getInstance() {
    static LoadXml s_instance;
    return s_instance;
}

const char* LoadXml::getRootElementName() const {
    return LOAD;
}

Load& LoadXml::readRootElementAttributes(LoadAdder& loadAdder, NetworkXmlReaderContext& context) const {
    const std::string& loadTypeName = context.getReader().getOptionalAttributeValue(LOAD_TYPE, "");
    LoadType loadType = loadTypeName.empty() ? LoadType::UNDEFINED : getLoadType(loadTypeName);
    const auto& p0 = context.getReader().getAttributeValue<double>(P0);
    const auto& q0 = context.getReader().getAttributeValue<double>(Q0);
    readNodeOrBus(loadAdder, context);
    Load& load = loadAdder.setLoadType(loadType)
        .setP0(p0)
        .setQ0(q0)
        .add();
    readPQ(load.getTerminal(), context.getReader());
    return load;
}

void LoadXml::readSubElements(Load& load, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(LOAD, [this, &load, &context]() {
        AbstractIdentifiableXml::readSubElements(load, context);
    });
}

void LoadXml::writeRootElementAttributes(const Load& load, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(LOAD_TYPE, getLoadTypeName(load.getLoadType()));
    context.getWriter().writeOptionalAttribute(P0, load.getP0());
    context.getWriter().writeOptionalAttribute(Q0, load.getQ0());
    writeNodeOrBus(load.getTerminal(), context);
    writePQ(load.getTerminal(), context.getWriter());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

