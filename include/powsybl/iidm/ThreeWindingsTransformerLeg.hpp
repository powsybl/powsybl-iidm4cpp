/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_THREEWINDINGSTRANSFORMERLEG_HPP
#define POWSYBL_IIDM_THREEWINDINGSTRANSFORMERLEG_HPP

#include <powsybl/iidm/FlowsLimitsHolder.hpp>
#include <powsybl/iidm/LoadingLimits.hpp>
#include <powsybl/iidm/OperationalLimitsGroups.hpp>
#include <powsybl/iidm/PhaseTapChangerHolder.hpp>
#include <powsybl/iidm/RatioTapChangerHolder.hpp>
#include <powsybl/iidm/ThreeSides.hpp>

namespace powsybl {

namespace iidm {

class Terminal;
class ThreeWindingsTransformer;
class ThreeWindingsTransformerAdder;

namespace three_windings_transformer {

class Leg : public virtual RatioTapChangerHolder, public virtual PhaseTapChangerHolder, public FlowsLimitsHolder {
public:  // Validable
    std::string getMessageHeader() const override;

public:  // TapChangerHolder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

    const Network& getParentNetwork() const override;

    Network& getParentNetwork() override;

public:  // RatioTapChangerHolder
    const RatioTapChanger& getRatioTapChanger() const override;

    RatioTapChanger& getRatioTapChanger() override;

    bool hasRatioTapChanger() const override;

    RatioTapChangerAdder newRatioTapChanger() override;

public:  // PhaseTapChanger
    const PhaseTapChanger& getPhaseTapChanger() const override;

    PhaseTapChanger& getPhaseTapChanger() override;

    bool hasPhaseTapChanger() const override;

    PhaseTapChangerAdder newPhaseTapChanger() override;

public:
    Leg(const ThreeSides& side, double r, double x, double g, double b, double ratedU, double ratedS);

    Leg(const Leg&) = delete;

    // NOLINTNEXTLINE(performance-noexcept-move-constructor): move constructor of FlowsLimitsHolder is not marked noexcept
    Leg(Leg&&) = default;  // NOSONAR

    Leg& operator=(const Leg&) = delete;

    Leg& operator=(Leg&&) = default;

    ~Leg() noexcept override = default;

    double getB() const;

    double getG() const;

    double getR() const;

    double getRatedS() const;

    double getRatedU() const;

    const Terminal& getTerminal() const;

    Terminal& getTerminal();

    ThreeSides getSide() const;

    stdcxx::CReference<LoadingLimits> getLimits(const LimitType& type) const;

    stdcxx::Reference<LoadingLimits> getLimits(const LimitType& type);

    double getX() const;

    Leg& setB(double b);

    Leg& setG(double g);

    Leg& setR(double r);

    Leg& setRatedS(double ratedS);

    Leg& setRatedU(double ratedU);

    Leg& setX(double x);

    std::string toString() const;

// FlowsLimitsHolder
    stdcxx::const_range<OperationalLimitsGroup> getOperationalLimitsGroups() const override;
    stdcxx::range<OperationalLimitsGroup> getOperationalLimitsGroups() override;
    const stdcxx::optional<std::string>& getSelectedOperationalLimitsGroupId() const override;
    stdcxx::CReference<OperationalLimitsGroup> getOperationalLimitsGroup(const std::string& id) const override;
    stdcxx::Reference<OperationalLimitsGroup> getOperationalLimitsGroup(const std::string& id) override;
    stdcxx::CReference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup() const override;
    stdcxx::Reference<OperationalLimitsGroup> getSelectedOperationalLimitsGroup() override;
    stdcxx::Reference<OperationalLimitsGroup> newOperationalLimitsGroup(const std::string& id) override;
    void setSelectedOperationalLimitsGroup(const std::string& id) override;
    void removeOperationalLimitsGroup(const std::string& id) override;
    void cancelSelectedOperationalLimitsGroup() override;

    ActivePowerLimitsAdder newActivePowerLimits() override;
    ApparentPowerLimitsAdder newApparentPowerLimits() override;
    CurrentLimitsAdder newCurrentLimits() override;


private:  // TapChangerHolder
    unsigned long getRegulatingTapChangerCount() const override;

private:  // RatioTapChangerHolder
    void setRatioTapChanger(std::unique_ptr<RatioTapChanger>&& ratioTapChanger) override;

private:  // PhaseTapChangerHolder
    void setPhaseTapChanger(std::unique_ptr<PhaseTapChanger>&& phaseTapChanger) override;

private:
    const std::string& getTypeDescription() const;

    Leg& setTransformer(ThreeWindingsTransformer& transformer);

    friend class iidm::ThreeWindingsTransformer;

private:
    stdcxx::Reference<ThreeWindingsTransformer> m_transformer;

    ThreeSides m_side;

    double m_r;

    double m_x;

    double m_g;

    double m_b;

    double m_ratedU;

    double m_ratedS;

    std::unique_ptr<PhaseTapChanger> m_phaseTapChanger;

    std::unique_ptr<RatioTapChanger> m_ratioTapChanger;

    OperationalLimitsGroups m_operationalLimitsGroups;
};

}  // namespace three_windings_transformer

std::ostream& operator<<(std::ostream& stream, const three_windings_transformer::Leg& leg);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_THREEWINDINGSTRANSFORMERLEG_HPP
