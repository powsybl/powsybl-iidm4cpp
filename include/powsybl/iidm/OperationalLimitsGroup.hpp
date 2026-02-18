/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OPERATIONALLIMITSGROUP_HPP
#define POWSYBL_IIDM_OPERATIONALLIMITSGROUP_HPP

#include <map>
#include <memory>
#include <string>

#include <powsybl/iidm/LimitType.hpp>
#include <powsybl/iidm/OperationalLimits.hpp>
#include <powsybl/iidm/Validable.hpp>

#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class ActivePowerLimits;
class ActivePowerLimitsAdder;
class ApparentPowerLimits;
class ApparentPowerLimitsAdder;
class CurrentLimits;
class CurrentLimitsAdder;

class Identifiable;
class Network;

class OperationalLimitsGroup : public Validable {
public:
    OperationalLimitsGroup(const std::string& id, Identifiable& identifiable, const std::string& attributeName, const std::string& selectedGroupId);
    OperationalLimitsGroup(const std::string& id, Identifiable& identifiable, const std::string& attributeName);

    OperationalLimitsGroup(const OperationalLimitsGroup&) = default;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of std::map is not marked noexcept
    OperationalLimitsGroup(OperationalLimitsGroup&&) noexcept = default; // NOSONAR

    ~OperationalLimitsGroup() noexcept override = default;

    OperationalLimitsGroup& operator=(const OperationalLimitsGroup&) = default;

    OperationalLimitsGroup& operator=(OperationalLimitsGroup&&) noexcept = default;

    // Validable
    virtual std::string getMessageHeader() const override;

    const Validable& getValidable() const;

    const Network& getNetwork() const;
    Network& getNetwork();

    const std::string& getId() const;

    void setSelectedGroupId(const std::string& selectedGroupId);
    void cancelSelectedGroupId();

    stdcxx::CReference<ActivePowerLimits> getActivePowerLimits() const;
    stdcxx::Reference<ActivePowerLimits> getActivePowerLimits();
    stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits() const;
    stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits();
    stdcxx::CReference<CurrentLimits> getCurrentLimits() const;
    stdcxx::Reference<CurrentLimits> getCurrentLimits();
    template <typename T>
    stdcxx::CReference<T> getOperationalLimits(const LimitType& type) const;
    template <typename T>
    stdcxx::Reference<T> getOperationalLimits(const LimitType& type);

    ActivePowerLimitsAdder newActivePowerLimits();
    ApparentPowerLimitsAdder newApparentPowerLimits();
    CurrentLimitsAdder newCurrentLimits();
    ActivePowerLimitsAdder newActivePowerLimits(const ActivePowerLimits& limits);
    ApparentPowerLimitsAdder newApparentPowerLimits(const ApparentPowerLimits& limits);
    CurrentLimitsAdder newCurrentLimits(const CurrentLimits& limits);

    stdcxx::Reference<OperationalLimits> setOperationalLimits(const LimitType& limitType, const std::shared_ptr<OperationalLimits>& operationalLimits);

    void removeCurrentLimits();
    void removeActivePowerLimits();
    void removeApparentPowerLimits();
    void removeOperationalLimits(const LimitType& type);

    bool isEmpty() const;

private:
    std::string m_id;

    std::map<LimitType, std::shared_ptr<OperationalLimits>> m_operationalLimits;

    Identifiable& m_identifiable;

    std::string m_attributeName;

    stdcxx::optional<std::string> m_selectedGroupId;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/OperationalLimitsGroup.hxx>

#endif  // POWSYBL_IIDM_OPERATIONALLIMITSGROUP_HPP
