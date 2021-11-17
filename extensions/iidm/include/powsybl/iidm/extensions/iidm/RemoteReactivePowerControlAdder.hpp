/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_REMOTEREACTIVEPOWERCONTROLADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_REMOTEREACTIVEPOWERCONTROLADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Terminal;

namespace extensions {

namespace iidm {

class RemoteReactivePowerControlAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit RemoteReactivePowerControlAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~RemoteReactivePowerControlAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    RemoteReactivePowerControlAdder(const RemoteReactivePowerControlAdder&) = default;

    /**
     * Move constructor
     */
    RemoteReactivePowerControlAdder(RemoteReactivePowerControlAdder&&) = default;

    /**
     * Copy assignment operator
     */
    RemoteReactivePowerControlAdder& operator=(const RemoteReactivePowerControlAdder&) = delete;

    /**
     * Move assignment operator
     */
    RemoteReactivePowerControlAdder& operator=(RemoteReactivePowerControlAdder&&) = delete;

    RemoteReactivePowerControlAdder& withEnabled(bool enabled);

    RemoteReactivePowerControlAdder& withRegulatingTerminal(Terminal& regulatingTerminal);

    RemoteReactivePowerControlAdder& withTargetQ(double targetQ);

protected:
    /**
     * Creates the RemoteReactivePowerControl extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    double m_targetQ = 0.0;

    stdcxx::Reference<Terminal> m_regulatingTerminal;

    bool m_enabled = false;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_REMOTEREACTIVEPOWERCONTROLADDER_HPP
