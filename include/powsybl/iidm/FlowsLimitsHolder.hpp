/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_FLOWSLIMITSHOLDER_HPP
#define POWSYBL_IIDM_FLOWSLIMITSHOLDER_HPP

#include <functional>
#include <list>
#include <vector>

#include <powsybl/iidm/OperationalLimits.hpp>
#include <powsybl/iidm/OperationalLimitsGroup.hpp>
#include <powsybl/stdcxx/Predicate.hpp>
#include <powsybl/stdcxx/range.hpp>
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
class LoadingLimits;

class FlowsLimitsHolder {
public:
    FlowsLimitsHolder() = default;

    FlowsLimitsHolder(const FlowsLimitsHolder&) = default;

    FlowsLimitsHolder(FlowsLimitsHolder&&) = default;

    virtual ~FlowsLimitsHolder() noexcept = default;

    FlowsLimitsHolder& operator=(const FlowsLimitsHolder&) = default;

    FlowsLimitsHolder& operator=(FlowsLimitsHolder&&) = default;

    virtual stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups() const = 0;
    virtual stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups() = 0;

    /**
     * Get the ID of the last OperationalLimitsGroup selected.
     * If the last selected group was deselected, it is the last selected before that one which is retrieved (repeated until there is no selected group in the stack)
     * return an empty optional if no selected group left.
     */
    virtual stdcxx::optional<std::string> getSelectedOperationalLimitsGroupId() const = 0;

    /**
     * Get the ordered list of selected OperationalLimitsGroup IDs.
     * If an element already selected, is selected again, it is replaced at the end of that list to be considered as the last selected again.
     * For example, if two groups A and B are selected in that order : A, B, A, the ordered list is {B, A}
     */
    virtual const std::list<std::string>& getAllSelectedOperationalLimitsGroupIds() const = 0;

    virtual stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup(const std::string& id) const = 0;
    virtual stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup(const std::string& id) = 0;

    /**
     * Get the last selected OperationalLimitsGroup.
     * Return an empty reference if no selected group found.
     */
    virtual stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup() const = 0;
    /**
     * Get the last selected OperationalLimitsGroup.
     * Return an empty reference if no selected group found.
     */
    virtual stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup() = 0;

    /**
     * Get all the selected OperationalLimitsGroup.
     * Retrieved in the groups storage order so it is not guaranteed to match the selection order.
     */
    virtual stdcxx::const_range<OperationalLimitsGroup> getAllSelectedOperationalLimitsGroups() const = 0;
    /**
     * Get all the selected OperationalLimitsGroup.
     * Retrieved in the groups storage order so it is not guaranteed to match the selection order.
     */
    virtual stdcxx::range<OperationalLimitsGroup> getAllSelectedOperationalLimitsGroups() = 0;

    virtual OperationalLimitsGroup& newOperationalLimitsGroup(const std::string& id) = 0;

    /**
     * Set the given OperationalLimitsGroup as the only selected one.
     * If other groups were selected before, they are all deselected.
     * Throw a PowsyblException if there is no OperationalLimits Group corresponding to the given ID.
     * If the given ID is empty, does nothing.
     * To deselect a selected group, use deselectOperationalLimitsGroups(const std::list<std::string>&).
     * To deselect all the selected groups, use cancelSelectedOperationalLimitsGroup().
     */
    virtual void setSelectedOperationalLimitsGroup(const std::string& id) = 0;

    /**
     * Set the given OperationalLimitsGroup IDs as selected, in the order of the given list.
     * Throw a PowsyblException if any of the IDs does not correspond to any OperationalLimitsGroup.
     * Throw a PowsyblException if an empty ID is encountered.
     * If the list is empty, does nothing.
     * To deselect a selected group, use deselectOperationalLimitsGroups(const std::list<std::string>&).
     * To deselect all the selected groups, use cancelSelectedOperationalLimitsGroup().
     */
    virtual void addSelectedOperationalLimitsGroups(const std::list<std::string>& ids) = 0;

