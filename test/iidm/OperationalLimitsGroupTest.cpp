/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/test/unit_test.hpp>

#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/Line.hpp>
#include <powsybl/iidm/LineAdder.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/ThreeWindingsTransformerAdder.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/network/EurostagFactory.hpp>

#include <powsybl/stdcxx/math.hpp>
#include <powsybl/stdcxx/memory.hpp>

#include <powsybl/test/AssertionUtils.hpp>

namespace powsybl {

namespace iidm {

BOOST_AUTO_TEST_SUITE(OperationalLimitsGroupTestSuite)



// The purpose of this test is to check that it is possible to use a custom OperationalLimitsGroup
// to add OperationalLimits which are not directly linked in a Network element. For instance,
// it could be used to define limits in an extension without having to define custom limits implementation.
BOOST_AUTO_TEST_CASE(CustomOperationalLimitsGroupTest) {
    Network network = powsybl::network::EurostagFactory::createTutorial1Network();
    Bus& bus1 = network.getVoltageLevel("VLHV1").getBusBreakerView().getBus("NHV1").get();

    class CustomOperationalLimitsGroup : public OperationalLimitsGroup {
        public: 
        CustomOperationalLimitsGroup(const std::string& id, Identifiable& identifiable, const std::string& attributeName) :
            OperationalLimitsGroup(id, identifiable, attributeName) {
        }

        std::string getMessageHeader() const override {
            return "Custom Validable : ";
        }
    };

    CustomOperationalLimitsGroup customGroup("group1", bus1, "limits");
    customGroup.newCurrentLimits().setPermanentLimit(100.0).add();

    BOOST_CHECK_EQUAL("Custom Validable : ", customGroup.getMessageHeader());

    ApparentPowerLimitsAdder adder = customGroup.newApparentPowerLimits()
                                                    .setPermanentLimit(-1);
    POWSYBL_ASSERT_THROW(adder.add(), ValidationException, "Custom Validable : permanent limit must be >= 0");

}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace iidm

}  // namespace powsybl
