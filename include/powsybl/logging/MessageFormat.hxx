/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_LOGGING_MESSAGEFORMAT_HXX
#define POWSYBL_LOGGING_MESSAGEFORMAT_HXX

#include <powsybl/logging/MessageFormat.hpp>

#include <iterator>
#include <sstream>

#include <boost/format.hpp>

namespace powsybl {

namespace logging {

inline void format(boost::format& /*fmt*/) {
    // This function is used to stop the recursion
}

template <typename Arg, typename... Args>
inline void format(boost::format& fmt, const Arg& arg, const Args&... args) {
    fmt % arg;
    format(fmt, args...);
}

template <typename... Args>
std::string format(const std::string& message, const Args&... args) {
    boost::format fmt(message);
    format(fmt, args...);

    return fmt.str();
}

template <typename Iterator>
void serialize(std::ostream& stream, const Iterator& begin, const Iterator& end) {
    bool first = true;

    for (Iterator it = begin; it != end; ++it) {
        if (!first) {
            stream << ", ";
        }
        stream << *it;
        first = false;
    }
}

template <typename T>
std::string toString(const std::initializer_list<T>& list) {
    std::ostringstream oss;

    oss << "[";
    serialize(oss, list.begin(), list.end());
    oss << "]";

    return oss.str();
}

template <typename T>
std::string toString(const std::set<T>& set) {
    std::ostringstream oss;

    oss << "{";
    serialize(oss, set.begin(), set.end());
    oss << "}";

    return oss.str();
}

template <typename T>
std::string toString(const std::vector<T>& vect) {
    std::ostringstream oss;

    oss << "{";
    serialize(oss, vect.begin(), vect.end());
    oss << "}";

    return oss.str();
}

template <std::size_t N, typename T, typename>
std::string toString(const std::array<std::string, N>& names, T value) {
    auto val = static_cast<unsigned int>(value);
    return (val < N) ? names.at(val) : format("%1%", val);
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const std::reference_wrapper<T>& ref) {
    stream << ref.get();

    return stream;
}

}  // namespace logging

}  // namespace powsybl

#endif  // POWSYBL_LOGGING_MESSAGEFORMAT_HXX
