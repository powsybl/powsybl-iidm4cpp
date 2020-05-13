/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_PROPERTIES_HPP
#define POWSYBL_IIDM_CONVERTER_PROPERTIES_HPP

#include <map>
#include <string>

namespace powsybl {

namespace iidm {

namespace converter {

class Properties {
public:
    Properties() = default;

    explicit Properties(const std::map<std::string, std::string>& properties);

    ~Properties() = default;

    Properties(const Properties&) = default;

    Properties(Properties&&) noexcept = default;

    Properties& operator=(const Properties&) = default;

    Properties& operator=(Properties&&) noexcept = default;

    bool contains(const std::string& property) const;

    template <typename T>
    T get(const std::string& property) const;

    Properties& put(const std::string& property, const std::string& value);

    Properties& remove(const std::string& property);

private:
    std::map<std::string, std::string> m_properties;
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_PROPERTIES_HPP
