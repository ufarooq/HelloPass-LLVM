# HelloPass-LLVM
Getting started for LLVM pass writing

## Setup and Run
1. Clone this repo, see the file details below:
- [Pass](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass "Pass") - root directory for build and source code
   - [Transforms](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass/Transforms "Transforms") is top level directory for project.
   - [CMakeLists.txt](https://github.com/ufarooq/HelloPass-LLVM/blob/master/Pass/Transforms/CMakeLists.txt "CMakeLists.txt")  file is CMakeLists for the Project.
     - [ValueNumbering](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass/Transforms/ValueNumbering "ValueNumbering") is top level directory for the Pass
	     - [ValueNumbering.cpp](https://github.com/ufarooq/HelloPass-LLVM/blob/master/Pass/Transforms/ValueNumbering/ValueNumbering.cpp "ValueNumbering.cpp") contains code for the Pass
	     - [CMakeLists.txt](https://github.com/ufarooq/HelloPass-LLVM/blob/master/Pass/Transforms/ValueNumbering/CMakeLists.txt "CMakeLists.txt") is CMakeLists for this Pass
   - [build](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass/build "build") will build Pass in this directory
- [test](https://github.com/ufarooq/HelloPass-LLVM/tree/master/test "test") contains Test code
2. For Mac OSX users, uncomment the following line in Pass/Transforms/ValueNumbering/CMakeLists.txt
```bash
SET(CMAKE_MODULE_LINKER_FLAGS "-undefined dynamic_lookup")
```
3. Move to [Pass](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass)/**build**/ directory using cd command on your local system. Next, execute the following command. If it executes successfully, proceed to next step.
```bash
cmake -DCMAKE_BUILD_TYPE=Release ../Transforms/ValueNumbering
```
4. Next execute make and it will generate *.so files under build directory. 
```bash
make -j4
```
5. Move to **test**/ directory and generate ***test.ll*** file for test.c using following command. 
```c++
clang -S -emit-llvm test.c -o test.ll
```
6. After generating test.ll, run the following command to test the LLVM Pass. 
```bash
opt -load ../Pass/build/libLLVMValueNumberingPass.so  -ValueNumbering test.ll
```
## Code Explanation 
1. The implemented Pass extends from ``FunctionPass`` class and overrides ``runOnFunction(Function &F)`` function.
2. ``runOnFunction(Function &F)`` function gets called for each function in the test code. Name of the function being analyzed is accessible using the following code snippet. 
```c++
bool runOnFunction(Function &F) override {
	F.getName();
}
```
3. We can iterate over basic blocks of the given function as:
```c++
bool runOnFunction(Function &F) override {
	for (auto& basic_block : F)
	{
		...
	}
}
```
4. Next, we can iterate over the instructions in a basic block (BB). **Note:** instructions are in LLVM IR.
```c++
bool runOnFunction(Function &F) override {
	for (auto& basic_block : F)
	{
		for (auto& inst : basic_block)
		{
			...
		}
	}
}
```
5. Once we get an instruction, then we can cast it as ``User`` and iterate over operands of that instruction. 
```c++
auto* ptr = dyn_cast<User>(&inst);
for (auto it = ptr->op_begin(); it != ptr->op_end(); ++it) 
{
...
}
```
6. Use Following API to check whether instruction is a binary operation (Assignment)
```c++
if (inst.isBinaryOp())
{
	...
}
```
7. Use Following APIs to compare and find operator types
```c++

if (inst.isBinaryOp())
{
	inst.getOpcodeName(); //prints OpCode by name such as add, mul etc.
	if(inst.getOpcode() == Instruction::Add)
	{
		errs() << "This is Addition"<<"\n";
	}
	if(inst.getOpcode() == Instruction::Mul)
	{
		errs() << "This is Multiplication"<<"\n";
	}
    // See Other classes Instruction::Sub, Instruction::UDiv, Instruction::SDiv
}
```
8. Implementation of ``runOnFunction(Function &F)`` looks as following in whole.  
```c++
string func_name = "test";
bool runOnFunction(Function &F) override {
	errs() << "ValueNumbering: ";
	errs() << F.getName() << "\n";
	if (F.getName() != func_name) return false;
	for (auto& basic_block : F)
	{
		for (auto& inst : basic_block)
		{
			errs() << inst << "\n";
			auto* ptr = dyn_cast<User>(&inst);
			for (auto it = ptr->op_begin(); it != ptr->op_end(); ++it) 
			{
				errs() << "\t" << *(*it) << "\n";
			}
		}
	}
	return false;
}
```
