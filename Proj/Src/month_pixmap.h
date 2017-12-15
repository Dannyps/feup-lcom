/** Format of a xpm-like (there is a real xpm format) pic:

 * <pre>

 * static char *picture_name[] = {

 * "number_of_x_pixels number_of_y_pixels number_of_colors",

 * "one_char_symbol the_char_symbol_color",

 * ... exactly number_of_colors lines as above

 * "any of the above one_char_symbol, exactly number_of_x_pixels times",

 * ... exactly number_of_y_pixels lines as above

 * };

 *

 * Example:

 *

 * static char *pic1[] = {  // the name os the picture, "pic1" in this case

 * "32 13 4",               // number of pixels in the x and y dimension,

 *                          //   and the number of colors, which follows.

 * ". 0",                   // mapping between symbols and colors; in this

 *                          //   case, the "." will be mapped to color 0

 * "x 2",                   // and the "x" to color 2

 * ...                      // the next symbol/color pairs

 * "................................", // first row, exactly 32 pixels,

 *                                     // all color 0, BLACK

 * "..............xxx...............", // next row has three GREEN pixels

 *                                     // at the center

 * </pre>

 * Colors names/colors number equivalence are at c:/djgpp/include/conio.h

 *

 * Any real xpm picture can be read, as long as there are only 16

 * colors on it, and the colors are the ones available in conio.h.

 *

 *  To use other/more colors than the basic ones, the graphic card

 *  pallete must be programmed.

 *

 */

static char *pic1[] = {

  "32 13 4",

  ". 0",

  "x 2",

  "o 3",

  "+ 1",

  "................................",

  "..............xxx...............",

  "............xxxxxxx.............",

  ".........xxxxxx+xxxxxx..........",

  "......xxxxxxx+++++xxxxxxx.......",

  "....xxxxxxx+++++++++xxxxxxx.....",

  "....xxxxxxx+++++++++xxxxxxx.....",

  "......xxxxxxx+++++xxxxxxx.......",

  ".........xxxxxx+xxxxxx..........",

  "..........ooxxxxxxxoo...........",

  ".......ooo...........ooo........",

  ".....ooo...............ooo......",

  "...ooo...................ooo...."

};

// SLIGHT CHANGE TO ACCOMODATE FOR ONLY 5 COLOURS
// pixel_t colours[] = {black_c, red_c, green_c, blue_c, white_c};
// defined in video.h

static char *January[] = {
		"48 48 63 1",
		". DF7DCF",
		"X 965875",
		"o 71C671",
		"O A699A2",
		"+ 965892",
		"@ 8E3841",
		"# D75C7D",
		"$ F7DECF",
		"% 96588A",
		"& A69992",
		"* 8E3886",
		"= 104008",
		"- 596510",
		"; C71B30",
		": C71B9A",
		"> 618561",
		", 20811C",
		"< 104000",
		"1 861720",
		"2 DF7D4D",
		"3 79E769",
		"4 861782",
		"5 41033C",
		"6 000000",
		"7 49241C",
		"8 492445",
		"9 082008",
		"0 69A618",
		"q B6DA71",
		"w 410330",
		"e CF3CBA",
		"r 71C645",
		"t EFBEDB",
		"y 28A208",
		"u 186110",
		"i 596528",
		"p 71C661",
		"a A69996",
		"s 30C228",
		"d BEFBA2",
		"f 596545",
		"g 30C230",
		"h 8E3882",
		"j 208118",
		"k 38E30C",
		"l 30C220",
		"z 38E328",
		"x 514438",
		"c 618555",
		"v 30C220",
		"b 38E328",
		"n 28A228",
		"m 41032C",
		"M 104010",
		"N 492438",
		"B 28A220",
		"V A69959",
		"C 30C21C",
		"Z 30C218",
		"A 965865",
		"S 618534",
		"D 38E31C",
		"F 082000",
		"                                                ",
		"          .XoO                                  ",
		"         +@#$%o&                                ",
		"         *=-;#::o+                              ",
		"           >,<12#:34                            ",
		"             45671#:X3                          ",
		"               +89<02qwo                        ",
		"e*                >,67;ro                       ",
		"ty>                 459@>+&&                    ",
		"$2u+                  ><ipas8*                  ",
		"%$;=*                *3:.Xa.dfg>                ",
		"Oh$;ya             *3d.a8j,Xe.d3g8+             ",
		" Oh$;ka          *3d$a8lz,,xxc:.e3g54           ",
		"  Oh$;kO       *pd$%svbzz,sxxxxfX..&wn>         ",
		"   Oh$@mO    *3dthwlsslszjzxxxxxxx3:td8M4       ",
		"    Oh$@g& *3d$XNlvvvlllm,mNwxxxxxxxfa.:,B*     ",
		"     Oh$@,Od.czlllllzlmmqV@V#V@fxxxxxxxf:%j5&   ",
		"      Oh$1hd5lllslllCCZrV#r#:#2AxxxxxxxxxcdwM*  ",
		"       OXq6c.%8vvvllZZiqqApA:mq:Xxcpcxxxxxfdc9* ",
		"        2r<6gde3bllZZrVi7S@SV77A::qApxxxxxxfdcM ",
		"        :,q-6MN.dfmZZrrSS:#riirDSAX@Af5xxxxxfevo",
		"         +A26jguXtAZZZC7iDiCCrVVii7Cmmmxxxxxx%3g",
		"          *#16jszN..3DZZZZrCVSA2rZrV7Dmmwxxxx&en",
		"           p2yFvzssXe:fCZZCiiD7iiZDiDSSZwwxx8e*>",
		"           OA1<jzxwwc:$d%NDZZZZCCCZCCZZCmxxfd.B ",
		"            3206Bwxxszx%et.eaAp77m77mmmf3&eeeg* ",
		"             @26MvzxNzvlbwfpdettttttttttt.c,n&  ",
		"             *;16=lsNwwNwgsvslbwwvccc3pcfu<o    ",
		"              p;<69BvwwsszslllbBlllllllu<5+     ",
		"              OS0y6FBlvvvzvzss,u=Blllj=54       ",
		"               c1-699Blvlllllu7k96MMMg4         ",
		"               *10y8n6FjvllllB<166668           ",
		"                S-kg+>666<M<996-y6n<8*          ",
		"                p71=4 m69996kD8Z-66698&&        ",
		"                &i0ycm6n4 ogk17,0<6666g         ",
		"                 N-k-<>     >=01-kuu666>        ",
		"                 ,6ky&      &46-10ul,66,        ",
		"                 Ou0<>       o66y<ulw<66&       ",
		"                  *kk5       >66By7=xu664       ",
		"                   <<M4      466lj<Mxu66o       ",
		"                   *>>       +66uv,zN666*       ",
		"                              566,xxj669        ",
		"                              4666FF666>        ",
		"                               >966666M         ",
		"                                oM6668+         ",
		"                                  *4            ",
		"                                                ",
		"                                                "
};
