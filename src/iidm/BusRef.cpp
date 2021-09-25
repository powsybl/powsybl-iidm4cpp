/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/BusRef.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

stdcxx::CReference<Bus> BusRef::chooseBusByLevel(const Terminal& terminal, const TopologyLevel& level) {
    if (level == TopologyLevel::BUS_BRANCH) {
        return terminal.getBusView().getBus();
    }
    return terminal.getBusBreakerView().getBus();
}

stdcxx::CReference<Bus> BusRef::resolve(const Network& network, const TopologyLevel& level) {
    if (level == TopologyLevel::NODE_BREAKER) {
        throw PowsyblException(stdcxx::format("%1% is not supported in resolve a BusRef", level));
    }
    return resolveByLevel(network, level);
}

}  // namespace iidm

}  // namespace powsybl
