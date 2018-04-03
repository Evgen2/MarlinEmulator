

#if (THERMISTORHEATER_0 == 150) || (THERMISTORHEATER_1 == 150)  || (THERMISTORHEATER_2 == 150) || (THERMISTORBED == 150) //100k bed thermistor

const short temptable_150[][2] PROGMEM = {
{       35*OVERSAMPLENR ,       258     },
{       41*OVERSAMPLENR ,       250     },
{       48*OVERSAMPLENR ,       241     },
{       56*OVERSAMPLENR ,       233     },
{       66*OVERSAMPLENR ,       225     },
{       78*OVERSAMPLENR ,       216     },
{       92*OVERSAMPLENR ,       210     },
{       109*OVERSAMPLENR        ,       200     },
{       131*OVERSAMPLENR        ,       190     },
{       156*OVERSAMPLENR        ,       180     },
{       187*OVERSAMPLENR        ,       170     },
{       224*OVERSAMPLENR        ,       160     },
{       379*OVERSAMPLENR        ,       130     },
{       591*OVERSAMPLENR        ,       100     },
{       665*OVERSAMPLENR        ,       90      },
{       737*OVERSAMPLENR        ,       80      },
{       801*OVERSAMPLENR        ,       70      },
{       857*OVERSAMPLENR        ,       60      },
{       939*OVERSAMPLENR        ,       40      },
{       966*OVERSAMPLENR        ,       30      },
{       1008*OVERSAMPLENR       ,       0       } //safety
};
#endif

#if (THERMISTORHEATER_0 == 160) || (THERMISTORHEATER_1 == 160)  || (THERMISTORHEATER_2 == 160) || (THERMISTORBED == 160) //100k bed thermistor

const short temptable_160[][2] PROGMEM = {
{       35*OVERSAMPLENR ,       268     },
{       41*OVERSAMPLENR ,       259     },
{       48*OVERSAMPLENR ,       247     },
{       56*OVERSAMPLENR ,       241     },
{       66*OVERSAMPLENR ,       232     },
{       78*OVERSAMPLENR ,       222     },
{       92*OVERSAMPLENR ,       213     },
{       109*OVERSAMPLENR        ,       204     },
{       131*OVERSAMPLENR        ,       194     },
{       156*OVERSAMPLENR        ,       184     },
{       187*OVERSAMPLENR        ,       173     },
{       224*OVERSAMPLENR        ,       164     },
{       379*OVERSAMPLENR        ,       133     },
{       591*OVERSAMPLENR        ,       102     },
{       665*OVERSAMPLENR        ,       92      },
{       737*OVERSAMPLENR        ,       81      },
{       801*OVERSAMPLENR        ,       71      },
{       857*OVERSAMPLENR        ,       61      },
{       939*OVERSAMPLENR 		,       32      },
{       999*OVERSAMPLENR        ,       10      },
{       1008*OVERSAMPLENR       ,       0       } //safety
};
#endif

#if (THERMISTORHEATER_0 == 111) || (THERMISTORHEATER_1 == 111)  || (THERMISTORHEATER_2 == 111) || (THERMISTORBED == 111) //100k bed thermistor

const short temptable_111[][2] PROGMEM = {
{       31*OVERSAMPLENR ,       256     },
{       33*OVERSAMPLENR ,       250     },
{       35*OVERSAMPLENR ,       247     },
{       38*OVERSAMPLENR ,       241     },
{       41*OVERSAMPLENR ,       236     },
{       44*OVERSAMPLENR ,       232     },
{       48*OVERSAMPLENR ,       226     },
{       52*OVERSAMPLENR ,       221     },
{       56*OVERSAMPLENR ,       219     },
{       61*OVERSAMPLENR ,       214     },
{       66*OVERSAMPLENR ,       206     },
{       92*OVERSAMPLENR ,       193     },
{       109*OVERSAMPLENR        ,       182     },
{       156*OVERSAMPLENR        ,       168     },
{       224*OVERSAMPLENR        ,       146     },
{       268*OVERSAMPLENR        ,       135     },
{       516*OVERSAMPLENR        ,       100     },
{       801*OVERSAMPLENR        ,       65      },
{       966*OVERSAMPLENR        ,       30      },
{       999*OVERSAMPLENR        ,       10      },
{       1008*OVERSAMPLENR       ,       0       } //safety
};
#endif

#if (THERMISTORHEATER_0 == 214) || (THERMISTORHEATER_1 == 214)  || (THERMISTORHEATER_2 == 214) || (THERMISTORBED == 214) //100k bed thermistor

const short temptable_214[][2] PROGMEM = {
{       31*OVERSAMPLENR ,       280     },
{       35*OVERSAMPLENR ,       276     },
{       44*OVERSAMPLENR ,       258     },
{       48*OVERSAMPLENR ,       250     },
{       52*OVERSAMPLENR ,       245     },
{       61*OVERSAMPLENR ,       234     },
{       66*OVERSAMPLENR ,       232     },
{       92*OVERSAMPLENR ,       214     },
{       109*OVERSAMPLENR        ,       204     },
{       156*OVERSAMPLENR        ,       183     },
{       224*OVERSAMPLENR        ,       154    },
{       268*OVERSAMPLENR        ,       135     },
{       516*OVERSAMPLENR        ,       114     },
{       801*OVERSAMPLENR        ,       72      },
{       966*OVERSAMPLENR        ,       30      },
{       999*OVERSAMPLENR        ,       10      },
{       1008*OVERSAMPLENR       ,       0       } //safety
};
#endif


