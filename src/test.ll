; ModuleID = 'examples/litmus/C-litmus/CO-LB.c'
source_filename = "examples/litmus/C-litmus/CO-LB.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@__fence_var = dso_local global i32 0, align 4, !dbg !0
@fr = dso_local global i32 0, align 4, !dbg !8
@label = dso_local global i32 0, align 4, !dbg !11
@vars = common dso_local global [1 x i32] zeroinitializer, align 4, !dbg !13
@v2_r0 = common dso_local global i32 0, align 4, !dbg !24
@v8 = common dso_local global i32 0, align 4, !dbg !36
@atom_0_r0_2 = common dso_local global i32 0, align 4, !dbg !18
@v4_r0 = common dso_local global i32 0, align 4, !dbg !28
@v9 = common dso_local global i32 0, align 4, !dbg !38
@atom_1_r0_1 = common dso_local global i32 0, align 4, !dbg !20
@v5 = common dso_local global i32 0, align 4, !dbg !30
@v6 = common dso_local global i32 0, align 4, !dbg !32
@v7_conj = common dso_local global i32 0, align 4, !dbg !34
@.str = private unnamed_addr constant [2 x i8] c"0\00", align 1
@.str.1 = private unnamed_addr constant [33 x i8] c"examples/litmus/C-litmus/CO-LB.c\00", align 1
@__PRETTY_FUNCTION__.__VERIFIER_atomic_check = private unnamed_addr constant [31 x i8] c"void __VERIFIER_atomic_check()\00", align 1
@v1 = common dso_local global i32 0, align 4, !dbg !22
@v3 = common dso_local global i32 0, align 4, !dbg !26

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__VERIFIER_atomic_t0() #0 !dbg !44 {
  store i32 1, i32* @label, align 4, !dbg !47
  %1 = load i32, i32* getelementptr inbounds ([1 x i32], [1 x i32]* @vars, i64 0, i64 0), align 4, !dbg !48
  store i32 %1, i32* @v2_r0, align 4, !dbg !49
  store i32 1, i32* getelementptr inbounds ([1 x i32], [1 x i32]* @vars, i64 0, i64 0), align 4, !dbg !50
  %2 = load i32, i32* @v2_r0, align 4, !dbg !51
  %3 = icmp eq i32 %2, 2, !dbg !52
  %4 = zext i1 %3 to i32, !dbg !52
  store i32 %4, i32* @v8, align 4, !dbg !53
  %5 = load i32, i32* @v8, align 4, !dbg !54
  store i32 %5, i32* @atom_0_r0_2, align 4, !dbg !55
  ret void, !dbg !56
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @t0(i8* %0) #0 !dbg !57 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  call void @llvm.dbg.declare(metadata i8** %2, metadata !60, metadata !DIExpression()), !dbg !61
  call void @__VERIFIER_atomic_t0(), !dbg !62
  ret i8* null, !dbg !63
}

; Function Attrs: nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__VERIFIER_atomic_t1() #0 !dbg !64 {
  store i32 2, i32* @label, align 4, !dbg !65
  %1 = load i32, i32* getelementptr inbounds ([1 x i32], [1 x i32]* @vars, i64 0, i64 0), align 4, !dbg !66
  store i32 %1, i32* @v4_r0, align 4, !dbg !67
  store i32 2, i32* getelementptr inbounds ([1 x i32], [1 x i32]* @vars, i64 0, i64 0), align 4, !dbg !68
  %2 = load i32, i32* @v4_r0, align 4, !dbg !69
  %3 = icmp eq i32 %2, 1, !dbg !70
  %4 = zext i1 %3 to i32, !dbg !70
  store i32 %4, i32* @v9, align 4, !dbg !71
  %5 = load i32, i32* @v9, align 4, !dbg !72
  store i32 %5, i32* @atom_1_r0_1, align 4, !dbg !73
  ret void, !dbg !74
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @t1(i8* %0) #0 !dbg !75 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  call void @llvm.dbg.declare(metadata i8** %2, metadata !76, metadata !DIExpression()), !dbg !77
  call void @__VERIFIER_atomic_t1(), !dbg !78
  ret i8* null, !dbg !79
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__VERIFIER_atomic_check() #0 !dbg !80 {
  %1 = load i32, i32* @atom_0_r0_2, align 4, !dbg !81
  store i32 %1, i32* @v5, align 4, !dbg !82
  %2 = load i32, i32* @atom_1_r0_1, align 4, !dbg !83
  store i32 %2, i32* @v6, align 4, !dbg !84
  %3 = load i32, i32* @v5, align 4, !dbg !85
  %4 = load i32, i32* @v6, align 4, !dbg !86
  %5 = and i32 %3, %4, !dbg !87
  store i32 %5, i32* @v7_conj, align 4, !dbg !88
  %6 = load i32, i32* @v7_conj, align 4, !dbg !89
  %7 = icmp eq i32 %6, 1, !dbg !91
  br i1 %7, label %8, label %9, !dbg !92

