/* ed:set tabstop=8 noexpandtab: */
/*****************************************************************************
 *
 * unicode.c	Unicode to ISO-8859-1 conversion
 *
 * Copyright by Juergen Buchmueller <pullmoll@t-online.de>
 *
 *****************************************************************************/
#include "unicode.h"

/**
 * @brief mapping of ISO-8859-1 characters to Unicode symbols
 */
typedef struct {
	/** @brief ISO-8859-1 code */
	uint16_t code;
	/** @brief Unicode */
	uint16_t ucs;
}	unicode_map_t;

static const unicode_map_t iso8859_1[] = {
#if	0
	{0x01,0x25C6},/* BLACK DIAMOND */
	{0x02,0x2592},/* MEDIUM SHADE */
	{0x03,0x2409},/* SYMBOL FOR HORIZONTAL TABULATION */
	{0x04,0x240C},/* SYMBOL FOR FORM FEED */
	{0x05,0x240D},/* SYMBOL FOR CARRIAGE RETURN */
	{0x06,0x240A},/* SYMBOL FOR LINE FEED */
	{0x07,0x00B0},/* DEGREE SIGN */
	{0x08,0x00B1},/* PLUS-MINUS SIGN */
	{0x09,0x2424},/* SYMBOL FOR NEWLINE */
	{0x0A,0x240B},/* SYMBOL FOR VERTICAL TABULATION */
	{0x0B,0x2518},/* BOX DRAWINGS LIGHT UP AND LEFT */
	{0x0C,0x2510},/* BOX DRAWINGS LIGHT DOWN AND LEFT */
	{0x0D,0x250C},/* BOX DRAWINGS LIGHT DOWN AND RIGHT */
	{0x0E,0x2514},/* BOX DRAWINGS LIGHT UP AND RIGHT */
	{0x0F,0x253C},/* BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL */
	{0x12,0x2500},/* BOX DRAWINGS LIGHT HORIZONTAL */
	{0x15,0x251C},/* BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
	{0x16,0x2524},/* BOX DRAWINGS LIGHT VERTICAL AND LEFT */
	{0x17,0x2534},/* BOX DRAWINGS LIGHT UP AND HORIZONTAL */
	{0x18,0x252C},/* BOX DRAWINGS LIGHT DOWN AND HORIZONTAL */
	{0x19,0x2502},/* BOX DRAWINGS LIGHT VERTICAL */
	{0x1A,0x2264},/* LESS-THAN OR EQUAL TO */
	{0x1B,0x2265},/* GREATER-THAN OR EQUAL TO */
	{0x1C,0x03C0},/* GREEK SMALL LETTER PI */
	{0x1D,0x2260},/* NOT EQUAL TO */
	{0x1E,0x00A3},/* POUND SIGN */
	{0x1F,0x00B7},/* MIDDLE DOT */
#else
	{0x01,0x0001},/* SOH */
	{0x02,0x0002},/* STX */
	{0x03,0x0003},/* ETX */
	{0x04,0x0004},/* EOT */
	{0x05,0x0005},/* ENQ */
	{0x06,0x0006},/* ACK */
	{0x07,0x0007},/* ITB */
	{0x08,0x0008},/* BSP */
	{0x09,0x0009},/* HT  */
	{0x0A,0x000A},/* LF  */
	{0x0B,0x000B},/* VT  */
	{0x0C,0x000C},/* FF  */
	{0x0D,0x000D},/* CR  */
	{0x0E,0x000E},/* SI  */
	{0x0F,0x000F},/* SO  */
	{0x10,0x0010},/* DLE */
	{0x11,0x0011},/* DC1 */
	{0x12,0x0012},/* DC2 */
	{0x13,0x0013},/* DC3 */
	{0x14,0x0014},/* DC4 */
	{0x15,0x0015},/* NAK */
	{0x16,0x0016},/* SYN */
	{0x17,0x0017},/* ETB */
	{0x18,0x0018},/* CAN */
	{0x19,0x0019},/* EM  */
	{0x1A,0x001A},/* SUB */
	{0x1B,0x001B},/* ESC */
	{0x1C,0x001C},/* FS  */
	{0x1D,0x001D},/* GS  */
	{0x1E,0x001E},/* RS  */
	{0x1F,0x001F},/* US  */
#endif
	{0x20,0x0020},/* SPACE */
	{0x21,0x0021},/* EXCLAMATION MARK */
	{0x22,0x0022},/* QUOTATION MARK */
	{0x23,0x0023},/* NUMBER SIGN */
	{0x24,0x0024},/* DOLLAR SIGN */
	{0x25,0x0025},/* PERCENT SIGN */
	{0x26,0x0026},/* AMPERSAND */
	{0x27,0x0027},/* APOSTROPHE */
	{0x28,0x0028},/* LEFT PARENTHESIS */
	{0x29,0x0029},/* RIGHT PARENTHESIS */
	{0x2A,0x002A},/* ASTERISK */
	{0x2B,0x002B},/* PLUS SIGN */
	{0x2C,0x002C},/* COMMA */
	{0x2D,0x002D},/* HYPHEN-MINUS */
	{0x2E,0x002E},/* FULL STOP */
	{0x2F,0x002F},/* SOLIDUS */
	{0x30,0x0030},/* DIGIT ZERO */
	{0x31,0x0031},/* DIGIT ONE */
	{0x32,0x0032},/* DIGIT TWO */
	{0x33,0x0033},/* DIGIT THREE */
	{0x34,0x0034},/* DIGIT FOUR */
	{0x35,0x0035},/* DIGIT FIVE */
	{0x36,0x0036},/* DIGIT SIX */
	{0x37,0x0037},/* DIGIT SEVEN */
	{0x38,0x0038},/* DIGIT EIGHT */
	{0x39,0x0039},/* DIGIT NINE */
	{0x3A,0x003A},/* COLON */
	{0x3B,0x003B},/* SEMICOLON */
	{0x3C,0x003C},/* LESS-THAN SIGN */
	{0x3D,0x003D},/* EQUALS SIGN */
	{0x3E,0x003E},/* GREATER-THAN SIGN */
	{0x3F,0x003F},/* QUESTION MARK */
	{0x40,0x0040},/* COMMERCIAL AT */
	{0x41,0x0041},/* LATIN CAPITAL LETTER A */
	{0x42,0x0042},/* LATIN CAPITAL LETTER B */
	{0x43,0x0043},/* LATIN CAPITAL LETTER C */
	{0x44,0x0044},/* LATIN CAPITAL LETTER D */
	{0x45,0x0045},/* LATIN CAPITAL LETTER E */
	{0x46,0x0046},/* LATIN CAPITAL LETTER F */
	{0x47,0x0047},/* LATIN CAPITAL LETTER G */
	{0x48,0x0048},/* LATIN CAPITAL LETTER H */
	{0x49,0x0049},/* LATIN CAPITAL LETTER I */
	{0x4A,0x004A},/* LATIN CAPITAL LETTER J */
	{0x4B,0x004B},/* LATIN CAPITAL LETTER K */
	{0x4C,0x004C},/* LATIN CAPITAL LETTER L */
	{0x4D,0x004D},/* LATIN CAPITAL LETTER M */
	{0x4E,0x004E},/* LATIN CAPITAL LETTER N */
	{0x4F,0x004F},/* LATIN CAPITAL LETTER O */
	{0x50,0x0050},/* LATIN CAPITAL LETTER P */
	{0x51,0x0051},/* LATIN CAPITAL LETTER Q */
	{0x52,0x0052},/* LATIN CAPITAL LETTER R */
	{0x53,0x0053},/* LATIN CAPITAL LETTER S */
	{0x54,0x0054},/* LATIN CAPITAL LETTER T */
	{0x55,0x0055},/* LATIN CAPITAL LETTER U */
	{0x56,0x0056},/* LATIN CAPITAL LETTER V */
	{0x57,0x0057},/* LATIN CAPITAL LETTER W */
	{0x58,0x0058},/* LATIN CAPITAL LETTER X */
	{0x59,0x0059},/* LATIN CAPITAL LETTER Y */
	{0x5A,0x005A},/* LATIN CAPITAL LETTER Z */
	{0x5B,0x005B},/* LEFT SQUARE BRACKET */
	{0x5C,0x005C},/* REVERSE SOLIDUS */
	{0x5D,0x005D},/* RIGHT SQUARE BRACKET */
	{0x5E,0x005E},/* CIRCUMFLEX ACCENT */
	{0x5F,0x005F},/* LOW LINE */
	{0x60,0x0060},/* GRAVE ACCENT */
	{0x61,0x0061},/* LATIN SMALL LETTER A */
	{0x62,0x0062},/* LATIN SMALL LETTER B */
	{0x63,0x0063},/* LATIN SMALL LETTER C */
	{0x64,0x0064},/* LATIN SMALL LETTER D */
	{0x65,0x0065},/* LATIN SMALL LETTER E */
	{0x66,0x0066},/* LATIN SMALL LETTER F */
	{0x67,0x0067},/* LATIN SMALL LETTER G */
	{0x68,0x0068},/* LATIN SMALL LETTER H */
	{0x69,0x0069},/* LATIN SMALL LETTER I */
	{0x6A,0x006A},/* LATIN SMALL LETTER J */
	{0x6B,0x006B},/* LATIN SMALL LETTER K */
	{0x6C,0x006C},/* LATIN SMALL LETTER L */
	{0x6D,0x006D},/* LATIN SMALL LETTER M */
	{0x6E,0x006E},/* LATIN SMALL LETTER N */
	{0x6F,0x006F},/* LATIN SMALL LETTER O */
	{0x70,0x0070},/* LATIN SMALL LETTER P */
	{0x71,0x0071},/* LATIN SMALL LETTER Q */
	{0x72,0x0072},/* LATIN SMALL LETTER R */
	{0x73,0x0073},/* LATIN SMALL LETTER S */
	{0x74,0x0074},/* LATIN SMALL LETTER T */
	{0x75,0x0075},/* LATIN SMALL LETTER U */
	{0x76,0x0076},/* LATIN SMALL LETTER V */
	{0x77,0x0077},/* LATIN SMALL LETTER W */
	{0x78,0x0078},/* LATIN SMALL LETTER X */
	{0x79,0x0079},/* LATIN SMALL LETTER Y */
	{0x7A,0x007A},/* LATIN SMALL LETTER Z */
	{0x7B,0x007B},/* LEFT CURLY BRACKET */
	{0x7C,0x007C},/* VERTICAL LINE */
	{0x7D,0x007D},/* RIGHT CURLY BRACKET */
	{0x7E,0x007E},/* TILDE */
	{0xA0,0x00A0},/* NO-BREAK SPACE */
	{0xA1,0x00A1},/* INVERTED EXCLAMATION MARK */
	{0xA2,0x00A2},/* CENT SIGN */
	{0xA3,0x00A3},/* POUND SIGN */
	{0xA4,0x00A4},/* CURRENCY SIGN */
	{0xA5,0x00A5},/* YEN SIGN */
	{0xA6,0x00A6},/* BROKEN BAR */
	{0xA7,0x00A7},/* SECTION SIGN */
	{0xA8,0x00A8},/* DIAERESIS */
	{0xA9,0x00A9},/* COPYRIGHT SIGN */
	{0xAA,0x00AA},/* FEMININE ORDINAL INDICATOR */
	{0xAB,0x00AB},/* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
	{0xAC,0x00AC},/* NOT SIGN */
	{0xAD,0x00AD},/* SOFT HYPHEN */
	{0xAE,0x00AE},/* REGISTERED SIGN */
	{0xAF,0x00AF},/* MACRON */
	{0xB0,0x00B0},/* DEGREE SIGN */
	{0xB1,0x00B1},/* PLUS-MINUS SIGN */
	{0xB2,0x00B2},/* SUPERSCRIPT TWO */
	{0xB3,0x00B3},/* SUPERSCRIPT THREE */
	{0xB4,0x00B4},/* ACUTE ACCENT */
	{0xB5,0x00B5},/* MICRO SIGN */
	{0xB6,0x00B6},/* PILCROW SIGN */
	{0xB7,0x00B7},/* MIDDLE DOT */
	{0xB8,0x00B8},/* CEDILLA */
	{0xB9,0x00B9},/* SUPERSCRIPT ONE */
	{0xBA,0x00BA},/* MASCULINE ORDINAL INDICATOR */
	{0xBB,0x00BB},/* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
	{0xBC,0x00BC},/* VULGAR FRACTION ONE QUARTER */
	{0xBD,0x00BD},/* VULGAR FRACTION ONE HALF */
	{0xBE,0x00BE},/* VULGAR FRACTION THREE QUARTERS */
	{0xBF,0x00BF},/* INVERTED QUESTION MARK */
	{0xC0,0x00C0},/* LATIN CAPITAL LETTER A WITH GRAVE */
	{0xC1,0x00C1},/* LATIN CAPITAL LETTER A WITH ACUTE */
	{0xC2,0x00C2},/* LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
	{0xC3,0x00C3},/* LATIN CAPITAL LETTER A WITH TILDE */
	{0xC4,0x00C4},/* LATIN CAPITAL LETTER A WITH DIAERESIS */
	{0xC5,0x00C5},/* LATIN CAPITAL LETTER A WITH RING ABOVE */
	{0xC6,0x00C6},/* LATIN CAPITAL LETTER AE */
	{0xC7,0x00C7},/* LATIN CAPITAL LETTER C WITH CEDILLA */
	{0xC8,0x00C8},/* LATIN CAPITAL LETTER E WITH GRAVE */
	{0xC9,0x00C9},/* LATIN CAPITAL LETTER E WITH ACUTE */
	{0xCA,0x00CA},/* LATIN CAPITAL LETTER E WITH CIRCUMFLEX */
	{0xCB,0x00CB},/* LATIN CAPITAL LETTER E WITH DIAERESIS */
	{0xCC,0x00CC},/* LATIN CAPITAL LETTER I WITH GRAVE */
	{0xCD,0x00CD},/* LATIN CAPITAL LETTER I WITH ACUTE */
	{0xCE,0x00CE},/* LATIN CAPITAL LETTER I WITH CIRCUMFLEX */
	{0xCF,0x00CF},/* LATIN CAPITAL LETTER I WITH DIAERESIS */
	{0xD0,0x00D0},/* LATIN CAPITAL LETTER ETH (Icelandic) */
	{0xD1,0x00D1},/* LATIN CAPITAL LETTER N WITH TILDE */
	{0xD2,0x00D2},/* LATIN CAPITAL LETTER O WITH GRAVE */
	{0xD3,0x00D3},/* LATIN CAPITAL LETTER O WITH ACUTE */
	{0xD4,0x00D4},/* LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
	{0xD5,0x00D5},/* LATIN CAPITAL LETTER O WITH TILDE */
	{0xD6,0x00D6},/* LATIN CAPITAL LETTER O WITH DIAERESIS */
	{0xD7,0x00D7},/* MULTIPLICATION SIGN */
	{0xD8,0x00D8},/* LATIN CAPITAL LETTER O WITH STROKE */
	{0xD9,0x00D9},/* LATIN CAPITAL LETTER U WITH GRAVE */
	{0xDA,0x00DA},/* LATIN CAPITAL LETTER U WITH ACUTE */
	{0xDB,0x00DB},/* LATIN CAPITAL LETTER U WITH CIRCUMFLEX */
	{0xDC,0x00DC},/* LATIN CAPITAL LETTER U WITH DIAERESIS */
	{0xDD,0x00DD},/* LATIN CAPITAL LETTER Y WITH ACUTE */
	{0xDE,0x00DE},/* LATIN CAPITAL LETTER THORN (Icelandic) */
	{0xDF,0x00DF},/* LATIN SMALL LETTER SHARP S (German) */
	{0xE0,0x00E0},/* LATIN SMALL LETTER A WITH GRAVE */
	{0xE1,0x00E1},/* LATIN SMALL LETTER A WITH ACUTE */
	{0xE2,0x00E2},/* LATIN SMALL LETTER A WITH CIRCUMFLEX */
	{0xE3,0x00E3},/* LATIN SMALL LETTER A WITH TILDE */
	{0xE4,0x00E4},/* LATIN SMALL LETTER A WITH DIAERESIS */
	{0xE5,0x00E5},/* LATIN SMALL LETTER A WITH RING ABOVE */
	{0xE6,0x00E6},/* LATIN SMALL LETTER AE */
	{0xE7,0x00E7},/* LATIN SMALL LETTER C WITH CEDILLA */
	{0xE8,0x00E8},/* LATIN SMALL LETTER E WITH GRAVE */
	{0xE9,0x00E9},/* LATIN SMALL LETTER E WITH ACUTE */
	{0xEA,0x00EA},/* LATIN SMALL LETTER E WITH CIRCUMFLEX */
	{0xEB,0x00EB},/* LATIN SMALL LETTER E WITH DIAERESIS */
	{0xEC,0x00EC},/* LATIN SMALL LETTER I WITH GRAVE */
	{0xED,0x00ED},/* LATIN SMALL LETTER I WITH ACUTE */
	{0xEE,0x00EE},/* LATIN SMALL LETTER I WITH CIRCUMFLEX */
	{0xEF,0x00EF},/* LATIN SMALL LETTER I WITH DIAERESIS */
	{0xF0,0x00F0},/* LATIN SMALL LETTER ETH (Icelandic) */
	{0xF1,0x00F1},/* LATIN SMALL LETTER N WITH TILDE */
	{0xF2,0x00F2},/* LATIN SMALL LETTER O WITH GRAVE */
	{0xF3,0x00F3},/* LATIN SMALL LETTER O WITH ACUTE */
	{0xF4,0x00F4},/* LATIN SMALL LETTER O WITH CIRCUMFLEX */
	{0xF5,0x00F5},/* LATIN SMALL LETTER O WITH TILDE */
	{0xF6,0x00F6},/* LATIN SMALL LETTER O WITH DIAERESIS */
	{0xF7,0x00F7},/* DIVISION SIGN */
	{0xF8,0x00F8},/* LATIN SMALL LETTER O WITH STROKE */
	{0xF9,0x00F9},/* LATIN SMALL LETTER U WITH GRAVE */
	{0xFA,0x00FA},/* LATIN SMALL LETTER U WITH ACUTE */
	{0xFB,0x00FB},/* LATIN SMALL LETTER U WITH CIRCUMFLEX */
	{0xFC,0x00FC},/* LATIN SMALL LETTER U WITH DIAERESIS */
	{0xFD,0x00FD},/* LATIN SMALL LETTER Y WITH ACUTE */
	{0xFE,0x00FE},/* LATIN SMALL LETTER THORN (Icelandic) */
	{0xFF,0x00FF},/* LATIN SMALL LETTER Y WITH DIAERESIS */
	{0x00,0x0000}
};

