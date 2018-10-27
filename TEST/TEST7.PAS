{TEST7.PAS test CASE}

{
case-statement = "case" expression "of" caselimb < ";" caselimb > [";"] "end".
caselimb = case-label-list ":" statement.
case-label-list = constant < "," constant >.
}

type card_rank = (ace,two,three,four,five,six,seven,eight,nine,ten,
                jack,queen,king);

type card_suite = (heart,diamond,spade,club);

type card_type = 
  record
    rank : card_rank;
    suite: card_suite
  end;
    

function test7(card : card_type) : integer;
var value : integer;
begin

 
 { Test case with only 1 case caselimb }
 value := 1;
 case value of
   1: value := 99
 end;

 writeln('1 mapped to ',value);

 { Test case with 2 caselimbs }
 value := 1;
 case value of
   1: value := 2;
   2: value := 3
 end;
 writeln('1 mapped to ',value);

 { and with a semicolon after last caselimb }
 value := 1;
 case value of
   1: value := 2;
   2: value := 3;
 end;

 writeln('Card is ',card.suite,',',card.rank);

 { Test case with enumerated type }
 case card.suite of
   heart :    value := 10;
   diamond :  value := 20;
   spade :    value := 30;
   club :     value := 40
 end;
 writeln('Value of ',card.suite,' is ',value);

 { Test case with more one entry in case-label-list }
 
 case card.rank of
   ace :   value := 1;
   two :   value := 2;
   three : value := 3;
   four :  value := 4;
   five :  value := 5;
   six :   value := 6;
   seven : value := 7;
   eight : value := 8;
   nine :  value := 9;
   ten,jack,queen,king : value := 10;
 end;

 writeln('value of ',card.rank,' is ',value);
 test7 := value

end

var card : card_type;

card.suite := diamond;
card.rank := king;

writeln('Card is ',card.suite,',',card.rank);
var i:integer;

i := test7(card);

