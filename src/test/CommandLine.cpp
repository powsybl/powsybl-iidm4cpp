/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/test/CommandLine.hpp>

#include <boost/program_options/parsers.hpp>
#include <boost/test/unit_test.hpp>
 
namespace powsybl {

namespace test {

const boost::program_options::variable_value& CommandLine::getOptionValue(const std::string& name) const {
    return m_options[name];
}

void CommandLine::parse(const boost::program_options::options_description& desc) {
    m_options.clear();
    boost::program_options::store(
        boost::program_options::parse_command_line(
            boost::unit_test::framework::master_test_suite().argc,
            boost::unit_test::framework::master_test_suite().argv,
            desc),
        m_options);
    boost::program_options::notify(m_options);
}

}  // namespace test

}  // namespace powsybl
