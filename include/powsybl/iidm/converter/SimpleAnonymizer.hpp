/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_SIMPLEANONYMIZER_HPP
#define POWSYBL_IIDM_CONVERTER_SIMPLEANONYMIZER_HPP

#include <boost/bimap.hpp>

#include <powsybl/iidm/converter/Anonymizer.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

class SimpleAnonymizer : public Anonymizer {
public:  // Anonymizer
    Country anonymizeCountry(const Country& country) override;

    std::string anonymizeString(const std::string& str) override;

    Country deanonymizeCountry(const Country& country) const override;

    std::string deanonymizeString(const std::string& anonymousStr) const override;

    void read(std::istream& stream) override;

    void write(std::ostream& stream) const override;

public:
    static std::string getAlpha(unsigned long num);

public:
    SimpleAnonymizer() = default;

    ~SimpleAnonymizer() override = default;

    SimpleAnonymizer(const SimpleAnonymizer&) = default;

    SimpleAnonymizer(SimpleAnonymizer&&) = default;

    SimpleAnonymizer& operator=(const SimpleAnonymizer&) = default;

    SimpleAnonymizer& operator=(SimpleAnonymizer&&) = default;

    unsigned long getStringCount() const;

private:
    using Mapping = boost::bimap<std::string, std::string>;

private:
    Mapping m_mapping;
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_SIMPLEANONYMIZER_HPP
