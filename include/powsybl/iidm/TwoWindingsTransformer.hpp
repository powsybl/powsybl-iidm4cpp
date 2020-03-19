/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TWOWINDINGSTRANSFORMER_HPP
#define POWSYBL_IIDM_TWOWINDINGSTRANSFORMER_HPP

#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/PhaseTapChangerHolder.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/RatioTapChangerHolder.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

class Substation;

class IIDM_DECLSPEC TwoWindingsTransformer : public virtual Branch, public virtual RatioTapChangerHolder, public virtual PhaseTapChangerHolder {
public:  // TapChangerHolder
    bool hasPhaseTapChanger() const override;

    bool hasRatioTapChanger() const override;

    const Network& getNetwork() const override;

    Network& getNetwork() override;

public:  // RatioTapChangerHolder
    stdcxx::CReference<RatioTapChanger> getRatioTapChanger() const override;

    stdcxx::Reference<RatioTapChanger> getRatioTapChanger() override;

    RatioTapChangerAdder newRatioTapChanger() override;

public:  // PhaseTapChangerHolder
    stdcxx::CReference<PhaseTapChanger> getPhaseTapChanger() const override;

    stdcxx::Reference<PhaseTapChanger> getPhaseTapChanger() override;

    PhaseTapChangerAdder newPhaseTapChanger() override;

public:
    TwoWindingsTransformer(const std::string& id, const std::string& name, Substation& substation, double r, double x, double g, double b, double ratedU1, double ratedU2);

    ~TwoWindingsTransformer() noexcept override = default;

    double getB() const;

    double getG() const;

    double getR() const;

    double getRatedU1() const;

    double getRatedU2() const;

    stdcxx::CReference<Substation> getSubstation() const;

    stdcxx::Reference<Substation> getSubstation();

    double getX() const;

    TwoWindingsTransformer& setB(double b);

    TwoWindingsTransformer& setG(double g);

    TwoWindingsTransformer& setR(double r);

    TwoWindingsTransformer& setRatedU1(double ratedU1);

    TwoWindingsTransformer& setRatedU2(double ratedU2);

    TwoWindingsTransformer& setX(double x);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private:  // Identifiable
    const std::string& getTypeDescription() const override;

private:
    unsigned long getRegulatingTapChangerCount() const override;

private:  // RatioTapChangerHolder
    void setRatioTapChanger(std::unique_ptr<RatioTapChanger> ratioTapChanger) override;

private:  // PhaseTapChangerHolder
    void setPhaseTapChanger(std::unique_ptr<PhaseTapChanger> phaseTapChanger) override;

private:
    friend class PhaseTapChanger;

    friend class PhaseTapChangerAdder;

    friend class TwoWindingsTransformerAdder;

private:
    stdcxx::Reference<Substation> m_substation;

    double m_r;

    double m_x;

    double m_g;

    double m_b;

    double m_ratedU1;

    double m_ratedU2;

    std::unique_ptr<RatioTapChanger> m_ratioTapChanger;

    std::unique_ptr<PhaseTapChanger> m_phaseTapChanger;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TWOWINDINGSTRANSFORMER_HPP
