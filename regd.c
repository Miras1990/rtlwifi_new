// SPDX-License-Identifier: GPL-2.0
/* Copyright(c) 2018  Realtek Corporation.
 */

#include "main.h"
#include "regd.h"
#include "debug.h"
#include "phy.h"

#define COUNTRY_CHPLAN_ENT(_alpha2, _chplan, _txpwr_regd) \
	{.alpha2 = (_alpha2), \
	 .chplan = (_chplan), \
	 .txpwr_regd = (_txpwr_regd) \
	}

/* If country code is not correctly defined in efuse,
 * use worldwide country code and txpwr regd.
 */
static const struct rtw_regulatory rtw_defined_chplan =
	COUNTRY_CHPLAN_ENT("00", RTW_CHPLAN_REALTEK_DEFINE, RTW_REGD_WW);

static const struct rtw_regulatory all_chplan_map[] = {
	COUNTRY_CHPLAN_ENT("AD", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("AE", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("AF", RTW_CHPLAN_ETSI1_ETSI4, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("AG", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("AI", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("AL", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("AM", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("AN", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("AO", RTW_CHPLAN_WORLD_ETSI6, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("AQ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("AR", RTW_CHPLAN_FCC2_FCC7, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("AS", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("AT", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("AU", RTW_CHPLAN_WORLD_ACMA1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("AW", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("AZ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("BA", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("BB", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("BD", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("BE", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("BF", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("BG", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("BH", RTW_CHPLAN_WORLD_ETSI6, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("BI", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("BJ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("BN", RTW_CHPLAN_WORLD_ETSI6, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("BO", RTW_CHPLAN_WORLD_FCC7, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("BR", RTW_CHPLAN_FCC2_FCC1, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("BS", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("BW", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("BY", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("BZ", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("CA", RTW_CHPLAN_IC1_IC2, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("CC", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("CD", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("CF", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("CG", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("CH", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("CI", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("CK", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("CL", RTW_CHPLAN_WORLD_CHILE1, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("CM", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("CN", RTW_CHPLAN_WORLD_ETSI7, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("CO", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("CR", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("CV", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("CX", RTW_CHPLAN_WORLD_ACMA1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("CY", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("CZ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("DE", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("DJ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("DK", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("DM", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("DO", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("DZ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("EC", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("EE", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("EG", RTW_CHPLAN_WORLD_ETSI6, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("EH", RTW_CHPLAN_WORLD_ETSI6, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("ER", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("ES", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("ET", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("FI", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("FJ", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("FK", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("FM", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("FO", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("FR", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GA", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GB", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GD", RTW_CHPLAN_FCC1_FCC7, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("GE", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GF", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GG", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GH", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GI", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GL", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GM", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GN", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GP", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GQ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GR", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GS", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GT", RTW_CHPLAN_FCC2_FCC7, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("GU", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("GW", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("GY", RTW_CHPLAN_FCC1_NCC3, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("HK", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("HM", RTW_CHPLAN_WORLD_ACMA1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("HN", RTW_CHPLAN_WORLD_FCC5, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("HR", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("HT", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("HU", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("ID", RTW_CHPLAN_ETSI1_ETSI12, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("IE", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("IL", RTW_CHPLAN_WORLD_ETSI6, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("IM", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("IN", RTW_CHPLAN_WORLD_ETSI7, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("IQ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("IR", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("IS", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("IT", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("JE", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("JM", RTW_CHPLAN_WORLD_ETSI10, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("JO", RTW_CHPLAN_WORLD_ETSI8, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("JP", RTW_CHPLAN_MKK1_MKK1, RTW_REGD_MKK),
	COUNTRY_CHPLAN_ENT("KE", RTW_CHPLAN_WORLD_ETSI6, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("KG", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("KH", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("KI", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("KN", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("KR", RTW_CHPLAN_KCC1_KCC2, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("KW", RTW_CHPLAN_WORLD_ETSI6, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("KY", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("KZ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("LA", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("LB", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("LC", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("LI", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("LK", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("LR", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("LS", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("LT", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("LU", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("LV", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("LY", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MA", RTW_CHPLAN_WORLD_ETSI6, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MC", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MD", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("ME", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MF", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("MG", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MH", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("MK", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("ML", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MM", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MN", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MO", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MP", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("MQ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MR", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MS", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MT", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MU", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MV", RTW_CHPLAN_WORLD_ETSI6, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MW", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MX", RTW_CHPLAN_FCC2_FCC7, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("MY", RTW_CHPLAN_WORLD_ETSI20, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("MZ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("NA", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("NC", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("NE", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("NF", RTW_CHPLAN_WORLD_ACMA1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("NG", RTW_CHPLAN_WORLD_ETSI20, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("NI", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("NL", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("NO", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("NP", RTW_CHPLAN_WORLD_ETSI6, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("NR", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("NU", RTW_CHPLAN_WORLD_ACMA1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("NZ", RTW_CHPLAN_WORLD_ACMA1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("OM", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("PA", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("PE", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("PF", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("PG", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("PH", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("PK", RTW_CHPLAN_WORLD_ETSI10, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("PL", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("PM", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("PR", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("PT", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("PW", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("PY", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("QA", RTW_CHPLAN_WORLD_ETSI10, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("RE", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("RO", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("RS", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("RU", RTW_CHPLAN_WORLD_ETSI14, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("RW", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("SA", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("SB", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("SC", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("SE", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("SG", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("SH", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("SI", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("SJ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("SK", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("SL", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("SM", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("SN", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("SO", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("SR", RTW_CHPLAN_FCC2_FCC17, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("ST", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("SV", RTW_CHPLAN_WORLD_FCC3, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("SX", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("SZ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("TC", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("TD", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("TF", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("TG", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("TH", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("TJ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("TK", RTW_CHPLAN_WORLD_ACMA1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("TM", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("TN", RTW_CHPLAN_WORLD_ETSI6, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("TO", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("TR", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("TT", RTW_CHPLAN_ETSI1_ETSI4, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("TW", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("TZ", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("UA", RTW_CHPLAN_WORLD_ETSI3, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("UG", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("US", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("UY", RTW_CHPLAN_WORLD_FCC3, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("UZ", RTW_CHPLAN_WORLD_ETSI6, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("VA", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("VC", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("VE", RTW_CHPLAN_WORLD_FCC3, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("VI", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("VN", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("VU", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("WF", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("WS", RTW_CHPLAN_FCC2_FCC11, RTW_REGD_FCC),
	COUNTRY_CHPLAN_ENT("YE", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("YT", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("ZA", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("ZM", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
	COUNTRY_CHPLAN_ENT("ZW", RTW_CHPLAN_WORLD_ETSI1, RTW_REGD_ETSI),
};

static void rtw_regd_apply_beaconing_flags(struct wiphy *wiphy,
					   enum nl80211_reg_initiator initiator)
{
	enum nl80211_band band;
	struct ieee80211_supported_band *sband;
	const struct ieee80211_reg_rule *reg_rule;
	struct ieee80211_channel *ch;
	unsigned int i;

	for (band = 0; band < NUM_NL80211_BANDS; band++) {
		if (!wiphy->bands[band])
			continue;

		sband = wiphy->bands[band];
		for (i = 0; i < sband->n_channels; i++) {
			ch = &sband->channels[i];

			reg_rule = freq_reg_info(wiphy,
						 MHZ_TO_KHZ(ch->center_freq));
			if (IS_ERR(reg_rule))
				continue;

			ch->flags &= ~IEEE80211_CHAN_DISABLED;

			if (!(reg_rule->flags & NL80211_RRF_NO_IR))
				ch->flags &= ~IEEE80211_CHAN_NO_IR;
		}
	}
}

static void rtw_regd_apply_hw_cap_flags(struct wiphy *wiphy)
{
	struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
	struct ieee80211_supported_band *sband;
	struct ieee80211_channel *ch;
	struct rtw_dev *rtwdev = hw->priv;
	struct rtw_efuse *efuse = &rtwdev->efuse;
	int i;

	if (efuse->hw_cap.bw & BIT(RTW_CHANNEL_WIDTH_80))
		return;

	sband = wiphy->bands[NL80211_BAND_2GHZ];
	if (!sband)
		goto out_5g;

	for (i = 0; i < sband->n_channels; i++) {
		ch = &sband->channels[i];
		ch->flags |= IEEE80211_CHAN_NO_80MHZ;
	}

out_5g:
	sband = wiphy->bands[NL80211_BAND_5GHZ];
	if (!sband)
		return;

	for (i = 0; i < sband->n_channels; i++) {
		ch = &sband->channels[i];
		ch->flags |= IEEE80211_CHAN_NO_80MHZ;
	}
}

static void rtw_regd_apply_world_flags(struct wiphy *wiphy,
				       enum nl80211_reg_initiator initiator)
{
	rtw_regd_apply_beaconing_flags(wiphy, initiator);
}

static struct rtw_regulatory rtw_regd_find_reg_by_name(char *alpha2)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(all_chplan_map); i++) {
		if (!memcmp(all_chplan_map[i].alpha2, alpha2, 2))
			return all_chplan_map[i];
	}

	return rtw_defined_chplan;
}

static int rtw_regd_notifier_apply(struct rtw_dev *rtwdev,
				   struct wiphy *wiphy,
				   struct regulatory_request *request)
{
	if (request->initiator == NL80211_REGDOM_SET_BY_USER)
		return 0;
	rtwdev->regd = rtw_regd_find_reg_by_name(request->alpha2);
	rtw_regd_apply_world_flags(wiphy, request->initiator);

	return 0;
}

static int
rtw_regd_init_wiphy(struct rtw_regulatory *reg, struct wiphy *wiphy,
		    void (*reg_notifier)(struct wiphy *wiphy,
					 struct regulatory_request *request))
{
	wiphy->reg_notifier = reg_notifier;

	wiphy->regulatory_flags &= ~REGULATORY_CUSTOM_REG;
	wiphy->regulatory_flags &= ~REGULATORY_STRICT_REG;
	wiphy->regulatory_flags &= ~REGULATORY_DISABLE_BEACON_HINTS;

	rtw_regd_apply_hw_cap_flags(wiphy);

	return 0;
}

int rtw_regd_init(struct rtw_dev *rtwdev,
		  void (*reg_notifier)(struct wiphy *wiphy,
				       struct regulatory_request *request))
{
	struct wiphy *wiphy = rtwdev->hw->wiphy;

	if (!wiphy)
		return -EINVAL;

	rtwdev->regd = rtw_regd_find_reg_by_name(rtwdev->efuse.country_code);
	rtw_regd_init_wiphy(&rtwdev->regd, wiphy, reg_notifier);

	return 0;
}

void rtw_regd_notifier(struct wiphy *wiphy, struct regulatory_request *request)
{
	struct ieee80211_hw *hw = wiphy_to_ieee80211_hw(wiphy);
	struct rtw_dev *rtwdev = hw->priv;
	struct rtw_hal *hal = &rtwdev->hal;

	rtw_regd_notifier_apply(rtwdev, wiphy, request);
	rtw_dbg(rtwdev, RTW_DBG_REGD,
		"get alpha2 %c%c from initiator %d, mapping to chplan 0x%x, txregd %d\n",
		request->alpha2[0], request->alpha2[1], request->initiator,
		rtwdev->regd.chplan, rtwdev->regd.txpwr_regd);

	rtw_phy_set_tx_power_level(rtwdev, hal->current_channel);
}
