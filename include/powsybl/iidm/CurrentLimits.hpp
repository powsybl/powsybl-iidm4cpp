/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CURRENTLIMITS_HPP
#define POWSYBL_IIDM_CURRENTLIMITS_HPP

#include <map>
#include <string>
#include <vector>

namespace powsybl {

namespace iidm {

class Validable;

class CurrentLimits {
public:
    class TemporaryLimit {
    public:
        TemporaryLimit(const std::string& name, double value, unsigned long acceptableDuration, bool fictitious);

        ~TemporaryLimit() noexcept = default;

        unsigned long getAcceptableDuration() const;

        const std::string& getName() const;

        double getValue() const;

        bool isFictitious() const;

    private:
        std::string m_name;

        double m_value;

        unsigned long m_acceptableDuration;

        bool m_fictitious;
    };

    typedef std::map<unsigned long, TemporaryLimit, std::greater<unsigned long>> TemporaryLimits;

public:
    CurrentLimits(Validable& owner, double permanentLimit, TemporaryLimits temporaryLimits);

    ~CurrentLimits() noexcept = default;

    double getPermanentLimit() const;

    const TemporaryLimit& getTemporaryLimit(unsigned long acceptableDuration) const;

    TemporaryLimit& getTemporaryLimit(unsigned long acceptableDuration);

    std::vector<std::reference_wrapper<const TemporaryLimit>> getTemporaryLimits() const;

    std::vector<std::reference_wrapper<TemporaryLimit>> getTemporaryLimits();

    double getTemporaryLimitValue(unsigned long acceptableDuration) const;

    CurrentLimits& setPermanentLimit(double permanentLimit);

private:
    Validable& m_owner;

    double m_permanentLimit;

    TemporaryLimits m_temporaryLimits;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CURRENTLIMITS_HPP
