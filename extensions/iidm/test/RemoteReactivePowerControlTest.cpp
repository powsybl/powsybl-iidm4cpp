/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/iidm/RemoteReactivePowerControl.hpp>
#include <powsybl/iidm/extensions/iidm/RemoteReactivePowerControlAdder.hpp>
#include <powsybl/network/EurostagFactory.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BOOST_AUTO_TEST_SUITE(RemoteReactivePowerControlTestSuite)

BOOST_AUTO_TEST_CASE(integrity) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();

    Generator& generator = network.getGenerator("GEN");
    generator.newExtension<RemoteReactivePowerControlAdder>().withTargetQ(1.0).withEnabled(true).add();

    const RemoteReactivePowerControl& extension = generator.getExtension<RemoteReactivePowerControl>();
    BOOST_CHECK_CLOSE(1.0, extension.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(!extension.getRegulatingTerminal());
    BOOST_CHECK(extension.isEnabled());

    Terminal& terminal = network.getLine("NHV1_NHV2_1").getTerminal1();
    generator.newExtension<RemoteReactivePowerControlAdder>().withTargetQ(2.0).withRegulatingTerminal(terminal).withEnabled(false).add();
    auto& extensionRegulatingTerm = generator.getExtension<RemoteReactivePowerControl>();
    BOOST_CHECK_CLOSE(2.0, extensionRegulatingTerm.getTargetQ(), std::numeric_limits<double>::epsilon());
    BOOST_CHECK(stdcxx::areSame(terminal, extensionRegulatingTerm.getRegulatingTerminal().get()));
    BOOST_CHECK(!extensionRegulatingTerm.isEnabled());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
