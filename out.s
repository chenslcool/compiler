FUNCTION printArr :
PARAM arr
i := #0
j := #0
LABEL L0 :
IF i != #2 GOTO L1
GOTO L2 
LABEL L1 :
j := #0
LABEL L3 :
IF j != #10 GOTO L4
GOTO L5 
LABEL L4 :
t0 := i * #40
t1 := arr + t0
t2 := j * #4
t3 := t1 + t2
t4 := *t3
WRITE t4
t5 := j + #1
j := t5
GOTO L3 
LABEL L5 :
t6 := i + #1
i := t6
GOTO L0 
LABEL L2 :
RETURN #0
FUNCTION f :
PARAM a
ii := #0
jj := #1
LABEL L6 :
IF #10 >= ii GOTO L7
GOTO L8 
LABEL L7 :
t7 := #0 * #40
t8 := a + t7
t9 := ii * #4
t10 := t8 + t9
*t10 := ii
t11 := ii + #1
ii := t11
GOTO L6 
LABEL L8 :
LABEL L9 :
IF #10 != jj GOTO L10
GOTO L11 
LABEL L10 :
t12 := #1 * #40
t13 := a + t12
t14 := jj * #4
t15 := t13 + t14
t16 := #0 * #40
t17 := a + t16
t18 := jj - #1
t19 := t18 * #4
t20 := t17 + t19
t21 := *t20
t22 := #0 * #40
t23 := a + t22
t24 := jj * #4
t25 := t23 + t24
t26 := *t25
t27 := t21 * t26
*t15 := t27
t28 := jj + #1
jj := t28
GOTO L9 
LABEL L11 :
RETURN #0
FUNCTION main :
DEC a2 80
t29 := &a2
ARG t29
t30 := CALL f
t31 := &a2
ARG t31
t32 := CALL printArr
RETURN #0
FUNCTION f2 :
PARAM tt1
PARAM tt2
PARAM tt3
PARAM tt4
PARAM tt5
PARAM tt6
PARAM tt7
t33 := tt1 + tt2
RETURN t33
