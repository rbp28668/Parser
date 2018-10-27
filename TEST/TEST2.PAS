{ Test2.txt: Test simple procedure and functions }
{ Set up a simple procedure and function }

procedure tproc(var ip1 : integer; ip2 : integer);
var local1 : integer;
begin
 local1 := 1;
 local1 := local1 * 2
end


function tfn(var ip3 : integer; ip4 : integer) : integer;
begin
  tfn := ip3 + ip4
end


{ Set up some variables to call fns & procedures with 
  and test calls to the procedure and function 
}
var ap1,ap2 : integer;
var itemp : integer;

ap1 := 1
ap2 := 2

tproc(ap1,ap2)

itemp := tfn(ap1,ap2)


{ Now set up a procedure and a function without 
/ formal parameter lists.
}

procedure proc_nofplist ;
begin
  ap1 := 3
end

function fn_nofplist : integer;
begin
  fn_nofplist := ap1;
end

{and call them }
proc_nofplist
itemp := fn_nofplist


{ Now set up a function taking a real and pass it an integer }
function fn_copy(r : real) : real ;
begin
  fn_copy := r
end

var rval : real;

rval := fn_copy(99)


{pass a subrange type}
type tiny = 1..10;
var little:tiny;

little := 2
little := tfn(little,little)


{Enumerated type for fn}
type fish = (halibut,cod,sardine,tuna,trout,goldfish);
type freshwater = trout..goldfish;

var cecil:fish;
var bert:freshwater;

cecil := halibut
bert := goldfish

function swim(what : fish) : fish;
begin
  swim := what
end

cecil := swim(cecil)
bert := swim(bert)

{
Don't run this: it will (correctly) cause a bounds fault
bert := swim(cecil)
}




