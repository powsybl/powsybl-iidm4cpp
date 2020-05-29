/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/stdcxx/DateTime.hpp>

#include <ctime>

#include <boost/shared_ptr.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace stdcxx {

DateTime DateTime::parse(const std::string& date) {
    static std::string s_iso8601 = "%Y-%m-%dT%H:%M:%S%F%ZP";

    boost::local_time::local_date_time localDateTime(boost::local_time::special_values::not_a_date_time);

    std::istringstream buffer(date);

    auto facet = stdcxx::make_unique<boost::local_time::local_time_input_facet>(s_iso8601);
    buffer.imbue(std::locale(std::locale::classic(), facet.release()));

    std::string dummy;
    if ((buffer >> localDateTime) && (buffer >> dummy).eof()) {
        return DateTime(localDateTime);
    }

    throw powsybl::PowsyblException(stdcxx::format("Unable to parse the date '%1%'", date));
}

DateTime::DateTime() :
    m_dateTime(boost::posix_time::second_clock::universal_time(), boost::local_time::time_zone_ptr()) {
}

DateTime::DateTime(const boost::local_time::local_date_time& dateTime) :
    m_dateTime(dateTime) {
}

std::string DateTime::toString() const {
    static std::string s_iso8601 = "%Y-%m-%dT%H:%M:%S%F%Q";

    auto facet = stdcxx::make_unique<boost::local_time::local_time_facet>(s_iso8601.c_str());

    std::ostringstream oss;
    oss.imbue(std::locale(std::locale::classic(), facet.release()));
    oss << m_dateTime;

    return oss.str();
}

bool operator==(const DateTime& dt1, const DateTime& dt2) {
    return dt1.m_dateTime.utc_time() == dt2.m_dateTime.utc_time();
}

bool operator!=(const DateTime& dt1, const DateTime& dt2) {
    return dt1.m_dateTime.utc_time() != dt2.m_dateTime.utc_time();
}

std::ostream& operator<<(std::ostream& stream, const DateTime& dt) {
    stream << dt.toString();

    return stream;
}

}  // namespace stdcxx
