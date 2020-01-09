/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/cvg/CurrentLimitsPerSeason.hpp>
#include <powsybl/iidm/extensions/cvg/CurrentLimitsPerSeasonXmlSerializer.hpp>
#include <powsybl/network/EurostagFactory.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include "iidm/converter/ResourceFixture.hpp"
#include "iidm/converter/RoundTrip.hpp"

#include "AssertionUtils.hpp"

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

BOOST_AUTO_TEST_SUITE(CurrentLimitsPerSeasonTestSuite)

BOOST_AUTO_TEST_CASE(CurrentLimitsPerSeasonConstructor) {
    Network network = ::powsybl::network::EurostagFactory::createWithCurrentLimits();
    Line& line1 = network.getLine("NHV1_NHV2_1");
    Line& line2 = network.getLine("NHV1_NHV2_2");
    Load& load = network.getLoad("LOAD");

    CurrentLimitsPerSeason extension(load);
    BOOST_CHECK_EQUAL("currentLimitsPerSeason", extension.getName());
    BOOST_CHECK(stdcxx::areSame(load, extension.getExtendable().get()));
    BOOST_CHECK(extension.getCurrentLimits().empty());

    std::map<std::string, CurrentLimitsPerSeason::CurrentLimitsTriple> map;
    map["line1_line2"] = CurrentLimitsPerSeason::CurrentLimitsTriple(stdcxx::make_unique<CurrentLimits>(line1.getCurrentLimits1().get()), stdcxx::make_unique<CurrentLimits>(line1.getCurrentLimits2().get()),
                                                                     stdcxx::make_unique<CurrentLimits>(line2.getCurrentLimits1().get()), stdcxx::make_unique<CurrentLimits>(line2.getCurrentLimits2().get()));
    map["line1"] = CurrentLimitsPerSeason::CurrentLimitsTriple(stdcxx::make_unique<CurrentLimits>(line1.getCurrentLimits1().get()), stdcxx::make_unique<CurrentLimits>(line1.getCurrentLimits2().get()),
                                                               std::unique_ptr<CurrentLimits>(), std::unique_ptr<CurrentLimits>());
    map["line2"] = CurrentLimitsPerSeason::CurrentLimitsTriple(std::unique_ptr<CurrentLimits>(), std::unique_ptr<CurrentLimits>(),
                                                               stdcxx::make_unique<CurrentLimits>(line2.getCurrentLimits1().get()), stdcxx::make_unique<CurrentLimits>(line2.getCurrentLimits2().get()));

    CurrentLimitsPerSeason extension2(load, map);
    BOOST_CHECK(extension2.getCurrentLimits().size() == 3);
    BOOST_CHECK(extension2.getCurrentLimits().find("line1_line2")->second.getCurrentLimits());
    BOOST_CHECK(extension2.getCurrentLimits().find("line1_line2")->second.getCurrentLimits1());
    BOOST_CHECK(extension2.getCurrentLimits().find("line1_line2")->second.getCurrentLimits2());
    BOOST_CHECK(extension2.getCurrentLimits().find("line1_line2")->second.getCurrentLimits3());

    BOOST_CHECK(extension2.getCurrentLimits().find("line1")->second.getCurrentLimits());
    BOOST_CHECK(extension2.getCurrentLimits().find("line1")->second.getCurrentLimits1());
    BOOST_CHECK(!extension2.getCurrentLimits().find("line1")->second.getCurrentLimits2());
    BOOST_CHECK(!extension2.getCurrentLimits().find("line1")->second.getCurrentLimits3());

    BOOST_CHECK(!extension2.getCurrentLimits().find("line2")->second.getCurrentLimits());
    BOOST_CHECK(!extension2.getCurrentLimits().find("line2")->second.getCurrentLimits1());
    BOOST_CHECK(extension2.getCurrentLimits().find("line2")->second.getCurrentLimits2());
    BOOST_CHECK(extension2.getCurrentLimits().find("line2")->second.getCurrentLimits3());

    BOOST_CHECK(extension2.getSeason("line1_line2"));
    BOOST_CHECK(!extension2.getSeason("unknown_season"));
    BOOST_CHECK_EQUAL("currentLimitsPerSeason", extension2.getSeason("line1_line2").get().getMessageHeader());
}

BOOST_FIXTURE_TEST_CASE(CurrentLimitsPerSeasonXmlSerializerTest, ResourceFixture) {
    Network network = ::powsybl::network::EurostagFactory::createWithCurrentLimits();
    Line& line1 = network.getLine("NHV1_NHV2_1");
    Line& line2 = network.getLine("NHV1_NHV2_2");
    Load& load = network.getLoad("LOAD");

    std::map<std::string, CurrentLimitsPerSeason::CurrentLimitsTriple> map;
    map["PRINTEMPS"] = CurrentLimitsPerSeason::CurrentLimitsTriple(stdcxx::make_unique<CurrentLimits>(line1.getCurrentLimits1().get()), stdcxx::make_unique<CurrentLimits>(line1.getCurrentLimits2().get()),
                                                                   stdcxx::make_unique<CurrentLimits>(line2.getCurrentLimits1().get()), stdcxx::make_unique<CurrentLimits>(line2.getCurrentLimits2().get()));
    map["ETE"] = CurrentLimitsPerSeason::CurrentLimitsTriple(stdcxx::make_unique<CurrentLimits>(line1.getCurrentLimits1().get()), stdcxx::make_unique<CurrentLimits>(line1.getCurrentLimits2().get()),
                                                             std::unique_ptr<CurrentLimits>(), std::unique_ptr<CurrentLimits>());
    map["AUTOMNE"] = CurrentLimitsPerSeason::CurrentLimitsTriple(std::unique_ptr<CurrentLimits>(), std::unique_ptr<CurrentLimits>(),
                                                                 stdcxx::make_unique<CurrentLimits>(line2.getCurrentLimits1().get()), stdcxx::make_unique<CurrentLimits>(line2.getCurrentLimits2().get()));

    load.addExtension(Extension::create<CurrentLimitsPerSeason>(load, map));

    ExtensionProviders<converter::xml::ExtensionXmlSerializer>::registerExtension("currentLimitsPerSeason", stdcxx::make_unique<CurrentLimitsPerSeasonXmlSerializer>());

    const std::string& networkStr = ResourceFixture::getResource("/extensions/cvg/currentLimitsPerSeason.xml");

    converter::RoundTrip::runXml(network, networkStr);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

