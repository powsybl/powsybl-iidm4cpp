/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTOR_XML_ABSTRACTSHUNTCOMPENSATORXML_HPP
#define POWSYBL_IIDM_CONVERTOR_XML_ABSTRACTSHUNTCOMPENSATORXML_HPP

#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ShuntCompensatorAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/xml/AbstractComplexIdentifiableXml.hpp>
#include <powsybl/iidm/converter/xml/AbstractConnectableXml.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class AbstractShuntCompensatorXml : public AbstractComplexIdentifiableXml<ShuntCompensator, ShuntCompensatorAdder, VoltageLevel>, public AbstractConnectableXml {
protected:  
    // AbstractIdentifiableXml
    ShuntCompensatorAdder createAdder(VoltageLevel& voltageLevel) const override;

    // AbstractComplexIdentifiableXml
    void readRootElementAttributes(ShuntCompensatorAdder& adder, std::vector<std::function<void(Identifiable&)>>& toApply ,NetworkXmlReaderContext& context) const override;

    // AbstractComplexIdentifiableXml
    void readSubElements(const std::string& id, ShuntCompensatorAdder& adder, std::vector<std::function<void(Identifiable&)>>& toApply, NetworkXmlReaderContext& context) const override;

    // AbstractComplexIdentifiableXml
    void readAndPostponeCreation(VoltageLevel& voltageLevel, NetworkXmlReaderContext& context) const override;

    // AbstractIdentifiableXml
    void writeRootElementAttributes(const ShuntCompensator& shuntCompensator, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const ShuntCompensator& sc, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

    //AbstractShuntCompensatorXml
    virtual void assertReadCompatibilityVersion(NetworkXmlReaderContext& context) const = 0;
    virtual void assertWriteCompatibilityVersion(NetworkXmlWriterContext& context) const = 0;

private:
    static void writeModel(const ShuntCompensator& sc, NetworkXmlWriterContext& context);

    void readShuntLinearModel(const std::string& id, ShuntCompensatorAdder& adder, NetworkXmlReaderContext& context) const;
    void readNonLinearShuntModel(const std::string& id, ShuntCompensatorAdder& adder, NetworkXmlReaderContext& context) const;

protected:
    AbstractShuntCompensatorXml() = default;

    ~AbstractShuntCompensatorXml() noexcept override = default;

};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTOR_XML_ABSTRACTSHUNTCOMPENSATORXML_HPP

