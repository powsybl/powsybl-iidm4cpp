/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BOUNDARYLINEADDER_HPP
#define POWSYBL_IIDM_BOUNDARYLINEADDER_HPP

#include <powsybl/iidm/BoundaryLineGenerationAdder.hpp>
#include <powsybl/iidm/InjectionAdder.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class BoundaryLine;
class Network;
class VoltageLevel;

class BoundaryLineAdder : public InjectionAdder<BoundaryLine, BoundaryLineAdder> {
public:
    using GenerationAdder = boundary_line::GenerationAdder;

public:
    BoundaryLine& add() override;

    GenerationAdder newGeneration();

    BoundaryLineAdder& setB(double b);

    BoundaryLineAdder& setG(double g);

    BoundaryLineAdder& setP0(double p0);

    BoundaryLineAdder& setQ0(double q0);

    BoundaryLineAdder& setR(double r);

    BoundaryLineAdder& setPairingKey(const std::string& pairingKey);

    BoundaryLineAdder& setX(double x);

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

private:
    explicit BoundaryLineAdder(VoltageLevel& voltageLevel);

    void setGenerationAdder(const GenerationAdder& generationAdder);

    friend class VoltageLevel;

    friend class boundary_line::GenerationAdder;

private:
    stdcxx::optional<GenerationAdder> m_generationAdder;

    double m_r = stdcxx::nan();

    double m_x = stdcxx::nan();

    double m_g = 0.0;

    double m_b = 0.0;

    double m_p0 = stdcxx::nan();

    double m_q0 = stdcxx::nan();

    std::string m_pairingKey;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BOUNDARYLINEADDER_HPP
