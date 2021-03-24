/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TIELINE_HPP
#define POWSYBL_IIDM_TIELINE_HPP

#include <powsybl/iidm/HalfLine.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class TieLine : public Line {
public:
    using HalfLine = tie_line::HalfLine;

public: // Line
    double getB1() const override;

    double getB2() const override;

    double getG1() const override;

    double getG2() const override;

    double getR() const override;

    double getX() const override;

    bool isTieLine() const override;

    TieLine& setB1(double b1) override;

    TieLine& setB2(double b2) override;

    TieLine& setG1(double g1) override;

    TieLine& setG2(double g2) override;

    TieLine& setR(double r) override;

    TieLine& setX(double x) override;

public:
    TieLine(const std::string& id, const std::string& name, bool fictitious, const std::string& ucteXnodeCode, HalfLine&& half1, HalfLine&& half2);

    ~TieLine() noexcept override = default;

    const HalfLine& getHalf(const Side& side) const;

    HalfLine& getHalf(const Side& side);

    const HalfLine& getHalf1() const;

    HalfLine& getHalf1();

    const HalfLine& getHalf2() const;

    HalfLine& getHalf2();

    const std::string& getUcteXnodeCode() const;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    HalfLine& attach(HalfLine& halfLine, const std::function<const Terminal&()>& terminalSupplier);

private:
    HalfLine m_half1;

    HalfLine m_half2;

    std::string m_ucteXnodeCode;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TIELINE_HPP
