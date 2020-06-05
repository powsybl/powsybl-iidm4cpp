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

#include <boost/any.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

class Parameter {
public:
    enum class ParameterType : char {
        BOOLEAN,
        STRING,
        STRING_LIST
    };

public:
    Parameter(std::string&& name, const ParameterType& type, std::string&& description, std::string&& defaultValue);

    Parameter(std::string&& name, const ParameterType& type, std::string&& description, bool defaultValue);

    Parameter(std::string&& name, const ParameterType& type, std::string&& description, std::set<std::string>&& defaultValue);

    ~Parameter() = default;

    Parameter(const Parameter&) = default;

    Parameter(Parameter&&) noexcept = default;

    Parameter& operator=(const Parameter&) = default;

    Parameter& operator=(Parameter&&) = delete;

    Parameter& addAdditionalNames(const std::initializer_list<std::string>& additionalNames);

    bool getBooleanDefaultValue() const;

    const std::string& getDescription() const;

    const std::string& getName() const;

    const std::vector<std::string>& getNames() const;

    const std::string& getStringDefaultValue() const;

    const std::set<std::string>& getStringListDefaultValue() const;

    const ParameterType& getType() const;

private:
    std::vector<std::string> m_names;

    ParameterType m_type;

    std::string m_description;

    std::set<std::string> m_defaultValue;
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_PARAMETER_HPP
