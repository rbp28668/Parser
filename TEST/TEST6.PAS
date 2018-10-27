
{TEST6.PAS : Test FOR }
{
for-statement = "for" variable-identifier ":=" 
	initial-expression ( "to" | "downto" )
	final-expression "do" statement.

initial-expression = expression.

final-expression = expression.
}

type t6type = (one,two,three,four);


procedure tproc6(var fp : integer);
var i,j : integer;
var t6 : t6type;
begin

  {Test FOR TO }
  writeln('for i := 1 to 10 do');
  for i := 1 to 10 do
    writeln('i: ',i);

  {Test FOR DOWNTO }
  writeln('for i := 10 downto 1 do');
  for i := 10 downto 1 do
    writeln('i: ',i);


  {Test FOR TO with compound statement}
  writeln('for i := 1 to 10 do');
  for i := 1 to 10 do
    begin
    j := i + 1;
    fp := i * 7;
    writeln('i: ',i);
    end;

  {Test FOR DOWNTO with compound statement}
  writeln('for i := 10 downto 1 do');
  for i := 10 downto 1 do
    begin
    j := i + 1;
    fp := i * 7;
    writeln('i: ',i);
    end;

  {Test FOR TO with enumerated index}
  writeln('for t6 := one to four do');
  for t6 := one to four do
    begin
    writeln('t6: ',t6);
    end 
  
end


var i:integer;
i:=2;
tproc6(i);
