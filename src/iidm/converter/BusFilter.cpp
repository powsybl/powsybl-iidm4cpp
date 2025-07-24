/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/BusFilter.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/TopologyLevel.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

BusFilter::BusFilter(const std::set<std::string>& buses, const ExportOptions& options) :
    m_buses(buses),
    m_options(options) {

}

BusFilter::BusFilter(const ExportOptions& options) :
    m_options(options) {

}

BusFilter BusFilter::create(const Network& /*network*/, const ExportOptions& options) {
    std::set<std::string> buses;
    // TODO(sebalaig) add Component support
//    if (options.isOnlyMainCc()) {
//        addBusOfMainCC(buses, network, options);
//        addBusOfOtherSideOfOpenBranches(buses, network, options);
//
//        if (network.getThreeWindingsTransformerCount() != 0) {
//            throw AssertionError("TODO");
//        }
//    }
    return BusFilter(buses, options);
}

bool BusFilter::test(const Bus& bus) const {
    return m_buses.empty() || m_buses.find(bus.getId()) != m_buses.end();
}

bool BusFilter::test(const Connectable& connectable) const {
    if (m_buses.empty()) {
        return true;
    }

    return (std::none_of(connectable.getTerminals().begin(), connectable.getTerminals().end(), [=](const Terminal& t) {
        const stdcxx::CReference<Bus>& b = m_options.getTopologyLevel() == TopologyLevel::BUS_BRANCH ? t.getBusView().getConnectableBus() : t.getBusBreakerView().getConnectableBus();
        return (b && m_buses.find(b.get().getId()) != m_buses.end());
    } ));
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

