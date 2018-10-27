{ Test 4: Check REPEAT-UNTIL }
{ repeat-statement = "repeat" statement-sequence "until" expression. }

procedure tproc4(var fp5 : integer);
var i : integer;
begin

  {Test REPEAT UNTIL}
  i:=2;
  repeat
    i := i + 1
  until i > 20;

  {Test REPEAT UNTIL with compound statement}
  i:=2;
  repeat
    begin
    i := i + 1;
    fp5 := i * 7
    end
  until i > 20;

  {Test REPEAT UNTIL with statement sequence}
  i:=2;
  repeat
    i := i + 1;
    fp5 := i * 7
  until i > 20;

  i:=0;
  repeat
    i:=i+1;
    writeln('i: ',i);
  until(i>5)

end

var i:integer;

i:=2;
tproc4(i);

