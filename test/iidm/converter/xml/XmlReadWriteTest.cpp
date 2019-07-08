/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Properties.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/FakeAnonymizer.hpp>
#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/logging/ConsoleLogger.hpp>
#include <powsybl/network/EurostagFactory.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BOOST_AUTO_TEST_SUITE(XmlReadWriteTest)

class ReadWriteTester {
public:
    ReadWriteTester(std::string testName) :
        m_enableDebug(false),
        m_testName(std::move(testName)) {

    }

    ReadWriteTester& setDebugEnabled(bool enableDebug) {
        m_enableDebug = enableDebug;
        return *this;
    }

    void readWriteOptions(const Network& network,
                          const ImportOptions& importOptions,
                          const ExportOptions& exportOptions,
                          const stdcxx::CReference<Anonymizer>& anonymizer) {
        std::stringstream streamReferer;
        std::stringstream streamChallenger;

        // write a first time
        Network::writeXml(streamReferer, network, exportOptions);

        // read the first output and dump from read result
        {
            std::stringstream istream;
            istream << streamReferer.str();
            Network otherNetwork(Network::readXml(istream, importOptions, anonymizer));

            Network::writeXml(streamChallenger, otherNetwork, exportOptions);
        }

        if (m_enableDebug) {
            m_logger.debug("expecting:\n" + streamReferer.str());
            m_logger.debug("got:\n" + streamChallenger.str());
        }

        BOOST_CHECK_EQUAL(streamReferer.str(), streamChallenger.str());
    }

    void readWrite(const Network& network) {
        FakeAnonymizer anonymizer;
        stdcxx::CReference<Anonymizer> crefAnonymizer(anonymizer);
        readWriteOptions(network, ImportOptions(), ExportOptions(), crefAnonymizer);
    }

private:
    bool m_enableDebug;

    logging::ConsoleLogger m_logger;

    std::string m_testName;
};

#define ENABLE_DEBUG 0

BOOST_AUTO_TEST_CASE(SubstationOptions) {

    iidm::Network network("eurostag1", "test");

    // network properties a ignored for now on
    network.getProperties().set("propNetwork1", "valNetwork1");
    network.getProperties().set("propNetwork3", "valNetwork3");
    network.getProperties().set("propNetwork2", "valNetwork2");

    network.setCaseDate(stdcxx::DateTime::parse("2019-06-17T13:16:03Z"));
    Substation& s1 = network.newSubstation()
                            .setId("S1")
                            .setCountry(iidm::Country::BE)
                            .setTso("TSO1")
                            .setGeographicalTags({"A"})
                            .add();
    s1.getProperties().set("prop1S1", "val1S1");
    s1.getProperties().set("prop3S1", "val3S1");
    Substation& s2 = network.newSubstation()
                            .setId("S2")
                            .setCountry(iidm::Country::FR)
                            .setTso("TSO2")
                            .setGeographicalTags({"A", "B", "C", "D", "E"})
                            .add();
    s2.getProperties().set("prop4S2", "val4S2");
    s2.getProperties().set("prop6S2", "val6S2");
    s2.getProperties().set("prop5S2", "val5S2");

    ImportOptions importOptions;
    importOptions.setThrowExceptionIfExtensionNotFound(false);
    ExportOptions exportOptions;
    exportOptions.setAnonymised(true)
                 .setIndent(true)
                 .setThrowExceptionIfExtensionNotFound(false)
                 .setTopologyLevel(TopologyLevel::NODE_BREAKER);
    FakeAnonymizer anonymizer;
    stdcxx::CReference<Anonymizer> crefAnonymizer(anonymizer);
    ReadWriteTester(boost::unit_test::framework::current_test_case().p_name)
                    .setDebugEnabled(ENABLE_DEBUG)
                    .readWriteOptions(network, importOptions, exportOptions, crefAnonymizer);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
