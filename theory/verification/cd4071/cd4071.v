Record CD4071_Input : Type := mk_input {
    a0 : bool; b0 : bool;
    a1 : bool; b1 : bool;
    a2 : bool; b2 : bool;
    a3 : bool; b3 : bool;
}.

Record CD4071_Output : Type := mk_output {
    c0 : bool; 
    c1 : bool;
    c2 : bool; 
    c3 : bool;
}.


Definition or_gate (a b : bool) : bool := a || b.


Definition CD4071 (input : CD4071_Input) : CD4071_Output :=
    let c0 := or_gate (a0 input) (b0 input) in  
    let c1 := or_gate (a1 input) (b1 input) in 
    let c2 := or_gate (a2 input) (b2 input) in 
    let c3 := or_gate (a3 input) (b3 input) in
        mk_output c0 c1 c2 c3.
    

Example test_cd4081_1 :
    CD4071 
    ( 
        mk_input 
        false false 
        false true 
        true false 
        true true 
    ) 
    = mk_output 
        false 
        true 
        true 
        true.
Proof. reflexivity. Qed. 