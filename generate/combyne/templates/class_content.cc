// This is a generated file, modify: generate/templates/class_content.cc.
#include <nan.h>
#include <string.h>

extern "C" {
  #include <git2.h>
  {% each cDependencies as dependency %}
    #include <{{ dependency }}>
  {% endeach %}
}


#include "../include/functions/copy.h"
#include "../include/macros.h"
#include "../include/{{ filename }}.h"

{% each dependencies as dependency %}
  #include "{{ dependency }}"
{% endeach %}

#include <iostream>

using namespace std;
using namespace v8;
using namespace node;

{% if cType %}
  {{ cppClassName }}::{{ cppClassName }}({{ cType }} *raw, bool selfFreeing) {
    this->raw = raw;
    this->selfFreeing = selfFreeing;
  }

  {{ cppClassName }}::~{{ cppClassName }}() {
    {% if freeFunctionName %}
      if (this->selfFreeing) {
        {{ freeFunctionName }}(this->raw);
      }
    {% endif %}
  }

  void {{ cppClassName }}::InitializeComponent(Handle<v8::Object> target) {
    NanScope();

    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);

    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(NanNew<String>("{{ jsClassName }}"));

    {% each functions as function %}
      {% if not function.ignore %}
        {% if function.isPrototypeMethod %}
          NODE_SET_PROTOTYPE_METHOD(tpl, "{{ function.jsFunctionName }}", {{ function.cppFunctionName }});
        {% else %}
          NODE_SET_METHOD(tpl, "{{ function.jsFunctionName }}", {{ function.cppFunctionName }});
        {% endif %}
      {% endif %}
    {% endeach %}

    {% each fields as field %}
      {% if not field.ignore %}
        NODE_SET_PROTOTYPE_METHOD(tpl, "{{ field.jsFunctionName }}", {{ field.cppFunctionName }});
      {% endif %}
    {% endeach %}

    Local<Function> _constructor_template = tpl->GetFunction();
    NanAssignPersistent(constructor_template, _constructor_template);
    target->Set(NanNew<String>("{{ jsClassName }}"), _constructor_template);
  }

  NAN_METHOD({{ cppClassName }}::New) {
    NanScope();

    if (args.Length() == 0 || !args[0]->IsExternal()) {
      {% if createFunctionName %}
        return NanThrowError("A new {{ cppClassName }} cannot be instantiated. Use {{ jsCreateFunctionName }} instead.");
      {% else %}
        return NanThrowError("A new {{ cppClassName }} cannot be instantiated.");
      {% endif %}
    }

    {{ cppClassName }}* object = new {{ cppClassName }}(static_cast<{{ cType }} *>(Handle<External>::Cast(args[0])->Value()), args[1]->BooleanValue());
    object->Wrap(args.This());

    NanReturnValue(args.This());
  }

  Handle<Value> {{ cppClassName }}::New(void *raw, bool selfFreeing) {
    NanEscapableScope();
    Handle<Value> argv[2] = { NanNew<External>((void *)raw), NanNew<Boolean>(selfFreeing) };
    return NanEscapeScope(NanNew<Function>({{ cppClassName }}::constructor_template)->NewInstance(2, argv));
  }

  {{ cType }} *{{ cppClassName }}::GetValue() {
    return this->raw;
  }

  {{ cType }} **{{ cppClassName }}::GetRefValue() {
    return &this->raw;
  }

{% else %}

  void {{ cppClassName }}::InitializeComponent(Handle<v8::Object> target) {
    NanScope();

    Local<Object> object = NanNew<Object>();

    {% each functions as function %}
      {% if not function.ignore %}
        NODE_SET_METHOD(object, "{{ function.jsFunctionName }}", {{ function.cppFunctionName }});
      {% endif %}
    {% endeach %}

    target->Set(NanNew<String>("{{ jsClassName }}"), object);
  }

{% endif %}

{% each functions as function %}
  {% if not function.ignore %}
    {% if function.isAsync %}
      {% partial asyncFunction function %}
    {% else %}
      {% partial syncFunction function %}
    {% endif %}
  {% endif %}
{% endeach %}

{% partial fields . %}

{% if not cTypeIsUndefined %}
  Persistent<Function> {{ cppClassName }}::constructor_template;
{% endif %}
