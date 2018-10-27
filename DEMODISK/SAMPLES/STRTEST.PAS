{
; Basic test of string manipulation on constant strings
}
writeln('String comparison: set 1');
writeln('one' < 'two');
writeln('one' <= 'two');
writeln('one' = 'two');
writeln('one' <> 'two');
writeln('one' >= 'two');
writeln('one' > 'two');

writeln('String comparison: set 2');
writeln('one' < 'one');
writeln('one' <= 'two');
writeln('one' = 'two');
writeln('one' <> 'two');
writeln('one' >= 'two');
writeln('one' > 'two');

writeln('str_length');
writeln(str_length(''));
writeln(str_length('12345'));

writeln('str_is_empty:');
writeln(str_is_empty(''));
writeln(str_is_empty('12345'));

writeln('str_concat');
writeln(str_concat('',''));
writeln(str_concat('12345',''));
writeln(str_concat('','12345'));
writeln(str_concat('123','45'));

writeln('str_left');
writeln(str_left('',0));
writeln(str_left('',1));
writeln(str_left('12345',0));
writeln(str_left('12345',1));
writeln(str_left('12345',3));
writeln(str_left('12345',5));
writeln(str_left('12345',6));

writeln('str_mid');
writeln(str_mid('',0,0));
writeln(str_mid('',0,1));
writeln(str_mid('',1,0));
writeln(str_mid('',1,1));
writeln(str_mid('12345',0,0));
writeln(str_mid('12345',1,0));
writeln(str_mid('12345',3,0));
writeln(str_mid('12345',5,0));
writeln(str_mid('12345',6,0));
writeln(str_mid('12345',1,1));
writeln(str_mid('12345',1,3));
writeln(str_mid('12345',1,5));
writeln(str_mid('12345',1,6));
writeln(str_mid('12345',2,3));
writeln(str_mid('12345',3,3));
writeln(str_mid('12345',3,4));

writeln('str_right');
writeln(str_right('',0));
writeln(str_right('',1));
writeln(str_right('12345',0));
writeln(str_right('12345',1));
writeln(str_right('12345',3));
writeln(str_right('12345',5));
writeln(str_right('12345',6));

writeln('str_span_including');
writeln(str_span_including('',''));
writeln(str_span_including('12345','123'));
writeln(str_span_including('12345','321'));
writeln(str_span_including('12345',''));
writeln(str_span_including('','123'));
writeln(str_span_including('12345','678'));

writeln('str_span_excluding');
writeln(str_span_excluding('',''));
writeln(str_span_excluding('12345','123'));
writeln(str_span_excluding('12345','321'));
writeln(str_span_excluding('12345',''));
writeln(str_span_excluding('','123'));
writeln(str_span_excluding('12345','678'));

writeln('str_is_in');
writeln(str_is_in('',''));
writeln(str_is_in('12345','123'));
writeln(str_is_in('12345','321'));
writeln(str_is_in('12345',''));
writeln(str_is_in('','123'));
writeln(str_is_in('12345','678'));


{ Test1. literal strings in procedures }
procedure StrTest1;
begin
  writeln('StrTest');
end

{ Test2. strings as value parameters }
procedure StrTest2(s : string);
begin
  writeln('StrTest2: ',s);
end

{ Test3. strings as var parameters}
procedure StrTest3(var s : string);
begin
  writeln('StrTest3');
  s := 'StrTest3';
end

{Test 4, returning a string from a function}
function StrTest4 : string;
begin
	StrTest4 := 'StrTest4';
end

{Test 5, returning a string from a function from a function}
function StrTest5 : string;
begin
	StrTest5 := StrTest4;
end

var sGlobal : string;
sGlobal := 'sGlobal';

StrTest1;
StrTest2('Literal');
StrTest2(sGlobal);
StrTest3(sGlobal);
writeln('sGlobal now ',sGlobal);
writeln('StrTest4 returns ',StrTest4);
writeln('StrTest5 returns ',StrTest5);


