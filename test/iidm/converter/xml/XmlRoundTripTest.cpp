/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/network/EurostagFactory.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(XmlRoundTrip)

BOOST_FIXTURE_TEST_CASE(Eurostag, ResourceFixture) {

    const powsybl::iidm::Network network = powsybl::network::EurostagFactory::createTutorial1Network();

    const std::string networkStr = ResourceFixture::getResource("/eurostag-tutorial1-lf.xml");

    // FIXME(mathbagu): RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
