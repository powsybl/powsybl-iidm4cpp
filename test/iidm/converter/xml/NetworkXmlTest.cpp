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

    void readWriteOptions(const std::string& str,
                          const ImportOptions& importOptions,
                          const ExportOptions& exportOptions,
                          const stdcxx::CReference<converter::Anonymizer>& anonymizer) {
        std::stringstream istream;
        istream << str;
        Network networkTmp(Network::readXml(istream, importOptions, anonymizer));

        std::stringstream ostream1;
        Network::writeXml(ostream1, networkTmp, exportOptions);

        istream.clear();
        istream << ostream1.str();
        Network network(Network::readXml(istream, importOptions, anonymizer));

        std::stringstream ostream2;
        Network::writeXml(ostream2, network, exportOptions);

        if (m_enableDebug) {
            std::cout << m_testName << ": " << std::endl;
            std::cout << "  1st unparse : " << ostream1.str();
            std::cout << "  2nd unparse : " << ostream2.str();
            std::cout << std::endl;
        }
        BOOST_CHECK_EQUAL(ostream1.str(), ostream2.str());
    }

    void readWrite(const std::string& str) {
        FakeAnonymizer anonymizer;
        readWriteOptions(str, ImportOptions(), ExportOptions(), stdcxx::CReference<converter::Anonymizer>(anonymizer));
    }

private:
    bool m_enableDebug;
    std::string m_testName;
};

#define ENABLE_DEBUG 0

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

    ReadWriteTester(boost::unit_test::framework::current_test_case().p_name).setDebugEnabled(ENABLE_DEBUG).readWrite(networkStr);
}

BOOST_AUTO_TEST_CASE(Basic) {

    const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                    "<iidm:network xmlns:iidm=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                                    "              id=\"terminalRef\" "
                                    "              caseDate=\"2000-11-08T19:00:00.000+01:00\" "
                                    "              forecastDistance=\"0\" "
                                    "              sourceFormat=\"test\">"
                                    "</iidm:network>";

    ReadWriteTester(boost::unit_test::framework::current_test_case().p_name).setDebugEnabled(ENABLE_DEBUG).readWrite(networkStr);
}

BOOST_AUTO_TEST_CASE(NoPrefix) {

    const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                    "<network xmlns=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                                    "              id=\"terminalRef\" "
                                    "              caseDate=\"2014-11-08T19:00:00.000+01:00\" "
                                    "              forecastDistance=\"0\" "
                                    "              sourceFormat=\"test\">"
                                    "</network>";

    ReadWriteTester(boost::unit_test::framework::current_test_case().p_name).setDebugEnabled(ENABLE_DEBUG).readWrite(networkStr);
}

BOOST_AUTO_TEST_CASE(SpecialChars) {

    const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                    "<network xmlns=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                                    "              id=\"ø/Ø - ö/Ö - æ/Æ - ä/Ä - å/Å (aa/Aa)\" "
                                    "              caseDate=\"2014-11-08T19:00:00.000+01:00\" "
                                    "              forecastDistance=\"0\" "
                                    "              sourceFormat=\"test\">"
                                    "</network>";

    ReadWriteTester(boost::unit_test::framework::current_test_case().p_name).setDebugEnabled(ENABLE_DEBUG).readWrite(networkStr);
}

BOOST_AUTO_TEST_CASE(FakeAnonymizerTest) {

    const std::string& networkStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                    "<network xmlns=\"http://www.itesla_project.eu/schema/iidm/1_0\" "
                                    "              id=\"ø/Ø - ö/Ö - æ/Æ - ä/Ä - å/Å (aa/Aa)\" "
                                    "              caseDate=\"2014-11-08T19:00:00.000+01:00\" "
                                    "              forecastDistance=\"0\" "
                                    "              sourceFormat=\"test\">"
                                    "</network>";

    ImportOptions importOptions;
    importOptions.setThrowExceptionIfExtensionNotFound(false);
    ExportOptions exportOptions;
    exportOptions.setAnonymised(true)
                 .setIndent(false)
                 .setThrowExceptionIfExtensionNotFound(false)
                 .setTopologyLevel(TopologyLevel::NODE_BREAKER);
    FakeAnonymizer anonymizer;
    ReadWriteTester(boost::unit_test::framework::current_test_case().p_name)
                    .setDebugEnabled(ENABLE_DEBUG)
                    .readWriteOptions(networkStr, importOptions, exportOptions, stdcxx::CReference<converter::Anonymizer>(anonymizer));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
