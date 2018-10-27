{ HANOI.PAS 
| Recursive test
}

{ Set up pole type }
type pole = (left,middle,right);

{ Main recursive bit }
procedure DoHanoi(nDisks : integer; src, temp, dest : pole);
begin
  if nDisks <= 1 then
    begin
      writeln('Move a disk from ',src, ' to ', dest);
    end
  else
    begin
      DoHanoi(nDisks-1, src,dest,temp);
      writeln('Move a disk from ',src, ' to ', dest);
      DoHanoi(nDisks-1, temp,src,dest);
    end
end

{ Entry pt }
procedure Hanoi(nDisks : integer);
begin
  writeln;
  writeln('Hanoi for ',nDisks,' disks');
  DoHanoi(nDisks,left,middle,right);
end