#include "vpss_alg_rwzb.h" 

#define ABS(x) (((x) < 0) ? -(x) : (x))

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif
/*notes:
*1.y must be 16X
*2.top/bottom pixel is same
*/
DET_SAMP_S tab_cbars100 =
{
    PAT_CBARS100,5,576,720, 
    {
        {0,45*8,2*8, { 126, 110, 107, 105, 106, 106, 105, 106  }},
        {0,12*8,6*8,{  234, 235, 235, 235, 234, 232, 223, 213    }},
        {0,77*8,4*8,{   41, 38, 28, 19, 16, 16, 16, 16   }},
        {0,56*8,26*8,{  82, 81, 81, 81, 81, 81, 81, 81   }},
        {0,12*8,70*8,{ 234, 235, 235, 235, 234, 232, 223, 213  }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S tab_its1_ntsc = 
{
    PAT_ITS1_NTSC,5,480,720, 
    {
        {0,47*8,2*8, { 116, 119, 122, 124, 126, 125, 123, 120  }},
        {0,45*8,4*8,{  17,  19,  21,  25,  30,  37,  42,  49  }},
        {0,41*8,10*8,{ 16,  15,  24,  98,  195, 234, 186, 89  }},
        {0,49*8,4*8,{  60,  52,  45,  38,  32,  26,  22,  19  }},
        {0,61*8,8*8,{  16,  16,  16,  16,  20,  29,  40,  49  }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S tab_ccitt033_ntsc =
{
    PAT_CCITT033_NTSC,5,480,720,
    {
        {0,56*8,10*8, { 71, 79, 138, 221, 221, 138, 79, 71   }},
        {0,83*8,18*8,{  73, 94, 133, 179, 216, 233, 236, 235   }},
        {0,61*8,20*8,{  49, 49, 47, 42, 34, 26, 19, 15  }},
        {0,66*8,42*8,{  234, 236, 235, 235, 227, 201, 158, 114  }},
        {0,26*8,42*8,{  182, 215, 234, 236, 235, 235, 236, 235  }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S tab_cbar704_576 = 
{
    PAT_CBAR704_576,5,576,704,
    {
        {0,11*8,7*8,  {231, 205, 174, 162, 161, 161, 161, 160  }},
        {0,66*8,9*8,  {65,  58,  44,  34,  35,  35,  34,  35   }},
        {0,44*8,33*8, {113, 108, 97,  87,  84,  84,  83,  84   }},
        {0,22*8,65*8, {161, 152, 139, 132, 131, 131, 131, 131  }},
        {0,55*8,65*8, { 84, 84,  84,  81,  74,  67,  65,  65  ,}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S tab_matrx525 = 
{
    PAT_MATRX525,5,480,704,
    {
        {0,10*8,5*8,  { 235, 233, 220, 196, 173, 162, 161, 161   }},
        {0,27*8,24*8, { 85,  74,  71,  81,  101, 125, 150, 169   }},
        {0,70*8,33*8, { 129, 38,  130, 214, 175, 173, 195, 175   }},
        {0,56*8,22*8, { 85,  153, 135, 107, 129, 128, 124, 125    }},
        {0,71*8,44*8, { 95,  95,  98,  105, 115, 125, 132, 134  ,}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};


DET_SAMP_S tab_codec43 = 
{
    PAT_CODEC43,5,480,720,
    {
        {0,24*8,30*8, {125,  125,  125,  124,  116, 93, 62, 35  }},
        {0,18*8,37*8, {221, 139, 79,  70,  71,  75,  85,  100  }},
        {0,45*8,52*8, {124, 109, 126, 201, 229, 162, 65,  16   }},
        {0,85*8,44*8, {70, 79, 138, 222, 222, 138, 79, 70   }},
        {0,17*8,54*8, {69,  75,  55,  76,  76,  37,  120, 213 ,}},
    }
};
DET_SAMP_S  tab_ccitt033 =
{
    PAT_CCITT033,5,576,720,
    {
        {0,27*8,2*8,{16,57,167,234,167,57,16,15}},
        //{0,31*8,16*8,{185,226,233,215,228,224,139,80}},
        {0,18*8,15*8,{221,220,138,79,70,69,70,69}},
        {0,35*8,2*8,{32,37,43,49,55,60,67,74}},
        //{0,35*8,41*8,{59,82,132,178,188,154,99,63}},
        {0,34*8,13*8,{68,68,70,68,70,79,139,222}},
        {0,15*8,22*8,{79,69,70,69,70,70,70,70}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_ccitt18 =
{
    PAT_CCITT18,5,576,720,
    {
        {0,17*8,12*8,{62,71,92,114,124,125,126,125}},
        {0,52*8,8*8,{189,178,126,74,68,111,135,128}},
        {0,61*8,11*8,{60,143,181,76,98,190,115,66}},
        {0,82*8,10*8,{160,70,189,63,173,101,121,157}},
        {0,52*8,48*8,{189,178,126,74,68,111,135,128}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0, 0 }},
    }
};

DET_SAMP_S  tab_M576i =
{
    PAT_M576i,5,576,720, 
    {
        {0,17*8,5*8,{16,16,16,50,169,235,169,50}},
        {0,58*8,21*8,{197,189,182,176,172,170,169,169}},
        {0,25*8,29*8,{125,106,89,79,78,85,100,121}},
        {0,25*8,42*8,{65,65,54,54,86,86,100,100}},
        {0,51*8,51*8,{81,79,79,77,77,75,75,74}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0, 0 }},
    }
};

DET_SAMP_S  tab_M480p16x9 =
{
    PAT_480,3,480,704,
    {
        {0,16*8,4*8,{17,17,23,94,191,235,191,94}},
        {0,57*8,14*8,{195,189,184,178,174,171,170,169}},
        {0,19*8,17*8,{141,136,132,125,118,110,104,96}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0, 0 }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0, 0 }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0, 0 }},
    }
};

DET_SAMP_S  tab_M720p50 =
{
    PAT_720P50,5,720,1280,
    {
        {0,30*8,9*8,{235,158,35,16,16,16,16,16}},
        {0,54*8,21*8,{132,210,228,177,98,53,64,110}},
        {0,98*8,23*8,{42,206,190,30,85,232,141,16}},
        {0,34*8,25*8,{16,16,16,35,158,235,158,35}},
        {0,51*8,63*8,{229,201,147,85,37,18,16,16}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0, 0 }},
    }
};

DET_SAMP_S  tab_M720p59 =
{
    PAT_720P59,5,720,1280,
    {
        {0,30*8,9*8,{235,158,36,17,16,17,17,16}},
        {0,54*8,21*8,{132,210,228,177,98,53,64,110}},
        {0,98*8,23*8,{42,206,190,30,85,232,141,16}},
        {0,34*8,25*8,{16,16,16,35,158,235,158,35}},
        {0,51*8,63*8,{236,217,154,72,24,17,17,16}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0, 0 }},
    }
};

DET_SAMP_S  tab_M1080 =
{
    PAT_1080,5,1080,1920,
    {
        {0,45*8,9*8,{158,235,158,035,016,016,016,016}},
        {0,154*8,15*8,{235,73,76,234,53,98,228,36}},
        {0,76*8,34*8,{16,16,16,18,27,48,79,117}},
        {0,152*8,40*8,{110,169,191,154,93,60,85,146}},
        {0,62*8,77*8,{26,26,82,82,172,172,63,64}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0, 0 }},
    }
};

DET_SAMP_S  tab_CBAR576_75 =
{
    PAT_CBAR576_75,5,576,720,
    {
        {0,12*8,1*8,{235,235,235,235,235,227,198,167}},
        {0,23*8,6*8,{162,162,161,158,146,134,131,131}},
        {0,34*8,8*8,{131,129,122,114,112,112,112,112}},
        {0,45*8,10*8,{101,87,84,84,84,84,84,84}},
        {0,55*8,17*8,{83,84,84,84,84,81,73,67}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0, 0 }},
    }
};

DET_SAMP_S  tab_ccir3311 =
{
    PAT_CCIR3311,5,576,720,
    {
        {0,3*8, 8*8,{16, 16, 16, 16, 18, 36, 71, 106 }},
        {0,4*8, 64*8,{123, 125, 125, 126, 125, 125, 125, 125}},
        {0,48*8,32*8,{125, 125, 125, 125, 125, 125, 125, 125}},
        {0,87*8,16*8,{125, 125, 125, 125, 126, 125, 125, 123}},
        {0,88*8,48*8,{106, 71, 35, 18, 17, 16, 16, 16}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_matrix625 =
{
    PAT_MATRIX625,6,576,704,
    {
        {0,21*8,32*8,{52, 61, 45, 67, 39, 73, 34, 76}},
        {0, 0*8,56*8,{26, 34, 44, 54, 62, 68, 71, 72}},
        {0,32*8,64*8,{33, 43, 55, 67, 80, 92, 103, 112}},
        {0,34*8, 4*8,{33, 38, 43, 49, 55, 61, 67, 74}},
        {0,56*8,20*8,{190, 117, 65, 168, 161, 62, 126, 188}},
        {0,87*8,64*8,{196, 126, 54, 20, 16, 16, 16, 16}},
    }
};

DET_SAMP_S  tab_CBAR576_75_B =
{
    PAT_CBAR576_75_B,5,576,720,
    {
        {0,1*8,2*8,{ 20, 56, 125, 197, 231, 235, 236, 235 }},
        {0,12*8,13*8,{ 232, 223, 198, 174, 163, 161, 161, 160 }},
        {0,23*8,26*8,{ 157, 146, 136, 131, 131, 132, 131, 132 }},
        {0,45*8,33*8,{ 108, 98, 90, 84, 83, 84, 83, 83 }},
        {0,67*8,66*8,{ 50, 40, 35, 35, 34, 34, 34, 34 }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_CBAR576_100 =
{
    PAT_CBAR576_100,5,576,720,
    {
        {0,1*8,71*8,{20, 56, 126, 198, 232, 235, 236, 237}},
        {0,23*8,27*8,{ 203, 189, 176, 170, 169, 169, 170, 169}},
        {0,34*8,35*8,{ 165, 157, 149, 145, 144, 144, 144, 143 }},
        {0,45*8,39*8,{ 138, 124, 113, 107, 105, 106, 106, 106 }},
        {0,56*8,50*8,{ 94, 86, 82, 80, 80, 81, 80, 80 }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_M576i_pulsebar =
{
    PAT_M576I_PULSEBAR,5,576,720,
    {
        {0,8*8,11*8,{ 168, 236, 168, 51, 16, 16, 17, 16 }},
        {0,15*8,24*8,{ 18, 34, 61, 92, 117, 125, 117, 92 }},
        {0,22*8,17*8,{ 43, 56, 66, 80, 92, 104, 112, 119 }},
        {0,24*8,23*8,{ 67, 54, 43, 33, 25, 19, 16, 16 }},
        {0,31*8,66*8,{ 55, 124, 195, 231, 235, 235, 235, 235 }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }   
};

DET_SAMP_S  tab_M576i_redfield =
{
    PAT_M576I_REDFIELD,5,576,720,
    {
        {0,8*8,11*8,{ 65, 65, 65, 65, 65, 65, 65, 65}},
        {0,71*8,49*8,{ 65, 65, 65, 65, 65, 65, 65, 65}},
        {0,59*8,7*8,{ 65, 65, 65, 65, 65, 65, 65, 65}},
        {0,29*8,53*8,{ 65, 65, 65, 65, 65, 65, 65, 65}},
        {0,56*8,62*8,{ 65, 65, 65, 65, 65, 65, 65, 65}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_M576i_duoboqun =
{
    PAT_M576I_DUOBOQUN,5,576,720,
    {
        {0,5*8,5*8,{ 191, 191, 191, 188, 168, 125, 83, 62 }},
        {0,10*8,29*8,{59, 60, 59, 61, 71, 92, 113, 124}},
        {0,13*8,51*8,{126, 127, 128, 120, 104, 89, 77, 68 }},
        {0,78*8,42*8,{ 61, 189, 75, 151, 129, 92, 180, 59 }},
        {0,45*8,69*8,{ 77, 60, 96, 155, 190, 174, 117, 68}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_M576i_5taijie =
{
    PAT_M576I_5TAIJIE,5,576,720,
    {
        {0,22*8,5*8,{ 15, 16, 16, 16, 16, 23, 38, 52}},
        {0,33*8,26*8,{ 60, 60, 60, 60, 61, 60, 67, 83 }},
        {0,44*8,28*8,{ 104, 104, 104, 104, 105, 104, 111, 127 }},
        {0,56*8,63*8,{170, 184, 191, 192, 192, 192, 192, 192}},
        {0,78*8,47*8,{196, 127, 56, 20, 17, 16, 16, 16 }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_M576i_xiebo =
{
    PAT_M576I_XIEBO,5,576,720,
    {
        {0,19*8,48*8,{ 19, 21, 20, 21, 21, 22, 21, 23 }},
        {0,31*8,27*8,{ 67, 69, 68, 69, 69, 70, 69, 71 }},
        {0,43*8,32*8,{115, 117, 116, 117, 117, 118, 117, 119 }},
        {0,59*8,31*8,{ 179, 181, 180, 181, 181, 182, 181, 183 }},
        {0,70*8,9*8,{223, 225, 224, 225, 225, 226, 225, 227}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_M576i_bowtie =
{
    PAT_M576I_BOWTIE,5,576,720, 
    {
        {0,4*8,3*8,{ 104, 93, 82, 77, 73, 70, 72, 77 }},
        {0,40*8,10*8,{181, 180, 178, 171, 164, 153, 142, 128 }},
        {0,11*8,32*8,{16, 44, 98, 126, 98, 43, 16, 16 }},
        {0,44*8,50*8,{16, 15, 18, 15, 17, 15, 16, 52}},
        {0,86*8,65*8,{ 83, 92, 102, 116, 127, 140, 152, 163 }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_NTSC480i_moto =
{
    PAT_M480I_NTSC,5,480,720, 
    {
        {0,34*8,3*8,{188,172,146,116,88,67,60,66}},
        {0,17*8,16*8,{164,187,183,151,110,69,61,81}},
        {0,35*8,23*8,{235,235,222,125,29,16,16,16}},
        {0,42*8,23*8,{190,235,191,94,22,16,16,16}},
        {0,30*8,31*8,{109,109,112,126,140,143,143,143}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_NTSC480i_moto_matrix =
{
    PAT_M480I_NTSC_MATRIX,5,480,720, 
    {
        {0,34*8,3*8,{188,172,146,116,88,67,60,66}},
        {0,17*8,16*8,{164,187,183,151,110,69,61,81}},
        {0,35*8,23*8,{ 235, 235, 221, 118, 16, 1, 1, 1 }},
        {0,42*8,23*8,{ 187, 235, 188, 84, 8, 1, 1, 1 }},
        {0,30*8,31*8,{109,109,112,126,140,143,143,143}},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_NTSC480i_moto_NTC7_COMBINATION =
{
    PAT_M480I_NTC7_COMBINATION,5,480,704, 
    {
        {0,10*8,7*8,{ 235, 235, 235, 235, 229, 210, 180, 152 }},
        {0,14*8,17*8,{ 125, 126, 137, 150, 161, 170, 176, 180 }},
        {0,16*8,36*8, { 107, 96, 86, 79, 73, 71, 72, 75 }},
        {0,42*8,44*8,{ 107, 73, 126, 170, 138, 102, 114, 132 }},
        {0,55*8,46*8, { 173, 83, 111, 180, 100, 91, 177, 122 }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_NTSC480i_moto_NTC7_COMPOSITE =
{
    PAT_M480I_NTC7_COMPOSITE,5,480,704, 
    {
        {0,4*8,7*8,{ 126, 222, 235, 235, 235, 235, 235, 236 }},
        {0,34*8,9*8,{ 235, 235, 222, 126, 29, 16, 16, 16 }},
        {0,41*8,27*8, { 223, 223, 145, 50, 16, 16, 16, 16 }},
        {0,44*8,33*8,{ 22, 26, 31, 37, 44, 51, 59, 67 }},
        {0,66*8,51*8, { 56, 56, 61, 70, 81, 90, 94, 95 }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_NTSC480i_moto_TSG130A_MATRIX_10MBPS =
{
    PAT_M480I_TSG130A_MATRIX_10MBPS,5,480,704, 
    {
        {0,10*8,10*8,{ 235, 233, 220, 196, 173, 162, 161, 160 }},
        {0,19*8,22*8, { 144, 132, 119, 106, 95, 85, 78, 73 }},
        {0,32*8,29*8, { 176, 172, 132, 85, 72, 100, 150, 179 }},
        {0,70*8,30*8, { 129, 38, 130, 214, 175, 173, 195, 175 }},
        {0,81*8,44*8, { 174, 174, 174, 177, 183, 193, 204, 211 }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};

DET_SAMP_S  tab_NTSC480i_moto_75_COLORBARS =
{
    PAT_M480I_75_COLORBARS,5,480,704, 
    {
        {0,10*8,26*8,{  235, 233 ,220, 196, 174 ,163 ,162, 162  }},
        {0,43*8,26*8, {  113, 113 ,113, 113, 113, 110 ,103, 93  }},
        {0,54*8,5*8, {  85, 84 ,84 ,84, 84 ,84 ,83 ,78  }},
        {0,66*8,42*8, {  58 ,48 ,39, 35 ,35, 35, 35, 35  }},
        {0,77*8,35*8, { 34, 31 ,25, 19, 16, 16 ,16, 16  }},
        {0,0,0,{ 0, 0, 0, 0, 0, 0, 0,  0 }},
    }
};
DET_SAMP_S *SamPool[]=
{
    
    &tab_cbars100,&tab_its1_ntsc,&tab_ccitt033_ntsc,&tab_ccitt18,&tab_M576i,
    &tab_M480p16x9,&tab_M720p50,&tab_M720p59,&tab_M1080,&tab_CBAR576_75,
    &tab_ccir3311,&tab_matrix625,&tab_CBAR576_75_B,&tab_CBAR576_100,&tab_M576i_pulsebar,
    &tab_M576i_redfield,&tab_M576i_duoboqun,&tab_ccitt033,&tab_M576i_5taijie,
    &tab_M576i_xiebo,&tab_M576i_bowtie,&tab_ccitt033,&tab_cbar704_576,
    &tab_matrx525,&tab_codec43,&tab_NTSC480i_moto, &tab_NTSC480i_moto_matrix,
    &tab_NTSC480i_moto_NTC7_COMBINATION, &tab_NTSC480i_moto_NTC7_COMPOSITE, &tab_NTSC480i_moto_TSG130A_MATRIX_10MBPS,&tab_NTSC480i_moto_75_COLORBARS,
    NULL,NULL,NULL,NULL,NULL,
    
    
};

HI_S32 Init_RWZB(DET_INFO_S *pDetInfo, HI_U32 height, HI_U32 width)
{
    HI_S32 i;
    DET_SAMPNODE_S *pCurrent;
    LIST *pos, *n;
    for (i = 0; SamPool[i] != NULL; i++)
    {
        if (SamPool[i]->height == height
            && SamPool[i]->width == width)
        {
            pCurrent = (DET_SAMPNODE_S*)VPSS_VMALLOC(sizeof(DET_SAMPNODE_S));
            if (pCurrent != HI_NULL)
            {
                pCurrent->pSamp = SamPool[i];
                list_add_tail(&(pCurrent->list), &(pDetInfo->nowRWZB));
            }
            else
            {
                goto Init_RWZB_Failed;
            }
        }
    }
  
    pDetInfo->target = (pDetInfo->nowRWZB).next;
    
    return HI_SUCCESS;
    
Init_RWZB_Failed:
    list_for_each_safe(pos, n, &(pDetInfo->nowRWZB))
    {
        pCurrent = list_entry(pos, DET_SAMPNODE_S, list);
        list_del_init(pos);
        VPSS_VFREE(pCurrent);
    }
    pDetInfo->target = (pDetInfo->nowRWZB).next;
    pDetInfo->btFlag = 1;
    pDetInfo->isRWZB = 0;
    return HI_FAILURE;
}

HI_S32 Free_RWZB(DET_INFO_S *pDetInfo)
{
    DET_SAMPNODE_S *pCurrent;
    LIST *pos, *n;
    
    list_for_each_safe(pos, n, &(pDetInfo->nowRWZB))
    {
        pCurrent = list_entry(pos, DET_SAMPNODE_S, list);
        list_del_init(pos);
        VPSS_VFREE(pCurrent);
    }

    return 1;
}


HI_VOID DetPic_Config_Addr(DET_INFO_S *pDetInfo,VPSS_RWZB_INFO_S *pstRwzbCfg)
{
    LIST *pCurrent;
    HI_S32 patCount;
    DET_SAMP_S *pRefSamp;
    //HI_S32 addr[2][2];

    pCurrent = pDetInfo->target;
    
    if (pCurrent != &(pDetInfo->nowRWZB))
    {
        pRefSamp = (list_entry(pCurrent,DET_SAMPNODE_S,list))->pSamp;

        for(patCount = 0; patCount < MAXSAMPNUM; patCount++)
        {
            pstRwzbCfg->u32Addr[patCount][0] = pRefSamp->sample[patCount].Px / 8;
            pstRwzbCfg->u32Addr[patCount][1] = pRefSamp->sample[patCount].Py / 8;

            
            VPSS_INFO("\n DetAddr[%d][0]=%d DetAddr[%d][1]=%d",patCount,
                            pstRwzbCfg->u32Addr[patCount][0],patCount,
                            pstRwzbCfg->u32Addr[patCount][1]);
                    
        }
    }
}

HI_VOID DetPic_Compare_Pattern(DET_INFO_S *pDetInfo,VPSS_RWZB_INFO_S *pstRwzbCfg)
{
    LIST *pCurrent;
    DET_PAT_S *pRefPat;
    DET_SAMP_S *pRefSamp;
    HI_S32 pixCount;
    HI_S32 patCount;
    HI_S32 MisCnt = 0;
    HI_U8 pix;
    HI_U8 data[8];
    
    pCurrent = pDetInfo->target;
    
    if (pCurrent != &(pDetInfo->nowRWZB))
    {
        pRefSamp = (list_entry(pCurrent,DET_SAMPNODE_S,list))->pSamp;
    
        for (patCount = 0; patCount < MAXSAMPNUM; patCount++)
        {
            pRefPat = &(pRefSamp->sample[patCount]);

            memcpy(data,&(pstRwzbCfg->u8Data[patCount][0]),sizeof(HI_U8)*PIX_NUM_IN_PATTERN);
            
            for (pixCount = 0; pixCount < PIX_NUM_IN_PATTERN
                               && ((pRefPat->Px != 0)||(pRefPat->Py != 0)); 
                 pixCount++)
            {
                pix = data[pixCount];
                MisCnt += ABS(pRefPat->Pix[pixCount] - pix)>PIX_MISMATCH_THR;
            }
        }

        if (MisCnt > PAT_MISMATCH_THR)
        {
            pDetInfo->target = (pDetInfo->target)->next;
            
            if (pDetInfo->target == &(pDetInfo->nowRWZB))
            {
                pDetInfo->target = (pDetInfo->nowRWZB).next;
            }
            
            pDetInfo->isRWZB = 0;
        }
        else
        {
            VPSS_INFO("\n Rwzb=%d",pRefSamp->type);
            pDetInfo->isRWZB = pRefSamp->type;
        }
    }
}

HI_S32 ALG_DetPic(DET_INFO_S *pDetInfo,
                     VPSS_RWZB_INFO_S *pstRwzbCfg)
{
    LIST *nowList;
    DET_SAMP_S *pRefSamp;
    HI_U32 nowHeight;
    HI_U32 nowWidth;
    HI_U32 u32Height;
    HI_U32 u32Width;
    HI_S32 s32Ret;
    
    nowList = &(pDetInfo->nowRWZB);
    
    u32Height = pstRwzbCfg->u32Height;
    u32Width = pstRwzbCfg->u32Width;
    
    if (nowList->next != nowList)
    {
        pRefSamp = (list_entry(nowList->next,DET_SAMPNODE_S,list))->pSamp;
        nowHeight = pRefSamp->height;
        nowWidth= pRefSamp->width;
    }
    else
    {
        nowHeight = 0;
        nowWidth= 0;
    }

    if (nowHeight == u32Height && nowWidth == u32Width)
    {
        DetPic_Compare_Pattern(pDetInfo,pstRwzbCfg);
        DetPic_Config_Addr(pDetInfo,pstRwzbCfg);
    }
    else
    {
        Free_RWZB(pDetInfo);
        s32Ret = Init_RWZB(pDetInfo, u32Height, u32Width);
        if (s32Ret == HI_SUCCESS)
        {
            DetPic_Config_Addr(pDetInfo,pstRwzbCfg);
        }
        else
        {
            pDetInfo->btFlag = 1;
            pDetInfo->isRWZB = 0;
            INIT_LIST_HEAD(&(pDetInfo->nowRWZB));
            pDetInfo->target = (pDetInfo->nowRWZB).next;
        }
    }
    VPSS_INFO("<---------------------------------------->func %s line %d pDetInfo->isRWZB %d \r\n", __func__, __LINE__, pDetInfo->isRWZB);

    return HI_SUCCESS;
}

HI_S32 ALG_InitRwzbInfo(DET_INFO_S *pDetInfo)
{
    pDetInfo->btFlag = 1;
    pDetInfo->isRWZB = 0;
    INIT_LIST_HEAD(&(pDetInfo->nowRWZB));
    pDetInfo->target = (pDetInfo->nowRWZB).next;
    //Vou_SetViDetEn(HAL_DISP_LAYER_VIDEO1, 1);

    return HI_SUCCESS;

}

HI_S32 ALG_DeInitRwzbInfo(DET_INFO_S *pDetInfo)
{
    pDetInfo->target = NULL;
    pDetInfo->btFlag = 1;
    pDetInfo->isRWZB = 0;
    Free_RWZB(pDetInfo);
    //Vou_SetViDetEn(HAL_DISP_LAYER_VIDEO1, 0);

    return HI_SUCCESS;
}


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */


