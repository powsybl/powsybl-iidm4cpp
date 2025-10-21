/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/sld/OperatingStatusAdder.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Identifiable.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

OperatingStatusAdder::OperatingStatusAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> OperatingStatusAdder::createExtension(Extendable& extendable) const {
    if (!m_status) {
        throw PowsyblException("Undefined status value");
    }
    if (stdcxx::isInstanceOf<Identifiable>(extendable)) {
        return std::unique_ptr<OperatingStatus>(new OperatingStatus(dynamic_cast<Identifiable&>(extendable), *m_status));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Identifiable>()));
}

OperatingStatusAdder& OperatingStatusAdder::withStatus(const OperatingStatus::Status& status) {
    m_status = status;
    return *this;
}

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
