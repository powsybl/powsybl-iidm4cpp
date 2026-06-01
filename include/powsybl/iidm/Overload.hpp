/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OVERLOAD_HPP
#define POWSYBL_IIDM_OVERLOAD_HPP

#include <string>

#include <powsybl/iidm/LoadingLimits.hpp>

namespace powsybl {

namespace iidm {

class Overload {
public:
    Overload(const LoadingLimits::TemporaryLimit &temporaryLimit, const std::string &previousLimitName, double previousLimit, double limitReductionCoefficient = 1.0);

    Overload(const std::string &previousLimitName, double previousLimit, double limitReductionCoefficient = 1.0);

    ~Overload() noexcept = default;

    double getPreviousLimit() const;

    const std::string &getPreviousLimitName() const;

    const LoadingLimits::TemporaryLimit &getTemporaryLimit() const;

    double getLimitReductionCoefficient() const;

private:
    /**
     * returns a default "Unacceptable" fictitious limit of acceptable duration 0 and infinite value,
     * internally used for overload without an acceptable temporary limit.
     */
    static const LoadingLimits::TemporaryLimit& UNACCEPTABLE_LIMIT();

    LoadingLimits::TemporaryLimit m_temporaryLimit;

    std::string m_previousLimitName;

    double m_previousLimit;

    double m_limitReductionCoefficient;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_OVERLOAD_HPP
