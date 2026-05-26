/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/iidm/DynamicModelInfo.hpp>
#include <powsybl/iidm/extensions/iidm/DynamicModelInfoAdder.hpp>

#include <powsybl/network/EurostagFactory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(DynamicModelInfoTestSuite)

BOOST_AUTO_TEST_CASE(extensionTest) {

    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Generator& gen = network.getGenerator("GEN");

    gen.newExtension<DynamicModelInfoAdder>().setModelName("TestModel").add();

    auto& dynamicModelInfo = gen.getExtension<DynamicModelInfo>();
    BOOST_CHECK_EQUAL("dynamicModel", dynamicModelInfo.getName());
    BOOST_CHECK(stdcxx::areSame(gen, dynamicModelInfo.getExtendable().get()));
    BOOST_CHECK_EQUAL("TestModel", dynamicModelInfo.getModelName());

    dynamicModelInfo.setModelName("TestModel2");
    BOOST_CHECK_EQUAL("TestModel2", dynamicModelInfo.getModelName());

}

BOOST_AUTO_TEST_CASE(variantTest) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Generator& gen = network.getGenerator("GEN");

    gen.newExtension<DynamicModelInfoAdder>().setModelName("TestModel").add();
    auto& dynamicModelInfo = gen.getExtension<DynamicModelInfo>();

    // Testing variant cloning
    VariantManager& variantManager = network.getVariantManager();
    variantManager.cloneVariant(VariantManager::getInitialVariantId(), "variant1");
    variantManager.cloneVariant("variant1", "variant2");
    variantManager.setWorkingVariant("variant1");
    BOOST_CHECK_EQUAL("TestModel", dynamicModelInfo.getModelName());

    // Testing setting different values in the cloned variant and going back to the initial one
    dynamicModelInfo.setModelName("TestModel2");
    BOOST_CHECK_EQUAL("TestModel2", dynamicModelInfo.getModelName());
    variantManager.setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_EQUAL("TestModel", dynamicModelInfo.getModelName());

    // Removes a variant then adds another variant to test variant recycling (hence calling allocateVariantArrayElement)
    variantManager.removeVariant("variant1");
    variantManager.cloneVariant(VariantManager::getInitialVariantId(), {"variant1", "variant3"});
    variantManager.setWorkingVariant("variant1");
    BOOST_CHECK_EQUAL("TestModel", dynamicModelInfo.getModelName());
    variantManager.setWorkingVariant("variant3");
    BOOST_CHECK_EQUAL("TestModel", dynamicModelInfo.getModelName());

    // Test removing current variant
    variantManager.removeVariant("variant3");
    POWSYBL_ASSERT_THROW(dynamicModelInfo.setModelName("TEST"), PowsyblException, "Variant index not set");
    POWSYBL_ASSERT_THROW(dynamicModelInfo.getModelName(), PowsyblException, "Variant index not set");
    
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
