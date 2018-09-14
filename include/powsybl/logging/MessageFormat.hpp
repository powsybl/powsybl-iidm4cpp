/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_LOGGING_MESSAGEFORMAT_HPP
#define POWSYBL_LOGGING_MESSAGEFORMAT_HPP

#include <set>
#include <string>

namespace powsybl {

namespace logging {

/**
 * Format a message with the specified parameters
 *
 * @tparam Args The type of the parameters
 *
 * @param message the parametrized message to format
 * @param parameters the parameters of the message
 *
 * @return the formatted message
 */
template <typename... Args>
std::string format(const std::string& message, const Args&... args);

/**
 * A convenient function to convert an instance of T to a string
 *
 * @tparam T the type of the data to convert to a string
 *
 * @param value the value to convert to string
 *
 * @return a string representation of the value
 */
template <typename T>
std::string toString(const T& value);

}  // namespace logging

}  // namespace powsybl

#include <powsybl/logging/MessageFormat.hxx>

#endif  // POWSYBL_LOGGING_MESSAGEFORMAT_HPP
