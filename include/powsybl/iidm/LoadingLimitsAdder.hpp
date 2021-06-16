/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOADINGLIMITSADDER_HPP
#define POWSYBL_IIDM_LOADINGLIMITSADDER_HPP

#include <powsybl/iidm/LoadingLimits.hpp>
#include <powsybl/iidm/OperationalLimitsAdder.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class OperationalLimitsHolder;

template <typename L, typename A>
class LoadingLimitsAdder : public OperationalLimitsAdder<L> {
public:
    class TemporaryLimitAdder {
    public:
        TemporaryLimitAdder(OperationalLimitsHolder& owner, LoadingLimitsAdder<L, A>& parent);

        LoadingLimitsAdder<L, A>& endTemporaryLimit();

        TemporaryLimitAdder& ensureNameUnicity();

        TemporaryLimitAdder& setAcceptableDuration(unsigned long duration);

        TemporaryLimitAdder& setFictitious(bool fictitious);

        TemporaryLimitAdder& setName(const std::string& name);

        TemporaryLimitAdder& setValue(double value);

    private:
        void checkAndGetUniqueName();

    private:
        OperationalLimitsHolder& m_owner;

        LoadingLimitsAdder<L, A>& m_parent;

        std::string m_name;

        double m_value = stdcxx::nan();

        stdcxx::optional<unsigned long> m_acceptableDuration;

        bool m_fictitious = false;

        bool m_ensureNameUnicity = false;
    };

public:
    explicit LoadingLimitsAdder(OperationalLimitsHolder& owner);

    LoadingLimitsAdder(const LoadingLimitsAdder&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of std::map is not marked noexcept
    LoadingLimitsAdder(LoadingLimitsAdder&&) = default;  // NOSONAR

    ~LoadingLimitsAdder() noexcept override = default;

    LoadingLimitsAdder& operator=(const LoadingLimitsAdder&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of std::map is not marked noexcept
    LoadingLimitsAdder& operator=(LoadingLimitsAdder&&) = default;  // NOSONAR

    TemporaryLimitAdder beginTemporaryLimit();

    bool hasTemporaryLimits() const;

    A& setPermanentLimit(double limit);

protected:
    void checkLoadingLimits() const;

    double getPermanentLimit() const;

    const LoadingLimits::TemporaryLimits& getTemporaryLimits() const;

protected:
    OperationalLimitsHolder& m_owner;

private:
    LoadingLimitsAdder<L, A>& addTemporaryLimit(const std::string& name, double value, unsigned long acceptableDuration, bool fictitious);

    void checkTemporaryLimits() const;

    bool nameExists(const std::string& name) const;

private:
    double m_permanentLimit = stdcxx::nan();

    LoadingLimits::TemporaryLimits m_temporaryLimits;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/LoadingLimitsAdder.hxx>

#endif  // POWSYBL_IIDM_LOADINGLIMITSADDER_HPP
