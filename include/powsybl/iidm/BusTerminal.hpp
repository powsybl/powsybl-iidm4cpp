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

namespace powsybl {

namespace iidm {

class BusTerminal : public Terminal {
public: // Terminal
    double getAngle() const override;

    double getV() const override;

    bool isConnected() const override;

public:
    BusTerminal(MultiStateObject& network, const std::string& connectableBusId, bool connected);

    ~BusTerminal() noexcept override = default;

    const std::string& getConnectableBusId() const;

    BusTerminal& setConnectableBusId(const std::string& connectableBusId);

    BusTerminal& setConnected(bool connected);

protected: // Stateful
    void allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteStateArrayElement(unsigned long index) override;

    void extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceStateArraySize(unsigned long number) override;

private:
    BusTerminal(const BusTerminal& nodeTerminal) = delete;

    BusTerminal(BusTerminal&& nodeTerminal) = delete;

    BusTerminal& operator=(const BusTerminal& nodeTerminal) = delete;

    BusTerminal& operator=(BusTerminal&& nodeTerminal) = delete;

private:
    std::vector<bool> m_connected;

    std::vector<std::string> m_connectableBusId;
};

std::ostream& operator<<(std::ostream& stream, const BusTerminal& busTerminal);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSTERMINAL_HPP
