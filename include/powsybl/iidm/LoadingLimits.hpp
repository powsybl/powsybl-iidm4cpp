/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOADINGLIMITS_HPP
#define POWSYBL_IIDM_LOADINGLIMITS_HPP

#include <functional>
#include <map>
#include <string>
#include <vector>

#include <powsybl/iidm/OperationalLimits.hpp>
#include <powsybl/iidm/OperationalLimitsGroup.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class LoadingLimits : public OperationalLimits {
private:
    // defines an epsilon to filter out small temporary limits changes from log warnings
    static const double TEMPORARY_LIMIT_EPSILON;

public:
    class TemporaryLimit {
    public:
        TemporaryLimit(const std::string& name, double value, unsigned long acceptableDuration, bool hasOverloadingProtection);

        ~TemporaryLimit() noexcept = default;

        unsigned long getAcceptableDuration() const;

        const std::string& getName() const;

        double getValue() const;

        bool isFictitious() const;

    private:
        std::string m_name;

        double m_value;

        unsigned long m_acceptableDuration;

        bool m_isFictitious;
    };

    using TemporaryLimits = std::map<unsigned long, TemporaryLimit, std::greater<unsigned long> >;
    
    using FictitiousLimits = std::map<std::string, TemporaryLimit, std::greater<std::string > >;

public:
    LoadingLimits(OperationalLimitsGroup& owner, double permanentLimit, const TemporaryLimits& temporaryLimits, const FictitiousLimits& fictitiousLimits);

    LoadingLimits(const LoadingLimits&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of std::map is not marked noexcept
    LoadingLimits(LoadingLimits&&) = default;  // NOSONAR

    ~LoadingLimits() override = default;

    LoadingLimits& operator=(const LoadingLimits&) = delete;

    LoadingLimits& operator=(LoadingLimits&&) noexcept = delete;

    double getPermanentLimit() const;

    const TemporaryLimit& getTemporaryLimit(unsigned long acceptableDuration) const;

    TemporaryLimit& getTemporaryLimit(unsigned long acceptableDuration);

    stdcxx::const_range<TemporaryLimit> getTemporaryLimits() const;

    stdcxx::range<TemporaryLimit> getTemporaryLimits();

    stdcxx::const_range<TemporaryLimit> getFictitiousLimits() const;

    stdcxx::range<TemporaryLimit> getFictitiousLimits();

    double getTemporaryLimitValue(unsigned long acceptableDuration) const;

    LoadingLimits& setPermanentLimit(double permanentLimit);

    LoadingLimits& setTemporaryLimitValue(unsigned long acceptableDuration, double temporaryLimitValue);

protected:
    bool isTemporaryLimitValueValid(unsigned long acceptableDuration, double temporaryLimitValue);

    stdcxx::Reference<OperationalLimitsGroup> m_limitsGroup;

private:
    double m_permanentLimit;

    TemporaryLimits m_temporaryLimits;

    FictitiousLimits m_fictitiousLimits;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOADINGLIMITS_HPP
