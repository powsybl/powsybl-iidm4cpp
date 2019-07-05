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
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename>
stdcxx::CReference<T> VoltageLevel::getConnectable(const std::string& id) const {
    stdcxx::CReference<T> connectable = getNetwork().find<T>(id);

    if (static_cast<bool>(connectable)) {
        if (stdcxx::isInstanceOf<Injection>(connectable.get())) {
            const auto& injection = dynamic_cast<const Injection&>(connectable.get());
            if (!stdcxx::areSame(injection.getTerminal().getVoltageLevel(), *this)) {
                throw PowsyblException(logging::format("The injection '%1%' is not connected to the voltage level '%2%'", id, getId()));
            }
        } else if (stdcxx::isInstanceOf<Branch>(connectable.get())) {
            const auto& branch = dynamic_cast<const Branch&>(connectable.get());
            if (!stdcxx::areSame(branch.getTerminal1().getVoltageLevel(), *this) &&
                !stdcxx::areSame(branch.getTerminal2().getVoltageLevel(), *this)) {
                throw PowsyblException(logging::format("The branch '%1%' is not connected to the voltage level '%2%'", id, getId()));
            }
        } else if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(connectable.get())) {
            const auto& transformer = dynamic_cast<const ThreeWindingsTransformer&>(connectable.get());
            if (!stdcxx::areSame(transformer.getTerminal(ThreeWindingsTransformer::Side::ONE).getVoltageLevel(), *this) &&
                !stdcxx::areSame(transformer.getTerminal(ThreeWindingsTransformer::Side::TWO).getVoltageLevel(), *this) &&
                !stdcxx::areSame(transformer.getTerminal(ThreeWindingsTransformer::Side::THREE).getVoltageLevel(), *this)) {
                throw PowsyblException(logging::format("The 3 windings transformer '%1%' is not connected to the voltage level '%2%'", id, getId()));
            }
        } else {
            throw AssertionError(logging::format("Unexpected ConnectableType value: %1%", connectable.get().getType()));
        }
    }

    return connectable;
}

template <typename T, typename>
stdcxx::Reference<T> VoltageLevel::getConnectable(const std::string& id) {
    const auto& connectable = static_cast<const VoltageLevel*>(this)->getConnectable<T>(id);

    return static_cast<bool>(connectable) ? stdcxx::ref(const_cast<T&>(connectable.get())) : stdcxx::ref<T>();
}

template <typename T, typename>
unsigned long VoltageLevel::getConnectableCount() const {
    const auto& terminals = getTerminals();
    return std::count_if(terminals.cbegin(), terminals.cend(), [](const std::reference_wrapper<Terminal>& terminal) {
        return stdcxx::isInstanceOf<T>(terminal.get().getConnectable().get());
    });
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VOLTAGELEVEL_HXX
