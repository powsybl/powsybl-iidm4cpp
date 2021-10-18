/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/CoordinatedReactiveControlAdder.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/extensions/iidm/CoordinatedReactiveControl.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

CoordinatedReactiveControlAdder::CoordinatedReactiveControlAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> CoordinatedReactiveControlAdder::createExtension(Extendable& extendable) {
    if (stdcxx::isInstanceOf<Generator>(extendable)) {
        return std::unique_ptr<CoordinatedReactiveControl>(new CoordinatedReactiveControl(dynamic_cast<Generator&>(extendable), m_qPercent));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
}

CoordinatedReactiveControlAdder& CoordinatedReactiveControlAdder::withQPercent(double qPercent) {
    m_qPercent = qPercent;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
