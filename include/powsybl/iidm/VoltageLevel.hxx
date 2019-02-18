/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGELEVEL_HXX
#define POWSYBL_IIDM_VOLTAGELEVEL_HXX

#include <powsybl/iidm/VoltageLevel.hpp>

#include <cassert>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

template <typename T>
T& VoltageLevel::getConnectable(const std::string& id) {
    Connectable& connectable = getNetwork().getConnectable(id);
    if (stdcxx::isInstanceOf<Injection>(connectable)) {
        Injection& injection = dynamic_cast<Injection&>(connectable);
        if (!stdcxx::areSame(injection.getTerminal().getVoltageLevel(), *this)) {
            throw PowsyblException(logging::format("The injection '%1%' is not connected to the voltage level '%2%'", id, getId()));
        }
    } else if (stdcxx::isInstanceOf<Branch>(connectable)) {
        Branch& branch = dynamic_cast<Branch&>(connectable);
        if (!stdcxx::areSame(branch.getTerminal1().getVoltageLevel(), *this) &&
            !stdcxx::areSame(branch.getTerminal2().getVoltageLevel(), *this)) {
            throw PowsyblException(logging::format("The branch '%1%' is not connected to the voltage level '%2%'", id, getId()));
        }
    } else if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(connectable)) {
        throw AssertionError("TODO");
    } else {
        throw AssertionError(logging::format("Unexpected ConnectableType value: %1%", connectable.getType()));
    }

    return dynamic_cast<T&>(connectable);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VOLTAGELEVEL_HXX
