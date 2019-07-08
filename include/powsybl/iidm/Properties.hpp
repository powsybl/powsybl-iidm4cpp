/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_PROPERTIES_HPP
#define POWSYBL_IIDM_PROPERTIES_HPP

#include <map>
#include <string>

namespace powsybl {

namespace iidm {

class Properties {
public:
    using const_iterator = std::map<std::string, std::string>::const_iterator;

    using iterator = std::map<std::string, std::string>::iterator;

public:
    Properties() = default;

    ~Properties() = default;

    const_iterator begin() const;

    iterator begin();

    const_iterator cbegin() const;

    const_iterator cend() const;

    Properties& clear();

    bool contains(const std::string& key) const;

    const_iterator end() const;

    iterator end();

    const std::string& get(const std::string& key) const;

    const std::string& get(const std::string& key, const std::string& defaultValue) const;

    bool isEmpty() const;

    Properties& remove(const std::string& key);

    Properties& set(const std::string& key, const std::string& value);

    unsigned long size() const;

private:
    std::map<std::string, std::string> m_properties;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_PROPERTIES_HPP
