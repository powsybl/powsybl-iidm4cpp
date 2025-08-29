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

class TieLine : public Identifiable {
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

private: // Identifiable
    const std::string& getTypeDescription() const;

private:
    TieLine(Network& network, const std::string& id, const std::string& name, bool fictitious);
    void attachDanglingLines(DanglingLine& dl1, DanglingLine& dl2);
    DanglingLine& attach(DanglingLine& dl);

    friend class TieLineAdder;

private:
    NetworkRef m_network;

    stdcxx::Reference<DanglingLine> m_danglingLine1;

    stdcxx::Reference<DanglingLine> m_danglingLine2;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TIELINE_HPP
