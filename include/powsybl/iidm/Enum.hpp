/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_ENUM_HPP
#define POWSYBL_IIDM_ENUM_HPP

#include <initializer_list>
#include <string>
#include <type_traits>

namespace powsybl {

namespace iidm {

namespace Enum {

template <typename E, typename = typename std::enable_if<std::is_enum<E>::value>::type>
const std::initializer_list<std::string>& getNames();

template <typename E, typename = typename std::enable_if<std::is_enum<E>::value>::type>
std::string toString(const E& value);

template <typename E, typename = typename std::enable_if<std::is_enum<E>::value>::type>
E fromString(const std::string& name);

}  // namespace Enum

template <typename E>
inline typename std::enable_if<std::is_enum<E>::value, std::ostream&>::type operator<<(std::ostream& stream, const E& value) {
    stream << iidm::Enum::toString(value);
    return stream;
}

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Enum.hxx>

#endif  // POWSYBL_IIDM_ENUM_HPP
