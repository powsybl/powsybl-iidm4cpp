/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/extensions/entsoe/EntsoeArea.hpp>
#include <powsybl/iidm/extensions/entsoe/EntsoeAreaXmlSerializer.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>
#include <powsybl/test/ResourceFixture.hpp>
#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

BOOST_AUTO_TEST_SUITE(EntsoeAreaTestSuite)

Network createNetwork() {
    Network network("test", "test");
    network.setCaseDate(stdcxx::DateTime::parse("2016-06-27T12:27:58.535+02:00"));
    Substation& s = network.newSubstation()
        .setId("S")
        .setCountry(Country::FR)
        .add();
    s.addExtension(Extension::create<EntsoeArea>(s, EntsoeGeographicalCode::FR));

    return network;
}

BOOST_AUTO_TEST_CASE(EntsoeAreaConstructor) {
    Network network = createNetwork();
    Substation& substation = network.getSubstation("S");

    EntsoeArea extension(substation, EntsoeGeographicalCode::FR);
    BOOST_CHECK_EQUAL("entsoeArea", extension.getName());
    BOOST_CHECK(static_cast<int>(EntsoeGeographicalCode::FR) == static_cast<int>(extension.getCode()));
    BOOST_CHECK(stdcxx::areSame(substation, extension.getExtendable().get()));

    extension.setCode(EntsoeGeographicalCode::DE);
    BOOST_CHECK(static_cast<int>(EntsoeGeographicalCode::DE) == static_cast<int>(extension.getCode()));
}

BOOST_FIXTURE_TEST_CASE(EntsoeAreaXmlSerializerTest, test::ResourceFixture) {
    Network network = createNetwork();

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::initialize(ResourceFixture::getExtensionsBuildDir());

    const std::string& networkStr = ResourceFixture::getResource("entsoeArea.xml");

    test::converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
