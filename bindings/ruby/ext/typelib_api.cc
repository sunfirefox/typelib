#include "typelib.hh"

extern "C" {
#include <dl.h>
}
#include <typelib/typevisitor.hh>
#include <typelib/pluginmanager.hh>
#include <typelib/importer.hh>
#include <utilmm/configfile/configset.hh>

using namespace Typelib;
using utilmm::config_set;
using std::string;

static VALUE mTypelib   = Qnil;

/**********************************************************************
 *
 * Extension of the standard libraries
 *
 */

static VALUE kernel_is_immediate(VALUE klass, VALUE object)
{ return IMMEDIATE_P(object) ? Qtrue : Qfalse; }
static VALUE dl_ptr_to_ptr(VALUE ptr)
{
    VALUE newptr = rb_dlptr_malloc(sizeof(void*), free);
    *reinterpret_cast<void**>(rb_dlptr2cptr(newptr)) = rb_dlptr2cptr(ptr);
    // Protect ptr against GC
    rb_iv_set(newptr, "@points_on", newptr);
    return newptr;
}

extern "C" void Init_typelib_api()
{
    mTypelib  = rb_define_module("Typelib");
    Typelib_init_functions(mTypelib);
    Typelib_init_values(mTypelib);
    Typelib_init_strings();
    Typelib_init_registry(mTypelib);
    
    rb_define_method(rb_mKernel, "immediate?", RUBY_METHOD_FUNC(kernel_is_immediate), 1);
    rb_define_method(rb_cDLPtrData, "to_ptr", RUBY_METHOD_FUNC(dl_ptr_to_ptr), 0);
}

