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

#include <powsybl/iidm/Network.hpp>

namespace powsybl {

namespace test {

namespace converter {

class RoundTrip {
public:
    using Comparator = std::function<void(const std::string&, const std::string&)>;

    using Reader = std::function<iidm::Network(const std::string&)>;

    using Writer = std::function<void(const iidm::Network&, std::ostream&)>;

public:
    static void compareTxt(const std::string& expected, const std::string& actual);

    static void compareXml(const std::string& expected, const std::string& actual);

    static iidm::Network run(const iidm::Network& network, const Writer& out, const Reader& in, const Comparator& compare, const std::string& ref);

    static iidm::Network runXml(const iidm::Network& network, const std::string& ref);

private:
    static std::string write(const iidm::Network& network, const Writer& out, const Comparator& compare, const std::string& ref);
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_ROUNDTRIP_HPP
