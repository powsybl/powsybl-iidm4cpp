/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCEPRIORITY_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCEPRIORITY_HPP

#include <powsybl/stdcxx/reference.hpp>

#include <powsybl/iidm/ThreeSides.hpp>
#include <powsybl/iidm/TwoSides.hpp>

namespace powsybl {

namespace iidm {

class Branch;
class Injection;
class Terminal;
class ThreeWindingsTransformer;

namespace extensions {

namespace iidm {

/**
 * ReferencePriority iIDM extension allow to specify priority for a Terminal.
 * A priority 0 means should not be used.
 * 1 is highest priority for selection.
 * 2 is second highest priority, etc ...
 */
class ReferencePriority {
public:
    ReferencePriority(Terminal& terminal, unsigned long priority);
    ReferencePriority() = delete;
    ReferencePriority(const ReferencePriority& coord) = default;
    ReferencePriority(ReferencePriority&&) noexcept = default;
    ~ReferencePriority() noexcept = default;
    ReferencePriority& operator=(const ReferencePriority&) = default;
    ReferencePriority& operator=(ReferencePriority&&) noexcept = default;

    const Terminal& getTerminal() const;
    Terminal& getTerminal();

    unsigned long getPriority() const;

    static int get(const Injection& injection);

    static int get(const Branch& branch, TwoSides side);

    static int get(const ThreeWindingsTransformer& threeWindingsTransformer, ThreeSides side);

    static void set(Injection& injection, unsigned long priority);

    static void set(Branch& branch, TwoSides side, unsigned long priority);

    static void set(ThreeWindingsTransformer& threeWindingsTransformer, ThreeSides side, unsigned long priority);


private:
    stdcxx::Reference<Terminal> m_terminal;

    unsigned long m_priority;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_REFERENCEPRIORITY_HPP
