{ TEST3.TXT:  Test IF-THEN-ELSE }

procedure tproc3(var fp5 : integer);
var i : integer;
begin
  i := 2;

  {Test simple IF THEN}
  if i > 1 then
    i := 1;

  {Test simple IF THEN ELSE }
  if i < 2 then
    i := 2
  else
    i := 3;

  {Test IF THEN with compound statement}
  if i > 1 
  then begin
    i := 3;
    fp5 := i*3
  end;

  {Test IF THEN ELSE with compound statements}
  if i > 1 
  then begin
    i := 3;
    fp5 := i * 6
  end
  else begin
    i := 4;
    fp5 := i * 7
  end;

  if true then
    writeln('OK');

  if false then
    writeln('BAD');

  if true then
    writeln('OK');
  else
    writeln('BAD');

  if false then
    writeln('BAD');
  else
    writeln('OK');


end


var i:integer;
i := 3;

tproc3(i);

