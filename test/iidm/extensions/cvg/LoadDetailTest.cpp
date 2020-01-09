/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/cvg/LoadDetail.hpp>
#include <powsybl/iidm/extensions/cvg/LoadDetailXmlSerializer.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "iidm/NetworkFactory.hpp"
#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

BOOST_AUTO_TEST_SUITE(LoadDetailTestSuite)

BOOST_AUTO_TEST_CASE(LoadDetailConstructor) {
    Network network = createNetwork();
    Load& load = network.getLoad("LOAD1");

    LoadDetail extension(load, 1.1, 2.2, 3.3, 4.4);
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
}

BOOST_FIXTURE_TEST_CASE(LoadDetailXmlSerializerTest, ResourceFixture) {
    Network network = createNetwork();
    network.setCaseDate(stdcxx::DateTime::parse("2013-01-15T18:45:00.000+01:00"));
    Load& load = network.getLoad("LOAD1");
    load.addExtension(Extension::create<LoadDetail>(load, 0.1, 0.2, 0.3, 0.4));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("detail", stdcxx::make_unique<LoadDetailXmlSerializer>());

    const std::string& networkStr = ResourceFixture::getResource("/extensions/cvg/loadDetail.xml");

    converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

