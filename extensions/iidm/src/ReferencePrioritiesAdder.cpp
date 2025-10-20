/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ReferencePrioritiesAdder.hpp>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePriorities.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ReferencePrioritiesAdder::ReferencePrioritiesAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> ReferencePrioritiesAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Connectable>(extendable)) {
        return std::unique_ptr<ReferencePriorities>(new ReferencePriorities(dynamic_cast<Connectable&>(extendable)));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Connectable>()));
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
