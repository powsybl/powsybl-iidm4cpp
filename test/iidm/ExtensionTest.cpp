/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/network/LoadBarExt.hpp>
#include <powsybl/network/LoadFooExt.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(ExtensionTestSuite)

BOOST_AUTO_TEST_CASE(getExtensionByName) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Load& load = network.getLoad("LOAD");
    load.addExtension(stdcxx::make_unique<powsybl::network::LoadFooExt>(load));
    const Load& cLoad = load;

    BOOST_CHECK(!cLoad.getExtensionByName<powsybl::network::LoadFooExt>("test"));
    BOOST_CHECK(!load.getExtensionByName<powsybl::network::LoadFooExt>("test"));

    BOOST_CHECK(cLoad.getExtensionByName<Extension>("loadFoo"));
    BOOST_CHECK(load.getExtensionByName<Extension>("loadFoo"));

    BOOST_CHECK(!cLoad.getExtensionByName<powsybl::network::LoadBarExt>("loadFoo"));
    BOOST_CHECK(!load.getExtensionByName<powsybl::network::LoadBarExt>("loadFoo"));

    auto cExt = cLoad.getExtensionByName<powsybl::network::LoadFooExt>("loadFoo");
    auto ext = load.getExtensionByName<powsybl::network::LoadFooExt>("loadFoo");
    BOOST_CHECK(cExt);
    BOOST_CHECK(ext);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