    /**
     * Set all the OperationalLimitsGroup whose ID match the Predicate as selected.
     * Selection ordered is performed following the storage order of the groups (which is based on their IDs)
     * Does not deselect the previously selected group.
     */
    virtual void addSelectedOperationalLimitsGroupsByPredicate(const stdcxx::Predicate<std::string>& operationalLimitsGroupIdPredicate);

    /**
     * Remove the OperationalLimitsGroup that match the given ID
     * If the given ID is empty or no corresponding group found, does nothing.
     */
    virtual void removeOperationalLimitsGroup(const std::string& id) = 0;

    /**
     * Deselect all the previously selected OperationalLimitsGroup
     */
    virtual void cancelSelectedOperationalLimitsGroup() = 0;

    /**
     * Deselect the OperationalLimitsGroup corresponding to the given IDs.
     * If any ID is empty or has no corresponding group, it is ignored.
     */
    virtual void deselectOperationalLimitsGroups(const std::list<std::string>& ids) = 0;

    /**
     * Get the OperationalLimitsGroup selected or create a new one if it does not exist and set it as the only selected one.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup() = 0;
    /**
     * Get the OperationalLimitsGroup corresponding to the given id or create a new one if it does not exist and set it as the only selected one.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup(const std::string& id);

    virtual stdcxx::CReference<CurrentLimits> getCurrentLimits() const;
    virtual stdcxx::Reference<CurrentLimits> getCurrentLimits();
    virtual stdcxx::const_range<CurrentLimits> getAllSelectedCurrentLimits() const;
    virtual stdcxx::range<CurrentLimits> getAllSelectedCurrentLimits();
    virtual stdcxx::CReference<CurrentLimits> getCurrentLimitsFromId(const std::string & groupId) const;
    virtual stdcxx::Reference<CurrentLimits> getCurrentLimitsFromId(const std::string & groupId);

    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimits() const;
    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimits();
    virtual stdcxx::const_range<ActivePowerLimits> getAllSelectedActivePowerLimits() const;
    virtual stdcxx::range<ActivePowerLimits> getAllSelectedActivePowerLimits();
    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimitsFromId(const std::string & groupId) const;
    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimitsFromId(const std::string & groupId);

    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits() const;
    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits();
    virtual stdcxx::const_range<ApparentPowerLimits> getAllSelectedApparentPowerLimits() const;
    virtual stdcxx::range<ApparentPowerLimits> getAllSelectedApparentPowerLimits();
    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimitsFromId(const std::string & groupId) const;
    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimitsFromId(const std::string & groupId);

    virtual stdcxx::const_range<LoadingLimits> getAllSelectedLoadingLimits(const LimitType& type) const;
    virtual stdcxx::range<LoadingLimits> getAllSelectedLoadingLimits(const LimitType& type);

    /** \deprecated
     *  Use OperationalLimitsGroup::newActivePowerLimits() instead.
     */
    virtual ActivePowerLimitsAdder newActivePowerLimits() = 0;
    /** \deprecated
     *  Use OperationalLimitsGroup::newActivePowerLimits() instead.
     */
    ActivePowerLimitsAdder newActivePowerLimits(const ActivePowerLimits& activePowerLimits);

    /** \deprecated
     *  Use OperationalLimitsGroup::newApparentPowerLimits() instead.
     */
    virtual ApparentPowerLimitsAdder newApparentPowerLimits() = 0;
    /** \deprecated
     *  Use OperationalLimitsGroup::newApparentPowerLimits() instead.
     */
    ApparentPowerLimitsAdder newApparentPowerLimits(const ApparentPowerLimits& apparentPowerLimits);

    /** \deprecated
     *  Use OperationalLimitsGroup::newCurrentLimits() instead.
     */
    virtual CurrentLimitsAdder newCurrentLimits() = 0;
    /** \deprecated
     *  Use OperationalLimitsGroup::newCurrentLimits() instead.
     */
    CurrentLimitsAdder newCurrentLimits(const CurrentLimits& currentLimits);

// protected:
//     void setIdentifiable(Identifiable& identifiable);

// private:
//     OperationalLimitsHolder m_operationalLimitsHolder;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_FLOWSLIMITSHOLDER_HPP

