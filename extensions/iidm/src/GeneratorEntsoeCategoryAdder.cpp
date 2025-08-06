/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/GeneratorEntsoeCategoryAdder.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorEntsoeCategory.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorEntsoeCategoryAdder::GeneratorEntsoeCategoryAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> GeneratorEntsoeCategoryAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Generator>(extendable)) {
        return stdcxx::make_unique<GeneratorEntsoeCategory>(dynamic_cast<Generator&>(extendable), m_code);
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
}

GeneratorEntsoeCategoryAdder& GeneratorEntsoeCategoryAdder::withCode(unsigned long code) {
    m_code = code;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
