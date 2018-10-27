{Test1.txt:  This tests:
| const declarations 
| type declarations of simple and compound types
| var declarations
| assigment statements (& hence expressions)
}


{First create some basic constants}
{
constant-definition = "const" identifier "=" constant ";".

constant = integer-number | real-number | string | [sign] constant-identifier.

constant-identifier = identifier.  ; N.B. this is an identifier defined by 
                                   ; constant-definition or a member of
                                   ; an enumerated type.
}

const intconst = 5;
const realconst = 3.14;
const strconst = 'Hello World';
const intconst2 = intconst;
const realconst2 = realconst;
const intconst3 = -intconst;
const realconst3 = -realconst;
{ -String is meaningless }



{Create some basic types}

{Simple Type:}
type range = 1..10;
type suite = (heart,diamond,spade,club);

{Structured type:}
  {array type}
  type aiList = array[0..9] of integer;
  type arList = array[suite] of real;


 {record type}

  type arecord = 
    record
      x,y:integer;
      val:real
    end;
        
 {Create a simple hierarchy of records}
 type point = record x,y: integer end;

 type rect = record p1,p2: point end;

 type pixel = 
   record
     p:point;
     value:integer
   end;

 {Array of record}
 type line = array[range] of pixel;

 {record containing an array}
 type note = 
   record
     data : array[0..9] of integer;
     count : integer
   end;

  {an array of arrays}
  type block = array [range] of aiList;


{Create some simple variables}
var itest : integer; 
var rtest : real;
var btest : boolean;
var stest : string;

var itest2,itest3:integer;

{Create a simple const, type, var hierarchy}
const lbound = 1;
const ubound = 10;

type ListT = array[lbound..ubound] of integer;


var ListInst : ListT;


{Create a record type which includes a record }
type Group = 
  record
    data : array[lbound..ubound] of integer;
    count : integer
  end;

var GroupInst : Group;


{==========================================}
{ Now try to parse some simple expressions }
{==========================================}

{ First simple assignment }

itest := 1                      { Integer }
btest := true                   { Enumeration }
rtest := 3.141                  { Real }
stest := 'Hello World'          { String }


{ Basic integer arithmetic }

itest := not 3;     writeln('not 3 -->',itest);

itest := 3 * 4;     writeln('3 * 4 -->',itest);
itest := 10 div 3;  writeln('10 div 3 -->',itest);
itest := 10 mod 3;  writeln('10 mod 3 -->',itest);
itest := 5 and 3;   writeln('5 and 3 -->',itest);

itest := 4 + 5;     writeln('4 + 5 -->',itest);
itest := 7 - 5;     writeln('7 - 5 -->',itest);
itest := 3 or 8;    writeln('3 or 8 -->',itest);


itest := 1 + 2;     writeln('1 + 2 -->',itest);
itest := 3 * 4;     writeln('3 * 4 -->',itest);
itest := 1 + 2 * 4; writeln('1 + 2 * 4 -->',itest);
itest := 5 * 6 + 7; writeln('5 * 6 + 7 -->',itest);
itest := 10 div 2;  writeln('10 div 2 -->',itest);
itest := 11 mod 2;  writeln('11 mod 2 -->',itest);
itest := (1 + 2) * 4; writeln('(1 + 2) * 4 -->',itest);
itest := 5 * (6 + 7); writeln('5 * (6 + 7) -->',itest);
itest := -1;        writeln('-1 -->',itest);
itest := -2 * 5;    writeln('-2 * 5 -->',itest);
itest := -2 * (-5); writeln('-2 * (-5) -->',itest);
itest := 1 + 2 + 3; writeln('1 + 2 + 3 -->',itest);
itest := 7 - 2 - 1; writeln('7 - 2 - 1 -->',itest);
itest := 20 div 2 div 3; writeln('20 div 2 div 3 -->',itest);

{ Real Arithmetic }
rtest := 3.1 * 5.2; writeln('3.1 * 5.2 -->',rtest);
rtest := 3.3 / 2.6; writeln('3.3 / 2.6 -->',rtest);
rtest := 8.9 + 3.6; writeln('8.9 + 3.6 -->',rtest);
rtest := 3.69 - 1.45; writeln('3.69 - 1.45 -->',rtest);


{ Some booleans }

btest := not true;        writeln('not true -->',btest);
btest := true and false;  writeln('true and false -->',btest);
btest := true or false;   writeln('true or false -->',btest);
btest := 1 < 2;           writeln('1 < 2 -->',btest);
btest := 1 <= 2;          writeln('1 <= 2 -->',btest);
btest := 1 = 2;           writeln('1 = 2 -->',btest);
btest := 1 <> 2;          writeln('1 <> 2 -->',btest);
btest := 1 > 2;           writeln('1 > 2 -->',btest);
btest := 1 >= 2;          writeln('1 >= 2 -->',btest);

btest := 1 < 2;           writeln('1 < 2 -->',btest);
btest := (1 < 2) and (3 < 4); writeln('(1 < 2) and (3 < 4) -->',btest);
btest := (1 < 2) or (3 < 4); writeln('(1 < 2) or (3 < 4) -->',btest);
btest := not true;        writeln('not true -->',btest);

{ Real comparison }
btest := 1.1 < 2.2;       writeln('1.1 < 2.2 -->',btest);
btest := 1.1 <= 2.2;      writeln('1.1 <= 2.2 -->',btest);
btest := 1.1 = 2.2;       writeln('1.1 = 2.2 -->',btest);
btest := 1.1 <> 2.2;      writeln('1.1 <> 2.2 -->',btest);
btest := 1.1 > 2.2;       writeln('1.1 > 2.2 -->',btest);
btest := 1.1 >= 2.2;      writeln('1.1 >= 2.2 -->',btest);


{ Mixed real and integer arithmetic }
rtest := 3.1 * 5;  writeln('3.1 * 5 -->',rtest);
rtest := 3.3 / 2;  writeln('3.3 / 2 -->',rtest);
rtest := 8.9 + 3;  writeln('8.9 + 3 -->',rtest);
rtest := 3.69 - 1; writeln('3.69 - 1 -->',rtest);

rtest := 3 * 5.2;  writeln('3 * 5.2 -->',rtest);
rtest := 3 / 2.6;  writeln('3 / 2.6 -->',rtest);
rtest := 8 + 3.6;  writeln('8 + 3.6 -->',rtest);
rtest := 3 - 1.45; writeln('3 - 1.45 -->',rtest);

{ Mixed integer and real comparison }
btest := 1 < 2.2;  writeln('1 < 2.2 -->',btest);
btest := 1 <= 2.2; writeln('1 <= 2.2 -->',btest);
btest := 1 = 2.2;  writeln('1 = 2.2 -->',btest);
btest := 1 <> 2.2; writeln('1 <> 2.2 -->',btest);
btest := 1 > 2.2;  writeln('1 > 2.2 -->',btest);
btest := 1 >= 2.2; writeln('1 >= 2.2 -->',btest);

btest := 1.1 < 2;  writeln('1.1 < 2 -->',btest);
btest := 1.1 <= 2; writeln('1.1 <= 2 -->',btest);
btest := 1.1 = 2;  writeln('1.1 = 2 -->',btest);
btest := 1.1 <> 2; writeln('1.1 <> 2 -->',btest);
btest := 1.1 > 2;  writeln('1.1 > 2 -->',btest);
btest := 1.1 >= 2; writeln('1.1 >= 2 -->',btest);

{ Assign integer to real }
rtest := 1;         writeln('1 -->',rtest);
