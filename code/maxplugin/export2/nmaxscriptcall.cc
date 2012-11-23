
#include "stdneb.h"
#include <maxscrpt/maxscrpt.h>
#include <maxscrpt/strings.h>
#include <maxscrpt/Parser.h>

#include "export2/nmax.h"
#include "export2/nmaxscriptcall.h"

//-----------------------------------------------------------------------------
/**
    Call max script and evaluate it in C++.

    @param script script string which to be evaluate.
*/
bool nMaxScriptCall(const char* script)
{
    TCHAR* cmds = (TCHAR*)script;

    static bool initialized = false;
    if (false == initialized)
    {
        init_MAXScript();
        initialized = true;
    }

    init_thread_locals();

    push_alloc_frame();

    two_typed_value_locals(StringStream* ss, Value* result);

    vl.ss = new StringStream(cmds);

    //bool ret = false;
    try 
    {
        //vl.result = LocalExecuteScript(vl.ss,&res);
        //res = true;
        CharStream* source = vl.ss;

        init_thread_locals();
        push_alloc_frame();
        three_typed_value_locals(Parser* parser, Value* code, Value* result);
        CharStream* out = thread_local(current_stdout);
        vl.parser = new Parser (out);

        try 
        {
            source->flush_whitespace();

            while (!source->at_eos()) 
            {
                vl.code   = vl.parser->compile(source);

                // evaluate the given script.
                vl.result = vl.code->eval()->get_heap_ptr();

                source->flush_whitespace();
            }
            source->close();
        } 
        catch (...) 
        {
            return false;
        }

        if (vl.result == NULL)
            vl.result = &ok;

        pop_alloc_frame();
        //return_value(vl.result);
    }
    catch(...)
    {
        return false;
    }

    thread_local(current_result) = vl.result;
    thread_local(current_locals_frame) = vl.link;
    pop_alloc_frame();

    return true;
}