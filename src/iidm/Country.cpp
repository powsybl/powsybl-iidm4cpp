/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Country.hpp>

#include <array>

namespace powsybl {

namespace iidm {

const std::string& getCountryName(const Country& country) {
    static std::array<std::string, 249> s_countryNames = {{
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

    return s_countryNames.at(static_cast<unsigned int>(country));
}

}

}