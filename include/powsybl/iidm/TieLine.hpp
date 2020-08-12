/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TIELINE_HPP
#define POWSYBL_IIDM_TIELINE_HPP

#include <powsybl/iidm/Line.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

class TieLine : public Line {
public:
    class HalfLine : public Validable {
    public: // Validable
        std::string getMessageHeader() const override;

    public:
        double getB1() const;

        double getB2() const;

        double getG1() const;

        double getG2() const;

        const std::string& getId() const;

        const std::string& getName() const;

        double getR() const;

        double getX() const;

        double getXnodeP() const;

        double getXnodeQ() const;

        HalfLine& setB1(double b1);

        HalfLine& setB2(double b2);

        HalfLine& setG1(double g1);

        HalfLine& setG2(double g2);

        HalfLine& setR(double r);

        HalfLine& setX(double x);

        HalfLine& setXnodeP(double xnodeP);

        HalfLine& setXnodeQ(double xnodeQ);

    private:
        HalfLine();

        void setId(const std::string& id);

        void setName(const std::string& name);

        void setParent(TieLine& parent);

        friend class TieLine;

        friend class TieLineAdder;

    private:
        stdcxx::Reference<TieLine> m_parent;

        std::string m_id;

        std::string m_name;

        LineCharacteristics m_lineCharacteristics;

        double m_xnodeP = stdcxx::nan();

        double m_xnodeQ = stdcxx::nan();
    };

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
    TieLine(const std::string& id, const std::string& name, bool fictitious, const std::string& ucteXnodeCode, HalfLine& half1, HalfLine& half2);

    ~TieLine() noexcept override = default;

    const HalfLine& getHalf1() const;

    HalfLine& getHalf1();

    const HalfLine& getHalf2() const;

    HalfLine& getHalf2();

    const std::string& getUcteXnodeCode() const;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    HalfLine& attach(HalfLine& halfLine);

private:
    HalfLine m_half1;

    HalfLine m_half2;

    std::string m_ucteXnodeCode;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TIELINE_HPP
