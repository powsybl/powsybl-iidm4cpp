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
#include <powsybl/iidm/ValidationLevel.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

template <typename L, typename A>
class LoadingLimitsAdder : public OperationalLimitsAdder<L> {
public:
    class TemporaryLimitAdder : public PropertiesHolder {
    public:
        TemporaryLimitAdder(OperationalLimitsGroup& owner, LoadingLimitsAdder<L, A>& parent);

        LoadingLimitsAdder<L, A>& endTemporaryLimit();

        TemporaryLimitAdder& ensureNameUnicity();

        TemporaryLimitAdder& setAcceptableDuration(unsigned long duration);

        TemporaryLimitAdder& setFictitious(bool fictitious);

        TemporaryLimitAdder& setName(const std::string& name);

        TemporaryLimitAdder& setValue(double value);

        TemporaryLimitAdder& addProperty(const std::string& key, const std::string& value);

    private:
        void checkAndGetUniqueName();

    private:
        OperationalLimitsGroup& m_owner;

        LoadingLimitsAdder<L, A>& m_parent;

        std::string m_name;

        double m_value = stdcxx::nan();

        stdcxx::optional<unsigned long> m_acceptableDuration;

        bool m_fictitious = false;

        bool m_ensureNameUnicity = false;
    };

public:
    explicit LoadingLimitsAdder(OperationalLimitsGroup& owner);

    /**
     * Initialize this new limits adder with given limits values
     */
    explicit LoadingLimitsAdder(OperationalLimitsGroup& owner, const L& limits);

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
    ValidationLevel checkLoadingLimits() const;

public:
    double getPermanentLimit() const;

    const LoadingLimits::TemporaryLimits& getTemporaryLimits() const;

    const LoadingLimits::FictitiousLimits& getFictitiousLimits() const;

    double getTemporaryLimitValue(unsigned long acceptableDuration) const;
    double getTemporaryLimitValue(const std::string& name) const;
    unsigned long getTemporaryLimitAcceptableDuration(const std::string& name) const;
    double getLowestTemporaryLimitValue();
    const std::vector<std::string> getTemporaryLimitNames() const;

    void removeTemporaryLimit(const std::string& name);
    
    A& fixLimits();
    A& fixLimits(double missingPermanentLimitPercentage);


protected:
    OperationalLimitsGroup& m_owner;

private:
    LoadingLimitsAdder<L, A>& addTemporaryLimit(LoadingLimits::TemporaryLimit&& temporaryLimit);
    LoadingLimitsAdder<L, A>& addFictitiousLimit(LoadingLimits::TemporaryLimit&& fictitiousLimit);

    stdcxx::optional<LoadingLimits::TemporaryLimit> getTemporaryLimitByName(const std::string& name) const;

    bool nameExists(const std::string& name) const;

private:
    double m_permanentLimit = stdcxx::nan();

    LoadingLimits::TemporaryLimits m_temporaryLimits;

    LoadingLimits::FictitiousLimits m_fictitiousLimits;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/LoadingLimitsAdder.hxx>

#endif  // POWSYBL_IIDM_LOADINGLIMITSADDER_HPP
