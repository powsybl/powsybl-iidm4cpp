/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/LoadExponentialModelAdder.hpp>
#include <powsybl/iidm/LoadModel.hpp>
#include <powsybl/iidm/LoadZipModelAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

#include "NetworkFactory.hpp"

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(LoadTestSuite)

BOOST_AUTO_TEST_CASE(constructor) {
    Network network = createNetwork();
    unsigned long loadCount = network.getLoadCount();

    Load& load = network.getLoad("LOAD1");
    BOOST_CHECK_EQUAL("LOAD1", load.getId());
    BOOST_CHECK_EQUAL("LOAD1_NAME", load.getOptionalName());
    BOOST_CHECK_EQUAL(IdentifiableType::LOAD, load.getType());
    std::ostringstream oss;
    oss << load.getType();
    BOOST_CHECK_EQUAL("LOAD", oss.str());
    BOOST_CHECK_EQUAL(LoadType::UNDEFINED, load.getLoadType());
    BOOST_CHECK_CLOSE(50.0, load.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40.0, load.getQ0(), std::numeric_limits<double>::epsilon());

    VoltageLevel& vl1 = network.getVoltageLevel("VL1");
    LoadAdder adder = vl1.newLoad()
        .setId("LOAD1")
        .setBus("VL1_BUS1");

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Load 'LOAD1': p0 is invalid");
    adder.setP0(50.0);

    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Load 'LOAD1': q0 is invalid");
    adder.setQ0(40.0);

    adder.setLoadType(static_cast<LoadType>(5));
    POWSYBL_ASSERT_THROW(adder.add(), AssertionError, "Unexpected LoadType value: 5");
    adder.setLoadType(LoadType::AUXILIARY);

    POWSYBL_ASSERT_THROW(adder.add(), PowsyblException, "The network test already contains an object 'Load' with the id 'LOAD1'");
    adder.setEnsureIdUnicity(true);

    BOOST_CHECK_NO_THROW(adder.add());
    BOOST_CHECK_EQUAL(loadCount + 1, network.getLoadCount());
    BOOST_CHECK_EQUAL(loadCount + 1, boost::size(network.getLoads()));
    const Network& cNetwork = network;
    BOOST_CHECK_EQUAL(loadCount + 1, boost::size(cNetwork.getLoads()));
}

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = createNetwork();

    Load& load1 = network.getLoad("LOAD1");
    BOOST_CHECK_EQUAL(LoadType::UNDEFINED, load1.getLoadType());
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());

    BOOST_TEST(stdcxx::areSame(load1, load1.setLoadType(LoadType::FICTITIOUS)));
    BOOST_CHECK_EQUAL(LoadType::FICTITIOUS, load1.getLoadType());
    POWSYBL_ASSERT_THROW(load1.setLoadType(static_cast<LoadType>(7)), AssertionError, "Unexpected LoadType value: 7");

    BOOST_TEST(stdcxx::areSame(load1, load1.setP0(100)));
    BOOST_CHECK_CLOSE(100, load1.getP0(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(load1.setP0(stdcxx::nan()), ValidationException, "Load 'LOAD1': p0 is invalid");

    BOOST_TEST(stdcxx::areSame(load1, load1.setQ0(100)));
    BOOST_CHECK_CLOSE(100, load1.getQ0(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(load1.setQ0(stdcxx::nan()), ValidationException, "Load 'LOAD1': q0 is invalid");

    load1.setFictitious(true);
    BOOST_CHECK(load1.isFictitious());
    load1.setFictitious(false);
    BOOST_CHECK(!load1.isFictitious());

    load1.remove();
    POWSYBL_ASSERT_THROW(network.getLoad("LOAD1"), PowsyblException, "Unable to find to the identifiable 'LOAD1'");
}

BOOST_AUTO_TEST_CASE(multivariant) {
    Network network = createNetwork();

    Load& load1 = network.getLoad("LOAD1");
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().cloneVariant(VariantManager::getInitialVariantId(), {"s1", "s2"});
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().setWorkingVariant("s1");
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());
    load1.setP0(100);
    load1.setQ0(80);
    BOOST_CHECK_CLOSE(100, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(80, load1.getQ0(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant("s2");
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());
    load1.setP0(150);
    load1.setQ0(120);
    BOOST_CHECK_CLOSE(150, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(120, load1.getQ0(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().setWorkingVariant(VariantManager::getInitialVariantId());
    BOOST_CHECK_CLOSE(50, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(40, load1.getQ0(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s1");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().cloneVariant("s2", "s3");
    network.getVariantManager().setWorkingVariant("s3");
    BOOST_CHECK_CLOSE(150, load1.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(120, load1.getQ0(), std::numeric_limits<double>::epsilon());

    network.getVariantManager().removeVariant("s3");
    BOOST_CHECK_EQUAL(3UL, network.getVariantManager().getVariantArraySize());

    network.getVariantManager().removeVariant("s2");
    BOOST_CHECK_EQUAL(1UL, network.getVariantManager().getVariantArraySize());
}

BOOST_AUTO_TEST_CASE(setOptionalName) {
    Network network = createNetwork();

    Load& load1 = network.getLoad("LOAD1");
    BOOST_CHECK_EQUAL("LOAD1", load1.getId());
    BOOST_CHECK_EQUAL("LOAD1_NAME", load1.getOptionalName());
    BOOST_CHECK_EQUAL("LOAD1_NAME", load1.getNameOrId());

    load1.setOptionalName("");
    BOOST_CHECK(load1.getOptionalName().empty());
    BOOST_CHECK_EQUAL("LOAD1", load1.getNameOrId());

    load1.setOptionalName("LOAD1#");
    BOOST_CHECK_EQUAL("LOAD1#", load1.getOptionalName());
    BOOST_CHECK_EQUAL("LOAD1#", load1.getNameOrId());

}

BOOST_AUTO_TEST_CASE(ZipLoadModel) {
    Network network = createNetwork();
    VoltageLevel& vl1 = network.getVoltageLevel("VL1");

    Load& load = vl1.newLoad()
                        .setId("newZipLoad")
                        .setBus("VL1_BUS1")
                        .setP0(2.0)
                        .setQ0(2.0)
                        .newZipModel()
                            .setC0p(0.3)
                            .setC1p(0.5)
                            .setC2p(0.2)
                            .setC0q(0.1)
                            .setC1q(0.2)
                            .setC2q(0.7)
                            .add()
                        .add();
    BOOST_CHECK_CLOSE(2.0, load.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, load.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(load.hasModel());
    BOOST_CHECK(load.getModelType() == LoadModelType::ZIP);
    LoadZipModel& loadModel = load.getModel<LoadZipModel>();
    BOOST_CHECK_CLOSE(0.3, loadModel.getC0p(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.5, loadModel.getC1p(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, loadModel.getC2p(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.1, loadModel.getC0q(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.2, loadModel.getC1q(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.7, loadModel.getC2q(), std::numeric_limits<double>::epsilon());

    POWSYBL_ASSERT_THROW(LoadZipModelAdder::addNewModel(load, 0.31, 0.51, 0.21, 0.11, 0.22, 0.71), ValidationException, "Load 'newZipLoad': Sum of c0p, c1p and c2p should be 1");
    POWSYBL_ASSERT_THROW(LoadZipModelAdder::addNewModel(load, stdcxx::nan(), 0.51, 0.21, 0.11, 0.22, 0.71), ValidationException, "Load 'newZipLoad': Invalid zip load model coefficient: nan");
}

BOOST_AUTO_TEST_CASE(ExponentialLoadModel) {
    Network network = createNetwork();
    VoltageLevel& vl1 = network.getVoltageLevel("VL1");

    Load& load = vl1.newLoad()
                        .setId("newExpLoad")
                        .setBus("VL1_BUS1")
                        .setP0(2.0)
                        .setQ0(2.0)
                        .newExponentialModel()
                            .setNp(0.6)
                            .setNq(0.5)
                            .add()
                        .add();
    BOOST_CHECK_CLOSE(2.0, load.getP0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(2.0, load.getQ0(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(load.hasModel());
    BOOST_CHECK(load.getModelType() == LoadModelType::EXPONENTIAL);
    LoadExponentialModel& loadModel = load.getModel<LoadExponentialModel>();
    BOOST_CHECK_CLOSE(0.6, loadModel.getNp(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.5, loadModel.getNq(), std::numeric_limits<double>::epsilon());
    LoadExponentialModelAdder::addNewModel(load, 0.61, 0.51);
    loadModel = load.getModel<LoadExponentialModel>();
    BOOST_CHECK_CLOSE(0.61, loadModel.getNp(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK_CLOSE(0.51, loadModel.getNq(), std::numeric_limits<double>::epsilon());
    POWSYBL_ASSERT_THROW(LoadExponentialModelAdder::addNewModel(load, -2, 0.51), ValidationException, "Load 'newExpLoad': Invalid load model exponential value: -2");

}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
