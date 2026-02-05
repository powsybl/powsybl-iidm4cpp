/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_PARAMETER_HPP
#define POWSYBL_IIDM_CONVERTER_PARAMETER_HPP

#include <initializer_list>
#include <set>
#include <string>
#include <vector>

namespace powsybl {

namespace iidm {

namespace converter {

class Parameter {
public:
    enum class Type : unsigned char {
        BOOLEAN,
        STRING,
        STRING_LIST,
        DOUBLE,
        INTEGER
    };

    enum class Scope : unsigned char {
        FUNCTIONAL,
        TECHNICAL
    };

public:
    Parameter(std::string&& name, const Type& type, std::string&& description, std::string&& defaultValue, const Scope& scope = Scope::FUNCTIONAL, std::string&& categoryKey = std::string(""));

    ~Parameter() = default;

    Parameter(const Parameter&) = default;

    Parameter(Parameter&&) noexcept = default;

    Parameter& operator=(const Parameter&) = default;

    Parameter& operator=(Parameter&&) = default;

    Parameter& addAdditionalNames(const std::initializer_list<std::string>& additionalNames);

    bool getBooleanDefaultValue() const;

    const std::string& getDescription() const;

    double getDoubleDefaultValue() const;

    const std::string& getName() const;

    const std::vector<std::string>& getNames() const;

    const std::string& getStringDefaultValue() const;

    std::vector<std::string> getStringListDefaultValue() const;

    int getIntDefaultValue() const;

    const Type& getType() const;

    const Scope& getScope() const;

    const std::string& getCategoryKey() const;

private:
    std::vector<std::string> m_names;

    Type m_type;

    std::string m_description;

    std::string m_defaultValue;

    Scope m_scope;

    std::string m_categoryKey;
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_PARAMETER_HPP
