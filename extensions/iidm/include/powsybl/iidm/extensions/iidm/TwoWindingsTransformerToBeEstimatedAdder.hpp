/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERTOBEESTIMATEDADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERTOBEESTIMATEDADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class TwoWindingsTransformerToBeEstimatedAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit TwoWindingsTransformerToBeEstimatedAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    TwoWindingsTransformerToBeEstimatedAdder(const TwoWindingsTransformerToBeEstimatedAdder&) = default;

    /**
     * Move constructor
     */
    TwoWindingsTransformerToBeEstimatedAdder(TwoWindingsTransformerToBeEstimatedAdder&&) = default;

    /**
     * Destructor
     */
    ~TwoWindingsTransformerToBeEstimatedAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    TwoWindingsTransformerToBeEstimatedAdder& operator=(const TwoWindingsTransformerToBeEstimatedAdder&) = delete;

    /**
     * Move assignment operator
     */
    TwoWindingsTransformerToBeEstimatedAdder& operator=(TwoWindingsTransformerToBeEstimatedAdder&&) = delete;

    TwoWindingsTransformerToBeEstimatedAdder& withPhaseTapChangerStatus(bool toBeEstimated);

    TwoWindingsTransformerToBeEstimatedAdder& withRatioTapChangerStatus(bool toBeEstimated);

protected:
    /**
     * Creates the TwoWindingsTransformerToBeEstimated extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    bool m_ptcStatus = false;

    bool m_rtcStatus = false;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERTOBEESTIMATEDADDER_HPP
