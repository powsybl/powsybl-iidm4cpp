/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Version.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/exception/exception.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

Version::Version(const std::string& versionStr) {
    std::vector<std::string> numbersStr;
    boost::split(numbersStr, versionStr, boost::is_any_of("_"));
    m_numbers.reserve(numbersStr.size());
    for (const auto& str : numbersStr) {
        m_numbers.push_back(std::stoi(str));
    }
}

const std::vector<int>& Version::getArray() const {
    return m_numbers;
}

bool Version::operator==(const Version& other) const {
    return m_numbers == other.m_numbers;
}

bool Version::operator<(const Version& other) const {
    return m_numbers < other.m_numbers;
}

bool Version::operator<=(const Version& other) const {
    return !operator>(other);
}

bool Version::operator>(const Version& other) const {
    return m_numbers > other.m_numbers;
}

bool Version::operator>=(const Version& other) const {
    return !operator<(other);
}

std::string Version::toString(const std::string& separator) const {
    return boost::algorithm::join(m_numbers |
                                  boost::adaptors::transformed(
                                      static_cast<std::string(*)(
                                          int)>(std::to_string)), separator);
}

bool operator==(const Version& version, const std::string& versionStr) {
    return version == Version(versionStr);
}

bool operator<(const Version& version, const std::string& versionStr) {
    return version < Version(versionStr);
}

bool operator<=(const Version& version, const std::string& versionStr) {
    return version <= Version(versionStr);
}

bool operator>(const Version& version, const std::string& versionStr) {
    return version > Version(versionStr);
}

bool operator>=(const Version& version, const std::string& versionStr) {
    return version >= Version(versionStr);
}

bool operator==(const std::string& versionStr, const Version& version) {
    return Version(versionStr) == version;
}

bool operator<(const std::string& versionStr, const Version& version) {
    return Version(versionStr) < version;
}

bool operator<=(const std::string& versionStr, const Version& version) {
    return Version(versionStr) <= version;
}

bool operator>(const std::string& versionStr, const Version& version) {
    return Version(versionStr) > version;
}

bool operator>=(const std::string& versionStr, const Version& version) {
    return Version(versionStr) >= version;
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
