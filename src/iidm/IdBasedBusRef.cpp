/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/IdBasedBusRef.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

IdBasedBusRef::IdBasedBusRef(const std::string& id) :
    m_id(id) {
}

IdBasedBusRef::IdBasedBusRef(const std::string& id, const Branch::Side& side) :
    m_id(id), m_side(side) {
}

stdcxx::CReference<Bus> IdBasedBusRef::resolveByLevel(const Network& network, const TopologyLevel& level) {
    const auto& identifiable = network.find(m_id);
    if (!identifiable) {
        return stdcxx::cref<Bus>();
    }

    if (!m_side) {
        if (stdcxx::isInstanceOf<Bus>(identifiable.get())) {
            const auto& bus = dynamic_cast<const Bus&>(identifiable.get());
            if (level == TopologyLevel::BUS_BRANCH) {
                for (const auto& terminal : bus.getConnectedTerminals()) {
                    if (terminal.getBusView().getBus()) {
                        return terminal.getBusView().getBus();
                    }
                }
            }
            return stdcxx::cref(bus);
        }
        if (stdcxx::isInstanceOf<Injection>(identifiable.get())) {
            const auto& injection = dynamic_cast<const Injection&>(identifiable.get());
            return chooseBusByLevel(injection.getTerminal(), level);
        }
        throw PowsyblException(stdcxx::format("%1% is not a bus or injection.", m_id));
    }
    if (stdcxx::isInstanceOf<Branch>(identifiable.get())) {
        const auto& branch = dynamic_cast<const Branch&>(identifiable.get());
        stdcxx::Reference<Terminal> terminal;
        switch (*m_side) {
            case Branch::Side::ONE:
                terminal = stdcxx::ref(branch.getTerminal1());
                break;
            case Branch::Side::TWO:
                terminal = stdcxx::ref(branch.getTerminal2());
                break;
            default:
                throw AssertionError(stdcxx::format("Unexpected side: ", *m_side));
        }
        return chooseBusByLevel(terminal, level);
    }
    throw PowsyblException(stdcxx::format("%1% is not a branch.", m_id));
}

}  // namespace iidm

}  // namespace powsybl
