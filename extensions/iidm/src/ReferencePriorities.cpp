/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ReferencePriorities.hpp>

// #include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/indirected.hpp>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/extensions/iidm/ReferencePriorityAdder.hpp>
#include <powsybl/stdcxx/map.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

std::map<IdentifiableType, unsigned long> ReferencePriorities::DEFAULT_CONNECTABLE_TYPE_PRIORITIES = {
    {IdentifiableType::GENERATOR, 1},
    {IdentifiableType::BUSBAR_SECTION, 2},
    {IdentifiableType::LOAD, 3}
};

ReferencePriorities::ReferencePriorities(Connectable& connectable) :
    AbstractMultiVariantIdentifiableExtension(connectable) {
        m_referencePriorities.resize(getVariantManagerHolder().getVariantManager().getVariantArraySize(), std::vector<std::shared_ptr<ReferencePriority>>());
}

const std::string& ReferencePriorities::getName() const {
    static std::string s_name = "referencePriorities";
    return s_name;
}

const std::type_index& ReferencePriorities::getType() const {
    static std::type_index s_type = typeid(ReferencePriorities);
    return s_type;
}

void ReferencePriorities::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        // unregister (if needed) terminals before overwriting them 
        unregisterReferencedTerminalIfNeeded(index);
        m_referencePriorities[index] = m_referencePriorities[sourceIndex];
        //no need to register them additionaly, since they already are referenced in sourceIndex Variant
    }
}
void ReferencePriorities::deleteVariantArrayElement(unsigned long index) {
    unregisterReferencedTerminalIfNeeded(index);
    m_referencePriorities[index].clear();
}
void ReferencePriorities::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    //No need to register terminals, since they already have been referenced when added in sourceIndex Variant
    m_referencePriorities.resize(m_referencePriorities.size() + number, m_referencePriorities[sourceIndex]);
}
void ReferencePriorities::reduceVariantArraySize(unsigned long number) {
    //reduce variant size one by one to ensure references checked before each variant is removed
    for(unsigned long index = 0; index < number ; index ++) {
        unregisterReferencedTerminalIfNeeded(m_referencePriorities.size() - 1);
        m_referencePriorities.resize(m_referencePriorities.size() - 1);
    }
}


ReferencePriorities& ReferencePriorities::add(const std::shared_ptr<ReferencePriority>& referencePriority) {
    if(!referencePriority) {
        return *this;
    }

    std::vector<std::reference_wrapper<Terminal>> connectableTerminals = getExtendable<Connectable>().get().getTerminals();
    bool containsTerminal = false;
    for(auto& terminal : connectableTerminals) {
        if(stdcxx::areSame(terminal.get(), referencePriority->getTerminal())) {
            containsTerminal = true;
            break;
        }
    }
    if (!containsTerminal) {
        throw PowsyblException("The provided terminal for this ReferencePriorities does not belong to this connectable");
    }

    // If the provided terminal already got a ReferencePriority, replace it.
    for (auto& existingRefPrioPtr : m_referencePriorities[getVariantIndex()]) {
        if(existingRefPrioPtr && stdcxx::areSame(existingRefPrioPtr->getTerminal(), referencePriority->getTerminal())) {
            existingRefPrioPtr = referencePriority;
            return *this;
        }
    }

    registerReferencedTerminalIfNeeded(referencePriority->getTerminal());
    m_referencePriorities[getVariantIndex()].push_back(referencePriority);
    return *this;
}

ReferencePriorityAdder ReferencePriorities::newReferencePriority(){
    return ReferencePriorityAdder(*this);
}

std::vector<std::shared_ptr<ReferencePriority>>& ReferencePriorities::getReferencePriorities(){
    return m_referencePriorities[getVariantIndex()];
}

stdcxx::const_range<ReferencePriority> ReferencePriorities::getReferencePriorities() const {
    return m_referencePriorities[getVariantIndex()] | boost::adaptors::indirected;
}

void ReferencePriorities::deleteReferencePriorities() {
    unregisterReferencedTerminalIfNeeded(getVariantIndex());
    m_referencePriorities[getVariantIndex()].clear();
}

void ReferencePriorities::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Connectable>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Connectable>()));
    }
}

std::vector<std::shared_ptr<ReferencePriority>> ReferencePriorities::get(Network& network) {
    std::vector<std::shared_ptr<ReferencePriority>> networkReferencePriorities;

    for (auto& connectable : network.getConnectables()) {
        stdcxx::Reference<ReferencePriorities> refExtension = connectable.findExtension<ReferencePriorities>();
        if(!refExtension) {
            continue;
        }
        std::vector<std::shared_ptr<ReferencePriority>> referencePriorities = refExtension.get().getReferencePriorities();
        for (auto referencePrioritiyPtr : referencePriorities) {
            if(referencePrioritiyPtr && referencePrioritiyPtr->getPriority() > 0) {
                networkReferencePriorities.push_back(referencePrioritiyPtr);
            }
        }
    }

    //sort
    std::sort(networkReferencePriorities.begin(), networkReferencePriorities.end(),
        [](const std::shared_ptr<ReferencePriority>& refPrioPtr1, const std::shared_ptr<ReferencePriority>& refPrioPtr2) {
            if(refPrioPtr1->getPriority() == refPrioPtr2->getPriority()){
                unsigned long defaultPrio1 = stdcxx::getOrDefault(DEFAULT_CONNECTABLE_TYPE_PRIORITIES, 
                    refPrioPtr1->getTerminal().getConnectable().get().getType(), std::numeric_limits<unsigned long>::max());
                unsigned long defaultPrio2 = stdcxx::getOrDefault(DEFAULT_CONNECTABLE_TYPE_PRIORITIES, 
                    refPrioPtr2->getTerminal().getConnectable().get().getType(), std::numeric_limits<unsigned long>::max());
                return defaultPrio1 < defaultPrio2;
            }
            return refPrioPtr1->getPriority() < refPrioPtr2->getPriority();
    });

    return networkReferencePriorities;
}

