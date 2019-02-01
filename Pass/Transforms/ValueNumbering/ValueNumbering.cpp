#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include <string>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "ValueNumbering"

using namespace llvm;

namespace {
struct ValueNumbering : public FunctionPass {
  string func_name = "test";
  static char ID;
  ValueNumbering() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    errs() << "ValueNumbering: ";
    errs() << F.getName() << "\n";
        if (F.getName() != func_name) return false;

        for (auto& basic_block : F)
        {

            for (auto& inst : basic_block)
            {
            	errs() << inst << "\n";
              if(inst.getOpcode() == Instruction::Load){
                      errs() << "This is Load"<<"\n";
              }
              if(inst.getOpcode() == Instruction::Store){
                      errs() << "This is Store"<<"\n";
              }
                if (inst.isBinaryOp())
                {
                    errs() << "Op Code:" << inst.getOpcodeName()<<"\n"; // print opcode name
                    if(inst.getOpcode() == Instruction::Add){
                      errs() << "This is Addition"<<"\n";
                    }
                    if(inst.getOpcode() == Instruction::Add){
                      errs() << "This is Addition"<<"\n";
                    }
                    if(inst.getOpcode() == Instruction::Mul){
                      errs() << "This is Multiplication"<<"\n";
                    }
                    
                    // See Other classes, Instruction::Sub, Instruction::UDiv, Instruction::SDiv
                //	errs() << "Operand(0)" << (*inst.getOperand(0))<<"\n";
                    auto* ptr = dyn_cast<User>(&inst);
					//errs() << "\t" << *ptr << "\n";
                    for (auto it = ptr->op_begin(); it != ptr->op_end(); ++it) {
                        errs() << "\t" <<  *(*it) << "\n";
                        //if ((*it)->hasName()) errs() << (*it)->getName() << "\n";                      
                    }
                }
            }
        }
    return false;
  }
}; // end of struct ValueNumbering
}  // end of anonymous namespace

char ValueNumbering::ID = 0;
static RegisterPass<ValueNumbering> X("ValueNumbering", "ValueNumbering Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);