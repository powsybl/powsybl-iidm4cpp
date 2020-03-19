/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_ANONYMIZER_HPP
#define POWSYBL_IIDM_CONVERTER_ANONYMIZER_HPP

#include <string>

#include <powsybl/iidm/Country.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

class IIDM_DECLSPEC Anonymizer {
public:
    Anonymizer() = default;

    virtual ~Anonymizer() = default;

    virtual Country anonymizeCountry(const Country& country) = 0;

    virtual std::string anonymizeString(const std::string& str) = 0;

    virtual Country deanonymizeCountry(const Country& country) const = 0;

    virtual std::string deanonymizeString(const std::string& anonymousStr) const = 0;

    // TODO(sebalaig) Add missing methods:
    // void read(BufferedReader reader);
    // void write(BufferedWriter writer);
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_ANONYMIZER_HPP