void ReferencePriorities::deleteReferencePriorities(Network& network) {
    for (auto& connectable : network.getConnectables()) {
        stdcxx::Reference<ReferencePriorities> refExtension = connectable.findExtension<ReferencePriorities>();
        if(static_cast<bool>(refExtension)) {
            refExtension.get().deleteReferencePriorities();
        }
    }
}

void ReferencePriorities::cleanup() {
    for (auto& referencePriorities : m_referencePriorities) {
        for (auto& referencePriority : referencePriorities) {
            if(static_cast<bool>(referencePriority) ) {
                referencePriority->getTerminal().unregisterReferrer(*this);
            }
        }
    }
}

void ReferencePriorities::onReferencedRemoval(Terminal& removedReference) {
    // Called on deletion of the Connectable this extends, so we could as well do nothing since all its terminals and this extension will be removed anyway afterward

    //remove all ReferencePriority referencing the given Terminal
    for (auto& referencePriorities : m_referencePriorities) {
        referencePriorities.erase(std::remove_if(referencePriorities.begin(), referencePriorities.end(), [&removedReference](const std::shared_ptr<ReferencePriority>& refPrioPtr) {
            return (static_cast<bool>(refPrioPtr) && stdcxx::areSame(removedReference, refPrioPtr->getTerminal()));
        }), referencePriorities.end());
    }
}

void ReferencePriorities::onReferencedReplacement(Terminal& oldReference, Terminal& newReference) {
    //check that newReference Terminal is on the same Connectable:
    if(!stdcxx::areSame(newReference.getConnectable().get(), oldReference.getConnectable().get())) {
        throw PowsyblException("The provided terminal for this ReferencePriorities does not belong to this connectable");
    }
    bool bRegisterOnlyOnce = true;

    for(auto& referencePriorities : m_referencePriorities) {
        // If the newReference terminal already got a ReferencePriority, skip this variant
        auto itNewReference = std::find_if(referencePriorities.begin(), referencePriorities.end(), [&newReference](const std::shared_ptr<ReferencePriority>& refPrioPtr) {
            return (static_cast<bool>(refPrioPtr) && stdcxx::areSame(newReference, refPrioPtr->getTerminal()));
        });
        if (itNewReference != referencePriorities.end()) {
            //already referenced, go to next variant
            continue;
        }

        //Else search oldReference :
        auto itOldReference = std::find_if(referencePriorities.begin(), referencePriorities.end(), [&oldReference](const std::shared_ptr<ReferencePriority>& refPrioPtr) {
            return (static_cast<bool>(refPrioPtr) && stdcxx::areSame(oldReference, refPrioPtr->getTerminal()));
        });
        if (itOldReference != referencePriorities.end()) {
            //oldReference found, we replace it
            if(bRegisterOnlyOnce) { //register only once for all the variants
                bRegisterOnlyOnce = false;
                registerReferencedTerminalIfNeeded(newReference);
            }
            (*itOldReference)->replaceTerminal(oldReference, newReference);
        }
    }
    //remove any remaining ReferencePriority holding oldReference
    onReferencedRemoval(oldReference);
}

void ReferencePriorities::unregisterReferencedTerminalIfNeeded(unsigned long variantIndex) {
    auto currentVariantRefPriorities = m_referencePriorities[variantIndex];

    for (auto& referencePriorityPtr : currentVariantRefPriorities) {
        if(!referencePriorityPtr) {
            continue;
        }
        unsigned int count = 0; //Count of variants on which the referenced terminal of this refPriority is referenced

        for(auto& priorities : m_referencePriorities) {
            auto it = std::find_if(priorities.begin(), priorities.end(), [&referencePriorityPtr](const std::shared_ptr<ReferencePriority>& refPrioPtr) {
                return (static_cast<bool>(refPrioPtr) && stdcxx::areSame(refPrioPtr->getTerminal(), referencePriorityPtr->getTerminal()));
            });
            if (it != priorities.end()) {
                count++;
            }
        }

        if(count == 1) { //current Priority's Terminal is referenced only in the current variant
            referencePriorityPtr->getTerminal().unregisterReferrer(*this);
        }
    }
}

void ReferencePriorities::registerReferencedTerminalIfNeeded(Terminal& terminal) {
    //Register the given terminal only if not already referenced (by another variant also)

    for(auto& referencePriorities : m_referencePriorities) {
        auto it = std::find_if(referencePriorities.begin(), referencePriorities.end(), [&terminal](const std::shared_ptr<ReferencePriority>& refPrioPtr) {
            return (static_cast<bool>(refPrioPtr) && stdcxx::areSame(terminal, refPrioPtr->getTerminal()));
        });
        if (it != referencePriorities.end()) {
            //already referenced
            return;
        }
    }

    //given terminal not found : register it
    terminal.registerReferrer(*this);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
