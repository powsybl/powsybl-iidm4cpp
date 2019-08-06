/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSVIEW_HPP
#define POWSYBL_IIDM_BUSVIEW_HPP

#include <functional>
#include <string>
#include <vector>

#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Bus;

class BusView {
public:
    virtual ~BusView() noexcept = default;

    virtual stdcxx::Reference<Bus> getBus(const std::string& busId) const = 0;

    virtual stdcxx::const_range<Bus> getBuses() const = 0;

    virtual stdcxx::range<Bus> getBuses() = 0;

    virtual stdcxx::Reference<Bus> getMergedBus(const std::string& configuredBusId) const = 0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSVIEW_HPP
