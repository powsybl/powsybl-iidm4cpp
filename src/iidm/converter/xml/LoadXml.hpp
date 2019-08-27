/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_LOADXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_LOADXML_HPP

#include "AbstractConnectableXml.hpp"

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class LoadXml : public AbstractConnectableXml<Load, LoadAdder, VoltageLevel> {
public:
    static const LoadXml& getInstance();

protected:  // AbstractIdentifiableXml
    LoadAdder createAdder(VoltageLevel& voltageLevel) const override;

    const char* getRootElementName() const override;

    bool hasSubElements(const Load& load) const override;

    Load& readRootElementAttributes(LoadAdder& loadAdder, const NetworkXmlReaderContext& context) const override;

    void readSubElements(Load& load, NetworkXmlReaderContext& context) const override;

    void writeRootElementAttributes(const Load& load, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const override;

private:
    LoadXml() = default;

    ~LoadXml() override = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_LOADXML_HPP

