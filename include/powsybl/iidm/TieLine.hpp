/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TIELINE_HPP
#define POWSYBL_IIDM_TIELINE_HPP

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/NetworkRef.hpp>

#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/Predicate.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class TieLine : public Branch {
public: // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

    const Network& getParentNetwork() const override;

    Network& getParentNetwork() override;

    const IdentifiableType& getType() const;

    // LineCharacteristics
    double getB1() const;

    double getB2() const;

    double getG1() const;

    double getG2() const;

    double getR() const;

    double getX() const;

public:
    ~TieLine() noexcept override = default;

    const BoundaryLine& getBoundaryLine1() const;
    BoundaryLine& getBoundaryLine1();

    const BoundaryLine& getBoundaryLine2() const;
    BoundaryLine& getBoundaryLine2();

    const BoundaryLine& getBoundaryLine(const TwoSides& side) const;
    BoundaryLine& getBoundaryLine(const TwoSides& side);
    
    const BoundaryLine& getBoundaryLine(const std::string& voltageLevelId) const;
    BoundaryLine& getBoundaryLine(const std::string& voltageLevelId);

    std::string getPairingKey() const;

    void remove();
    void remove(bool updateBoundaryLines);

//Branch:
    virtual stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups1() const override;
    virtual stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups1()override;
    virtual const stdcxx::optional<std::string>& getSelectedOperationalLimitsGroupId1() const override;
    virtual stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup1(const std::string& id) const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup1(const std::string& id) override;
    virtual stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup1() const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup1() override;
    virtual OperationalLimitsGroup& newOperationalLimitsGroup1(const std::string& id) override;
    virtual void setSelectedOperationalLimitsGroup1(const std::string& id) override;
    virtual void removeOperationalLimitsGroup1(const std::string& id) override;
    virtual void cancelSelectedOperationalLimitsGroup1() override;
    /**
     * Get the OperationalLimitsGroup selected on side 1 or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup1() override;
    /**
     * Get the OperationalLimitsGroup corresponding to the given id from side 1 or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup1(const std::string& id) override;

    virtual stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups2() const override;
    virtual stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups2()override;
    virtual const stdcxx::optional<std::string>& getSelectedOperationalLimitsGroupId2() const override;
    virtual stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup2(const std::string& id) const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup2(const std::string& id) override;
    virtual stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup2() const override;
    virtual stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup2() override;
    virtual OperationalLimitsGroup& newOperationalLimitsGroup2(const std::string& id) override;
    virtual void setSelectedOperationalLimitsGroup2(const std::string& id) override;
    virtual void removeOperationalLimitsGroup2(const std::string& id) override;
    virtual void cancelSelectedOperationalLimitsGroup2() override;
    /**
     * Get the OperationalLimitsGroup selected on side 2 or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup2() override;
    /**
     * Get the OperationalLimitsGroup corresponding to the given id from side 2 or create a new one if it does not exist and set it as selected.
     */
    virtual OperationalLimitsGroup& getOrCreateSelectedOperationalLimitsGroup2(const std::string& id) override;

    virtual const Terminal& getTerminal1() const override;

    virtual Terminal& getTerminal1() override;

    virtual const Terminal& getTerminal2() const override;

    virtual Terminal& getTerminal2() override;

    /** \deprecated
     * Use OperationalLimitsGroup::newActivePowerLimits() instead.
     */
    virtual ActivePowerLimitsAdder newActivePowerLimits1() override;
    /** \deprecated
     * Use OperationalLimitsGroup::newActivePowerLimits() instead.
     */
    virtual ActivePowerLimitsAdder newActivePowerLimits2() override;
    /** \deprecated
     * Use OperationalLimitsGroup::newApparentPowerLimits() instead.
     */
    virtual ApparentPowerLimitsAdder newApparentPowerLimits1() override;
    /** \deprecated
     * Use OperationalLimitsGroup::newApparentPowerLimits() instead.
     */
    virtual ApparentPowerLimitsAdder newApparentPowerLimits2() override;
    /** \deprecated
     * Use OperationalLimitsGroup::newCurrentLimits() instead.
     */
    virtual CurrentLimitsAdder newCurrentLimits1() override;
    /** \deprecated
     * Use OperationalLimitsGroup::newCurrentLimits() instead.
     */
    virtual CurrentLimitsAdder newCurrentLimits2() override;

    bool connectBoundaryLines();
    bool connectBoundaryLines(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate);
    bool connectBoundaryLines(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate, const stdcxx::optional<TwoSides>& side);

    bool disconnectBoundaryLines();
    bool disconnectBoundaryLines(const stdcxx::Predicate<Switch>& isSwitchOpenable);
    bool disconnectBoundaryLines(const stdcxx::Predicate<Switch>& isSwitchOpenable, const stdcxx::optional<TwoSides>& side);

private: // Identifiable
    const std::string& getTypeDescription() const;

private:
    TieLine(Network& network, const std::string& id, const std::string& name, bool fictitious);
    void attachBoundaryLines(BoundaryLine& bl1, BoundaryLine& bl2);
    BoundaryLine& attach(BoundaryLine& bl);

    friend class TieLineAdder;

    void updateBoundaryLine(BoundaryLine& boundaryLine);

    std::vector<std::reference_wrapper<Terminal>> getTerminalsOfBoundaryLines(const stdcxx::optional<TwoSides>& side);

private:
    NetworkRef m_network;

    stdcxx::Reference<BoundaryLine> m_boundaryLine1;

    stdcxx::Reference<BoundaryLine> m_boundaryLine2;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TIELINE_HPP
