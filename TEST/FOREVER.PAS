procedure forever;
var i: integer;
begin
  i := 1;
  while true do 
    begin
    writeln('Ho Hum number ', i);
   i := i + 1;
   if i > 10000 then i := 1
   end
end
