/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "VoltageAngleLimitXml.hpp"

#include <powsybl/AssertionError.hpp>
#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/Constants.hpp>

#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/PropertiesXml.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>

#include <powsybl/stdcxx/Properties.hpp>
#include <powsybl/xml/XmlStreamException.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

const VoltageAngleLimitXml& VoltageAngleLimitXml::getInstance(){
    static VoltageAngleLimitXml s_instance;
    return s_instance;
}

void VoltageAngleLimitXml::read(Network& network, NetworkXmlReaderContext& context) const {
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_11(), context.getVersion(), [&context, &network]() {
        const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));
        double lowLimit = context.getReader().getOptionalAttributeValue(LOW_LIMIT, stdcxx::nan());
        double highLimit = context.getReader().getOptionalAttributeValue(HIGH_LIMIT, stdcxx::nan());

        VoltageAngleLimitAdder adder = network.newVoltageAngleLimit();
        adder.setId(id)
            .setLowLimit(lowLimit)
            .setHighLimit(highLimit);

        context.getReader().readUntilEndElement(VOLTAGE_ANGLE_LIMIT, [&adder, &network, &context]() {
            if (context.getReader().getLocalName() == PROPERTY) {
                PropertiesXml::read(adder, context);
            } else if (context.getReader().getLocalName() == FROM) {
                adder.from(stdcxx::ref<Terminal>(TerminalRefXml::readTerminal(network, context)));
            } else if (context.getReader().getLocalName() == TO) {
                adder.to(stdcxx::ref<Terminal>(TerminalRefXml::readTerminal(network, context)));
            } else {
                throw PowsyblException(stdcxx::format("Unsupported element '%1%' in 'voltageAngleLimit'", context.getReader().getLocalName()));
            }
        });

        adder.add();
    });
}

void VoltageAngleLimitXml::write(const VoltageAngleLimit& limit, const Network& network, NetworkXmlWriterContext& context) const {
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_11(), context.getVersion(), [&limit, &context, &network]() {
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), VOLTAGE_ANGLE_LIMIT);

        context.getWriter().writeAttribute(ID, context.getAnonymizer().anonymizeString(limit.getId()));
        context.getWriter().writeOptionalAttribute(LOW_LIMIT, limit.getLowLimit());
        context.getWriter().writeOptionalAttribute(HIGH_LIMIT, limit.getHighLimit());

        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_16(), context.getVersion(), [&context, &limit](){
            PropertiesXml::write(limit, context);
        });

        TerminalRefXml::writeTerminalRef(limit.getTerminalFrom(), context, FROM);
        TerminalRefXml::writeTerminalRef(limit.getTerminalTo(), context, TO);

        context.getWriter().writeEndElement();
    });
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
