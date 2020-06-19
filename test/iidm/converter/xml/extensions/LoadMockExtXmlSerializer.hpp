/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_LOADMOCKXMLSERIALIZER_HPP
#define POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_LOADMOCKXMLSERIALIZER_HPP

#include <powsybl/iidm/converter/xml/AbstractVersionableExtensionXmlSerializer.hpp>

#include "LoadMockExt.hpp"

namespace powsybl {

namespace iidm {

class Extendable;

namespace converter {

namespace xml {

namespace extensions {

class LoadMockExtXmlSerializer : public AbstractVersionableExtensionXmlSerializer {
public:  // ExtensionXmlSerializer
    LoadMockExt& read(Extendable& extendable, NetworkXmlReaderContext& context) const override;

    void write(const Extension& extension, NetworkXmlWriterContext& context) const override;

public:
    LoadMockExtXmlSerializer();

    ~LoadMockExtXmlSerializer() noexcept override = default;
};

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_EXTENSIONS_LOADMOCKXMLSERIALIZER_HPP
