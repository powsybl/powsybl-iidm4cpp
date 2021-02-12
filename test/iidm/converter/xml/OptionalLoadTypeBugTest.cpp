/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(OptionalLoadTypeTest)

BOOST_AUTO_TEST_CASE(OptionalLoadTypeFromXml) {
    for (const auto& version : IidmXmlVersion::all()) {
        Network network = Network::readXml(test::converter::RoundTrip::getVersionedNetworkPath("optionalLoadTypeBug.xml", version.get()));
        BOOST_CHECK_EQUAL(LoadType::UNDEFINED, network.getLoad("L").getLoadType());
    }
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
