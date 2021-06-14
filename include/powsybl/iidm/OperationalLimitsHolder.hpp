/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OPERATIONALLIMITSHOLDER_HPP
#define POWSYBL_IIDM_OPERATIONALLIMITSHOLDER_HPP

#include <map>
#include <memory>
#include <string>

#include <powsybl/iidm/LimitType.hpp>
#include <powsybl/iidm/OperationalLimits.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class ActivePowerLimitsAdder;
class ApparentPowerLimitsAdder;
class CurrentLimitsAdder;
class Identifiable;

namespace three_windings_transformer {

class Leg;

}  // namespace three_windings_transformer

class OperationalLimitsHolder : public Validable {
public:  // Validable
    std::string getMessageHeader() const override;

public:
    OperationalLimitsHolder(Identifiable& identifiable, std::string&& attributeName);

    OperationalLimitsHolder(std::string&& attributeName);

    OperationalLimitsHolder(const OperationalLimitsHolder&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of std::map is not marked noexcept
    OperationalLimitsHolder(OperationalLimitsHolder&&) = default;  // NOSONAR

    ~OperationalLimitsHolder() noexcept override = default;

    OperationalLimitsHolder& operator=(const OperationalLimitsHolder&) = default;

    OperationalLimitsHolder& operator=(OperationalLimitsHolder&&) = default;

    stdcxx::const_range<OperationalLimits> getOperationalLimits() const;

    stdcxx::range<OperationalLimits> getOperationalLimits();

    template <typename T>
    stdcxx::CReference<T> getOperationalLimits(const LimitType& type) const;

    template <typename T>
    stdcxx::Reference<T> getOperationalLimits(const LimitType& type);

    ActivePowerLimitsAdder newActivePowerLimits();

    ApparentPowerLimitsAdder newApparentPowerLimits();

    CurrentLimitsAdder newCurrentLimits();

    template <typename T>
    stdcxx::Reference<T> setOperationalLimits(const LimitType& limitType, std::unique_ptr<T>&& operationalLimits);

private:
    friend class three_windings_transformer::Leg;

private:
    void setIdentifiable(Identifiable& identifiable);

private:
    std::map<LimitType, std::unique_ptr<OperationalLimits>> m_operationalLimits;

    stdcxx::Reference<Identifiable> m_identifiable;

    std::string m_attributeName;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/OperationalLimitsHolder.hxx>

#endif  // POWSYBL_IIDM_OPERATIONALLIMITSHOLDER_HPP
