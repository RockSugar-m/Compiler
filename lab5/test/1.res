@0   lineno:4   Program children: @1 @5 @8 @11 @14 @17 @22 @105 
@1   lineno:4   StmtType:declare           children: @2 @3 @4 
@2   lineno:4   TYPE:int         
@3   lineno:4   VarType:int   VarName:a          
@4   lineno:4   VarType:int   VarName:s          
@5   lineno:5   Operator:=   children: @6 @7 
@6   lineno:5   VarType:int   VarName:a          
@7   lineno:5   CONST:Interger   value:10    
@8   lineno:6   Operator:=   children: @9 @10 
@9   lineno:6   VarType:int   VarName:s          
@10  lineno:6   CONST:Interger   value:0     
@11  lineno:7   StmtType:declare           children: @12 @13 
@12  lineno:7   TYPE:char        
@13  lineno:7   VarType:char  VarName:ch         
@14  lineno:8   StmtType:declare           children: @15 @16 
@15  lineno:8   TYPE:int         
@16  lineno:8   VarType:int   VarName:a          
@17  lineno:9   StmtType:scanf             children: @18 
@18  lineno:9   Operator:,   children: @19 @20 
@19  lineno:9   CONST:ConstStr   value:"%d"  
@20  lineno:9   Operator:&   children: @21 
@21  lineno:9   VarType:char  VarName:ch         
@22  lineno:10  StmtType:while             children: @23 @41 @44 @47 @50 @53 @66 
@23  lineno:10  Operator:&&  children: @24 @37 
@24  lineno:10  Operator:||  children: @25 @32 
@25  lineno:10  Operator:&&  children: @26 @29 
@26  lineno:10  Operator:>   children: @27 @28 
@27  lineno:10  VarType:int   VarName:a          
@28  lineno:10  CONST:Interger   value:0     
@29  lineno:10  Operator:<=  children: @30 @31 
@30  lineno:10  VarType:int   VarName:a          
@31  lineno:10  CONST:Interger   value:10    
@32  lineno:10  Operator:==  children: @33 @36 
@33  lineno:10  Operator:%   children: @34 @35 
@34  lineno:10  VarType:int   VarName:a          
@35  lineno:10  CONST:Interger   value:100   
@36  lineno:10  CONST:Interger   value:10    
@37  lineno:10  Operator:!   children: @38 
@38  lineno:10  Operator:==  children: @39 @40 
@39  lineno:10  VarType:int   VarName:a          
@40  lineno:10  CONST:Interger   value:10    
@41  lineno:11  Operator:-=  children: @42 @43 
@42  lineno:11  VarType:int   VarName:a          
@43  lineno:11  CONST:Interger   value:1     
@44  lineno:12  StmtType:declare           children: @45 @46 
@45  lineno:12  TYPE:int         
@46  lineno:12  VarType:int   VarName:a          
@47  lineno:13  Operator:=   children: @48 @49 
@48  lineno:13  VarType:int   VarName:a          
@49  lineno:13  CONST:Interger   value:10    
@50  lineno:14  Operator:+=  children: @51 @52 
@51  lineno:14  VarType:int   VarName:s          
@52  lineno:14  VarType:int   VarName:a          
@53  lineno:15  StmtType:for               children: @54 @61 
@54  lineno:15  StmtType:for_condition     children: @55 @56 @59 
@55  lineno:15  null_node
@56  lineno:15  Operator:<   children: @57 @58 
@57  lineno:15  VarType:int   VarName:a          
@58  lineno:15  CONST:Interger   value:15    
@59  lineno:15  Operator:++  children: @60 
@60  lineno:15  VarType:int   VarName:a          
@61  lineno:16  StmtType:declare           children: @62 @63 
@62  lineno:16  TYPE:int         
@63  lineno:16  Operator:=   children: @64 @65 
@64  lineno:16  VarType:int   VarName:c          
@65  lineno:16  CONST:Interger   value:10    
@66  lineno:18  StmtType:if                children: @67 @72 @99 
@67  lineno:18  Operator:<   children: @68 @70 
@68  lineno:18  Operator:-   children: @69 
@69  lineno:18  VarType:int   VarName:s          
@70  lineno:18  Operator:-   children: @71 
@71  lineno:18  CONST:Interger   value:10    
@72  lineno:18  StmtType:compound_stmt     children: @73 @77 @80 @83 
@73  lineno:19  StmtType:print             children: @74 
@74  lineno:19  Operator:,   children: @75 @76 
@75  lineno:19  CONST:ConstStr   value:"result is: %d\n" 
@76  lineno:19  VarType:int   VarName:s          
@77  lineno:20  StmtType:declare           children: @78 @79 
@78  lineno:20  TYPE:int         
@79  lineno:20  VarType:int   VarName:b          
@80  lineno:21  Operator:=   children: @81 @82 
@81  lineno:21  VarType:int   VarName:b          
@82  lineno:21  CONST:Interger   value:10    
@83  lineno:22  StmtType:for               children: @84 @95 
@84  lineno:22  StmtType:for_condition     children: @85 @90 @93 
@85  lineno:22  StmtType:declare           children: @86 @87 
@86  lineno:22  TYPE:int         
@87  lineno:22  Operator:=   children: @88 @89 
@88  lineno:22  VarType:int   VarName:i          
@89  lineno:22  CONST:Interger   value:0     
@90  lineno:22  Operator:<   children: @91 @92 
@91  lineno:22  VarType:int   VarName:i          
@92  lineno:22  VarType:int   VarName:b          
@93  lineno:22  Operator:++  children: @94 
@94  lineno:22  VarType:int   VarName:i          
@95  lineno:23  StmtType:print             children: @96 
@96  lineno:23  Operator:,   children: @97 @98 
@97  lineno:23  CONST:ConstStr   value:"Have fun: %d\n" 
@98  lineno:23  VarType:int   VarName:i          
@99  lineno:23  StmtType:compound_stmt     children: @100 
@100 lineno:26  StmtType:declare           children: @101 @102 
@101 lineno:26  TYPE:int         
@102 lineno:26  Operator:=   children: @103 @104 
@103 lineno:26  VarType:int   VarName:b          
@104 lineno:26  CONST:Interger   value:11    
@105 lineno:30  StmtType:return            children: @106 
@106 lineno:30  CONST:Interger   value:0     
