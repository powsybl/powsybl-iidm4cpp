/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/SimpleAnonymizer.hpp>

#include <istream>
#include <ostream>

#include <boost/algorithm/string.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

Country SimpleAnonymizer::anonymizeCountry(const Country& country) {
    return country;
}

std::string SimpleAnonymizer::anonymizeString(const std::string& str) {
    auto it = m_mapping.find(str);
    if (it != m_mapping.end()) {
        return it->second;
    }
    auto status = m_mapping.emplace(str, getAlpha(m_mapping.size() + 1));
    m_reverseMapping[status.first->second] = str;
    return status.first->second;
}

Country SimpleAnonymizer::deanonymizeCountry(const Country& country) const {
    return country;
}

std::string SimpleAnonymizer::deanonymizeString(const std::string& anonymousStr) const {
    if (anonymousStr.empty()) {
        return "";
    }
    auto it = m_reverseMapping.find(anonymousStr);
    if (it == m_reverseMapping.end()) {
        throw PowsyblException(stdcxx::format("Mapping not found for anonymized string '%1%'", anonymousStr));
    }
    return it->second;
}

std::string SimpleAnonymizer::getAlpha(unsigned long num) {
    std::string result;
    unsigned long n = num;
    while (n > 0U) {
        n--;
        unsigned long remainder = n % 26;
        auto digit = static_cast<unsigned char>(remainder + 'A');
        result.insert(result.begin(), digit);
        n = (n - remainder) / 26;
    }
    return result;
}

unsigned long SimpleAnonymizer::getStringCount() const {
    return m_mapping.size();
}

void SimpleAnonymizer::read(std::istream& stream) {
    std::string line;
    while (std::getline(stream, line))
    {
        std::vector<std::string> cells;
        boost::split(cells, line, boost::is_any_of(";"));

        if (cells.size() != 2) {
            throw PowsyblException(stdcxx::format("Invalid line '%1%'", line));
        }

        m_mapping[cells[0]] = cells[1];
        m_reverseMapping[cells[1]] = cells[0];
    }
}

void SimpleAnonymizer::write(std::ostream& stream) const {
    for (const auto& pair : m_mapping) {
        stream << pair.first << ';' << pair.second << std::endl;
    }
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
