/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTACDCCONVERTERXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTACDCCONVERTERXML_HPP

#include <powsybl/iidm/AcDcConverter.hpp>
#include <powsybl/iidm/AcDcConverterAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>
#include <powsybl/iidm/converter/xml/AbstractSimpleIdentifiableXml.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace xml {

class XmlStreamReader;
class XmlStreamWriter;

}  // namespace xml

namespace iidm {

namespace converter {

namespace xml {

template <typename Added, typename Adder>
class AbstractAcDcConverterXml : public AbstractSimpleIdentifiableXml<Added, Adder, VoltageLevel>, public AbstractConnectableXml {
protected:
    AbstractAcDcConverterXml() = default;

    virtual ~AbstractAcDcConverterXml() noexcept override = default;


    virtual void readRootElementPqiAttributes(Added& converter, NetworkXmlReaderContext& context) const;

    virtual void writeRootElementPqiAttributes(const Added& converter, NetworkXmlWriterContext& context) const;

    virtual void readRootElementCommonAttributes(Adder& adder, NetworkXmlReaderContext& context) const;


protected:   //AbstractSimpleIdentifiableXml
    virtual void readSubElements(Added& converter, NetworkXmlReaderContext& context) const override;


protected:   //IdentifiableXml
    virtual void writeRootElementAttributes(const Added& converter, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

    virtual void writeSubElements(const Added& converter, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include "AbstractAcDcConverterXml.hxx"

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTACDCCONVERTERXML_HPP

