/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "DcNodeXml.hpp"

#include <powsybl/iidm/converter/Constants.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

DcNodeAdder DcNodeXml::createAdder(Network& network) const {
    return network.newDcNode();
}

const DcNodeXml& DcNodeXml::getInstance() {
    static DcNodeXml s_instance;
    return s_instance;
}

const char* DcNodeXml::getRootElementName() const {
    return DC_NODE;
}

DcNode& DcNodeXml::readRootElementAttributes(DcNodeAdder& adder, Network& /*network*/, NetworkXmlReaderContext& context) const {
    const auto& nominalV = context.getReader().getAttributeValue<double>(NOMINAL_V);
    const double& v = context.getReader().getOptionalAttributeValue(V, stdcxx::nan());

    DcNode& dcNode = adder.setNominalV(nominalV)
                        .add();
    dcNode.setV(v);

    return dcNode;
}

void DcNodeXml::readSubElements(DcNode& dcNode, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(DC_NODE, [this, &dcNode, &context]() {
        AbstractSimpleIdentifiableXml::readSubElements(dcNode, context);
    });
}

void DcNodeXml::writeRootElementAttributes(const DcNode& dcNode, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(NOMINAL_V, dcNode.getNominalV());
    context.getWriter().writeOptionalAttribute(V, dcNode.getV());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

