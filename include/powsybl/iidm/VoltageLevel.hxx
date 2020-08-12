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

#include <boost/range/adaptor/filtered.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/util/DistinctPredicate.hpp>
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
                throw PowsyblException(stdcxx::format("The injection '%1%' is not connected to the voltage level '%2%'", id, getId()));
            }
        } else if (stdcxx::isInstanceOf<Branch>(connectable.get())) {
            const auto& branch = dynamic_cast<const Branch&>(connectable.get());
            if (!stdcxx::areSame(branch.getTerminal1().getVoltageLevel(), *this) &&
                !stdcxx::areSame(branch.getTerminal2().getVoltageLevel(), *this)) {
                throw PowsyblException(stdcxx::format("The branch '%1%' is not connected to the voltage level '%2%'", id, getId()));
            }
        } else if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(connectable.get())) {
            const auto& transformer = dynamic_cast<const ThreeWindingsTransformer&>(connectable.get());
            if (!stdcxx::areSame(transformer.getTerminal(ThreeWindingsTransformer::Side::ONE).getVoltageLevel(), *this) &&
                !stdcxx::areSame(transformer.getTerminal(ThreeWindingsTransformer::Side::TWO).getVoltageLevel(), *this) &&
                !stdcxx::areSame(transformer.getTerminal(ThreeWindingsTransformer::Side::THREE).getVoltageLevel(), *this)) {
                throw PowsyblException(stdcxx::format("The 3 windings transformer '%1%' is not connected to the voltage level '%2%'", id, getId()));
            }
        } else {
            throw AssertionError(stdcxx::format("Unexpected ConnectableType value: %1%", connectable.get().getType()));
        }
    }

    return connectable;
}

template <typename T, typename>
stdcxx::Reference<T> VoltageLevel::getConnectable(const std::string& id) {
    const auto& connectable = static_cast<const VoltageLevel*>(this)->getConnectable<T>(id);

    return stdcxx::ref(connectable);
}

template <typename T, typename>
unsigned long VoltageLevel::getConnectableCount() const {
    return boost::size(getConnectables<T>());
}

template <typename T, typename>
stdcxx::const_range<T> VoltageLevel::getConnectables() const {
    return getTerminals() | boost::adaptors::filtered(Terminal::isInstanceOf<T>) | boost::adaptors::transformed(Terminal::map<const T>) | boost::adaptors::filtered(DistinctPredicate());
}

template <typename T, typename>
stdcxx::range<T> VoltageLevel::getConnectables() {
    return getTerminals() | boost::adaptors::filtered(Terminal::isInstanceOf<T>) | boost::adaptors::transformed(Terminal::map<T>) | boost::adaptors::filtered(DistinctPredicate());
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VOLTAGELEVEL_HXX
