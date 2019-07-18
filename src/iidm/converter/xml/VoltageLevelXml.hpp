/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_VOLTAGELEVELXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_VOLTAGELEVELXML_HPP

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

#include "AbstractIdentifiableXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class VoltageLevelXml : public AbstractIdentifiableXml<VoltageLevel, VoltageLevelAdder, Substation> {
public:
    static VoltageLevelXml& instance();

protected:
    VoltageLevelAdder createAdder(Substation& substation) const override;

    const char* getRootElementName() const override;

    bool hasSubElements(const VoltageLevel& voltageLevel) const override;

    VoltageLevel& readRootElementAttributes(VoltageLevelAdder& adder, const NetworkXmlReaderContext& context) const override;

    void readSubElements(VoltageLevel& voltageLevel, const NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const VoltageLevel& voltageLevel, const Substation& substation, NetworkXmlWriterContext& context) const override;

    void writeSubElements(const VoltageLevel& voltageLevel, const Substation& substation, NetworkXmlWriterContext& context) const override;

private:
    VoltageLevelXml() = default;

    ~VoltageLevelXml() = default;

    void writeBusBreakerTopology(const VoltageLevel& voltageLevel, NetworkXmlWriterContext context) const;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_VOLTAGELEVELXML_HPP

