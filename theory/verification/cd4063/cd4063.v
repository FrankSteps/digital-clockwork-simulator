(* Import modules *)
Require Import Arith.
Require Import Nat.


(* Represents the 8 physical input pins of the chip *)
Record CD4063_Input : Type := mk_input {
    a3 : bool; a2 : bool; a1 : bool; a0 : bool;
    b3 : bool; b2 : bool; b1 : bool; b0 : bool;
}.


(* Converts 4 individual bits into a natural numb *)
Definition bits_to_nat (bit3 bit2 bit1 bit0 : bool) : nat :=
    (if bit3 then 8 else 0) + (if bit2 then 4 else 0) +
    (if bit1 then 2 else 0) + (if bit0 then 1 else 0).


(* The three possible outcomes of comparing two numbers. *)
Inductive comparison : Type := Less | Equal | Greater.


(* Compares two naturals using the standard order relations already proven in the Arith library *)
Definition compare (vA vB : nat) : comparison :=
  if vA <? vB then Less
  else if vB <? vA then Greater
  else Equal.


(* Full path from raw chip input to local comparison result. Converts each 4-bit to a number, then compares them *)
Definition CD4063_core (input : CD4063_Input) : comparison :=
  let vA := bits_to_nat (a3 input) (a2 input) (a1 input) (a0 input) in
  let vB := bits_to_nat (b3 input) (b2 input) (b1 input) (b0 input) in
    compare vA vB.


(* Tests *)
Example teste_CD4063_1 :
  CD4063_core 
  (
    mk_input 
    true false 
    true true 
    false true
    true false
  ) = Greater.
Proof. reflexivity. Qed.

  
Example teste_CD4063_2 :
  CD4063_core 
  (
    mk_input 
    false false 
    true true 
    true true 
    true false
  ) = Less.
Proof. reflexivity. Qed.


Example teste_CD4063_3 :
  CD4063_core 
  (
    mk_input 
    true false 
    false true 
    true false 
    false true
  ) = Equal.
Proof. reflexivity. Qed.



(* Full path from raw chip input to local comparison result *)
Record CD4063_Cascade : Type := mk_cascade {
    in_ls : bool;
    in_gt : bool;
    in_eq : bool;
}.


(* Final chip outputs *)
Record CD4063_Output : Type := mk_output {
    out_ls : bool;
    out_gt : bool;
    out_eq : bool;
}.


(* Decides the final output based on the local comparison result *)
Definition resolve_cascade (cmp : comparison) (casc : CD4063_Cascade) : CD4063_Output :=
  match cmp with
  | Less    => mk_output true false false
  | Greater => mk_output false true false
  | Equal   => mk_output (in_ls casc) (in_gt casc) (in_eq casc)
  end.


(* Full chip behavior *)
Definition CD4063 (input : CD4063_Input) (casc : CD4063_Cascade) : CD4063_Output :=
  resolve_cascade (CD4063_core input) casc.


(* test *)
Example teste_cascade_1 :
  CD4063 (mk_input true false true true true false true true) (mk_cascade true false false) = mk_output true false false.
Proof. reflexivity. Qed.
