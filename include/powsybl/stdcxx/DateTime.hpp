/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DATETIME_HPP
#define POWSYBL_IIDM_DATETIME_HPP

#include <iosfwd>
#include <string>

#include <boost/date_time/local_time/local_time.hpp>

namespace stdcxx {

class DateTime {
public:
    /**
     * Parse a string representation of a ISO-8601 date time.
     *
     * @param date The string to parse
     *
     * @return A {@link DateTime}
     */
    static DateTime parse(const std::string& date);

public:
    /**
     * Create a new {@link DateTime} object initialized at the current time, in UTC.
     */
    DateTime();

    /**
     * Return a string representation of this {@link DateTime} in ISO-8601 format.
     *
     * @return a string representation of this {@link DateTime} in ISO-8601 format.
     */
    std::string toString() const;

public:
    /**
     * Return true if the two specified {@link DateTime} objects are equivalent (not necessary in the same timezone).
     *
     * @param dt1 The first {@link DateTime} object to compare.
     * @param dt2 The second {@link DateTime} object to compare.
     *
     * @return true if the two {@link DateTime} objects are equivalent, false otherwise.
     */
    friend bool operator==(const DateTime& dt1, const DateTime& dt2);

    /**
     * Return true if the two specified {@link DateTime} objects are not equivalent.
     *
     * @param dt1 The first {@link DateTime} object to compare.
     * @param dt2 The second {@link DateTime} object to compare.
     *
     * @return true if the two {@link DateTime} objects are not equivalent, false otherwise.
     */
    friend bool operator!=(const DateTime& dt1, const DateTime& dt2);

private:
    explicit DateTime(const boost::local_time::local_date_time& dateTime);

private:
    boost::local_time::local_date_time m_dateTime;
};

/**
 * Serialize the specified {@link DateTime} object to a stream as an ISO-8601 date time.
 *
 * @param stream The output stream to use.
 * @param dt The {@link DateTime} object to serialize.
 *
 * @return The modified output stream.
 */
std::ostream& operator<<(std::ostream& stream, const DateTime& dt);

}  // namespace stdcxx

#endif  // POWSYBL_IIDM_DATETIME_HPP
