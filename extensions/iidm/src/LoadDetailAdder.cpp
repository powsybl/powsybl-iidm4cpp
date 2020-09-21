/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/LoadDetailAdder.hpp>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/extensions/iidm/LoadDetail.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

LoadDetailAdder::LoadDetailAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> LoadDetailAdder::createExtension(Extendable& extendable) {
    if (stdcxx::isInstanceOf<Load>(extendable)) {
        return stdcxx::make_unique<LoadDetail>(dynamic_cast<Load&>(extendable), m_fixedActivePower, m_fixedReactivePower, m_variableActivePower, m_variableReactivePower);
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Load>()));
}

LoadDetailAdder& LoadDetailAdder::withFixedActivePower(double fixedActivePower) {
    m_fixedActivePower = fixedActivePower;
    return *this;
}

LoadDetailAdder& LoadDetailAdder::withFixedReactivePower(double fixedReactivePower) {
    m_fixedReactivePower = fixedReactivePower;
    return *this;
}

LoadDetailAdder& LoadDetailAdder::withVariableActivePower(double variableActivePower) {
    m_variableActivePower = variableActivePower;
    return *this;
}

LoadDetailAdder& LoadDetailAdder::withVariableReactivePower(double variableReactivePower) {
    m_variableReactivePower = variableReactivePower;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
