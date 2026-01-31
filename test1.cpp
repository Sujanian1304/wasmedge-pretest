#include <iostream>
#include <wasmedge/wasmedge.h>
using namespace std;
int main() {
    int64_t a,b;
    cout<<"Enter the integers:";
    cin>>a>>b;
    WasmEdge_Result res;
    WasmEdge_LoaderContext *LoadCxt=WasmEdge_LoaderCreate(NULL);
    WasmEdge_ValidatorContext *ValidCxt=WasmEdge_ValidatorCreate(NULL);
    WasmEdge_ExecutorContext *ExecCxt=WasmEdge_ExecutorCreate(NULL,NULL);
    WasmEdge_StoreContext *StoreCxt=WasmEdge_StoreCreate();
    WasmEdge_ASTModuleContext *LibASTCxt=NULL;
    WasmEdge_ModuleInstanceContext *LibInstCxt=NULL;
    res=WasmEdge_LoaderParseFromFile(LoadCxt,&LibASTCxt,"lib.wasm");
    if (!WasmEdge_ResultOK(res)) {
        cerr<<"Load lib.wasm error: "<<WasmEdge_ResultGetMessage(res)<<"\n";
        return -1;
    }
    res=WasmEdge_ValidatorValidate(ValidCxt,LibASTCxt);
    if (!WasmEdge_ResultOK(res)) {
        cerr<<"Validate lib.wasm error: "<<WasmEdge_ResultGetMessage(res)<<"\n";
        return -1;
    }
    WasmEdge_String lib=WasmEdge_StringCreateByCString("lib");
    res=WasmEdge_ExecutorRegister(ExecCxt,&LibInstCxt,StoreCxt,LibASTCxt,lib);
    WasmEdge_StringDelete(lib);
    WasmEdge_ASTModuleDelete(LibASTCxt);
    if (!WasmEdge_ResultOK(res)) {
        cerr<<"Instantiate lib.wasm error: "<<WasmEdge_ResultGetMessage(res)<<"\n";
        return -1;
    }
    WasmEdge_ASTModuleContext *TestASTCxt=NULL;
    WasmEdge_ModuleInstanceContext *TestInstCxt=NULL;
    res=WasmEdge_LoaderParseFromFile(LoadCxt, &TestASTCxt,"calc.wasm");
    if (!WasmEdge_ResultOK(res)) {
        cerr<<"Load test.wasm error: "<<WasmEdge_ResultGetMessage(res)<<"\n";
        return -1;
    }
    res=WasmEdge_ValidatorValidate(ValidCxt,TestASTCxt);
    if (!WasmEdge_ResultOK(res)) {
        cerr<<"Validate test.wasm error: "<<WasmEdge_ResultGetMessage(res)<<"\n";
        return -1;
    }
    res=WasmEdge_ExecutorInstantiate(ExecCxt,&TestInstCxt,StoreCxt,TestASTCxt);
    WasmEdge_ASTModuleDelete(TestASTCxt);
    if (!WasmEdge_ResultOK(res)) {
        cerr<< "Instantiate test.wasm error: "<<WasmEdge_ResultGetMessage(res)<<"\n";
        return -1;
    }
    WasmEdge_String function=WasmEdge_StringCreateByCString("add_and_square");
    WasmEdge_FunctionInstanceContext *FuncCxt=WasmEdge_ModuleInstanceFindFunction(TestInstCxt,function);
    WasmEdge_StringDelete(function);
    if (FuncCxt==NULL) {
        cout<<"Function 'add_and_square' not found.\n";
        return -1;
    }
    WasmEdge_Value args[2],returns[1];
    args[0]=WasmEdge_ValueGenI64(a);
    args[1]=WasmEdge_ValueGenI64(b);
    res=WasmEdge_ExecutorInvoke(ExecCxt, FuncCxt,args, 2,returns, 1);
    if(WasmEdge_ResultOK(res)){
        int64_t answer= WasmEdge_ValueGetI64(returns[0]);
        cout<<"Calculation: ("<<a<<" + "<<b<<")^2"<<"\n";
        cout<<"Result: "<<answer<<"\n";
    }else{
        cerr<<"Execution Error: "<<WasmEdge_ResultGetMessage(res)<<"\n";
    }
    WasmEdge_LoaderDelete(LoadCxt);
    WasmEdge_ValidatorDelete(ValidCxt);
    WasmEdge_ExecutorDelete(ExecCxt);
    WasmEdge_StoreDelete(StoreCxt);
    WasmEdge_ModuleInstanceDelete(LibInstCxt);
    WasmEdge_ModuleInstanceDelete(TestInstCxt);
    return 0;
}
