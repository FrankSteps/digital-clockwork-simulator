Record CD4081_Input : Type := mk_input {
    a0 : bool; b0 : bool;
    a1 : bool; b1 : bool;
    a2 : bool; b2 : bool;
    a3 : bool; b3 : bool;
}.

Record CD4081_Output : Type := mk_output {
    c0 : bool; 
    c1 : bool; 
    c2 : bool; 
    c3 : bool;
}.


Definition and_gate (a b : bool) : bool := a && b.


Definition CD4081 (input : CD4081_Input) : CD4081_Output :=
    let c0 := and_gate (a0 input) (b0 input) in
    let c1 := and_gate (a1 input) (b1 input) in
    let c2 := and_gate (a2 input) (b2 input) in
    let c3 := and_gate (a3 input) (b3 input) in
        mk_output c0 c1 c2 c3.


Example test_cd4081_1 :
    CD4081 
    ( 
        mk_input 
        false false 
        false true 
        true false 
        true true 
    ) 
    = mk_output 
        false 
        false 
        false 
        true.
Proof. reflexivity. Qed. 