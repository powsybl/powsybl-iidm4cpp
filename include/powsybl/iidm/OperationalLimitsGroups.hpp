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

    virtual stdcxx::optional<std::string> getSelectedOperationalLimitsGroupId() const override;
    virtual const std::list<std::string>& getAllSelectedOperationalLimitsGroupIds() const override;

    virtual stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup(const std::string& id) const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup(const std::string& id) override;

    virtual stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup() const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup() override;

    virtual stdcxx::const_range<OperationalLimitsGroup> getAllSelectedOperationalLimitsGroups() const override;
    virtual stdcxx::range<OperationalLimitsGroup> getAllSelectedOperationalLimitsGroups() override;

    virtual OperationalLimitsGroup& newOperationalLimitsGroup(const std::string& id) override;

    virtual void setSelectedOperationalLimitsGroup(const std::string& id) override;

    virtual void addSelectedOperationalLimitsGroups(const std::list<std::string>& ids) override;

    virtual void removeOperationalLimitsGroup(const std::string& id) override;

    virtual void cancelSelectedOperationalLimitsGroup() override;

    virtual void deselectOperationalLimitsGroups(const std::list<std::string>& ids) override;

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
    bool isSelectedOperationalLimitsGroup(const std::string& groupId) const;

    static const char* const DEFAULT_SELECTED_OPERATIONAL_LIMITS_GROUP_ID;

    stdcxx::Reference<Identifiable> m_identifiable;

    std::string m_attributeName;

    std::list<std::string> m_selectedLimitsGroupIds;

    std::map<std::string, std::shared_ptr<OperationalLimitsGroup>> m_operationalLimitsGroupById;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_OPERATIONALLIMITSGROUPS_HPP
