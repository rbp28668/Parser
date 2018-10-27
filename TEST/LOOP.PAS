procedure forever;
  var i: integer;
  begin
  i:=0;
  while true do 
    begin
    writeln(i);
    i:=i+1;
    if i=10000 then i:=0
    end
  end
