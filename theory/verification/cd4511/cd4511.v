(*

7-segmets display decode list

input - out  =>   abcdefg
0000  -  0   =>   1111110 
0001  -  1   =>   0110000 
0010  -  2   =>   1101101 
0011  -  3   =>   1111001 
0100  -  4   =>   0110011
0101  -  5   =>   1011101
0110  -  6   =>   1011111
0111  -  7   =>   1110000 
1000  -  8   =>   1111111
1001  -  9   =>   1111011
____  -  _   =>   0000000

*)


Record CD4511_Input : Type := mk_input {
    bit3 : bool; bit2 : bool; bit1 : bool; bit0 : bool;

    lt : bool; bl : bool; le : bool;
}.


Record CD4511_Output : Type := mk_output {
    a : bool; b : bool; c : bool; d : bool; e : bool; f : bool; g : bool;
}.


Definition bits_to_nat (bit3 bit2 bit1 bit0 : bool) : nat :=
    (if bit3 then 8 else 0) + (if bit2 then 4 else 0) +
    (if bit1 then 2 else 0) + (if bit0 then 1 else 0).



Definition nat_to_segments (num : nat) : CD4511_Output := 
    match num with
    | 0 => (mk_output true true true true true true false)
    | 1 => (mk_output false true true false false false false)
    | 2 => (mk_output true true false true true false true)
    | 3 => (mk_output true true true true false false true)
    | 4 => (mk_output false true true false false true true)
    | 5 => (mk_output true false true true false true true)
    | 6 => (mk_output true false true true true true true)
    | 7 => (mk_output true true true false false false false)
    | 8 => (mk_output true true true true true true true)
    | 9 => (mk_output true true true true false true true)
    | _ => (mk_output false false false false false false false) (*invalid*)
    end.

    
(* 
    Simplification: LE (latch enable) is not modeled yet, since it requires the chip to hold state across calls 
    (last latched BCD value), which a pure function cannot express. The chip currently always decodes the live 
    BCD input, as if LE were permanently inactive. This will be revisited once Moore machine formalization is 
    introduced.   
*)

Definition CD4511 (input : CD4511_Input) : CD4511_Output :=
    if negb (lt input) then
        mk_output true true true true true true true
    else if negb (bl input) then
        mk_output false false false false false false false
    else
        nat_to_segments (bits_to_nat (bit3 input) (bit2 input) (bit1 input) (bit0 input)).



(* Tests*)

Example test_cd4511_lamp_test :
  CD4511 (mk_input false false false false false true false)
  = mk_output true true true true true true true.
Proof. reflexivity. Qed.


Example test_cd4511_blanking :
  CD4511 (mk_input false false false false true false false)
  = mk_output false false false false false false false.
Proof. reflexivity. Qed.