/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(NetworkXmlTest)

void assertNetwork(const Network& network) {
    BOOST_CHECK_EQUAL("terminalRef", network.getId());
    BOOST_CHECK_EQUAL("2000-11-08T19:00:00+01:00", network.getCaseDate().toString());
    BOOST_CHECK_EQUAL(0, network.getForecastDistance());
    BOOST_CHECK_EQUAL("test", network.getSourceFormat());

}

BOOST_AUTO_TEST_CASE(StartByComments) {

    const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                    "<!-- test --> "
                                    "<iidm:network xmlns:iidm=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                                    "              id=\"terminalRef\" "
                                    "              caseDate=\"2000-11-08T19:00:00.000+01:00\" "
                                    "              forecastDistance=\"0\" "
                                    "              sourceFormat=\"test\">"
                                    "    <!-- test comment --> "
                                    "</iidm:network>";

    const Network& network = Network::readXml(networkStr);
    assertNetwork(network);
}

BOOST_AUTO_TEST_CASE(Basic) {

    const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                    "<iidm:network xmlns:iidm=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                                    "              id=\"terminalRef\" "
                                    "              caseDate=\"2000-11-08T19:00:00.000+01:00\" "
                                    "              forecastDistance=\"0\" "
                                    "              sourceFormat=\"test\">"
                                    "</iidm:network>";

    const Network& network = Network::readXml(networkStr);
    assertNetwork(network);
}

BOOST_AUTO_TEST_CASE(NoPrefix) {

    const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                    "<network xmlns=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                                    "              id=\"terminalRef\" "
                                    "              caseDate=\"2000-11-08T19:00:00.000+01:00\" "
                                    "              forecastDistance=\"0\" "
                                    "              sourceFormat=\"test\">"
                                    "</network>";

    const Network& network = Network::readXml(networkStr);
    assertNetwork(network);
}

BOOST_AUTO_TEST_CASE(SpecialChars) {

    const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                    "<network xmlns:iidm=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                                    "              id=\"ø/Ø - ö/Ö - æ/Æ - ä/Ä - å/Å (aa/Aa)\" "
                                    "              caseDate=\"2014-11-08T19:00:00.000+01:00\" "
                                    "              forecastDistance=\"0\" "
                                    "              sourceFormat=\"test\">"
                                    "</network>";

    const Network& network = Network::readXml(networkStr);
    BOOST_CHECK_EQUAL("ø/Ø - ö/Ö - æ/Æ - ä/Ä - å/Å (aa/Aa)", network.getId());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
