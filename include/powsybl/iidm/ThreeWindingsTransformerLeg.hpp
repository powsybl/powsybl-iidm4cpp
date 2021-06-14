/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_THREEWINDINGSTRANSFORMERLEG_HPP
#define POWSYBL_IIDM_THREEWINDINGSTRANSFORMERLEG_HPP

#include <powsybl/iidm/FlowsLimitsHolder.hpp>
#include <powsybl/iidm/OperationalLimitsHolder.hpp>
#include <powsybl/iidm/PhaseTapChangerHolder.hpp>
#include <powsybl/iidm/RatioTapChangerHolder.hpp>

namespace powsybl {

namespace iidm {

class ActivePowerLimits;
class ActivePowerLimitsAdder;
class ApparentPowerLimits;
class ApparentPowerLimitsAdder;
class CurrentLimits;
class CurrentLimitsAdder;
class OperationalLimitsHolder;
class Terminal;
class ThreeWindingsTransformer;
class ThreeWindingsTransformerAdder;

namespace three_windings_transformer {

class Leg : public virtual RatioTapChangerHolder, public virtual PhaseTapChangerHolder, public FlowsLimitsHolder {
public:  // Validable
    std::string getMessageHeader() const override;

public:  // FlowsLimitsHolder
    stdcxx::CReference<ActivePowerLimits> getActivePowerLimits() const override;

    stdcxx::Reference<ActivePowerLimits> getActivePowerLimits() override;

    stdcxx::CReference<ApparentPowerLimits> getApparentPowerLimits() const override;

    stdcxx::Reference<ApparentPowerLimits> getApparentPowerLimits() override;

    stdcxx::CReference<CurrentLimits> getCurrentLimits() const override;

    stdcxx::Reference<CurrentLimits> getCurrentLimits() override;

    stdcxx::const_range<OperationalLimits> getOperationalLimits() const override;

    stdcxx::range<OperationalLimits> getOperationalLimits() override;

    ActivePowerLimitsAdder newActivePowerLimits() override;

    ApparentPowerLimitsAdder newApparentPowerLimits() override;

    CurrentLimitsAdder newCurrentLimits() override;

public:  // TapChangerHolder
    const Network& getNetwork() const override;

    Network& getNetwork() override;

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
    Leg(unsigned long legNumber, double r, double x, double g, double b, double ratedU, double ratedS);

    Leg(const Leg&) = delete;

    Leg(Leg&&) noexcept = default;

    Leg& operator=(const Leg&) = delete;

    Leg& operator=(Leg&&) noexcept = default;

    ~Leg() noexcept override = default;

    double getB() const;

    double getG() const;

    double getR() const;

    double getRatedS() const;

    double getRatedU() const;

    const Terminal& getTerminal() const;

    Terminal& getTerminal();

    double getX() const;

    Leg& setB(double b);

    Leg& setG(double g);

    Leg& setR(double r);

    Leg& setRatedS(double ratedS);

    Leg& setRatedU(double ratedU);

    Leg& setX(double x);

    std::string toString() const;

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

    OperationalLimitsHolder m_operationalLimitsHolder;

    unsigned long m_legNumber;

    double m_r;

    double m_x;

    double m_g;

    double m_b;

    double m_ratedU;

    double m_ratedS;

    std::unique_ptr<PhaseTapChanger> m_phaseTapChanger;

    std::unique_ptr<RatioTapChanger> m_ratioTapChanger;
};

}  // namespace three_windings_transformer

std::ostream& operator<<(std::ostream& stream, const three_windings_transformer::Leg& leg);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_THREEWINDINGSTRANSFORMERLEG_HPP
