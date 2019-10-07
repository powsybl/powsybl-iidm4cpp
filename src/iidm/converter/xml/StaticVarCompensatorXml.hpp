/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTOR_XML_STATICVARCOMPENSATORXML_HPP
#define POWSYBL_IIDM_CONVERTOR_XML_STATICVARCOMPENSATORXML_HPP

#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/StaticVarCompensatorAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

#include "AbstractConnectableXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class StaticVarCompensatorXml : public AbstractConnectableXml<StaticVarCompensator, StaticVarCompensatorAdder, VoltageLevel> {
public:
    static const StaticVarCompensatorXml& getInstance();

protected:  // AbstractIdentifiableXml
    StaticVarCompensatorAdder createAdder(VoltageLevel& voltageLevel) const override;

    const char* getRootElementName() const override;

    StaticVarCompensator& readRootElementAttributes(StaticVarCompensatorAdder& adder, NetworkXmlReaderContext& context) const override;

    void readSubElements(StaticVarCompensator& svc, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const StaticVarCompensator& svc, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

private:
    StaticVarCompensatorXml() = default;

    ~StaticVarCompensatorXml() noexcept override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTOR_XML_STATICVARCOMPENSATORXML_HPP

