/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Identifiable.hpp>

#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(TieLineTerminalRefBugTestSuite)

BOOST_FIXTURE_TEST_CASE(TieLineTerminalRefBugTest, test::ResourceFixture) {
    auto versionedPath = test::converter::RoundTrip::getVersionedNetworkPath("tieLineTerminalRefBug.xml", IidmXmlVersion::CURRENT_IIDM_XML_VERSION());
    
    Network networkRead = Network::readXml(versionedPath);

    Generator& gen = networkRead.getGenerator("GEN");
    BOOST_CHECK_EQUAL("NHV1_XNODE1", gen.getRegulatingTerminal().getConnectable().get().getId());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
