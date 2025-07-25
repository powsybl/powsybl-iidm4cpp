/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/test/converter/RoundTrip.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/test/ResourceFixture.hpp>

namespace powsybl {

namespace test {

namespace converter {

void RoundTrip::compareTxt(const std::string& expected, const std::string& actual) {
    if (expected != actual) {
        std::ostringstream oss;
        oss << '\n'
            << "=== Expected ===" << '\n' << expected << '\n' << std::flush
            << "=== Actual ===" << '\n' << actual << '\n' << std::flush;
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

    return fixture.getResource(getVersionDir(version) + filename);
}

boost::filesystem::path RoundTrip::getVersionedNetworkPath(const std::string& filename, const iidm::converter::xml::IidmXmlVersion& version) {
    ResourceFixture fixture;

    return fixture.getResourcePath(getVersionDir(version) + filename);
}

void RoundTrip::roundTripAllPreviousVersionedXmlTest(const std::string& filename) {
    iidm::converter::xml::IidmXmlVersions versions = iidm::converter::xml::IidmXmlVersion::all();
    versions.pop_back();

    roundTripVersionedXmlTest(filename, versions);
}

void RoundTrip::roundTripVersionedXmlFromMinToCurrentVersionTest(const std::string& filename, const iidm::converter::xml::IidmXmlVersion& minVersion) {
    auto filter = [&minVersion](const iidm::converter::xml::IidmXmlVersion& version) {
        return version >= minVersion && version < iidm::converter::xml::IidmXmlVersion::CURRENT_IIDM_XML_VERSION();
    };

    for (const auto& version : iidm::converter::xml::IidmXmlVersion::all() | boost::adaptors::filtered(filter)) {
        roundTripVersionedXmlTest(filename, version);
    }
}

void RoundTrip::roundTripVersionedXmlTest(const std::string& filename, const iidm::converter::xml::IidmXmlVersion& version) {
    const auto& writer = [&version, &filename](const iidm::Network& n, std::ostream& stream) {
        iidm::converter::ExportOptions options;
        options.setVersion(version.toString("."));
        iidm::Network::writeXml(filename, stream, n, options);
    };

    const auto& reader = [&filename](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return iidm::Network::readXml(filename, stream);
    };

    const std::string& expected = getVersionedNetwork(filename, version);
    std::istringstream stream(expected);
    iidm::Network network = iidm::Network::readXml(filename, stream);
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
    write(data2, out, compare, ref);
    return data2;
}

iidm::Network RoundTrip::runXml(const iidm::Network& network, const std::string& ref) {
    const std::string& filename = stdcxx::format("%1%.xiidm", network.getId());

    const auto& writer = [&filename](const iidm::Network& n, std::ostream& stream) {
        iidm::Network::writeXml(filename, stream, n);
    };

    const auto& reader = [&filename](const std::string& xmlBytes) {
        std::istringstream stream(xmlBytes);
        return iidm::Network::readXml(filename, stream);
    };

    return run(network, writer, reader, compareXml, ref);
}

std::string RoundTrip::write(const iidm::Network& network, const Writer& out, const Comparator& compare, const std::string& ref) {
    std::stringstream buffer;
    out(network, buffer);

    compare(ref, buffer.str());

    return buffer.str();
}

void RoundTrip::writeXmlTest(const iidm::Network& network, const Writer& out, const std::string& ref) {
    write(network, out, compareXml, ref);
}

}  // namespace converter

}  // namespace test

}  // namespace powsybl

