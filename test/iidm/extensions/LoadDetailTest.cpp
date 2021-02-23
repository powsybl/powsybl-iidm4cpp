/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sstream>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/extensions/LoadDetail.hpp>
#include <powsybl/iidm/extensions/LoadDetailAdder.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

Network createNetwork() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T12:27:58.535+02:00"));
    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();
    VoltageLevel& vl = s.newVoltageLevel()
        .setId("VL")
        .setNominalVoltage(400)
        .setTopologyKind(TopologyKind::BUS_BREAKER)
        .add();
    vl.getBusBreakerView().newBus()
        .setId("B")
        .add();
    vl.newLoad()
        .setId("L")
        .setBus("B")
        .setConnectableBus("B")
        .setP0(100)
        .setQ0(50)
        .add();
    return network;
}

BOOST_AUTO_TEST_SUITE(LoadDetailTestSuite)

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createNetwork();
    Load& load = network.getLoad("L");

    load.newExtension<LoadDetailAdder>()
            .withFixedActivePower(1.1)
            .withFixedReactivePower(2.2)
            .withVariableActivePower(3.3)
            .withVariableReactivePower(4.4)
            .add();
    auto& extension = load.getExtension<LoadDetail>();
    BOOST_CHECK_EQUAL("detail", extension.getName());
    BOOST_CHECK(stdcxx::areSame(load, extension.getExtendable().get()));
    BOOST_CHECK_CLOSE(1.1, extension.getFixedActivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.2, extension.getFixedReactivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(3.3, extension.getVariableActivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(4.4, extension.getVariableReactivePower(), std::numeric_limits<double>::epsilon());

    extension.setFixedActivePower(11.1);
    BOOST_CHECK_CLOSE(11.1, extension.getFixedActivePower(), std::numeric_limits<double>::epsilon());

    extension.setFixedReactivePower(22.2);
    BOOST_CHECK_CLOSE(22.2, extension.getFixedReactivePower(), std::numeric_limits<double>::epsilon());

    extension.setVariableActivePower(33.3);
    BOOST_CHECK_CLOSE(33.3, extension.getVariableActivePower(), std::numeric_limits<double>::epsilon());

    extension.setVariableReactivePower(44.4);
    BOOST_CHECK_CLOSE(44.4, extension.getVariableReactivePower(), std::numeric_limits<double>::epsilon());

    // error cases
    POWSYBL_ASSERT_THROW(extension.setFixedActivePower(stdcxx::nan()), PowsyblException, "Invalid fixedActivePower");
    POWSYBL_ASSERT_THROW(extension.setFixedReactivePower(stdcxx::nan()), PowsyblException, "Invalid fixedReactivePower");
    POWSYBL_ASSERT_THROW(extension.setVariableActivePower(stdcxx::nan()), PowsyblException, "Invalid variableActivePower");
    POWSYBL_ASSERT_THROW(extension.setVariableReactivePower(stdcxx::nan()), PowsyblException, "Invalid variableReactivePower");

    POWSYBL_ASSERT_THROW(LoadDetail(load, stdcxx::nan(), 2.2, 3.3, 4.4), PowsyblException, "Invalid fixedActivePower");
    POWSYBL_ASSERT_THROW(LoadDetail(load, 1.1, stdcxx::nan(), 3.3, 4.4), PowsyblException, "Invalid fixedReactivePower");
    POWSYBL_ASSERT_THROW(LoadDetail(load, 1.1, 2.2, stdcxx::nan(), 4.4), PowsyblException, "Invalid variableActivePower");
    POWSYBL_ASSERT_THROW(LoadDetail(load, 1.1, 2.2, 3.3, stdcxx::nan()), PowsyblException, "Invalid variableReactivePower");

    POWSYBL_ASSERT_THROW(load.newExtension<LoadDetailAdder>().withFixedActivePower(stdcxx::nan()).withFixedReactivePower(2.2).withVariableActivePower(3.3).withVariableReactivePower(4.4).add(), PowsyblException, "Invalid fixedActivePower");
    POWSYBL_ASSERT_THROW(load.newExtension<LoadDetailAdder>().withFixedActivePower(1.1).withFixedReactivePower(stdcxx::nan()).withVariableActivePower(3.3).withVariableReactivePower(4.4).add(), PowsyblException, "Invalid fixedReactivePower");
    POWSYBL_ASSERT_THROW(load.newExtension<LoadDetailAdder>().withFixedActivePower(1.1).withFixedReactivePower(2.2).withVariableActivePower(stdcxx::nan()).withVariableReactivePower(4.4).add(), PowsyblException, "Invalid variableActivePower");
    POWSYBL_ASSERT_THROW(load.newExtension<LoadDetailAdder>().withFixedActivePower(1.1).withFixedReactivePower(2.2).withVariableActivePower(3.3).withVariableReactivePower(stdcxx::nan()).add(), PowsyblException, "Invalid variableReactivePower");
}

BOOST_AUTO_TEST_CASE(multivariant) {
    std::string variant1("variant1");
    std::string variant2("variant2");
    std::string variant3("variant3");

    Network network = createNetwork();
    Load& load = network.getLoad("L");
    load.newExtension<LoadDetailAdder>().withFixedActivePower(40.0).withFixedReactivePower(20.0).withVariableActivePower(60.0).withVariableReactivePower(30.0).add();
    auto& ld = load.getExtension<LoadDetail>();

    VariantManager& variantManager = network.getVariantManager();
    variantManager.cloneVariant(VariantManager::getInitialVariantId(), variant1);
    variantManager.cloneVariant(variant1, variant2);
    variantManager.setWorkingVariant(variant1);
    BOOST_CHECK_CLOSE(40.0, ld.getFixedActivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20.0, ld.getFixedReactivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(60.0, ld.getVariableActivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(30.0, ld.getVariableReactivePower(), std::numeric_limits<double>::epsilon());

    // Testing setting different values in the cloned variant and going back to the initial one
    ld.setFixedActivePower(60.0).setFixedReactivePower(30.0).setVariableActivePower(40.0).setVariableReactivePower(20.0);
    BOOST_CHECK_CLOSE(60.0, ld.getFixedActivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(30.0, ld.getFixedReactivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40.0, ld.getVariableActivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20.0, ld.getVariableReactivePower(), std::numeric_limits<double>::epsilon());

    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_CLOSE(40.0, ld.getFixedActivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20.0, ld.getFixedReactivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(60.0, ld.getVariableActivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(30.0, ld.getVariableReactivePower(), std::numeric_limits<double>::epsilon());

    // Removes a variant then adds another variant to test variant recycling (hence calling allocateVariantArrayElement)
    variantManager.removeVariant(variant1);
    auto targetVariantIds = {variant1, variant3};
    variantManager.cloneVariant(VariantManager::getInitialVariantId(), targetVariantIds);
    variantManager.setWorkingVariant(variant1);
    BOOST_CHECK_CLOSE(40.0, ld.getFixedActivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20.0, ld.getFixedReactivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(60.0, ld.getVariableActivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(30.0, ld.getVariableReactivePower(), std::numeric_limits<double>::epsilon());

    variantManager.setWorkingVariant(variant3);
    BOOST_CHECK_CLOSE(40.0, ld.getFixedActivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(20.0, ld.getFixedReactivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(60.0, ld.getVariableActivePower(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(30.0, ld.getVariableReactivePower(), std::numeric_limits<double>::epsilon());

    // Test removing current variant
    variantManager.removeVariant(variant3);
    POWSYBL_ASSERT_THROW(ld.getFixedActivePower(), PowsyblException, "Variant index not set");
}

BOOST_FIXTURE_TEST_CASE(LoadDetailXmlSerializerTest, test::ResourceFixture) {
    Network network = createNetwork();
    Load& load = network.getLoad("L");
    load.newExtension<LoadDetailAdder>()
            .withFixedActivePower(40.0)
            .withFixedReactivePower(20.0)
            .withVariableActivePower(60.0)
            .withVariableReactivePower(30.0)
            .add();

    const std::string& networkStr = test::converter::RoundTrip::getVersionedNetwork("loadDetailRef.xml", converter::xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_FIXTURE_TEST_CASE(LoadDetailXmlSerializerOldRefTest, test::ResourceFixture) {
    const std::string& filename = "loadDetailOldRef.xml";
    Network network = Network::readXml(test::converter::RoundTrip::getVersionedNetworkPath(filename, converter::xml::IidmXmlVersion::V1_2()));
    const std::string& refNetwork = test::converter::RoundTrip::getVersionedNetwork("loadDetailRef.xml", converter::xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION());

    std::stringstream ostream;
    Network::writeXml(filename, ostream, network);
    BOOST_CHECK_EQUAL(refNetwork, ostream.str());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
