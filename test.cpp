#include <iostream>
#include <wasmedge/wasmedge.h>
using namespace std;
int main() {
    int64_t a,b;
    cout<<"Enter the integers:";
    cin>>a>>b;
    WasmEdge_Result res;
    WasmEdge_VMContext *vm_context=WasmEdge_VMCreate(NULL, NULL);
    if(!vm_context){
    cerr<<"Failed to create VM\n";
    return -1;
    }
    WasmEdge_String lib=WasmEdge_StringCreateByCString("lib");
    res= WasmEdge_VMRegisterModuleFromFile(vm_context,lib, "lib.wasm");
    WasmEdge_StringDelete(lib); 
    if(!WasmEdge_ResultOK(res)){
        cerr<<"Error registering lib.wasm: "<<WasmEdge_ResultGetMessage(res)<<"\n";
        WasmEdge_VMDelete(vm_context);
        return -1;
    }
    res=WasmEdge_VMLoadWasmFromFile(vm_context, "calc.wasm");
    if(!WasmEdge_ResultOK(res)){
        cerr<<"Error loading test.wasm: "<<WasmEdge_ResultGetMessage(res)<<"\n";
        WasmEdge_VMDelete(vm_context);
        return -1;
    }
    res=WasmEdge_VMValidate(vm_context);
    if(!WasmEdge_ResultOK(res)){
        cerr<<"Error validating test.wasm: "<<WasmEdge_ResultGetMessage(res)<<"\n";
        WasmEdge_VMDelete(vm_context);
        return -1;
    }
    res=WasmEdge_VMInstantiate(vm_context);
    if(!WasmEdge_ResultOK(res)){
        cerr<<"Error instantiating test.wasm: "<<WasmEdge_ResultGetMessage(res)<<"\n";
        WasmEdge_VMDelete(vm_context);
        return -1;
    }
    WasmEdge_String function=WasmEdge_StringCreateByCString("add_and_square");
    WasmEdge_Value args[2],returns[1];
    args[0] = WasmEdge_ValueGenI64(a);
    args[1] = WasmEdge_ValueGenI64(b);
    res=WasmEdge_VMExecute(vm_context,function,args,2,returns,1);
    WasmEdge_StringDelete(function);
    if(WasmEdge_ResultOK(res)){
        int64_t answer= WasmEdge_ValueGetI64(returns[0]);
        cout<<"Calculation: ("<<a<<" + "<<b<<")^2"<<"\n";
        cout<<"Result: "<<answer<<"\n";
    }else{
        cerr<<"Execution Error: "<<WasmEdge_ResultGetMessage(res)<<"\n";
    }
    WasmEdge_VMDelete(vm_context);
    return 0;
}