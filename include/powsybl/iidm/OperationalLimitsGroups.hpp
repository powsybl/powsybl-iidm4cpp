/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_OPERATIONALLIMITSGROUPS_HPP
#define POWSYBL_IIDM_OPERATIONALLIMITSGROUPS_HPP

#include <map>
#include <memory>
#include <string>

#include <powsybl/iidm/LimitType.hpp>
#include <powsybl/iidm/FlowsLimitsHolder.hpp>

namespace powsybl {

namespace iidm {

class OperationalLimitsGroups : public FlowsLimitsHolder {
public:
    OperationalLimitsGroups(Identifiable& identifiable, const std::string& attributeName);

    OperationalLimitsGroups(const std::string& attributeName);

    OperationalLimitsGroups(const OperationalLimitsGroups&) = default;

    OperationalLimitsGroups(OperationalLimitsGroups&&) noexcept = default;

    virtual ~OperationalLimitsGroups() noexcept override = default;

    OperationalLimitsGroups& operator=(const OperationalLimitsGroups&) = default;

    OperationalLimitsGroups& operator=(OperationalLimitsGroups&&) noexcept = default;

    void setIdentifiable(Identifiable& identifiable);

// FlowsLimitsHolder
    virtual stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups() const override;
    virtual stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups() override;

    virtual const stdcxx::optional<std::string>& getSelectedOperationalLimitsGroupId() const override;

    virtual stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup(const std::string& id) const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup(const std::string& id) override;

    virtual stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup() const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup() override;

    virtual OperationalLimitsGroup& newOperationalLimitsGroup(const std::string& id) override;

    virtual void setSelectedOperationalLimitsGroup(const std::string& id) override;

    virtual void removeOperationalLimitsGroup(const std::string& id) override;

    virtual void cancelSelectedOperationalLimitsGroup() override;
    /**
     * Get the OperationalLimitsGroup selected or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup() override;
    /**
     * Get the OperationalLimitsGroup corresponding to the given id or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup(const std::string& id) override;

    /** \deprecated
    * Use OperationalLimitsGroup::newActivePowerLimits() instead.
    */
    virtual ActivePowerLimitsAdder newActivePowerLimits() override;
    /** \deprecated
    * Use OperationalLimitsGroup::newApparentPowerLimits() instead.
    */
    virtual ApparentPowerLimitsAdder newApparentPowerLimits() override;
    /** \deprecated
    * Use OperationalLimitsGroup::newCurrentLimits() instead.
    */
    virtual CurrentLimitsAdder newCurrentLimits() override;

private:
    static const char* const DEFAULT_SELECTED_OPERATIONAL_LIMITS_GROUP_ID;

    stdcxx::Reference<Identifiable> m_identifiable;

    std::string m_attributeName;

    stdcxx::optional<std::string> m_selectedLimitsGroupId;

    std::map<std::string, std::shared_ptr<OperationalLimitsGroup>> m_operationalLimitsGroupById;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_OPERATIONALLIMITSGROUPS_HPP
