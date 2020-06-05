/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/Parameter.hpp>

#include <boost/algorithm/string/predicate.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

Parameter::Parameter(std::string&& name, const ParameterType& type, std::string&& description, std::string&& defaultValue) :
    m_type(type),
    m_description(std::move(description)),
    m_defaultValue({std::move(defaultValue)}) {
    m_names.emplace_back(std::move(name));
}

Parameter::Parameter(std::string&& name, const ParameterType& type, std::string&& description, bool defaultValue) :
    m_type(type),
    m_description(std::move(description)),
    m_defaultValue({defaultValue ? "true" : "false"}) {
    m_names.emplace_back(std::move(name));
}

Parameter::Parameter(std::string&& name, const ParameterType& type, std::string&& description, std::set<std::string>&& defaultValue) :
    m_type(type),
    m_description(std::move(description)),
    m_defaultValue(std::move(defaultValue)) {
    m_names.emplace_back(std::move(name));
}

Parameter& Parameter::addAdditionalNames(const std::initializer_list<std::string>& additionalNames) {
    std::copy(additionalNames.begin(), additionalNames.end(), std::back_inserter(m_names));
    return *this;
}

bool Parameter::getBooleanDefaultValue() const {
    return boost::iequals(*m_defaultValue.begin(), "true");
}

const std::string& Parameter::getDescription() const {
    return m_description;
}

const std::string& Parameter::getName() const {
    return m_names.front();
}

const std::vector<std::string>& Parameter::getNames() const {
    return m_names;
}

const std::string& Parameter::getStringDefaultValue() const {
    return *m_defaultValue.begin();
}

const std::set<std::string>& Parameter::getStringListDefaultValue() const {
    return m_defaultValue;
}

const Parameter::ParameterType& Parameter::getType() const {
    return m_type;
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
