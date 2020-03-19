/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CURRENTLIMITSADDER_HPP
#define POWSYBL_IIDM_CURRENTLIMITSADDER_HPP

#include <powsybl/iidm/CurrentLimits.hpp>
#include <powsybl/stdcxx/export.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

template <typename S, typename O>
class IIDM_DECLSPEC CurrentLimitsAdder {
public:
    class TemporaryLimitAdder {
    public:
        ~TemporaryLimitAdder() noexcept = default;

        CurrentLimitsAdder<S, O>& endTemporaryLimit();

        TemporaryLimitAdder& setAcceptableDuration(unsigned long duration);

        TemporaryLimitAdder& setFictitious(bool fictitious);

        TemporaryLimitAdder& setName(const std::string& name);

        TemporaryLimitAdder& setValue(double value);

    private:
        explicit TemporaryLimitAdder(CurrentLimitsAdder<S, O>& parent);

        friend class CurrentLimitsAdder;

    private:
        CurrentLimitsAdder<S, O>& m_parent;

        std::string m_name;

        double m_value{stdcxx::nan()};

        stdcxx::optional<unsigned long> m_acceptableDuration;

        bool m_fictitious{false};
    };

public:
    CurrentLimitsAdder(const S& side, O& owner);

    ~CurrentLimitsAdder() noexcept = default;

    CurrentLimits& add();

    TemporaryLimitAdder beginTemporaryLimit();

    CurrentLimitsAdder<S, O>& setPermanentLimit(double limit);

private:
    CurrentLimitsAdder<S, O>& addTemporaryLimit(const std::string& name, double value, unsigned long acceptableDuration, bool fictitious);

    void checkTemporaryLimits() const;

    const O& getOwner() const;

private:
    S m_side;

    O& m_owner;

    double m_permanentLimit = stdcxx::nan();

    CurrentLimits::TemporaryLimits m_temporaryLimits;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/CurrentLimitsAdder.hxx>

#endif  // POWSYBL_IIDM_CURRENTLIMITSADDER_HPP
