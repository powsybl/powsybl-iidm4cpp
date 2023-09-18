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
#include <powsybl/iidm/LoadExponentialModel.hpp>
#include <powsybl/iidm/LoadZipModel.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/Constants.hpp>

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
    const auto& loadType = loadTypeName.empty() ? LoadType::UNDEFINED : Enum::fromString<LoadType>(loadTypeName);
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
        if (context.getReader().getLocalName() == ZIP_MODEL) {
            IidmXmlUtil::assertMinimumVersion(LOAD, ZIP_MODEL, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_10(), context);
            const auto& c0p = context.getReader().getAttributeValue<double>(C0P);
            const auto& c1p = context.getReader().getAttributeValue<double>(C1P);
            const auto& c2p = context.getReader().getAttributeValue<double>(C2P);
            const auto& c0q = context.getReader().getAttributeValue<double>(C0Q);
            const auto& c1q = context.getReader().getAttributeValue<double>(C1Q);
            const auto& c2q = context.getReader().getAttributeValue<double>(C2Q);
            load.setModel(LoadZipModel::build(load, c0p, c1p, c2p, c0q, c1q, c2q));
        } else if (context.getReader().getLocalName() == EXPONENTIAL_MODEL) {
            IidmXmlUtil::assertMinimumVersion(LOAD, EXPONENTIAL_MODEL, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_10(), context);
            const auto& np = context.getReader().getAttributeValue<double>(NP);
            const auto& nq = context.getReader().getAttributeValue<double>(NQ);
            load.setModel(LoadExponentialModel::build(load, np, nq));
        } else {
            AbstractConnectableXml::readSubElements(load, context);
        }
    });
}

void LoadXml::writeRootElementAttributes(const Load& load, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(LOAD_TYPE, Enum::toString(load.getLoadType()));
    context.getWriter().writeOptionalAttribute(P0, load.getP0());
    context.getWriter().writeOptionalAttribute(Q0, load.getQ0());
    writeNodeOrBus(load.getTerminal(), context);
    writePQ(load.getTerminal(), context.getWriter());
}

void LoadXml::writeSubElements(const Load& load, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_10(), context.getVersion(), [&context, &load]() {
        if (LoadModelType::ZIP == load.getModelType()) {
            context.getWriter().writeStartElement(context.getVersion().getPrefix(), ZIP_MODEL);
            context.getWriter().writeAttribute(C0P, load.getModel<LoadZipModel>().getC0p());
            context.getWriter().writeAttribute(C1P, load.getModel<LoadZipModel>().getC1p());
            context.getWriter().writeAttribute(C2P, load.getModel<LoadZipModel>().getC2p());
            context.getWriter().writeAttribute(C0Q, load.getModel<LoadZipModel>().getC0q());
            context.getWriter().writeAttribute(C1Q, load.getModel<LoadZipModel>().getC1q());
            context.getWriter().writeAttribute(C2Q, load.getModel<LoadZipModel>().getC2q());
            context.getWriter().writeEndElement();
        } else if (LoadModelType::EXPONENTIAL == load.getModelType()) {
            context.getWriter().writeStartElement(context.getVersion().getPrefix(), EXPONENTIAL_MODEL);
            context.getWriter().writeAttribute(NP, load.getModel<LoadExponentialModel>().getNp());
            context.getWriter().writeAttribute(NQ, load.getModel<LoadExponentialModel>().getNq());
            context.getWriter().writeEndElement();
        }
    });
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

