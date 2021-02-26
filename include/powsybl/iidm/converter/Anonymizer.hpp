/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_ANONYMIZER_HPP
#define POWSYBL_IIDM_CONVERTER_ANONYMIZER_HPP

#include <iosfwd>

#include <powsybl/iidm/Country.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

class Anonymizer {
public:
    Anonymizer() = default;

    virtual ~Anonymizer() = default;

    virtual Country anonymizeCountry(const Country& country) = 0;

    virtual std::string anonymizeString(const std::string& str) = 0;

    virtual Country deanonymizeCountry(const Country& country) const = 0;

    virtual std::string deanonymizeString(const std::string& anonymousStr) const = 0;

    virtual void read(std::istream& stream) = 0;

    virtual void write(std::ostream& stream) const = 0;
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_ANONYMIZER_HPP
