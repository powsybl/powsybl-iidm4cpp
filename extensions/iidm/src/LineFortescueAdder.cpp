/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/LineFortescueAdder.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/extensions/iidm/LineFortescue.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

LineFortescueAdder::LineFortescueAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> LineFortescueAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Line>(extendable)) {
        return stdcxx::make_unique<LineFortescue>(dynamic_cast<Line&>(extendable), m_rz, m_xz);
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Line>()));
}

LineFortescueAdder& LineFortescueAdder::withRz(double rz) {
    m_rz = rz;
    return *this;
}
LineFortescueAdder& LineFortescueAdder::withXz(double xz) {
    m_xz = xz;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
