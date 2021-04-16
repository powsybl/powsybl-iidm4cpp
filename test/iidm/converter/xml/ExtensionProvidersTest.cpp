/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>
#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(ExtensionProvidersTestSuite)

BOOST_AUTO_TEST_CASE(findProvider) {
    auto& extProvider = ExtensionProviders<converter::xml::ExtensionXmlSerializer>::getInstance();
    BOOST_CHECK(!extProvider.findProvider("test"));
    POWSYBL_ASSERT_THROW(extProvider.findProviderOrThrowException("test"), PowsyblException, "No provider found for extension 'test'");

    BOOST_CHECK(extProvider.findProvider("loadBar"));
    BOOST_CHECK_NO_THROW(extProvider.findProviderOrThrowException("loadBar"));

    BOOST_CHECK_EQUAL(6, boost::size(extProvider.getProviders()));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
