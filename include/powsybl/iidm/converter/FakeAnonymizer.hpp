/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_FAKEANONYMIZER_HPP
#define POWSYBL_IIDM_CONVERTER_FAKEANONYMIZER_HPP

#include <powsybl/iidm/converter/Anonymizer.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

class FakeAnonymizer : public Anonymizer {
public:  // Anonymizer
    Country anonymizeCountry(const Country& country) override;

    std::string anonymizeString(const std::string& str) override;

    Country deanonymizeCountry(const Country& country) const override;

    std::string deanonymizeString(const std::string& anonymousStr) const override;

    void read(std::istream& stream) override;

    void write(std::ostream& stream) const override;

public:
    FakeAnonymizer() = default;

    ~FakeAnonymizer() override = default;
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_FAKEANONYMIZER_HPP
