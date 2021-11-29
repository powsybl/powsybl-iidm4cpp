/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_GENERATORXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_GENERATORXML_HPP

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class GeneratorXml : public AbstractConnectableXml<Generator, GeneratorAdder, VoltageLevel> {
public:
    static const GeneratorXml& getInstance();

protected:  // AbstractIdentifiableXml
    GeneratorAdder createAdder(VoltageLevel& voltageLevel) const override;

    const char* getRootElementName() const override;

    Generator& readRootElementAttributes(GeneratorAdder& generatorAdder, NetworkXmlReaderContext& context) const override;

    void readSubElements(Generator& generator, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const Generator& generator, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const Generator& generator, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

private:
    GeneratorXml() = default;

    ~GeneratorXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_GENERATORXML_HPP

