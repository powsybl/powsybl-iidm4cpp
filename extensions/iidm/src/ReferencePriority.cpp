/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ReferencePriority.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Branch.hpp>
#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePriorities.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePrioritiesAdder.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePriorityAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ReferencePriority::ReferencePriority(Terminal& terminal, unsigned long priority) :
    m_terminal(terminal),
    m_priority(priority) {
}

const Terminal& ReferencePriority::getTerminal() const {
    return m_terminal.get();
}
Terminal& ReferencePriority::getTerminal() {
    return m_terminal.get();
}

unsigned long ReferencePriority::getPriority() const {
    return m_priority;
}

int ReferencePriority::get(const Injection& injection) {
    stdcxx::CReference<ReferencePriorities> ext = injection.findExtension<ReferencePriorities>();
    if(!ext || ext.get().getReferencePriorities().empty()) {
        return 0;
    }
    return ext.get().getReferencePriorities().front().getPriority();
}

int ReferencePriority::get(const Branch& branch, TwoSides side) {
    stdcxx::CReference<ReferencePriorities> ext = branch.findExtension<ReferencePriorities>();
    if(!ext || ext.get().getReferencePriorities().empty()) {
        return 0;
    }
    for (const auto& referencePriority : ext.get().getReferencePriorities()) {
        if( referencePriority.getTerminal().getConnectable().get().getId() == branch.getId() && 
            Terminal::getConnectableSide(referencePriority.getTerminal()) == static_cast<ThreeSides>(side)) {
                return referencePriority.getPriority();
        }
    }
    return 0;
}

int ReferencePriority::get(const ThreeWindingsTransformer& threeWindingsTransformer, ThreeSides side) {
    stdcxx::CReference<ReferencePriorities> ext = threeWindingsTransformer.findExtension<ReferencePriorities>();
    if(!ext || ext.get().getReferencePriorities().empty()) {
        return 0;
    }
    for (const auto& referencePriority : ext.get().getReferencePriorities()) {
        if( referencePriority.getTerminal().getConnectable().get().getId() == threeWindingsTransformer.getId() && 
            Terminal::getConnectableSide(referencePriority.getTerminal()) == side) {
                return referencePriority.getPriority();
        }
    }
    return 0;
}

void ReferencePriority::set(Injection& injection, unsigned long priority) {
    stdcxx::Reference<ReferencePriorities> ext = injection.findExtension<ReferencePriorities>();
    if(!ext) {
        injection.newExtension<ReferencePrioritiesAdder>().add();
    }
    ReferencePriorities& extension = injection.getExtension<ReferencePriorities>();
    extension.newReferencePriority()
                .setTerminal(injection.getTerminal())
                .setPriority(priority)
                .add();
}

void ReferencePriority::set(Branch& branch, TwoSides side, unsigned long priority) {
    stdcxx::Reference<ReferencePriorities> ext = branch.findExtension<ReferencePriorities>();
    if(!ext) {
        branch.newExtension<ReferencePrioritiesAdder>().add();
    }
    ReferencePriorities& extension = branch.getExtension<ReferencePriorities>();
    extension.newReferencePriority()
                .setTerminal(branch.getTerminalFromSide(side))
                .setPriority(priority)
                .add();
}

void ReferencePriority::set(ThreeWindingsTransformer& threeWindingsTransformer, ThreeSides side, unsigned long priority) {
    stdcxx::Reference<ReferencePriorities> ext = threeWindingsTransformer.findExtension<ReferencePriorities>();
    if(!ext) {
        threeWindingsTransformer.newExtension<ReferencePrioritiesAdder>().add();
    }
    ReferencePriorities& extension = threeWindingsTransformer.getExtension<ReferencePriorities>();
    extension.newReferencePriority()
                .setTerminal(threeWindingsTransformer.getTerminal(side))
                .setPriority(priority)
                .add();
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl