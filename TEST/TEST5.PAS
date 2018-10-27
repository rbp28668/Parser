{ TEST5.PAS: Code to test WHILE-DO }
{ while-statement = "while" expression "do" statement. }

procedure tproc5(var fp : integer);
var i : integer;
begin

  {Test WHILE DO}
  i:=2;
  while i < 20 do
    i := i+1;

  {Test WHILE-DO with compound statement}
  i:=2;
  while i<20 do
    begin
    i := i + 1;
    fp := i * 7
    end;
  
  i:=0;
  while i<5 do
    begin
    writeln('i: ',i);
    i:=i+1;
    end;

end

var i:integer;
i:=3;
tproc5(i);
