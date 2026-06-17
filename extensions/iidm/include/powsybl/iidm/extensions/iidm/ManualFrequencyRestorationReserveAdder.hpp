/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_MANUALFREQUENCYRESTORATIONRESERVEADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_MANUALFREQUENCYRESTORATIONRESERVEADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ManualFrequencyRestorationReserveAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit ManualFrequencyRestorationReserveAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    ManualFrequencyRestorationReserveAdder(const ManualFrequencyRestorationReserveAdder&) = default;

    /**
     * Move constructor
     */
    ManualFrequencyRestorationReserveAdder(ManualFrequencyRestorationReserveAdder&&) noexcept = default;

    /**
     * Destructor
     */
    ~ManualFrequencyRestorationReserveAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    ManualFrequencyRestorationReserveAdder& operator=(const ManualFrequencyRestorationReserveAdder&) = delete;
    /**
     * Move assignment operator
     */
    ManualFrequencyRestorationReserveAdder& operator=(ManualFrequencyRestorationReserveAdder&&) = delete;

    ManualFrequencyRestorationReserveAdder& withParticipate(bool participate);

private:  // ExtensionAdder
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    bool m_participate = false;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_MANUALFREQUENCYRESTORATIONRESERVEADDER_HPP
