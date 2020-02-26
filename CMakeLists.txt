cmake_minimum_required(VERSION 3.16.4)
project(matbar)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD   11)
set(MB_EXECUTABLE "matbar")

include_directories(
    include
    compiler/asmjit/src
)

add_subdirectory(
    compiler/asmjit
)

add_executable(
    ${MB_EXECUTABLE}
    common/Controller.cc
    common/Debugger.cc
    common/FunctionContext.cc
    common/IRBasic.cc
    common/LineReader.cc
    common/Main.cc
    common/Throwable.cc
    compiler/IROptimizer.cc
    compiler/IRTranslator.cc
    IR/IRGenerator.cc
    IR/RuntimePool.cc
    IR/VFManager.cc
    IRVM/IRExecution.cc
    parser/y.tab.cc
    parser/lex.yy.cc
)

link_directories(compiler/asmjit)
target_link_libraries(${MB_EXECUTABLE} asmjit)
# target_link_libraries(${MB_EXECUTABLE} rt)
target_link_libraries(${MB_EXECUTABLE} readline)
