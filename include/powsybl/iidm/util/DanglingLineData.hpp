/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_DANGLINGLINEDATA_HPP
#define POWSYBL_IIDM_UTIL_DANGLINGLINEDATA_HPP

#include <string>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class DanglingLine;

class DanglingLineData {
public:
    explicit DanglingLineData(const DanglingLine& danglingLine);

    DanglingLineData(const DanglingLine& danglingLine, bool splitShuntAdmittance);

    double getBoundaryBusTheta() const;

    double getBoundaryBusU() const;

    const std::string& getId() const;

private:
    static double getTheta(const DanglingLine& danglingLine);

    static double getV(const DanglingLine& danglingLine);

    static bool valid(double v, double theta);

private:
    const DanglingLine& m_danglingLine;

    double m_boundaryBusU = stdcxx::nan();
    double m_boundaryBusTheta = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_DANGLINGLINEDATA_HPP
