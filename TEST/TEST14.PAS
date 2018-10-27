{ TEST14.PAS
| This tests the built-in functions
}

type t14enumT = (first,second,third);

var t14real : real;
var t14int : integer;
var t14bool : boolean;
var t14enum : t14enumT;
var t14str : string;

t14real := 3.141;
t14int := 72;
t14bool := true;
t14enum := second;
t14str := 'Hello World';

{Write global variables}
writeln(t14real);
writeln(t14int);
writeln(t14bool);
writeln(t14enum);
writeln(t14str);

{Write constants}
writeln(3.14);
writeln(22);
writeln(false);
writeln(first);
writeln('Goodbye Cruel');

{Build up a string of different values}
write(t14real);
write(':');
write(t14int);
write(':');
write(t14bool);
write(':');
write(t14enum);
write(':');
write(t14str);
writeln('.');

writeln;

writeln(abs(-1));
writeln(abs(1));
writeln(abs(-3.14));
writeln(abs(3.14));

writeln(arctan(1));

writeln(chr(65));

writeln(cos(3.14159/2));

writeln(exp(1));

writeln(ln(2.71828));

writeln(odd(3));
writeln(odd(4));

writeln(ord(0));
writeln(ord(1));
writeln(ord(false));
writeln(ord(true));
writeln(ord(first));
writeln(ord(second));
writeln(ord(third));
writeln(ord('ABC'));

page;

writeln(pred(true));
writeln(pred(second));
writeln(pred(third));
writeln(pred(1));
writeln(pred(2));

writeln(round(3.141));
writeln(round(5.6));
writeln(round(-3.141));
writeln(round(-5.6));

writeln(sin(3.141/2));

writeln(sqr(2));
writeln(sqr(2.5));

writeln(sqrt(2));

writeln(succ(false));
writeln(succ(first));
writeln(succ(second));
writeln(succ(1));
writeln(succ(2));

writeln(trunc(3.141));
writeln(trunc(5.6));
writeln(trunc(-3.141));
writeln(trunc(-5.6));


writeln('The answer is ',42);