8:                                                ; preds = %0
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i64 0, i64 0), i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.1, i64 0, i64 0), i32 60, i8* getelementptr inbounds ([31 x i8], [31 x i8]* @__PRETTY_FUNCTION__.__VERIFIER_atomic_check, i64 0, i64 0)) #5, !dbg !93
  unreachable, !dbg !93

9:                                                ; preds = %0
  ret void, !dbg !96
}

; Function Attrs: noreturn nounwind
declare dso_local void @__assert_fail(i8*, i8*, i32, i8*) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %0, i8** %1) #0 !dbg !97 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  call void @llvm.dbg.declare(metadata i32* %4, metadata !103, metadata !DIExpression()), !dbg !104
  store i8** %1, i8*** %5, align 8
  call void @llvm.dbg.declare(metadata i8*** %5, metadata !105, metadata !DIExpression()), !dbg !106
  call void @llvm.dbg.declare(metadata i64* %6, metadata !107, metadata !DIExpression()), !dbg !111
  call void @llvm.dbg.declare(metadata i64* %7, metadata !112, metadata !DIExpression()), !dbg !113
  store i32 0, i32* getelementptr inbounds ([1 x i32], [1 x i32]* @vars, i64 0, i64 0), align 4, !dbg !114
  store i32 0, i32* @atom_0_r0_2, align 4, !dbg !115
  store i32 0, i32* @atom_1_r0_1, align 4, !dbg !116
  store i32 0, i32* @__fence_var, align 4, !dbg !117
  %8 = call i32 @pthread_create(i64* %6, %union.pthread_attr_t* null, i8* (i8*)* @t0, i8* null) #6, !dbg !118
  %9 = call i32 @pthread_create(i64* %7, %union.pthread_attr_t* null, i8* (i8*)* @t1, i8* null) #6, !dbg !119
  %10 = load i64, i64* %6, align 8, !dbg !120
  %11 = call i32 @pthread_join(i64 %10, i8** null), !dbg !121
  %12 = load i64, i64* %7, align 8, !dbg !122
  %13 = call i32 @pthread_join(i64 %12, i8** null), !dbg !123
  call void @__VERIFIER_atomic_check(), !dbg !124
  ret i32 0, !dbg !125
}

; Function Attrs: nounwind
declare !callback !126 dso_local i32 @pthread_create(i64*, %union.pthread_attr_t*, i8* (i8*)*, i8*) #3

