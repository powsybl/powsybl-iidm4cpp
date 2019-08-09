/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "RoundTrip.hpp"

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/converter/Anonymizer.hpp>

namespace powsybl {

namespace iidm {

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

Network RoundTrip::run(const Network& network, const Writer& out, const Reader& in, const Comparator& compare, const std::string& ref) {
    const std::string& output1 = write(network, out, compare, ref);
    Network data2 = in(output1);
    write(network, out, compare, ref);
    return data2;
}

Network RoundTrip::runXml(const Network& network, const std::string& ref) {
    const auto& writer = [](const Network& n, std::ostream& stream) {
        Network::writeXml(stream, n);
    };

    const auto& reader = [](const std::string& xml) {
        std::istringstream stream(xml);
        return Network::readXml(stream);
    };

    return run(network, writer, reader, compareXml, ref);
}

std::string RoundTrip::write(const Network& network, const Writer& out, const Comparator& compare, const std::string& ref) {
    std::stringstream buffer;
    out(network, buffer);

    compare(ref, buffer.str());

    return buffer.str();
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

