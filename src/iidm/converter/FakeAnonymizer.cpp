/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/FakeAnonymizer.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

Country FakeAnonymizer::anonymizeCountry(const Country& country) {
    return country;
}

std::string FakeAnonymizer::anonymizeString(const std::string& str) {
    return str;
}

Country FakeAnonymizer::deanonymizeCountry(const Country& country) const {
    return country;
}

std::string FakeAnonymizer::deanonymizeString(const std::string& anonymousStr) const {
    return anonymousStr;
}

void FakeAnonymizer::read(std::istream& /*stream*/) {
    // Nothing to do
}

void FakeAnonymizer::write(std::ostream& /*stream*/) const {
    // Nothing to do
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
