{TEST10.PAS
Tests basic integer arithmetic
}

{Create some simple variables}
var itest : integer; 

{ First simple assignment }

itest := 1                      { Integer }

{ Basic integer arithmetic }

var itest1  : integer;
var itest2  : integer;
var itest3  : integer;
var itest4  : integer;
var itest5  : integer;
var itest6  : integer;
var itest7  : integer;
var itest8  : integer;
var itest9  : integer;
var itest10 : integer;
var itest11 : integer;
var itest12 : integer;
var itest13 : integer;
var itest14 : integer;
var itest15 : integer;
var itest16 : integer;
var itest17 : integer;
var itest18 : integer;
var itest19 : integer;
var itest20 : integer;
var itest21 : integer;
var itest22 : integer;

itest1  := not 3         {-4}     { non-standard bitwise NOT }

itest2  := 3 * 4         { 12 }   { integer multiply }
itest3  := 10 div 3      { 3  }   { integer division }
itest4  := 10 mod 3      { 1 }    { modulus }
itest5  := 5 and 3       { 1 }    { non-standard bitwise AND }

itest6  := 4 + 5         { 9 }    { integer addition }
itest7  := 7 - 5         { 2 }    { integer subtraction }
itest8  := 3 or 8        {11 }    { non-standard bitwise OR }


itest9  := 1 + 2         { 3 }
itest10 := 3 * 4         { 12 }
itest11 := 1 + 2 * 4     { 9 }
itest12 := 5 * 6 + 7     { 37 }
itest13 := 10 div 2      { 5 }
itest14 := 11 mod 2      { 1 }
itest15 := (1 + 2) * 4   { 12 }
itest16 := 5 * (6 + 7)   { 65 }
itest17 := -1            { -1 }
itest18 := -2 * 5        {-10 }
itest19 := -2 * (-5)     { 10 }
itest20 := 1 + 2 + 3     { 6 }
itest21 := 7 - 2 - 1     { 4 }
itest22 := 20 div 2 div 3{ 3 }