/** @brief put a character in a local buffer and flush to char* res on overrun */
#define	LBSIZE	256

/** @brief flush anything left in a local buffer */
#define	LBFLUSH() do { \
	if (local_buf_cnt > 0) { \
		int old = res ? strlen(res) : 0; \
		res = realloc(res, old + local_buf_cnt + 1); \
		if (NULL == res) { \
			perror("realloc()"); \
			exit(1); \
		} \
		res = memcpy(res + old, local_buf, local_buf_cnt); \
	} \
} while (0)


#define	LBPUT(c) do { \
	if (local_buf_cnt == LBSIZE) { \
		LBFLUSH(); \
		local_buf_cnt = 0; \
	} \
	local_buf[local_buf_cnt] = (c); \
	++local_buf_cnt; \
} while (0)

/**
 * @brief convert utf-8 to iso-8859-1
 *
 * @param src string with utf-8 characters
 * @result returns the iso-8859-1 encoded string
 */
char *utf8_to_iso8859_1(const char *src, int size)
{
	char local_buf[LBSIZE];
	int local_buf_cnt;
	uint32_t i, j, ucs, tc;
	uint8_t ch;
	int count;
	char *res = NULL;

	if (0 == size)
		size = strlen(src);
	local_buf_cnt = 0;
	for (i = 0, count = 0, ucs = 0, tc = 0; i < size; i++) {
		ch = (uint8_t)src[i];
		if (0x80 == (ch & 0x80)) {
			if (count > 0) {
				if (0x80 == (ch & 0xc0)) {
					ucs = (ucs << 6) | (ch & 0x3f);
#if	DEBUG_UNICODE
					fprintf(stderr,"UTF:%02x -> %04x (%d were left)\n",
						ch,ucs,count));
#endif
					if (--count == 0) {
						tc = ucs;
						if (tc < 0x80 || tc > 0xffff)
							tc = 0xfffd;
					}
				} else {
					fprintf(stderr,"UTF:%02x -> %04x (%d; invalid cont.)\n",
						ch,ucs,count);
					count = 0;
					tc = 0xfffd;
				}
			} else if (0xc0 == (ch & 0xe0)) {
				count = 1;
				ucs = ch & 0x1f;
#if	DEBUG_UNICODE
				fprintf(stderr,"UTF:%02x -> %04x (expect %d more)\n",
					ch,ucs,count);
#endif
			} else if (0xe0 == (ch & 0xf0)) {
				count = 2;
				ucs = ch & 0x0f;
#if	DEBUG_UNICODE
				fprintf(stderr,"UTF:%02x -> %04x (expect %d more)\n",
					ch,ucs,count);
#endif
			} else if (0xf0 == (ch & 0xf8)) {
				count = 3;
				ucs = ch & 0x07;
#if	DEBUG_UNICODE
				fprintf(stderr,"UTF:%02x -> %04x (expect %d more)\n",
					ch,ucs,count);
#endif
			} else if (0xf8 == (ch & 0xfc)) {
				count = 4;
				ucs = ch & 0x03;
#if	DEBUG_UNICODE
				fprintf(stderr,"UTF:%02x -> %04x (expect %d more)\n",
					ch,ucs,count);
#endif
			} else if (0xfc == (ch & 0xfe)) {
				count = 5;
				ucs = ch & 0x01;
#if	DEBUG_UNICODE
				fprintf(stderr,"UTF:%02x -> %04x (expect %d more)\n",
					ch,ucs,count);
#endif
			} else if (0xfc == ch) {
				count = 6;
				ucs = ch;
#if	DEBUG_UNICODE
				fprintf(stderr,"UTF:%02x -> %04x (expect %d more)\n",
					ch,ucs,count);
#endif
			} else {
				fprintf(stderr,"UTF:%02x -> %04x (%d; invalid lead in)\n",
					ch,ucs,count);
				count = 0;
				ucs = 0;
				tc = 0xfffd;
			}
		} else {
			if (count > 0) {
				fprintf(stderr,"UTF:%02x -> %04x (%d; missing cont.)\n",
					ch,ucs,count);
				count = 0;
				ucs = 0;
				tc = 0xfffd;
			} else {
				count = 0;
				ucs = ch;
				tc = ch;
#if	DEBUG_UNICODE
				fprintf(stderr,"UTF:%02x -> %04x (CS0 character)\n",
					ch,ucs);
#endif
			}
		}
		if (count > 0)
			continue;
		for (j = 0; iso8859_1[j].code; j++)
			if (tc == iso8859_1[j].ucs)
				break;
		if (iso8859_1[j].code) {
			LBPUT(iso8859_1[j].code);
		} else {
			/* FIXME: fallback character? */
			LBPUT('?');
		}
	}
	LBFLUSH();
	return res;	
}
