/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/test/converter/RoundTrip.hpp>

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/test/ResourceFixture.hpp>

namespace powsybl {

namespace test {

namespace converter {

void RoundTrip::compareTxt(const std::string& expected, const std::string& actual) {
    if (expected != actual) {
        std::ostringstream oss;
        oss << std::endl
            << "=== Expected ===" << std::endl << expected << std::endl
            << "=== Actual ===" << std::endl << actual << std::endl;
        BOOST_FAIL(oss.str());
    }
}

void RoundTrip::compareXml(const std::string& expected, const std::string& actual) {
    compareTxt(expected, actual);
}

std::string RoundTrip::getVersionDir(const iidm::converter::xml::IidmXmlVersion& version) {
    return "/V" + version.toString("_") + "/";
}

std::string RoundTrip::getVersionedNetwork(const std::string& filename, const iidm::converter::xml::IidmXmlVersion& version) {
    ResourceFixture fixture;

    return fixture.getResource(getVersionedNetworkPath(filename, version));
}

std::string RoundTrip::getVersionedNetworkPath(const std::string& filename, const iidm::converter::xml::IidmXmlVersion& version) {
    return getVersionDir(version) + filename;
}

void RoundTrip::roundTripVersionedXmlTest(const std::string& filename, const iidm::converter::xml::IidmXmlVersion& version) {
    const auto& writer = [&version](const iidm::Network& n, std::ostream& stream) {
        iidm::converter::ExportOptions options;
        options.setVersion(version.toString("."));
        iidm::Network::writeXml(stream, n, options);
    };

    const auto& reader = [](const std::string& xml) {
        std::istringstream stream(xml);
        return iidm::Network::readXml(stream);
    };

    const std::string& expected = getVersionedNetwork(filename, version);
    iidm::Network network = iidm::Network::readXml(expected);
    run(network, writer, reader, compareXml, expected);
}

void RoundTrip::roundTripVersionedXmlTest(const std::string& filename, const iidm::converter::xml::IidmXmlVersions& versions) {
    for (const auto& version : versions) {
        roundTripVersionedXmlTest(filename, version.get());
    }
}

iidm::Network RoundTrip::run(const iidm::Network& network, const Writer& out, const Reader& in, const Comparator& compare, const std::string& ref) {
    const std::string& output1 = write(network, out, compare, ref);
    iidm::Network data2 = in(output1);
    write(network, out, compare, ref);
    return data2;
}

iidm::Network RoundTrip::runXml(const iidm::Network& network, const std::string& ref) {
    const auto& writer = [](const iidm::Network& n, std::ostream& stream) {
        iidm::Network::writeXml(stream, n);
    };

    const auto& reader = [](const std::string& xml) {
        std::istringstream stream(xml);
        return iidm::Network::readXml(stream);
    };

    return run(network, writer, reader, compareXml, ref);
}

std::string RoundTrip::write(const iidm::Network& network, const Writer& out, const Comparator& compare, const std::string& ref) {
    std::stringstream buffer;
    out(network, buffer);

    compare(ref, buffer.str());

    return buffer.str();
}

}  // namespace converter

}  // namespace test

}  // namespace powsybl

