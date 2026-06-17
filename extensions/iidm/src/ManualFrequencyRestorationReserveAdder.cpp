/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ManualFrequencyRestorationReserveAdder.hpp>

#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/extensions/iidm/ManualFrequencyRestorationReserve.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ManualFrequencyRestorationReserveAdder::ManualFrequencyRestorationReserveAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> ManualFrequencyRestorationReserveAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Injection>(extendable)) {
        return std::unique_ptr<ManualFrequencyRestorationReserve>(new ManualFrequencyRestorationReserve(dynamic_cast<Injection&>(extendable), m_participate));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Injection>()));
}

ManualFrequencyRestorationReserveAdder& ManualFrequencyRestorationReserveAdder::withParticipate(bool participate) {
    m_participate = participate;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
