/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_THREEWINDINGSTRANSFORMERLEGADDER_HPP
#define POWSYBL_IIDM_THREEWINDINGSTRANSFORMERLEGADDER_HPP

#include <memory>
#include <string>

#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/Validable.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class Terminal;
class ThreeWindingsTransformerAdder;
class VoltageLevel;

namespace three_windings_transformer {

class LegAdder : public Validable {
public:
    ~LegAdder() noexcept override = default;

    ThreeWindingsTransformerAdder& add();

    LegAdder& setB(double b);

    LegAdder& setBus(const std::string& bus);

    LegAdder& setConnectableBus(const std::string& connectableBus);

    LegAdder& setG(double g);

    LegAdder& setNode(unsigned long node);

    LegAdder& setR(double r);

    LegAdder& setRatedS(double ratedS);

    LegAdder& setRatedU(double ratedU);

    LegAdder& setVoltageLevel(const std::string& voltageLevelId);

    LegAdder& setX(double x);

private:  // Validable
    std::string getMessageHeader() const override;

private:
    LegAdder(ThreeWindingsTransformerAdder& parent, unsigned long legNumber);

    ThreeWindingsTransformer::Leg build() const;

    std::unique_ptr<Terminal> checkAndGetTerminal(VoltageLevel& voltageLevel);

    VoltageLevel& checkAndGetVoltageLevel();

    friend class iidm::ThreeWindingsTransformerAdder;

private:
    ThreeWindingsTransformerAdder& m_parent;

    unsigned long m_legNumber;

    std::string m_voltageLevelId;

    stdcxx::optional<unsigned long> m_node;

    std::string m_bus;

    std::string m_connectableBus;

    double m_r = stdcxx::nan();

    double m_x = stdcxx::nan();

    double m_g = stdcxx::nan();

    double m_b = stdcxx::nan();

    double m_ratedU = stdcxx::nan();

    double m_ratedS = stdcxx::nan();
};

}  // namespace three_windings_transformer

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_THREEWINDINGSTRANSFORMERLEGADDER_HPP
