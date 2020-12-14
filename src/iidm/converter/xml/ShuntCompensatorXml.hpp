/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTOR_XML_SHUNTCOMPENSATORXML_HPP
#define POWSYBL_IIDM_CONVERTOR_XML_SHUNTCOMPENSATORXML_HPP

#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "AbstractConnectableXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class ShuntCompensatorXml : public AbstractConnectableXml<ShuntCompensator, ShuntCompensatorAdder, VoltageLevel> {
public:
    static const ShuntCompensatorXml& getInstance();

protected:  // AbstractIdentifiableXml
    ShuntCompensatorAdder createAdder(VoltageLevel& voltageLevel) const override;

    const char* getRootElementName() const override;

    void readElement(const std::string& id, ShuntCompensatorAdder& adder, NetworkXmlReaderContext& context) const override;

    ShuntCompensator& readRootElementAttributes(ShuntCompensatorAdder& adder, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const ShuntCompensator& shuntCompensator, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const ShuntCompensator& sc, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

private:
    static void writeModel(const ShuntCompensator& sc, NetworkXmlWriterContext& context);

private:
    ShuntCompensatorXml() = default;

    ~ShuntCompensatorXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTOR_XML_SHUNTCOMPENSATORXML_HPP

