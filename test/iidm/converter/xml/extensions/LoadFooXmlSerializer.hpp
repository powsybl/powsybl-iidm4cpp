/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_LOADFOOXMLSERIALIZER_HPP
#define POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_LOADFOOXMLSERIALIZER_HPP

#include <powsybl/iidm/converter/xml/AbstractExtensionXmlSerializer.hpp>

namespace powsybl {

namespace iidm {

class Extendable;

namespace converter {

namespace xml {

namespace extensions {

class LoadFooXmlSerializer : public AbstractExtensionXmlSerializer {
public:  // ExtensionXmlSerializer
    std::unique_ptr<Extension> read(Extendable& extendable, NetworkXmlReaderContext& context) const override;

    void write(const Extension& extension, NetworkXmlWriterContext& context) const override;

public:
    LoadFooXmlSerializer();

    ~LoadFooXmlSerializer() noexcept override = default;
};

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_LOADFOOXMLSERIALIZER_HPP
