/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/entsoe/EntsoeAreaAdder.hpp>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/extensions/entsoe/EntsoeArea.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

EntsoeAreaAdder::EntsoeAreaAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> EntsoeAreaAdder::createExtension(Extendable& extendable) {
    if (!m_code) {
        throw PowsyblException(stdcxx::format("code is undefined"));
    }
    if (stdcxx::isInstanceOf<Substation>(extendable)) {
        return std::unique_ptr<EntsoeArea>(new EntsoeArea(dynamic_cast<Substation&>(extendable), *m_code));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Substation>()));
}

EntsoeAreaAdder& EntsoeAreaAdder::withCode(const EntsoeGeographicalCode& code) {
    m_code = code;
    return *this;
}

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