declare dso_local i32 @pthread_join(i64, i8**) #4

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable willreturn }
attributes #2 = { noreturn nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { noreturn nounwind }
attributes #6 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!40, !41, !42}
!llvm.ident = !{!43}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "__fence_var", scope: !2, file: !3, line: 22, type: !10, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 10.0.0-4ubuntu1 ", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !5, globals: !7, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "examples/litmus/C-litmus/CO-LB.c", directory: "/home/omkar/Downloads/nidhugg/src")
!4 = !{}
!5 = !{!6}
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!7 = !{!0, !8, !11, !13, !18, !20, !22, !24, !26, !28, !30, !32, !34, !36, !38}
!8 = !DIGlobalVariableExpression(var: !9, expr: !DIExpression())
!9 = distinct !DIGlobalVariable(name: "fr", scope: !2, file: !3, line: 23, type: !10, isLocal: false, isDefinition: true)
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !DIGlobalVariableExpression(var: !12, expr: !DIExpression())
!12 = distinct !DIGlobalVariable(name: "label", scope: !2, file: !3, line: 24, type: !10, isLocal: false, isDefinition: true)
!13 = !DIGlobalVariableExpression(var: !14, expr: !DIExpression())
!14 = distinct !DIGlobalVariable(name: "vars", scope: !2, file: !3, line: 9, type: !15, isLocal: false, isDefinition: true)
!15 = !DICompositeType(tag: DW_TAG_array_type, baseType: !10, size: 32, elements: !16)
!16 = !{!17}
!17 = !DISubrange(count: 1)
!18 = !DIGlobalVariableExpression(var: !19, expr: !DIExpression())
!19 = distinct !DIGlobalVariable(name: "atom_0_r0_2", scope: !2, file: !3, line: 10, type: !10, isLocal: false, isDefinition: true)
!20 = !DIGlobalVariableExpression(var: !21, expr: !DIExpression())
!21 = distinct !DIGlobalVariable(name: "atom_1_r0_1", scope: !2, file: !3, line: 11, type: !10, isLocal: false, isDefinition: true)
!22 = !DIGlobalVariableExpression(var: !23, expr: !DIExpression())
!23 = distinct !DIGlobalVariable(name: "v1", scope: !2, file: !3, line: 12, type: !10, isLocal: false, isDefinition: true)
!24 = !DIGlobalVariableExpression(var: !25, expr: !DIExpression())
!25 = distinct !DIGlobalVariable(name: "v2_r0", scope: !2, file: !3, line: 13, type: !10, isLocal: false, isDefinition: true)
!26 = !DIGlobalVariableExpression(var: !27, expr: !DIExpression())
!27 = distinct !DIGlobalVariable(name: "v3", scope: !2, file: !3, line: 14, type: !10, isLocal: false, isDefinition: true)
!28 = !DIGlobalVariableExpression(var: !29, expr: !DIExpression())
!29 = distinct !DIGlobalVariable(name: "v4_r0", scope: !2, file: !3, line: 15, type: !10, isLocal: false, isDefinition: true)
!30 = !DIGlobalVariableExpression(var: !31, expr: !DIExpression())
!31 = distinct !DIGlobalVariable(name: "v5", scope: !2, file: !3, line: 16, type: !10, isLocal: false, isDefinition: true)
!32 = !DIGlobalVariableExpression(var: !33, expr: !DIExpression())
!33 = distinct !DIGlobalVariable(name: "v6", scope: !2, file: !3, line: 17, type: !10, isLocal: false, isDefinition: true)
!34 = !DIGlobalVariableExpression(var: !35, expr: !DIExpression())
!35 = distinct !DIGlobalVariable(name: "v7_conj", scope: !2, file: !3, line: 18, type: !10, isLocal: false, isDefinition: true)
!36 = !DIGlobalVariableExpression(var: !37, expr: !DIExpression())
!37 = distinct !DIGlobalVariable(name: "v8", scope: !2, file: !3, line: 19, type: !10, isLocal: false, isDefinition: true)
!38 = !DIGlobalVariableExpression(var: !39, expr: !DIExpression())
!39 = distinct !DIGlobalVariable(name: "v9", scope: !2, file: !3, line: 20, type: !10, isLocal: false, isDefinition: true)
!40 = !{i32 7, !"Dwarf Version", i32 4}
!41 = !{i32 2, !"Debug Info Version", i32 3}
!42 = !{i32 1, !"wchar_size", i32 4}
!43 = !{!"clang version 10.0.0-4ubuntu1 "}
!44 = distinct !DISubprogram(name: "__VERIFIER_atomic_t0", scope: !3, file: !3, line: 26, type: !45, scopeLine: 26, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!45 = !DISubroutineType(types: !46)
!46 = !{null}
!47 = !DILocation(line: 27, column: 6, scope: !44)
!48 = !DILocation(line: 28, column: 9, scope: !44)
!49 = !DILocation(line: 28, column: 7, scope: !44)
!50 = !DILocation(line: 29, column: 8, scope: !44)
!51 = !DILocation(line: 30, column: 7, scope: !44)
!52 = !DILocation(line: 30, column: 13, scope: !44)
!53 = !DILocation(line: 30, column: 4, scope: !44)
!54 = !DILocation(line: 31, column: 13, scope: !44)
!55 = !DILocation(line: 31, column: 12, scope: !44)
!56 = !DILocation(line: 33, column: 1, scope: !44)
!57 = distinct !DISubprogram(name: "t0", scope: !3, file: !3, line: 35, type: !58, scopeLine: 35, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!58 = !DISubroutineType(types: !59)
!59 = !{!6, !6}
!60 = !DILocalVariable(name: "arg", arg: 1, scope: !57, file: !3, line: 35, type: !6)
!61 = !DILocation(line: 35, column: 16, scope: !57)
!62 = !DILocation(line: 36, column: 1, scope: !57)
!63 = !DILocation(line: 37, column: 1, scope: !57)
!64 = distinct !DISubprogram(name: "__VERIFIER_atomic_t1", scope: !3, file: !3, line: 41, type: !45, scopeLine: 41, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!65 = !DILocation(line: 42, column: 6, scope: !64)
!66 = !DILocation(line: 43, column: 9, scope: !64)
!67 = !DILocation(line: 43, column: 7, scope: !64)
!68 = !DILocation(line: 44, column: 8, scope: !64)
!69 = !DILocation(line: 45, column: 7, scope: !64)
!70 = !DILocation(line: 45, column: 13, scope: !64)
!71 = !DILocation(line: 45, column: 4, scope: !64)
!72 = !DILocation(line: 46, column: 13, scope: !64)
!73 = !DILocation(line: 46, column: 12, scope: !64)
!74 = !DILocation(line: 48, column: 1, scope: !64)
!75 = distinct !DISubprogram(name: "t1", scope: !3, file: !3, line: 50, type: !58, scopeLine: 50, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!76 = !DILocalVariable(name: "arg", arg: 1, scope: !75, file: !3, line: 50, type: !6)
!77 = !DILocation(line: 50, column: 16, scope: !75)
!78 = !DILocation(line: 51, column: 1, scope: !75)
!79 = !DILocation(line: 52, column: 1, scope: !75)
!80 = distinct !DISubprogram(name: "__VERIFIER_atomic_check", scope: !3, file: !3, line: 56, type: !45, scopeLine: 56, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!81 = !DILocation(line: 57, column: 6, scope: !80)
!82 = !DILocation(line: 57, column: 4, scope: !80)
!83 = !DILocation(line: 58, column: 6, scope: !80)
!84 = !DILocation(line: 58, column: 4, scope: !80)
!85 = !DILocation(line: 59, column: 11, scope: !80)
!86 = !DILocation(line: 59, column: 16, scope: !80)
!87 = !DILocation(line: 59, column: 14, scope: !80)
!88 = !DILocation(line: 59, column: 9, scope: !80)
!89 = !DILocation(line: 60, column: 7, scope: !90)
!90 = distinct !DILexicalBlock(scope: !80, file: !3, line: 60, column: 7)
!91 = !DILocation(line: 60, column: 15, scope: !90)
!92 = !DILocation(line: 60, column: 7, scope: !80)
!93 = !DILocation(line: 60, column: 21, scope: !94)
!94 = distinct !DILexicalBlock(scope: !95, file: !3, line: 60, column: 21)
!95 = distinct !DILexicalBlock(scope: !90, file: !3, line: 60, column: 21)
!96 = !DILocation(line: 62, column: 1, scope: !80)
!97 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 64, type: !98, scopeLine: 64, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!98 = !DISubroutineType(types: !99)
!99 = !{!10, !10, !100}
!100 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !101, size: 64)
!101 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !102, size: 64)
!102 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!103 = !DILocalVariable(name: "argc", arg: 1, scope: !97, file: !3, line: 64, type: !10)
!104 = !DILocation(line: 64, column: 14, scope: !97)
!105 = !DILocalVariable(name: "argv", arg: 2, scope: !97, file: !3, line: 64, type: !100)
!106 = !DILocation(line: 64, column: 27, scope: !97)
!107 = !DILocalVariable(name: "thr0", scope: !97, file: !3, line: 65, type: !108)
!108 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !109, line: 27, baseType: !110)
!109 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h", directory: "")
!110 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!111 = !DILocation(line: 65, column: 13, scope: !97)
!112 = !DILocalVariable(name: "thr1", scope: !97, file: !3, line: 66, type: !108)
!113 = !DILocation(line: 66, column: 13, scope: !97)
!114 = !DILocation(line: 68, column: 8, scope: !97)
!115 = !DILocation(line: 69, column: 12, scope: !97)
!116 = !DILocation(line: 70, column: 12, scope: !97)
!117 = !DILocation(line: 71, column: 15, scope: !97)
!118 = !DILocation(line: 73, column: 3, scope: !97)
!119 = !DILocation(line: 74, column: 3, scope: !97)
!120 = !DILocation(line: 76, column: 16, scope: !97)
!121 = !DILocation(line: 76, column: 3, scope: !97)
!122 = !DILocation(line: 77, column: 16, scope: !97)
!123 = !DILocation(line: 77, column: 3, scope: !97)
!124 = !DILocation(line: 79, column: 1, scope: !97)
!125 = !DILocation(line: 80, column: 3, scope: !97)
!126 = !{!127}
!127 = !{i64 2, i64 3, i1 false}
