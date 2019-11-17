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
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/cvg/StateOfCharge.hpp>
#include <powsybl/iidm/extensions/cvg/StateOfChargeXmlSerializer.hpp>
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

BOOST_AUTO_TEST_SUITE(StateOfChargeTestSuite)

BOOST_AUTO_TEST_CASE(StateOfChargeConstructor) {
    Network network = createBatteryNetwork();
    Battery& battery = network.getBattery("BAT1");

    StateOfCharge extension(battery, 120.0, 5.0);
    BOOST_CHECK_EQUAL("stateOfCharge", extension.getName());
    BOOST_CHECK(stdcxx::areSame(battery, extension.getExtendable().get()));

    BOOST_CHECK_CLOSE(120.0, extension.getStorageCapacity(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(5.0, extension.getCurrent(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.0, extension.getMin(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(100.0, extension.getMax(), std::numeric_limits<double>::epsilon());

    extension.setCurrent(50.0);
    BOOST_CHECK_CLOSE(50.0, extension.getCurrent(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(extension.getCurrent() * extension.getStorageCapacity() / 100.0, extension.getCurrentEnergy(), std::numeric_limits<double>::epsilon());

    extension.setMin(1.0);
    BOOST_CHECK_CLOSE(1.0, extension.getMin(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(extension.getMin() * extension.getStorageCapacity() / 100.0, extension.getMinEnergy(), std::numeric_limits<double>::epsilon());

    extension.setMax(99.0);
    BOOST_CHECK_CLOSE(99.0, extension.getMax(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(extension.getMax() * extension.getStorageCapacity() / 100.0, extension.getMaxEnergy(), std::numeric_limits<double>::epsilon());

    extension.setStorageCapacity(80.0);
    BOOST_CHECK_CLOSE(80.0, extension.getStorageCapacity(), std::numeric_limits<double>::epsilon());

    extension.setCurrent(60.0);
    BOOST_CHECK_CLOSE(60.0, extension.getCurrent(), std::numeric_limits<double>::epsilon());

    extension.extendVariantArraySize(3UL, 10UL, 0UL);
    std::set<unsigned long> indexes;
    indexes.insert(0UL);
    indexes.insert(2UL);
    extension.allocateVariantArrayElement(indexes, 1UL);
    extension.deleteVariantArrayElement(0UL);
    extension.reduceVariantArraySize(5UL);

    // error cases
    POWSYBL_ASSERT_THROW(extension.setMin(-1.0), PowsyblException, "min value cannot be negative");
    POWSYBL_ASSERT_THROW(extension.setMax(101.0), PowsyblException, "max value cannot be greater than 100");
    POWSYBL_ASSERT_THROW(extension.setStorageCapacity(-1.0), PowsyblException, "storageCapacity value cannot be negative");
    POWSYBL_ASSERT_THROW(StateOfCharge soc(battery, 100.0, 0.0, 50.0, 25.0), PowsyblException, "min value cannot be greater than max");
}

BOOST_FIXTURE_TEST_CASE(StateOfChargeXmlSerializerTest, ResourceFixture) {
    Network network = createBatteryNetwork();
    Battery& battery = network.getBattery("BAT1");
    battery.addExtension(Extension::create<StateOfCharge>(battery, 1.0, 2.0, 3.0, 1.5));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("stateOfCharge", stdcxx::make_unique<StateOfChargeXmlSerializer>());

    const std::string& networkStr = ResourceFixture::getResource("/extensions/cvg/stateOfCharge.xml");

    converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

