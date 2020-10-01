/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Parameter.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

Parameter::Parameter(std::string&& name, const Type& type, std::string&& description, std::string&& defaultValue) :
    m_names({std::move(name)}),
    m_type(type),
    m_description(std::move(description)),
    m_defaultValue({std::move(defaultValue)}) {
}

Parameter& Parameter::addAdditionalNames(const std::initializer_list<std::string>& additionalNames) {
    std::move(additionalNames.begin(), additionalNames.end(), std::back_inserter(m_names));
    return *this;
}

bool Parameter::getBooleanDefaultValue() const {
    return boost::iequals(m_defaultValue, "true");
}

const std::string& Parameter::getDescription() const {
    return m_description;
}

double Parameter::getDoubleDefaultValue() const {
    return std::stod(m_defaultValue);
}

const std::string& Parameter::getName() const {
    return m_names.front();
}

const std::vector<std::string>& Parameter::getNames() const {
    return m_names;
}

const std::string& Parameter::getStringDefaultValue() const {
    return m_defaultValue;
}

std::vector<std::string> Parameter::getStringListDefaultValue() const {
    std::vector<std::string> values;
    if (!m_defaultValue.empty()) {
        boost::algorithm::split(values, m_defaultValue, boost::is_any_of(",:"));
    }
    return values;
}

const Parameter::Type& Parameter::getType() const {
    return m_type;
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
