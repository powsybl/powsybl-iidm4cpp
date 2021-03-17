/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/network/BatteryNetworkFactory.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Country.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/SubstationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/VoltageLevelAdder.hpp>

namespace powsybl {

namespace network {

iidm::Network BatteryNetworkFactory::create() {
    iidm::Network network("fictitious", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2017-06-25T17:43:00.000+01:00"));
    network.setForecastDistance(0);

    // 2 Substations
    iidm::Substation& p1 = network.newSubstation()
        .setId("P1")
        .setCountry(iidm::Country::FR)
        .setTso("R")
        .setGeographicalTags({"A"})
        .add();
    iidm::Substation& p2 = network.newSubstation()
        .setId("P2")
        .setCountry(iidm::Country::FR)
        .setTso("R")
        .setGeographicalTags({"B"})
        .add();

    // 2 VoltageLevels
    iidm::VoltageLevel& vlgen = p1.newVoltageLevel()
        .setId("VLGEN")
        .setNominalV(400)
        .setTopologyKind(iidm::TopologyKind::BUS_BREAKER)
        .add();
    iidm::VoltageLevel& vlbat = p2.newVoltageLevel()
        .setId("VLBAT")
        .setNominalV(400)
        .setTopologyKind(iidm::TopologyKind::BUS_BREAKER)
        .add();

    // 2 Bus
    iidm::Bus& ngen = vlgen.getBusBreakerView().newBus()
        .setId("NGEN")
        .add();
    iidm::Bus& nbat = vlbat.getBusBreakerView().newBus()
        .setId("NBAT")
        .add();

    // 2 lines
    network.newLine()
        .setId("NHV1_NHV2_1")
        .setVoltageLevel1(vlgen.getId())
        .setBus1(ngen.getId())
        .setConnectableBus1(ngen.getId())
        .setVoltageLevel2(vlbat.getId())
        .setBus2(nbat.getId())
        .setConnectableBus2(nbat.getId())
        .setR(3.0)
        .setX(33.0)
        .setG1(0.0)
        .setB1(386E-6 / 2)
        .setG2(0.0)
        .setB2(386E-6 / 2)
        .add();
    network.newLine()
        .setId("NHV1_NHV2_2")
        .setVoltageLevel1(vlgen.getId())
        .setBus1(ngen.getId())
        .setConnectableBus1(ngen.getId())
        .setVoltageLevel2(vlbat.getId())
        .setBus2(nbat.getId())
        .setConnectableBus2(nbat.getId())
        .setR(3.0)
        .setX(33.0)
        .setG1(0.0)
        .setB1(386E-6 / 2)
        .setG2(0.0)
        .setB2(386E-6 / 2)
        .add();

    // Add Components
    iidm::Generator& generator = vlgen.newGenerator()
        .setId("GEN")
        .setBus(ngen.getId())
        .setConnectableBus(ngen.getId())
        .setEnergySource(iidm::EnergySource::OTHER)
        .setMinP(-9999.99)
        .setMaxP(9999.99)
        .setVoltageRegulatorOn(true)
        .setTargetV(24.5)
        .setTargetP(607.0)
        .setTargetQ(301.0)
        .add();
    generator.getTerminal().setP(-605);
    generator.getTerminal().setQ(-225);
    generator.newMinMaxReactiveLimits()
        .setMinQ(-9999.99)
        .setMaxQ(9999.99)
        .add();
    iidm::Battery& battery = vlbat.newBattery()
        .setId("BAT")
        .setBus(nbat.getId())
        .setConnectableBus(nbat.getId())
        .setP0(9999.99)
        .setQ0(9999.99)
        .setMinP(-9999.99)
        .setMaxP(9999.99)
        .add();
    battery.newMinMaxReactiveLimits()
        .setMinQ(-9999.99)
        .setMaxQ(9999.99)
        .add();
    battery.getTerminal().setP(-605);
    battery.getTerminal().setQ(-225);
    iidm::Battery& battery2 = vlbat.newBattery()
        .setId("BAT2")
        .setBus(nbat.getId())
        .setConnectableBus(nbat.getId())
        .setP0(100)
        .setQ0(200)
        .setMinP(-200)
        .setMaxP(200)
        .add();
    battery2.newReactiveCapabilityCurve()
        .beginPoint()
        .setP(0)
        .setMinQ(-59.3)
        .setMaxQ(60.0)
        .endPoint()
        .beginPoint()
        .setP(70.0)
        .setMinQ(-54.55)
        .setMaxQ(46.25)
        .endPoint()
        .add();
    battery2.getTerminal().setP(-605);
    battery2.getTerminal().setQ(-225);
    vlbat.newLoad()
        .setId("LOAD")
        .setLoadType(iidm::LoadType::UNDEFINED)
        .setBus(nbat.getId())
        .setConnectableBus(nbat.getId())
        .setP0(600.0)
        .setQ0(200.0)
        .add();

    return network;
}

}  // namespace network

}  // namespace powsybl
