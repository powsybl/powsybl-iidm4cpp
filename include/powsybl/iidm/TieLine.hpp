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
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/NetworkRef.hpp>

#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class TieLine : public Branch {
public: // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

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

    const DanglingLine& getDanglingLine1() const;
    DanglingLine& getDanglingLine1();

    const DanglingLine& getDanglingLine2() const;
    DanglingLine& getDanglingLine2();

    const DanglingLine& getDanglingLine(const Branch::Side& branchSide) const;
    DanglingLine& getDanglingLine(const Branch::Side& branchSide);
    
    const DanglingLine& getDanglingLine(const std::string& voltageLevelId) const;
    DanglingLine& getDanglingLine(const std::string& voltageLevelId);

    std::string getUcteXnodeCode() const;

    void remove();
    void remove(bool updateDanglingLines);

//Branch:
    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimits1() const override;

    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimits1() override;

    virtual stdcxx::CReference<ActivePowerLimits> getActivePowerLimits2() const override;

    virtual stdcxx::Reference<ActivePowerLimits> getActivePowerLimits2() override;

    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits1() const override;

    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits1() override;

    virtual stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits2() const override;

    virtual stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits2() override;

    virtual stdcxx::CReference<CurrentLimits> getCurrentLimits1() const override;

    virtual stdcxx::Reference<CurrentLimits> getCurrentLimits1() override;

    virtual stdcxx::CReference<CurrentLimits> getCurrentLimits2() const override;

    virtual stdcxx::Reference<CurrentLimits> getCurrentLimits2() override;

    virtual const Terminal& getTerminal1() const override;

    virtual Terminal& getTerminal1() override;

    virtual const Terminal& getTerminal2() const override;

    virtual Terminal& getTerminal2() override;

    virtual ActivePowerLimitsAdder newActivePowerLimits1() override;

    virtual ActivePowerLimitsAdder newActivePowerLimits2() override;

    virtual ApparentPowerLimitsAdder newApparentPowerLimits1() override;

    virtual ApparentPowerLimitsAdder newApparentPowerLimits2() override;

    virtual CurrentLimitsAdder newCurrentLimits1() override;

    virtual CurrentLimitsAdder newCurrentLimits2() override;

private: // Identifiable
    const std::string& getTypeDescription() const;

private:
    TieLine(Network& network, const std::string& id, const std::string& name, bool fictitious);
    void attachDanglingLines(DanglingLine& dl1, DanglingLine& dl2);
    DanglingLine& attach(DanglingLine& dl);

    friend class TieLineAdder;

    void updateDanglingLine(DanglingLine& danglingLine);

private:
    NetworkRef m_network;

    stdcxx::Reference<DanglingLine> m_danglingLine1;

    stdcxx::Reference<DanglingLine> m_danglingLine2;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TIELINE_HPP
