/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SLACKTERMINALADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_SLACKTERMINALADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Terminal;

namespace extensions {

class SlackTerminalAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit SlackTerminalAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    SlackTerminalAdder(const SlackTerminalAdder&) = default;

    /**
     * Move constructor
     */
    SlackTerminalAdder(SlackTerminalAdder&&) = default;

    /**
     * Destructor
     */
    ~SlackTerminalAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    SlackTerminalAdder& operator=(const SlackTerminalAdder&) = delete;

    /**
     * Move assignment operator
     */
    SlackTerminalAdder& operator=(SlackTerminalAdder&&) = delete;

    /**
     * Set terminal
     *
     * @param terminal the terminal
     *
     * @return this SlackTerminalAdder object
     */
    SlackTerminalAdder& withTerminal(const Terminal& terminal);

    /**
     * Set terminal
     *
     * @param terminal the terminal
     *
     * @return this SlackTerminalAdder object
     */
    SlackTerminalAdder& withTerminal(Terminal& terminal);

protected:
    /**
     * Creates the SlackTerminal extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) override;

private:
    stdcxx::Reference<Terminal> m_terminal;
};

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SLACKTERMINALADDER_HPP
