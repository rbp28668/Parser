{
TEST12.PAS
Boolean expressions
}

var btest : boolean;

var btest1  : boolean;
var btest2  : boolean;
var btest3  : boolean;
var btest4  : boolean;
var btest5  : boolean;
var btest6  : boolean;
var btest7  : boolean;
var btest8  : boolean;
var btest9  : boolean;
var btest10 : boolean;
var btest11 : boolean;
var btest12 : boolean;
var btest13 : boolean;
var btest14 : boolean;
var btest15 : boolean;
var btest16 : boolean;
var btest17 : boolean;
var btest18 : boolean;
var btest19 : boolean;
var btest20 : boolean;
var btest21 : boolean;
var btest22 : boolean;
var btest23 : boolean;
var btest24 : boolean;
var btest25 : boolean;


btest := true                        {T}
btest1  := not true                  {F}
btest2  := true and false            {F}
btest3  := true or false             {T}
btest4  := 1 < 2                     {T}
btest5  := 1 <= 2                    {T}
btest6  := 1 = 2                     {F}
btest7  := 1 <> 2                    {T}
btest8  := 1 > 2                     {F}
btest9  := 1 >= 2                    {F}
btest10 := 1 < 2                     {T}
btest11 := (1 < 2) and (3 < 4)       {T}
btest12 := (1 < 2) or (3 < 4)        {T}
btest13 := not true                  {F}

{ Real comparison }
btest14 := 1.1 < 2.2                 {T}
btest15 := 1.1 <= 2.2                {T}
btest16 := 1.1 = 2.2                 {F}
btest17 := 1.1 <> 2.2                {T}
btest18 := 1.1 > 2.2                 {F}
btest19 := 1.1 >= 2.2                {F}

type colour = (red,green,blue);
btest20 := red = red                 {T}
btest21 := red <> red                {F}
btest22 := red = green               {F}
btest23 := red <> green              {T}
btest24 := red < blue                {T}
btest25 := red > blue                {F}



