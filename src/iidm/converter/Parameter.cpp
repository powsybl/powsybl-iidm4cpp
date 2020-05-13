/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/Parameter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

Parameter::Parameter(const std::string& name, const ParameterType& type, const std::string& description, const boost::any& defaultValue) :
    m_type(type), m_description(description), m_defaultValue(defaultValue) {
    m_names.emplace_back(name);
}

Parameter& Parameter::addAdditionalNames(const std::string& additionalName) {
    m_names.emplace_back(additionalName);
    return *this;
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

const ParameterType& Parameter::getType() const {
    return m_type;
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
