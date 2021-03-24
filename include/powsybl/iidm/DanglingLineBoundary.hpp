/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DANGLINGLINEBOUNDARY_HPP
#define POWSYBL_IIDM_DANGLINGLINEBOUNDARY_HPP

#include <powsybl/iidm/Boundary.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class DanglingLine;

namespace dangling_line {

class Boundary : public iidm::Boundary {
public:  // iidm::Boundary
    double getAngle() const override;

    double getP() const override;

    double getQ() const override;

    double getV() const override;

public:
    explicit Boundary(DanglingLine& parent);

    Boundary(const Boundary&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of std::function marked noexcept (see half_line::Boundary)
    Boundary(Boundary&&) = default;  // NOSONAR

    ~Boundary() noexcept override = default;

    Boundary& operator=(const Boundary&) = delete;

    Boundary& operator=(Boundary&&) noexcept = delete;

private:
    static double getAngle(const stdcxx::CReference<Bus>& bus);

    static double getV(const stdcxx::CReference<Bus>& bus);

private:
    DanglingLine& m_parent;
};

}  // namespace dangling_line

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DANGLINGLINEBOUNDARY_HPP
