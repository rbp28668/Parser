var iCount : integer;

procedure reset;
begin
  iCount := 1;
  page;
end

procedure count(iTimes : integer);
var i: integer;
begin
  for i:= 1 to iTimes do
    begin
    writeln(iCount);
    iCount := iCount + 1
    end
end   