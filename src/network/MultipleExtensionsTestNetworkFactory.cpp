/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/network/MultipleExtensionsTestNetworkFactory.hpp>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/network/LoadBarExt.hpp>
#include <powsybl/network/LoadFooExt.hpp>

namespace powsybl {

namespace network {

iidm::Network MultipleExtensionsTestNetworkFactory::create() {
    iidm::Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2017-11-17T12:00:00+01:00"));

    iidm::Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(iidm::Country::FR)
        .add();
    iidm::VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setTopologyKind(iidm::TopologyKind::BUS_BREAKER)
        .setNominalVoltage(20.0f)
        .setLowVoltageLimit(15.0f)
        .setHighVoltageLimit(25.0f)
        .add();
    vl.getBusBreakerView().newBus()
        .setId("BUS")
        .add();
    iidm::Load& load = vl.newLoad()
        .setId("LOAD")
        .setP0(0.0f)
        .setQ0(0.0f)
        .setBus("BUS")
        .setConnectableBus("BUS")
        .add();
    iidm::Load& load2 = vl.newLoad()
        .setId("LOAD2")
        .setP0(0.0f)
        .setQ0(0.0f)
        .setBus("BUS")
        .setConnectableBus("BUS")
        .add();
    load.addExtension(stdcxx::make_unique<LoadFooExt>(load));
    load.addExtension(stdcxx::make_unique<LoadBarExt>(load));
    load2.addExtension(stdcxx::make_unique<LoadFooExt>(load2));

    return network;
}

}  // namespace network

}  // namespace powsybl
