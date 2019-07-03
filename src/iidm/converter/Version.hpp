/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_VERSION_HPP
#define POWSYBL_IIDM_CONVERTER_VERSION_HPP

#include <string>
#include <vector>

namespace powsybl {

namespace iidm {

namespace converter {

class Version {
public:
    Version(const std::string& versionStr);

    const std::vector<int>& getArray() const;

    bool operator==(const Version& other) const;

    bool operator<(const Version& other) const;

    bool operator<=(const Version& other) const;

    bool operator>(const Version& other) const;

    bool operator>=(const Version& other) const;

    std::string toString(const std::string& separator) const;

private:
    std::vector<int> m_numbers;
};

// TODO(sla) to be discussed...
// those comparison operators are dead code for now on...
bool operator==(const Version& version, const std::string& versionStr);

bool operator<(const Version& version, const std::string& versionStr);

bool operator<=(const Version& version, const std::string& versionStr);

bool operator>(const Version& version, const std::string& versionStr);

bool operator>=(const Version& version, const std::string& versionStr);

bool operator==(const std::string& versionStr, const Version& version);

bool operator<(const std::string& versionStr, const Version& version);

bool operator<=(const std::string& versionStr, const Version& version);

bool operator>(const std::string& versionStr, const Version& version);

bool operator>=(const std::string& versionStr, const Version& version);

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_VERSION_HPP

