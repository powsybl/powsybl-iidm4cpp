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
protected:
    virtual const FlowsLimitsHolder& getOperationalLimitsHolder1() const override;
    virtual FlowsLimitsHolder& getOperationalLimitsHolder1() override;
    virtual const FlowsLimitsHolder& getOperationalLimitsHolder2() const override;
    virtual FlowsLimitsHolder& getOperationalLimitsHolder2() override;

public:
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
