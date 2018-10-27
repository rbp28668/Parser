{TEST13.PAS
Test mixed mode arithmetic and comparisons
}

var rtest1  : real;
var rtest2  : real;
var rtest3  : real;
var rtest4  : real;
var rtest5  : real;
var rtest6  : real;
var rtest7  : real;
var rtest8  : real;
var rtest9  : real;

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


{ Mixed real and integer arithmetic }
rtest1  := 3.1 * 5      { 15.5 }
rtest2  := 3.3 / 2      { 1.65 }
rtest3  := 8.9 + 3      { 11.9 }
rtest4  := 3.69 - 1     { 2.69 }

rtest5  := 3 * 5.2      { 15.6 }
rtest6  := 3 / 2.6      { 1.154 }
rtest7  := 8 + 3.6      { 11.6 }
rtest8  := 3 - 1.45     { 1.55 }

{ Mixed integer and real comparison }
btest1  := 1 < 2.2      { T }
btest2  := 1 <= 2.2     { T }
btest3  := 1 = 2.2      { F }
btest4  := 1 <> 2.2     { T }
btest5  := 1 > 2.2      { F }
btest6  := 1 >= 2.2     { F }

btest7  := 1.1 < 2      { T }
btest8  := 1.1 <= 2     { T }
btest9  := 1.1 = 2      { F }
btest10 := 1.1 <> 2     { T }
btest11 := 1.1 > 2      { F }
btest12 := 1.1 >= 2     { F }

{ Assign integer to real }
rtest9  := 1            { 1 }

