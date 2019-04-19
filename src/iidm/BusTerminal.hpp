/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSTERMINAL_HPP
#define POWSYBL_IIDM_BUSTERMINAL_HPP

#include <ostream>
#include <string>
#include <vector>

#include <powsybl/iidm/Terminal.hpp>

#include "BusTerminalViews.hpp"

namespace powsybl {

namespace iidm {

class BusTerminal : public Terminal {
public: // Terminal
    double getAngle() const override;

    const terminal::BusBreakerView& getBusBreakerView() const override;

    terminal::BusBreakerView& getBusBreakerView() override;

    const terminal::BusView& getBusView() const override;

    terminal::BusView& getBusView() override;

    const terminal::NodeBreakerView& getNodeBreakerView() const override;

    terminal::NodeBreakerView& getNodeBreakerView() override;

    double getV() const override;

    bool isConnected() const override;

public:
    BusTerminal(VariantManagerHolder& network, const std::string& connectableBusId, bool connected);

    BusTerminal(const BusTerminal& nodeTerminal) = delete;

    BusTerminal(BusTerminal&& nodeTerminal) = delete;

    ~BusTerminal() noexcept override = default;

    BusTerminal& operator=(const BusTerminal& nodeTerminal) = delete;

    BusTerminal& operator=(BusTerminal&& nodeTerminal) = delete;

    const std::string& getConnectableBusId() const;

    BusTerminal& setConnectableBusId(const std::string& connectableBusId);

    BusTerminal& setConnected(bool connected);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private:
    std::vector<bool> m_connected;

    std::vector<std::string> m_connectableBusId;

    bus_terminal::BusBreakerViewImpl m_busBreakerView;

    bus_terminal::BusViewImpl m_busView;
};

std::ostream& operator<<(std::ostream& stream, const BusTerminal& busTerminal);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSTERMINAL_HPP
