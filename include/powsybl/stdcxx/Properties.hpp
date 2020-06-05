/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_STDCXX_PROPERTIES_HPP
#define POWSYBL_STDCXX_PROPERTIES_HPP

#include <map>
#include <string>

#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/range.hpp>

namespace stdcxx {

class Properties {
public:
    using const_iterator = std::map<std::string, std::string>::const_iterator;

    using iterator = std::map<std::string, std::string>::iterator;

public:
    Properties() = default;

    Properties(const Properties&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of std::map is not marked noexcept
    Properties(Properties&&) = default;

    ~Properties() = default;

    Properties& operator=(const Properties&) = default;

    Properties& operator=(Properties&&) = default;

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

    stdcxx::const_range<std::string> getKeys() const;

    bool isEmpty() const;

    Properties& remove(const std::string& key);

    stdcxx::optional<std::string> set(const std::string& key, const std::string& value);

    unsigned long size() const;

private:
    std::map<std::string, std::string> m_properties;
};

}  // namespace stdcxx

#endif  // POWSYBL_STDCXX_PROPERTIES_HPP
