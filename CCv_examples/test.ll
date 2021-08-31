; ModuleID = 'examples/test5.c'
source_filename = "examples/test5.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@x = dso_local global i32 0, align 4, !dbg !0
@y = dso_local global i32 0, align 4, !dbg !8
@z = dso_local global i32 0, align 4, !dbg !11
@w = dso_local global i32 0, align 4, !dbg !13
@f = dso_local global i32 0, align 4, !dbg !15
@.str = private unnamed_addr constant [8 x i8] c"x1:=%d \00", align 1
@.str.1 = private unnamed_addr constant [8 x i8] c"y2:=%d \00", align 1
@.str.2 = private unnamed_addr constant [8 x i8] c"f4:=%d \00", align 1
@.str.3 = private unnamed_addr constant [8 x i8] c"f5:=%d \00", align 1
@.str.4 = private unnamed_addr constant [8 x i8] c"y6:=%d \00", align 1
@.str.5 = private unnamed_addr constant [2 x i8] c" \00", align 1
@.str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__VERIFIER_atomic_t1() #0 !dbg !21 {
  store i32 1, i32* @x, align 4, !dbg !24
  %1 = load i32, i32* @x, align 4, !dbg !25
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i64 0, i64 0), i32 %1), !dbg !26
  store i32 1, i32* @w, align 4, !dbg !27
  ret void, !dbg !28
}

declare dso_local i32 @printf(i8*, ...) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__VERIFIER_atomic_t2() #0 !dbg !29 {
  %1 = load i32, i32* @y, align 4, !dbg !30
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.1, i64 0, i64 0), i32 %1), !dbg !31
  store i32 2, i32* @y, align 4, !dbg !32
  %3 = load i32, i32* @y, align 4, !dbg !33
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.1, i64 0, i64 0), i32 %3), !dbg !34
  ret void, !dbg !35
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__VERIFIER_atomic_t4() #0 !dbg !36 {
  store i32 4, i32* @f, align 4, !dbg !37
  %1 = load i32, i32* @f, align 4, !dbg !38
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.2, i64 0, i64 0), i32 %1), !dbg !39
  ret void, !dbg !40
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__VERIFIER_atomic_t5() #0 !dbg !41 {
  store i32 5, i32* @f, align 4, !dbg !42
  %1 = load i32, i32* @f, align 4, !dbg !43
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.3, i64 0, i64 0), i32 %1), !dbg !44
  ret void, !dbg !45
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @__VERIFIER_atomic_t6() #0 !dbg !46 {
  store i32 6, i32* @y, align 4, !dbg !47
  %1 = load i32, i32* @y, align 4, !dbg !48
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.4, i64 0, i64 0), i32 %1), !dbg !49
  ret void, !dbg !50
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @thr1(i8* %0) #0 !dbg !51 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  call void @llvm.dbg.declare(metadata i8** %2, metadata !54, metadata !DIExpression()), !dbg !55
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.5, i64 0, i64 0)), !dbg !56
  call void @__VERIFIER_atomic_t1(), !dbg !57
  call void @__VERIFIER_atomic_t2(), !dbg !58
  ret i8* null, !dbg !59
}

; Function Attrs: nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @thr2(i8* %0) #0 !dbg !60 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  call void @llvm.dbg.declare(metadata i8** %2, metadata !61, metadata !DIExpression()), !dbg !62
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.5, i64 0, i64 0)), !dbg !63
  call void @__VERIFIER_atomic_t4(), !dbg !64
  ret i8* null, !dbg !65
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @thr3(i8* %0) #0 !dbg !66 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  call void @llvm.dbg.declare(metadata i8** %2, metadata !67, metadata !DIExpression()), !dbg !68
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.5, i64 0, i64 0)), !dbg !69
  call void @__VERIFIER_atomic_t5(), !dbg !70
  call void @__VERIFIER_atomic_t6(), !dbg !71
  ret i8* null, !dbg !72
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %0, i8** %1) #0 !dbg !73 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  call void @llvm.dbg.declare(metadata i32* %4, metadata !79, metadata !DIExpression()), !dbg !80
  store i8** %1, i8*** %5, align 8
  call void @llvm.dbg.declare(metadata i8*** %5, metadata !81, metadata !DIExpression()), !dbg !82
  call void @llvm.dbg.declare(metadata i64* %6, metadata !83, metadata !DIExpression()), !dbg !87
  call void @llvm.dbg.declare(metadata i64* %7, metadata !88, metadata !DIExpression()), !dbg !89
  call void @llvm.dbg.declare(metadata i64* %8, metadata !90, metadata !DIExpression()), !dbg !91
  %9 = call i32 @pthread_create(i64* %6, %union.pthread_attr_t* null, i8* (i8*)* @thr1, i8* null) #4, !dbg !92
  %10 = call i32 @pthread_create(i64* %7, %union.pthread_attr_t* null, i8* (i8*)* @thr2, i8* null) #4, !dbg !93
  %11 = call i32 @pthread_create(i64* %8, %union.pthread_attr_t* null, i8* (i8*)* @thr3, i8* null) #4, !dbg !94
  %12 = load i64, i64* %6, align 8, !dbg !95
  %13 = call i32 @pthread_join(i64 %12, i8** null), !dbg !96
  %14 = load i64, i64* %7, align 8, !dbg !97
  %15 = call i32 @pthread_join(i64 %14, i8** null), !dbg !98
  %16 = load i64, i64* %8, align 8, !dbg !99
  %17 = call i32 @pthread_join(i64 %16, i8** null), !dbg !100
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i64 0, i64 0)), !dbg !101
  ret i32 0, !dbg !102
}

