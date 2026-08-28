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

    /**
     * Checks if this object has some properties
     * @return true if has at least one property
     */
    virtual bool hasProperty() const;

    /**
     * Checks if this object has a property associated to a specified key
     * @return true if that key is associated to a property of this object
     */
    virtual bool hasProperty(const std::string& key) const;

    /**
     * Get the property value associated to the specified key, throw if there is no property associated with the specified key
     */
    virtual const std::string& getProperty(const std::string& key) const;

    /**
     * Get the property value associated to the specified key, return the defaultValue if there is no property associated with the specified key
     */
    virtual const std::string& getProperty(const std::string& key, const std::string& defaultValue) const;

    /**
     * Set a property value associated to the specified key
     * @return optional string, containing the previous value associated with that key if there was one. empty string if that key was not associated yet.
     */
    virtual stdcxx::optional<std::string> setProperty(const std::string& key, const std::string& value);

    /**
     * Remove the property associated to the specified key
     * @return true if a property was associated to the given key and has been removed, false otherwise (the key was not found).
     */
    virtual bool removeProperty(const std::string& key);

    /**
     * Gets all the property keys of this object
     */
    virtual stdcxx::const_range<std::string> getPropertyNames() const;

    /**
     * Copy all the properties of this object to another properties holder
     * The destination properties holder previous properties are not removed, though if some properties keys are common, they will be overriden by the value of this object.
     * @param propertiesHolder the destination properties holder
     */
    virtual void copyPropertiesTo(PropertiesHolder& propertiesHolder) const;

private:
    stdcxx::Properties m_properties;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_PROPERTIESHOLDER_HPP
