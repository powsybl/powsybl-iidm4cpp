/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_BUSFILTER_HPP
#define POWSYBL_IIDM_CONVERTER_BUSFILTER_HPP

#include <set>
#include <string>

#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class Connectable;
class Network;

namespace converter {

class ExportOptions;

class IIDM_DECLSPEC BusFilter {
public:
    static BusFilter create(const Network& network, const ExportOptions& options);

public:
    bool test(const Bus& bus) const;

    bool test(const Connectable& connectable) const;

private:
    BusFilter(const std::set<std::string>& buses, const ExportOptions& options);

    BusFilter(const ExportOptions& options);

private:
    std::set<std::string> m_buses;

    const ExportOptions& m_options;
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_BUSFILTER_HPP

