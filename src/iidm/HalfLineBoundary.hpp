/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_HALFLINEBOUNDARY_HPP
#define POWSYBL_IIDM_HALFLINEBOUNDARY_HPP

#include <powsybl/iidm/util/AbstractHalfLineBoundary.hpp>

namespace powsybl {

namespace iidm {

namespace half_line {

class Boundary : public util::half_line::AbstractHalfLineBoundary {
    // side represents the network side.
    // side here is Side.ONE for the half line 1 of a tie line.
    // side is Side.TWO for the half line 2 of a tie line.

public:  // iidm::Boundary
    const Connectable& getConnectable() const override;

    Connectable& getConnectable() override;

public:
    Boundary(tie_line::HalfLine& halfLine, const Branch::Side& side);

    Boundary(const Boundary&) = delete;

    Boundary(Boundary&&) = delete;

    ~Boundary() noexcept override = default;

    Boundary& operator=(const Boundary&) = delete;

    Boundary& operator=(Boundary&&) noexcept = delete;
};

}  // namespace half_line

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_HALFLINEBOUNDARY_HPP

