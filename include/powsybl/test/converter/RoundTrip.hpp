/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_TEST_CONVERTER_ROUNDTRIP_HPP
#define POWSYBL_TEST_CONVERTER_ROUNDTRIP_HPP

#include <functional>
#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>

namespace powsybl {

namespace test {

class ResourceFixture;

namespace converter {

class RoundTrip {
public:
    using Comparator = std::function<void(const std::string&, const std::string&)>;

    using Reader = std::function<iidm::Network(const std::string&)>;

    using Writer = std::function<void(const iidm::Network&, std::ostream&)>;

public:
    static void compareTxt(const std::string& expected, const std::string& actual);

    static void compareXml(const std::string& expected, const std::string& actual);

    static std::string getVersionDir(const iidm::converter::xml::IidmXmlVersion& version);

    static std::string getVersionedNetwork(const std::string& filename, const iidm::converter::xml::IidmXmlVersion& version);

    static boost::filesystem::path getVersionedNetworkPath(const std::string& filename, const iidm::converter::xml::IidmXmlVersion& version);

    static void roundTripAllPreviousVersionedXmlTest(const std::string& filename);

    static void roundTripVersionedXmlFromMinToCurrentVersionTest(const std::string& filename, const iidm::converter::xml::IidmXmlVersion& minVersion);

    static void roundTripVersionedXmlTest(const std::string& filename, const iidm::converter::xml::IidmXmlVersion& version);

    static void roundTripVersionedXmlTest(const std::string& filename, const iidm::converter::xml::IidmXmlVersions& versions);

    static iidm::Network run(const iidm::Network& network, const Writer& out, const Reader& in, const Comparator& compare, const std::string& ref);

    static iidm::Network runXml(const iidm::Network& network, const std::string& ref);

    template <typename Callback>
    static void testForAllPreviousVersions(const iidm::converter::xml::IidmXmlVersion& maxVersion, Callback callback);

    static void writeXmlTest(const iidm::Network& network, const Writer& out, const std::string& ref);

private:
    static std::string write(const iidm::Network& network, const Writer& out, const Comparator& compare, const std::string& ref);
};

}  // namespace converter

}  // namespace test

}  // namespace powsybl

#include <powsybl/test/converter/RoundTrip.hxx>

#endif  // POWSYBL_TEST_CONVERTER_ROUNDTRIP_HPP
