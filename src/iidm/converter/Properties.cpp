/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/Properties.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Enum.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

Properties::Properties(const std::map<std::string, std::string>& properties) :
    m_properties(properties) {
}

bool Properties::contains(const std::string& property) const {
    return m_properties.find(property) != m_properties.end();
}

template <>
bool Properties::get(const std::string& property) const {
    const auto& it = m_properties.find(property);
    if (it != m_properties.end()) {
        return boost::iequals(it->second, "true");
    }
    throw PowsyblException(logging::format("Property %1% does not exist", property));
}

template <>
std::string Properties::get(const std::string& property) const {
    const auto& it = m_properties.find(property);
    if (it != m_properties.end()) {
        return it->second;
    }
    throw PowsyblException(logging::format("Property %1% does not exist", property));
}

template <>
std::set<std::string> Properties::get(const std::string& property) const {
    const auto& it = m_properties.find(property);
    if (it != m_properties.end()) {
        const std::string& valuesStr = it->second;
        std::set<std::string> values;
        boost::algorithm::split(values, valuesStr, boost::is_any_of(",:"));
        return values;
    }
    throw PowsyblException(logging::format("Property %1% does not exist", property));
}

Properties& Properties::put(const std::string& property, const std::string& value) {
    m_properties[property] = value;
    return *this;
}

Properties& Properties::remove(const std::string& property) {
    m_properties.erase(property);
    return *this;
}


}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
