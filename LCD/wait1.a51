NAME wait

EXTRN DATA (Var1, Var2, Var3)

PUBLIC w1ms, w5mS, w50mS

Wait_ROUTINES SEGMENT CODE

RSEG Wait_ROUTINES

w1ms:
      MOV   Var2,#4
      MOV   Var1,#224
TT2:  DJNZ  Var1,TT2
      DJNZ  Var2,TT2
      RET

w5mS:
      MOV  Var2,#20
      MOV  Var1,#112
TT3:  DJNZ Var1,TT3
      DJNZ Var2,TT3
      RET

w50mS:
       MOV  Var2,#195
       MOV  Var1,#137
TT5:   DJNZ Var1,TT5
       DJNZ Var2,TT5
       RET
END
