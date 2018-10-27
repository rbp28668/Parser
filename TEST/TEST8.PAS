{ TEST8.PAS Test some of the more subtle aspects....}

{Function for testing integer->real promotion }
function test8_fn(rval : real) : real ;
begin
  test8_fn := rval
end

{Some types & subrange types}
type int_sr8 = 1..10;                {subrange of integer}
type enum8 = (one,two,three,four);   {base enumeration }
type sr_enum8 = one..two;            {subrange of enum8 }   
type enum8_2 = enum8;
type list8 = array[int_sr8] of integer;

type rec8 = record ix,iy : integer end;

var arr81 : list8;
var arr82 : list8;
var r81,r82 : rec8;

procedure test8(ival : integer);
var i,j : integer;
var x,y : real;
var e8 : enum8;
var e8_2 : enum8_2;  { should be same type as e8 }
var se8 : sr_enum8;
var tiny : int_sr8;
var fruit1,fruit2 : (figs,grapes);
begin


  { Rule 6: basic integer promotion }
  x := 5;
  i := 6;
  y := i;

  j := 7;
  x := i / j;       { real division }
  y := test8_fn(j); { promote fn argument }


  { Rule 8: values of a subrange type are regarded as being
  / of parent type.
  }
  e8 := two;
  se8 := e8;  {se8 is subrange of e8}
  e8 := se8;

  
  tiny := 5;  {tiny is subrange of integer}
  i := tiny;


  { Rule 9: type ident declared in terms of another type refer
  / to the same type
  }
  e8_2 := e8;   { these should be same type }


  { Rule 10: variables have same type if declared in same declaration.}
  fruit1 := figs;
  fruit2 := fruit1;

  { Rule 11: subrange is treated as host in expr }
  i := tiny * j;

  { Rule 13: in a compound statement an extra ; indicates a NULL statement}
  if true then 
    begin
    i := 1;
    j := i;
    end;
  
  { Rule 26: whole arrays can be assigned if of same type }
  for i:=1 to 10 do
    arr81[i] := 1;
  arr82 := arr81;

  { Ditto for records }
  r81.ix := 1;
  r81.iy := 2;

  r82 := r81;


  i := 1
end

