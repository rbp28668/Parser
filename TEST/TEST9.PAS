{TEST9.PAS function to check procedure and function parameters }

function pf1(val1 : real) : integer;
begin
  if val1 < 0 then 
    pf1 := -1
  else
    pf1 := 1
end


procedure pp1(val : real);
begin
  val := val + 1;
end


{ Function which takes a function of type as pf1 }
function f_takes_pf1(function ppf1(val2 : real): integer;v : real) : integer;
begin
   f_takes_pf1 := ppf1(v)
end

{ Procedure which takes a function of type as pf1 }
procedure p_takes_pf1(function ppf1(val2 : real): integer;v : real);
var return : integer;
begin
   return := ppf1(v)
end


{ Function which takes a procedure of type as pp1 }
function f_takes_pp1(procedure ppp1(val : real);v : real) : integer;
begin
   ppp1(v);
   f_takes_pp1 := 1
end

{ Procedure which takes a procedure of type as pp1 }
procedure p_takes_pp1(procedure ppp1(val : real);v : real);
begin
   ppp1(v)
end


{ Now try calling them }
var i: integer;

i := f_takes_pf1(pf1,5)
p_takes_pf1(pf1,5)
i := f_takes_pp1(pp1,5)
p_takes_pp1(pp1,5)

