/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_TEST_CONVERTER_ROUNDTRIP_HXX
#define POWSYBL_TEST_CONVERTER_ROUNDTRIP_HXX

#include <powsybl/test/converter/RoundTrip.hpp>

namespace powsybl {

namespace test {

namespace converter {

template <typename Callback>
void RoundTrip::testForAllPreviousVersions(const iidm::converter::xml::IidmXmlVersion& maxVersion, Callback callback) {
    for (const iidm::converter::xml::IidmXmlVersion& version : iidm::converter::xml::IidmXmlVersion::all()) {
        if (version < maxVersion) {
            callback(version);
        }
    }
}

}  // namespace converter

}  // namespace test

}  // namespace powsybl

#endif  // POWSYBL_TEST_CONVERTER_ROUNDTRIP_HXX
