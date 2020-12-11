/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_THREEWINDINGSTRANSFORMERLEG_HPP
#define POWSYBL_IIDM_THREEWINDINGSTRANSFORMERLEG_HPP

#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/PhaseTapChangerHolder.hpp>
#include <powsybl/iidm/RatioTapChangerHolder.hpp>

namespace powsybl {

namespace iidm {

class Terminal;
class ThreeWindingsTransformer;
class ThreeWindingsTransformerAdder;

namespace three_windings_transformer {

class Leg : public virtual RatioTapChangerHolder, public virtual PhaseTapChangerHolder {
public:  // Validable
    std::string getMessageHeader() const override;

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

    stdcxx::CReference<CurrentLimits> getCurrentLimits() const;

    stdcxx::Reference<CurrentLimits> getCurrentLimits();

    double getG() const;

    double getR() const;

    double getRatedS() const;

    double getRatedU() const;

    const Terminal& getTerminal() const;

    Terminal& getTerminal();

    double getX() const;

    CurrentLimitsAdder<const std::nullptr_t, Leg> newCurrentLimits();

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
    void setRatioTapChanger(std::unique_ptr<RatioTapChanger> ratioTapChanger) override;

private:  // PhaseTapChangerHolder
    void setPhaseTapChanger(std::unique_ptr<PhaseTapChanger> phaseTapChanger) override;

private:
    const std::string& getTypeDescription() const;

    void setCurrentLimits(std::nullptr_t side, std::unique_ptr<CurrentLimits> limits);

    void setTransformer(ThreeWindingsTransformer& transformer);

private:
    friend class CurrentLimitsAdder<const std::nullptr_t, Leg>;

    friend class iidm::ThreeWindingsTransformerAdder;

private:
    stdcxx::Reference<ThreeWindingsTransformer> m_transformer;

    unsigned long m_legNumber;

    double m_r;

    double m_x;

    double m_g;

    double m_b;

    double m_ratedU;

    double m_ratedS;

    std::unique_ptr<CurrentLimits> m_limits;

    std::unique_ptr<PhaseTapChanger> m_phaseTapChanger;

    std::unique_ptr<RatioTapChanger> m_ratioTapChanger;
};

}  // namespace three_windings_transformer

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_THREEWINDINGSTRANSFORMERLEG_HPP
