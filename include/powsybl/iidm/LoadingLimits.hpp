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
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class OperationalLimitsHolder;

class LoadingLimits : public OperationalLimits {
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

public:
    LoadingLimits(OperationalLimitsHolder& owner, double permanentLimit, TemporaryLimits temporaryLimits);

    LoadingLimits(const LoadingLimits&) = default;

    LoadingLimits(LoadingLimits&&) noexcept = default;

    ~LoadingLimits() override = default;

    LoadingLimits& operator=(const LoadingLimits&) = delete;

    LoadingLimits& operator=(LoadingLimits&&) noexcept = delete;

    double getPermanentLimit() const;

    const TemporaryLimit& getTemporaryLimit(unsigned long acceptableDuration) const;

    TemporaryLimit& getTemporaryLimit(unsigned long acceptableDuration);

    std::vector<std::reference_wrapper<const TemporaryLimit>> getTemporaryLimits() const;

    std::vector<std::reference_wrapper<TemporaryLimit>> getTemporaryLimits();

    double getTemporaryLimitValue(unsigned long acceptableDuration) const;

    LoadingLimits& setPermanentLimit(double permanentLimit);

protected:
    OperationalLimitsHolder& m_owner;

private:
    double m_permanentLimit;

    TemporaryLimits m_temporaryLimits;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOADINGLIMITS_HPP
