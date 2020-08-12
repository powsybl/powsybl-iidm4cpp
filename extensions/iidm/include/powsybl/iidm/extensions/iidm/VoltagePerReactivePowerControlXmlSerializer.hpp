/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEPERREACTIVEPOWERCONTROLXMLSERIALIZER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEPERREACTIVEPOWERCONTROLXMLSERIALIZER_HPP

#include <powsybl/iidm/converter/xml/AbstractExtensionXmlSerializer.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class VoltagePerReactivePowerControlXmlSerializer : public converter::xml::AbstractExtensionXmlSerializer {
public:  // ExtensionXmlSerializer
    Extension& read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const override;

    void write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const override;

public:
    VoltagePerReactivePowerControlXmlSerializer();

    ~VoltagePerReactivePowerControlXmlSerializer() noexcept override = default;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_VOLTAGEPERREACTIVEPOWERCONTROLXMLSERIALIZER_HPP
