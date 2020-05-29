/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_EXTENSIONXMLSERIALIZER_HXX
#define POWSYBL_IIDM_CONVERTER_XML_EXTENSIONXMLSERIALIZER_HXX

#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename E>
const E& ExtensionXmlSerializer::safeCast(const Extension& extension) const {
    if (!stdcxx::isInstanceOf<E>(extension)) {
        throw PowsyblException(stdcxx::format("Unexpected extension type: %1% (%2% expected)", stdcxx::demangle(extension), stdcxx::demangle<E>()));
    }

    return dynamic_cast<const E&>(extension);
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_EXTENSIONXMLSERIALIZER_HXX
