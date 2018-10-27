{ TEST15.PAS }

{
1. Reserved words may not be used as identifiers.
}
writeln('Test 1');
var begin : integer;


{
2. Div truncates towards zero 
}

writeln('Test 2');
writeln(3 div 2);
writeln(-3 div 2);

{
3. Mod: second argument must be > 0
}
writeln('Test 3');
writeln(-3 mod 2);  {should be OK}
writeln(3 mod (-2));


{
4. Division by zero is an error
}
writeln('Test 4');
writeln(4.0 / 0.0);
writeln(4 div 0);


{
5. Overflow is error 
}
writeln('Test 5');
writeln(65537 * 65537);


{
6. Integers can be used where reals are expectd 
}
writeln('Test 6');
writeln(5/4);

{
7. Using integer operands with / produces a real result
}
writeln('Test 7');
writeln(sin(1));


{
8.  Values of a subrange type are regarded as being of the
    parent host type.
}
writeln('Test 8');
type fruit = (apple, orange, lemon, lime, pear, banana, pineapple, peach);
type citrus = orange..lime;

var nice : citrus;
var squashy : fruit;

nice := lemon;  { a citrus }
squashy := nice; { assign to parent (fruit) type }

{
9.  If a type is defined in terms of another defined type the
    2 types are the same.
}
writeln('Test 9');
type newfruit = fruit;
var mouldy : newfruit;
mouldy := peach;


{
10. Variables have the same type if declared in the same 
    declaration, declared using the same type identifier or declared
    using 2 identifiers which denote the same type.
}
writeln('Test 10');

{
11. A subrange typed variable is considered to be of its
    host type in an expression.
}
writeln('Test 11');
type isubtype = 1..10;
var isub : isubtype;

isub := 5;
writeln(odd(isub));  {odd expects integer}
writeln(isub);       {should print as integer}

{
12. A real variable may have an integer assigned to it
}
writeln('Test 12');
var areal : real;
areal := 4;

{
13. In a compound statement a ; before the end keyword 
    implies an empty statement which has no effect
}
writeln('Test 13');
procedure atest;
  begin
  writeln('hello');
  writeln('world');
  end

{
14. In a for loop the control variable, initial expression 
    and final expression must all have the same type.
}
writeln('Test 14');
var i: integer;
var jam : fruit;

procedure test2;
  begin

    {should be ok}
    for i:=0 to 5 do
      write(i,' ');
    writeln;

    {should be ok}
    for jam := apple to banana do
      write(jam,' ');
    writeln;
  end

test2;

procedure test3;
  begin
    for i:= 0 to apple do {final expr wrong type}
      writeln('oops');
  end

procedure test4;
  begin
    for i:= apple to 5 do {intial expr wrong type}
      writeln('oops');
  end

procedure test5;
  begin
    for jam:= apple to 5 do  {final expr wrong type}
      writeln('oops');
  end

procedure test6;
  begin
    for jam:= 0 to peach do {initial expr wrong type}
      writeln('oops');
  end


{
15. The initial and final expressions are evaluated once
    only on entry to a for loop
}
writeln('Test 15');
var count : integer;
function twice(i: integer): integer;
  begin
  twice := 2*i;
  count := count + 1  {count no of times this is called}
  end


procedure test7;
  begin
    for i:=twice(1) to twice(2) do
      write(i,' ');
    writeln;
  end

count := 0;
test7;
writeln('Count is ',count);

