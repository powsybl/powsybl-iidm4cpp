/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_IDBASEDBUSREF_HPP
#define POWSYBL_IIDM_IDBASEDBUSREF_HPP

#include <string>

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/BusRef.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class IdBasedBusRef : public BusRef {
public:
    explicit IdBasedBusRef(const std::string& id);

    IdBasedBusRef(const std::string& id, const Branch::Side& side);

    IdBasedBusRef(const IdBasedBusRef&) = default;

    IdBasedBusRef(IdBasedBusRef&&) noexcept = default;

    ~IdBasedBusRef() noexcept override = default;

    IdBasedBusRef& operator=(const IdBasedBusRef&) = default;

    IdBasedBusRef& operator=(IdBasedBusRef&&) = default;

protected:
    stdcxx::CReference<Bus> resolveByLevel(const Network& network, const TopologyLevel& level) override;

private:
    std::string m_id;

    stdcxx::optional<Branch::Side> m_side;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_IDBASEDBUSREF_HPP
