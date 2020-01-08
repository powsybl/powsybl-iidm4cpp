/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/BatteryAdder.hpp>
#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/cvg/VoltageRegulation.hpp>
#include <powsybl/iidm/extensions/cvg/VoltageRegulationXmlSerializer.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

Network createBatteryNetwork();

BOOST_AUTO_TEST_SUITE(VoltageRegulationTestSuite)

BOOST_AUTO_TEST_CASE(VoltageRegulationConstructor) {
    Network network = createBatteryNetwork();
    Battery& battery = network.getBattery("BAT1");
    Network network2 = ::powsybl::network::EurostagFactory::createTutorial1Network();

    VoltageRegulation<Battery> extension(battery, true, 5.0);
    BOOST_CHECK_EQUAL("voltageRegulation", extension.getName());
    BOOST_CHECK(stdcxx::areSame(battery, extension.getExtendable().get()));

    BOOST_CHECK(extension.isVoltageRegulatorOn());
    BOOST_CHECK_CLOSE(5.0, extension.getTargetV(), std::numeric_limits<double>::epsilon());

    extension.setVoltageRegulatorOn(false);
    BOOST_CHECK(!extension.isVoltageRegulatorOn());

    BOOST_CHECK(extension.getRegulatingTerminal());
    extension.setRegulatingTerminal(stdcxx::ref<const Terminal>());
    BOOST_CHECK(stdcxx::areSame(battery.getTerminal(), extension.getRegulatingTerminal().get()));
    extension.setRegulatingTerminal(stdcxx::cref(battery.getTerminal()));
    BOOST_CHECK(stdcxx::areSame(battery.getTerminal(), extension.getRegulatingTerminal().get()));

    extension.setTargetV(6.0);
    BOOST_CHECK_CLOSE(6.0, extension.getTargetV(), std::numeric_limits<double>::epsilon());

    extension.extendVariantArraySize(3UL, 10UL, 0UL);
    std::set<unsigned long> indexes;
    indexes.insert(0UL);
    indexes.insert(2UL);
    extension.allocateVariantArrayElement(indexes, 1UL);
    extension.deleteVariantArrayElement(0UL);
    extension.reduceVariantArraySize(5UL);

    // error cases
    POWSYBL_ASSERT_THROW(extension.setRegulatingTerminal(stdcxx::cref(network2.getLoad("LOAD").getTerminal())), PowsyblException, "regulating terminal is not part of the same network");
}

BOOST_FIXTURE_TEST_CASE(VoltageRegulationXmlSerializerTest, ResourceFixture) {
    Network network = createBatteryNetwork();
    Battery& battery = network.getBattery("BAT1");
    battery.addExtension(Extension::create<VoltageRegulation<Battery>>(battery, true, 1.1));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("voltageRegulation", stdcxx::make_unique<VoltageRegulationXmlSerializer<Battery>>());

    const std::string& networkStr = ResourceFixture::getResource("/extensions/cvg/voltageRegulation.xml");

    converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

