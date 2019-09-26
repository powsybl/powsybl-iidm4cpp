/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TAPCHANGERSTEP_HPP
#define POWSYBL_IIDM_TAPCHANGERSTEP_HPP

#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class TapChangerHolder;

template<typename S>
class TapChangerStep {
public:
    virtual ~TapChangerStep() noexcept = default;

    double getB() const;

    double getG() const;

    double getR() const;

    double getRho() const;

    double getX() const;

    S& setB(double b);

    S& setG(double g);

    S& setR(double r);

    S& setRho(double rho);

    S& setX(double x);

protected:
    TapChangerStep(unsigned long position, double rho, double r, double x, double g, double b);

    void setParent(const TapChangerHolder& parent);

private:
    template<typename, typename, typename> friend class TapChanger;

private:
    stdcxx::CReference<TapChangerHolder> m_stepHolder;

    unsigned long m_position;

    double m_rho;

    double m_r;

    double m_x;

    double m_g;

    double m_b;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/TapChangerStep.hxx>

#endif  // POWSYBL_IIDM_TAPCHANGERSTEP_HPP
