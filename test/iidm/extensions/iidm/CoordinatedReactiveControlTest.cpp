/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/GeneratorAdder.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/iidm/CoordinatedReactiveControl.hpp>
#include <powsybl/iidm/extensions/iidm/CoordinatedReactiveControlXmlSerializer.hpp>

#include "AssertionUtils.hpp"
#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/extensions/ExtensionRoundTrip.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(CoordinatedReactiveControlTestSuite)

Network createNetwork() {
    Network network("networkId", "sourceFormat");
    Substation& p1 = network.newSubstation()
        .setId("P1")
        .setCountry(Country::FR)
        .setTso("RTE")
        .setGeographicalTags({"A"})
        .add();
    VoltageLevel& vlgen = p1.newVoltageLevel()
        .setId("VLGEN")
        .setNominalVoltage(24.0)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    Bus& ngen = vlgen.getBusBreakerView().newBus()
        .setId("NGEN")
        .add();
    vlgen.newGenerator()
        .setId("GEN")
        .setBus(ngen.getId())
        .setConnectableBus(ngen.getId())
        .setMinP(-9999.99)
        .setMaxP(9999.99)
        .setVoltageRegulatorOn(true)
        .setTargetV(24.5)
        .setTargetP(607.0)
        .setTargetQ(301.0)
        .add();

    return network;
}

BOOST_AUTO_TEST_CASE(CoordinatedReactiveControlTest) {
    Network network = createNetwork();
    Generator& gen = network.getGenerator("GEN");

    POWSYBL_ASSERT_THROW(CoordinatedReactiveControl(gen, -1.0), PowsyblException, "Unexpected value for qPercent: -1");
    POWSYBL_ASSERT_THROW(CoordinatedReactiveControl(gen, 101.0), PowsyblException, "Unexpected value for qPercent: 101");
    POWSYBL_ASSERT_THROW(CoordinatedReactiveControl(gen, stdcxx::nan()), PowsyblException, "Undefined value for qPercent");

    CoordinatedReactiveControl crc(gen, 50.0);
    BOOST_CHECK_EQUAL("coordinatedReactiveControl", crc.getName());
    BOOST_CHECK_CLOSE(50.0, crc.getQPercent(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(crc.setQPercent(101), PowsyblException, "Unexpected value for qPercent: 101");
    POWSYBL_ASSERT_THROW(crc.setQPercent(-1), PowsyblException, "Unexpected value for qPercent: -1");

    BOOST_CHECK(stdcxx::areSame(gen, crc.getExtendable<Generator>().get()));
    POWSYBL_ASSERT_THROW(crc.getExtendable<Load>(), AssertionError, "Unexpected extendable type: powsybl::iidm::Load (powsybl::iidm::Generator expected)");
}

BOOST_FIXTURE_TEST_CASE(CoordinatedReactiveControlXmlSerializerTest, ResourceFixture) {
    Network network = createNetwork();
    Generator& gen = network.getGenerator("GEN");

    CoordinatedReactiveControl ext(gen, 1.1);
    CoordinatedReactiveControlXmlSerializer serializer;

    const std::string& extensionStr = ResourceFixture::getResource("/extensions/iidm/coordinatedReactiveControl.xml");

    ExtensionRoundTrip::runXml(gen, ext, serializer, extensionStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