; Function Attrs: nounwind
declare !callback !103 dso_local i32 @pthread_create(i64*, %union.pthread_attr_t*, i8* (i8*)*, i8*) #3

declare dso_local i32 @pthread_join(i64, i8**) #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readnone speculatable willreturn }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!17, !18, !19}
!llvm.ident = !{!20}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "x", scope: !2, file: !3, line: 5, type: !10, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 10.0.0-4ubuntu1 ", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !5, globals: !7, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "examples/test5.c", directory: "/home/omkar/Downloads/nidhugg/src")
!4 = !{}
!5 = !{!6}
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!7 = !{!0, !8, !11, !13, !15}
!8 = !DIGlobalVariableExpression(var: !9, expr: !DIExpression())
!9 = distinct !DIGlobalVariable(name: "y", scope: !2, file: !3, line: 5, type: !10, isLocal: false, isDefinition: true)
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !DIGlobalVariableExpression(var: !12, expr: !DIExpression())
!12 = distinct !DIGlobalVariable(name: "z", scope: !2, file: !3, line: 5, type: !10, isLocal: false, isDefinition: true)
!13 = !DIGlobalVariableExpression(var: !14, expr: !DIExpression())
!14 = distinct !DIGlobalVariable(name: "w", scope: !2, file: !3, line: 5, type: !10, isLocal: false, isDefinition: true)
!15 = !DIGlobalVariableExpression(var: !16, expr: !DIExpression())
!16 = distinct !DIGlobalVariable(name: "f", scope: !2, file: !3, line: 5, type: !10, isLocal: false, isDefinition: true)
!17 = !{i32 7, !"Dwarf Version", i32 4}
!18 = !{i32 2, !"Debug Info Version", i32 3}
!19 = !{i32 1, !"wchar_size", i32 4}
!20 = !{!"clang version 10.0.0-4ubuntu1 "}
!21 = distinct !DISubprogram(name: "__VERIFIER_atomic_t1", scope: !3, file: !3, line: 8, type: !22, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!22 = !DISubroutineType(types: !23)
!23 = !{null}
!24 = !DILocation(line: 9, column: 4, scope: !21)
!25 = !DILocation(line: 11, column: 21, scope: !21)
!26 = !DILocation(line: 11, column: 2, scope: !21)
!27 = !DILocation(line: 13, column: 4, scope: !21)
!28 = !DILocation(line: 14, column: 1, scope: !21)
!29 = distinct !DISubprogram(name: "__VERIFIER_atomic_t2", scope: !3, file: !3, line: 16, type: !22, scopeLine: 16, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!30 = !DILocation(line: 19, column: 20, scope: !29)
!31 = !DILocation(line: 19, column: 2, scope: !29)
!32 = !DILocation(line: 20, column: 7, scope: !29)
!33 = !DILocation(line: 21, column: 23, scope: !29)
!34 = !DILocation(line: 21, column: 5, scope: !29)
!35 = !DILocation(line: 22, column: 1, scope: !29)
!36 = distinct !DISubprogram(name: "__VERIFIER_atomic_t4", scope: !3, file: !3, line: 25, type: !22, scopeLine: 25, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!37 = !DILocation(line: 27, column: 4, scope: !36)
!38 = !DILocation(line: 28, column: 20, scope: !36)
!39 = !DILocation(line: 28, column: 2, scope: !36)
!40 = !DILocation(line: 33, column: 1, scope: !36)
!41 = distinct !DISubprogram(name: "__VERIFIER_atomic_t5", scope: !3, file: !3, line: 35, type: !22, scopeLine: 35, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!42 = !DILocation(line: 36, column: 4, scope: !41)
!43 = !DILocation(line: 37, column: 20, scope: !41)
!44 = !DILocation(line: 37, column: 2, scope: !41)
!45 = !DILocation(line: 39, column: 1, scope: !41)
!46 = distinct !DISubprogram(name: "__VERIFIER_atomic_t6", scope: !3, file: !3, line: 41, type: !22, scopeLine: 41, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!47 = !DILocation(line: 43, column: 4, scope: !46)
!48 = !DILocation(line: 44, column: 20, scope: !46)
!49 = !DILocation(line: 44, column: 2, scope: !46)
!50 = !DILocation(line: 48, column: 1, scope: !46)
!51 = distinct !DISubprogram(name: "thr1", scope: !3, file: !3, line: 52, type: !52, scopeLine: 52, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!52 = !DISubroutineType(types: !53)
!53 = !{!6, !6}
!54 = !DILocalVariable(name: "arg", arg: 1, scope: !51, file: !3, line: 52, type: !6)
!55 = !DILocation(line: 52, column: 18, scope: !51)
!56 = !DILocation(line: 53, column: 2, scope: !51)
!57 = !DILocation(line: 54, column: 3, scope: !51)
!58 = !DILocation(line: 55, column: 3, scope: !51)
!59 = !DILocation(line: 57, column: 2, scope: !51)
!60 = distinct !DISubprogram(name: "thr2", scope: !3, file: !3, line: 59, type: !52, scopeLine: 59, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!61 = !DILocalVariable(name: "arg", arg: 1, scope: !60, file: !3, line: 59, type: !6)
!62 = !DILocation(line: 59, column: 18, scope: !60)
!63 = !DILocation(line: 60, column: 2, scope: !60)
!64 = !DILocation(line: 62, column: 2, scope: !60)
!65 = !DILocation(line: 63, column: 2, scope: !60)
!66 = distinct !DISubprogram(name: "thr3", scope: !3, file: !3, line: 66, type: !52, scopeLine: 66, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!67 = !DILocalVariable(name: "arg", arg: 1, scope: !66, file: !3, line: 66, type: !6)
!68 = !DILocation(line: 66, column: 18, scope: !66)
!69 = !DILocation(line: 67, column: 2, scope: !66)
!70 = !DILocation(line: 68, column: 2, scope: !66)
!71 = !DILocation(line: 69, column: 2, scope: !66)
!72 = !DILocation(line: 70, column: 2, scope: !66)
!73 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 74, type: !74, scopeLine: 74, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!74 = !DISubroutineType(types: !75)
!75 = !{!10, !10, !76}
!76 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !77, size: 64)
!77 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !78, size: 64)
!78 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!79 = !DILocalVariable(name: "argc", arg: 1, scope: !73, file: !3, line: 74, type: !10)
!80 = !DILocation(line: 74, column: 14, scope: !73)
!81 = !DILocalVariable(name: "argv", arg: 2, scope: !73, file: !3, line: 74, type: !76)
!82 = !DILocation(line: 74, column: 26, scope: !73)
!83 = !DILocalVariable(name: "t1", scope: !73, file: !3, line: 75, type: !84)
!84 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !85, line: 27, baseType: !86)
!85 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h", directory: "")
!86 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!87 = !DILocation(line: 75, column: 12, scope: !73)
!88 = !DILocalVariable(name: "t2", scope: !73, file: !3, line: 75, type: !84)
!89 = !DILocation(line: 75, column: 15, scope: !73)
!90 = !DILocalVariable(name: "t3", scope: !73, file: !3, line: 75, type: !84)
!91 = !DILocation(line: 75, column: 18, scope: !73)
!92 = !DILocation(line: 76, column: 2, scope: !73)
!93 = !DILocation(line: 77, column: 2, scope: !73)
!94 = !DILocation(line: 78, column: 2, scope: !73)
!95 = !DILocation(line: 79, column: 15, scope: !73)
!96 = !DILocation(line: 79, column: 2, scope: !73)
!97 = !DILocation(line: 80, column: 15, scope: !73)
!98 = !DILocation(line: 80, column: 2, scope: !73)
!99 = !DILocation(line: 81, column: 15, scope: !73)
!100 = !DILocation(line: 81, column: 2, scope: !73)
!101 = !DILocation(line: 82, column: 2, scope: !73)
!102 = !DILocation(line: 83, column: 2, scope: !73)
!103 = !{!104}
!104 = !{i64 2, i64 3, i1 false}
