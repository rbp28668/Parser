function factorial(int i:integer): integer;
  begin
  if i=0 then 
    factorial := 1
  else
    factorial := i * factorial(i-1)
  end

