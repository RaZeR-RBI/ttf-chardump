#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <ft2build.h>
#include FT_FREETYPE_H

static void check_ft(FT_Error code)
{
	if (!code)
		return;
	printf("FreeType error (%d): %s\n", code, FT_Error_String(code));
	exit(code);
}

static void print_usage()
{
	printf("Dumps all printable Basic Multilingual Plane characters "
	       "defined in a TTF font in a UTF-8 form.\n");
	printf("Usage: ttf-chardump <path-to-font.ttf> [font face index]\n");
	printf("Examples:\n");
	printf("\tttf-chardump MyFont.ttf\n");
	printf("\tttf-chardump MyFont.ttf 2\n");
	exit(-1);
}

int main(int argc, char const *argv[])
{
	FT_Library library;
	FT_Face font;
	FT_ULong character;
	mbstate_t ps;
	int faceIndex = 0;
	char mbbuf[MB_LEN_MAX + 1] = {0};
	const char *locale = "C.UTF-8";

	if (argc < 2)
		print_usage();
	if (argc > 2)
		faceIndex = atoi(argv[2]);

	if (!setlocale(LC_ALL, locale)) {
		printf("Unable to set locale %s\n", locale);
		exit(-2);
	}
	check_ft(FT_Init_FreeType(&library));
	check_ft(FT_New_Face(library, argv[1], faceIndex, &font));
	check_ft(FT_Select_Charmap(font, ft_encoding_unicode));

	/* run through Basic Multilingual Plane */
	for (character = 0; character <= 0xFFFF; character++) {
		/* skip non-printable ASCII characters */
		if (character < 0xFF && !isprint(character))
			continue;
		/* skip UTF-16 BOM mark */
		if (character == 0xFEFF)
			continue;
		/* skip characters not in font */
		if (!FT_Get_Char_Index(font, character))
			continue;

		/* convert to UTF-8 and print */
		wcrtomb(NULL, L'\0', &ps);
		wcrtomb(mbbuf, character, &ps);
		printf("%s", mbbuf);
	}
	return 0;
}
