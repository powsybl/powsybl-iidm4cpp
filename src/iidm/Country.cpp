/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Country.hpp>

#include <array>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

std::string getCountryName(const Country& country) {
    static std::array<std::string, 249> s_countryNames {{
        u8"AFGHANISTAN",
        u8"ÅLAND ISLANDS",
        u8"ALBANIA",
        u8"ALGERIA",
        u8"AMERICAN SAMOA",
        u8"ANDORRA",
        u8"ANGOLA",
        u8"ANGUILLA",
        u8"ANTARCTICA",
        u8"ANTIGUA AND BARBUDA",
        u8"ARGENTINA",
        u8"ARMENIA",
        u8"ARUBA",
        u8"AUSTRALIA",
        u8"AUSTRIA",
        u8"AZERBAIJAN",
        u8"BAHAMAS",
        u8"BAHRAIN",
        u8"BANGLADESH",
        u8"BARBADOS",
        u8"BELARUS",
        u8"BELGIUM",
        u8"BELIZE",
        u8"BENIN",
        u8"BERMUDA",
        u8"BHUTAN",
        u8"BOLIVIA, PLURINATIONAL STATE OF",
        u8"BONAIRE, SINT EUSTATIUS AND SABA",
        u8"BOSNIA AND HERZEGOVINA",
        u8"BOTSWANA",
        u8"BOUVET ISLAND",
        u8"BRAZIL",
        u8"BRITISH INDIAN OCEAN TERRITORY",
        u8"BRUNEI DARUSSALAM",
        u8"BULGARIA",
        u8"BURKINA FASO",
        u8"BURUNDI",
        u8"CAMBODIA",
        u8"CAMEROON",
        u8"CANADA",
        u8"CAPE VERDE",
        u8"CAYMAN ISLANDS",
        u8"CENTRAL AFRICAN REPUBLIC",
        u8"CHAD",
        u8"CHILE",
        u8"CHINA",
        u8"CHRISTMAS ISLAND",
        u8"COCOS  = KEELING) ISLANDS",
        u8"COLOMBIA",
        u8"COMOROS",
        u8"CONGO",
        u8"CONGO, THE DEMOCRATIC REPUBLIC OF THE",
        u8"COOK ISLANDS",
        u8"COSTA RICA",
        u8"CÔTE D'IVOIRE",
        u8"CROATIA",
        u8"CUBA",
        u8"CURAÇAO",
        u8"CYPRUS",
        u8"CZECH REPUBLIC",
        u8"DENMARK",
        u8"DJIBOUTI",
        u8"DOMINICA",
        u8"DOMINICAN REPUBLIC",
        u8"ECUADOR",
        u8"EGYPT",
        u8"EL SALVADOR",
        u8"EQUATORIAL GUINEA",
        u8"ERITREA",
        u8"ESTONIA",
        u8"ETHIOPIA",
        u8"FALKLAND ISLANDS  = MALVINAS)",
        u8"FAROE ISLANDS",
        u8"FIJI",
        u8"FINLAND",
        u8"FRANCE",
        u8"FRENCH GUIANA",
        u8"FRENCH POLYNESIA",
        u8"FRENCH SOUTHERN TERRITORIES",
        u8"GABON",
        u8"GAMBIA",
        u8"GEORGIA",
        u8"GERMANY",
        u8"GHANA",
        u8"GIBRALTAR",
        u8"GREECE",
        u8"GREENLAND",
        u8"GRENADA",
        u8"GUADELOUPE",
        u8"GUAM",
        u8"GUATEMALA",
        u8"GUERNSEY",
        u8"GUINEA",
        u8"GUINEA-BISSAU",
        u8"GUYANA",
        u8"HAITI",
        u8"HEARD ISLAND AND MCDONALD ISLANDS",
        u8"HOLY SEE  = VATICAN CITY STATE)",
        u8"HONDURAS",
        u8"HONG KONG",
        u8"HUNGARY",
        u8"ICELAND",
        u8"INDIA",
        u8"INDONESIA",
        u8"IRAN, ISLAMIC REPUBLIC OF",
        u8"IRAQ",
        u8"IRELAND",
        u8"ISLE OF MAN",
        u8"ISRAEL",
        u8"ITALY",
        u8"JAMAICA",
        u8"JAPAN",
        u8"JERSEY",
        u8"JORDAN",
        u8"KAZAKHSTAN",
        u8"KENYA",
        u8"KIRIBATI",
        u8"KOREA, DEMOCRATIC PEOPLE'S REPUBLIC OF",
        u8"KOREA, REPUBLIC OF",
        u8"KUWAIT",
        u8"KYRGYZSTAN",
        u8"LAO PEOPLE'S DEMOCRATIC REPUBLIC",
        u8"LATVIA",
        u8"LEBANON",
        u8"LESOTHO",
        u8"LIBERIA",
        u8"LIBYA",
        u8"LIECHTENSTEIN",
        u8"LITHUANIA",
        u8"LUXEMBOURG",
        u8"MACAO",
        u8"MACEDONIA, THE FORMER YUGOSLAV REPUBLIC OF",
        u8"MADAGASCAR",
        u8"MALAWI",
        u8"MALAYSIA",
        u8"MALDIVES",
        u8"MALI",
        u8"MALTA",
        u8"MARSHALL ISLANDS",
        u8"MARTINIQUE",
        u8"MAURITANIA",
        u8"MAURITIUS",
        u8"MAYOTTE",
        u8"MEXICO",
        u8"MICRONESIA, FEDERATED STATES OF",
        u8"MOLDOVA, REPUBLIC OF",
        u8"MONACO",
        u8"MONGOLIA",
        u8"MONTENEGRO",
        u8"MONTSERRAT",
        u8"MOROCCO",
        u8"MOZAMBIQUE",
        u8"MYANMAR",
        u8"NAMIBIA",
        u8"NAURU",
        u8"NEPAL",
        u8"NETHERLANDS",
        u8"NEW CALEDONIA",
        u8"NEW ZEALAND",
        u8"NICARAGUA",
        u8"NIGER",
        u8"NIGERIA",
        u8"NIUE",
        u8"NORFOLK ISLAND",
        u8"NORTHERN MARIANA ISLANDS",
        u8"NORWAY",
        u8"OMAN",
        u8"PAKISTAN",
        u8"PALAU",
        u8"PALESTINE, STATE OF",
        u8"PANAMA",
        u8"PAPUA NEW GUINEA",
        u8"PARAGUAY",
        u8"PERU",
        u8"PHILIPPINES",
        u8"PITCAIRN",
        u8"POLAND",
        u8"PORTUGAL",
        u8"PUERTO RICO",
        u8"QATAR",
        u8"RÉUNION",
        u8"ROMANIA",
        u8"RUSSIAN FEDERATION",
        u8"RWANDA",
        u8"SAINT BARTHÉLEMY",
        u8"SAINT HELENA, ASCENSION AND TRISTAN DA CUNHA",
        u8"SAINT KITTS AND NEVIS",
        u8"SAINT LUCIA",
        u8"SAINT MARTIN  = FRENCH PART)",
        u8"SAINT PIERRE AND MIQUELON",
        u8"SAINT VINCENT AND THE GRENADINES",
        u8"SAMOA",
        u8"SAN MARINO",
        u8"SAO TOME AND PRINCIPE",
        u8"SAUDI ARABIA",
        u8"SENEGAL",
        u8"SERBIA",
        u8"SEYCHELLES",
        u8"SIERRA LEONE",
        u8"SINGAPORE",
        u8"SINT MAARTEN  = DUTCH PART)",
        u8"SLOVAKIA",
        u8"SLOVENIA",
        u8"SOLOMON ISLANDS",
        u8"SOMALIA",
        u8"SOUTH AFRICA",
        u8"SOUTH GEORGIA AND THE SOUTH SANDWICH ISLANDS",
        u8"SOUTH SUDAN",
        u8"SPAIN",
        u8"SRI LANKA",
        u8"SUDAN",
        u8"SURINAME",
        u8"SVALBARD AND JAN MAYEN",
        u8"SWAZILAND",
        u8"SWEDEN",
        u8"SWITZERLAND",
        u8"SYRIAN ARAB REPUBLIC",
        u8"TAIWAN, PROVINCE OF CHINA",
        u8"TAJIKISTAN",
        u8"TANZANIA, UNITED REPUBLIC OF",
        u8"THAILAND",
        u8"TIMOR-LESTE",
        u8"TOGO",
        u8"TOKELAU",
        u8"TONGA",
        u8"TRINIDAD AND TOBAGO",
        u8"TUNISIA",
        u8"TURKEY",
        u8"TURKMENISTAN",
        u8"TURKS AND CAICOS ISLANDS",
        u8"TUVALU",
        u8"UGANDA",
        u8"UKRAINE",
        u8"UNITED ARAB EMIRATES",
        u8"UNITED KINGDOM",
        u8"UNITED STATES",
        u8"UNITED STATES MINOR OUTLYING ISLANDS",
        u8"URUGUAY",
        u8"UZBEKISTAN",
        u8"VANUATU",
        u8"VENEZUELA, BOLIVARIAN REPUBLIC OF",
        u8"VIET NAM",
        u8"VIRGIN ISLANDS, BRITISH",
        u8"VIRGIN ISLANDS, U.S.",
        u8"WALLIS AND FUTUNA",
        u8"WESTERN SAHARA",
        u8"YEMEN",
        u8"ZAMBIA",
        u8"ZIMBABWE"
    }};

    auto index = static_cast<unsigned long>(country);
    if (index >= s_countryNames.size()) {
        throw powsybl::AssertionError(powsybl::logging::format("Unexpected %1% value: %2%", stdcxx::simpleClassName<Country>(), index));
    }
    return *(s_countryNames.begin() + index);
}

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<Country>() {
    static std::initializer_list<std::string> s_countryCodes {
        u8"AF",
        u8"AX",
        u8"AL",
        u8"DZ",
        u8"AS",
        u8"AD",
        u8"AO",
        u8"AI",
        u8"AQ",
        u8"AG",
        u8"AR",
        u8"AM",
        u8"AW",
        u8"AU",
        u8"AT",
        u8"AZ",
        u8"BS",
        u8"BH",
        u8"BD",
        u8"BB",
        u8"BY",
        u8"BE",
        u8"BZ",
        u8"BJ",
        u8"BM",
        u8"BT",
        u8"BO",
        u8"BQ",
        u8"BA",
        u8"BW",
        u8"BV",
        u8"BR",
        u8"IO",
        u8"BN",
        u8"BG",
        u8"BF",
        u8"BI",
        u8"KH",
        u8"CM",
        u8"CA",
        u8"CV",
        u8"KY",
        u8"CF",
        u8"TD",
        u8"CL",
        u8"CN",
        u8"CX",
        u8"CC",
        u8"CO",
        u8"KM",
        u8"CG",
        u8"CD",
        u8"CK",
        u8"CR",
        u8"CI",
        u8"HR",
        u8"CU",
        u8"CW",
        u8"CY",
        u8"CZ",
        u8"DK",
        u8"DJ",
        u8"DM",
        u8"DO",
        u8"EC",
        u8"EG",
        u8"SV",
        u8"GQ",
        u8"ER",
        u8"EE",
        u8"ET",
        u8"FK",
        u8"FO",
        u8"FJ",
        u8"FI",
        u8"FR",
        u8"GF",
        u8"PF",
        u8"TF",
        u8"GA",
        u8"GM",
        u8"GE",
        u8"DE",
        u8"GH",
        u8"GI",
        u8"GR",
        u8"GL",
        u8"GD",
        u8"GP",
        u8"GU",
        u8"GT",
        u8"GG",
        u8"GN",
        u8"GW",
        u8"GY",
        u8"HT",
        u8"HM",
        u8"VA",
        u8"HN",
        u8"HK",
        u8"HU",
        u8"IS",
        u8"IN",
        u8"ID",
        u8"IR",
        u8"IQ",
        u8"IE",
        u8"IM",
        u8"IL",
        u8"IT",
        u8"JM",
        u8"JP",
        u8"JE",
        u8"JO",
        u8"KZ",
        u8"KE",
        u8"KI",
        u8"KP",
        u8"KR",
        u8"KW",
        u8"KG",
        u8"LA",
        u8"LV",
        u8"LB",
        u8"LS",
        u8"LR",
        u8"LY",
        u8"LI",
        u8"LT",
        u8"LU",
        u8"MO",
        u8"MK",
        u8"MG",
        u8"MW",
        u8"MY",
        u8"MV",
        u8"ML",
        u8"MT",
        u8"MH",
        u8"MQ",
        u8"MR",
        u8"MU",
        u8"YT",
        u8"MX",
        u8"FM",
        u8"MD",
        u8"MC",
        u8"MN",
        u8"ME",
        u8"MS",
        u8"MA",
        u8"MZ",
        u8"MM",
        u8"NA",
        u8"NR",
        u8"NP",
        u8"NL",
        u8"NC",
        u8"NZ",
        u8"NI",
        u8"NE",
        u8"NG",
        u8"NU",
        u8"NF",
        u8"MP",
        u8"NO",
        u8"OM",
        u8"PK",
        u8"PW",
        u8"PS",
        u8"PA",
        u8"PG",
        u8"PY",
        u8"PE",
        u8"PH",
        u8"PN",
        u8"PL",
        u8"PT",
        u8"PR",
        u8"QA",
        u8"RE",
        u8"RO",
        u8"RU",
        u8"RW",
        u8"BL",
        u8"SH",
        u8"KN",
        u8"LC",
        u8"MF",
        u8"PM",
        u8"VC",
        u8"WS",
        u8"SM",
        u8"ST",
        u8"SA",
        u8"SN",
        u8"RS",
        u8"SC",
        u8"SL",
        u8"SG",
        u8"SX",
        u8"SK",
        u8"SI",
        u8"SB",
        u8"SO",
        u8"ZA",
        u8"GS",
        u8"SS",
        u8"ES",
        u8"LK",
        u8"SD",
        u8"SR",
        u8"SJ",
        u8"SZ",
        u8"SE",
        u8"CH",
        u8"SY",
        u8"TW",
        u8"TJ",
        u8"TZ",
        u8"TH",
        u8"TL",
        u8"TG",
        u8"TK",
        u8"TO",
        u8"TT",
        u8"TN",
        u8"TR",
        u8"TM",
        u8"TC",
        u8"TV",
        u8"UG",
        u8"UA",
        u8"AE",
        u8"GB",
        u8"US",
        u8"UM",
        u8"UY",
        u8"UZ",
        u8"VU",
        u8"VE",
        u8"VN",
        u8"VG",
        u8"VI",
        u8"WF",
        u8"EH",
        u8"YE",
        u8"ZM",
        u8"ZW"
    };
    return s_countryCodes;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
