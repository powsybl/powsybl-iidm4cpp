/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_PROPERTIESHOLDER_HPP
#define POWSYBL_IIDM_PROPERTIESHOLDER_HPP

#include <powsybl/stdcxx/Properties.hpp>

namespace powsybl {

namespace iidm {

class PropertiesHolder {
public:
    PropertiesHolder() = default;

    PropertiesHolder(const PropertiesHolder&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of stdcxx::Properties is not marked noexcept
    PropertiesHolder(PropertiesHolder&&) = default;  // NOSONAR

    virtual ~PropertiesHolder() noexcept = default;

    PropertiesHolder& operator=(const PropertiesHolder&) = default;

    PropertiesHolder& operator=(PropertiesHolder&&) noexcept = default;


    bool hasProperty() const;

    bool hasProperty(const std::string& key) const;

    const std::string& getProperty(const std::string& key) const;

    const std::string& getProperty(const std::string& key, const std::string& defaultValue) const;

    stdcxx::optional<std::string> setProperty(const std::string& key, const std::string& value);

    bool removeProperty(const std::string& key);

    stdcxx::const_range<std::string> getPropertyNames() const;

private:
    stdcxx::Properties m_properties;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_PROPERTIESHOLDER_HPP
