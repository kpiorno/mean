 /*************************************************************************/
/*  mn_item.h                                                            */
/*************************************************************************/
/* Copyright (c) 2017 Karel Piorno Charchabal   (kpiorno@gmail.com)      */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#ifndef MNOBJECT_H
#define MNOBJECT_H
#include <iostream>
#include <vm/mn_variant.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>


class MNCounter
{
public:
    MNCounter(int count)
    {
        this->count = count;
    }

    int get_count() {return count;}
    void set_count(int count) {
        this->count  = count;
    }
    void inc()
    {
        count++;
    }

    void dec()
    {
        if (count > 0)
            count--;
    }

protected:
    int count;
};

template <typename T>
class MNSharedPtr
{
public:
    MNSharedPtr(T* obj)
    {
        this->obj = obj;
        counter = new MNCounter(0);
    }

    MNSharedPtr()
    {
        //counter = nullptr;
    }


    T* operator->()
    {
        return obj;
    }

    virtual ~MNSharedPtr()
    {
        if (counter)
        {
            counter->dec();
            if (!counter->get_count())
            {
                delete counter;
                delete obj;
            }
        }
    }

    MNCounter* get_counter() {return counter;}


    T* get()
    {
        return obj;
    }

    template<typename R>
    MNSharedPtr<R>&
    operator=(const MNSharedPtr<R>& obj) noexcept
    {
        counter = obj.get_counter();
        if (counter)
            counter->inc();
        return *this;
    }

    unsigned int get_count()
    {
        if (counter)
            return counter->get_count();
        return 0;
    }

protected:
    T* obj;
    MNCounter* counter;

};
#define CUSTOMPTR 0
#if CUSTOMPTR == 0
#define MAKE_OBJ(type, param) std::make_shared<type>(param)
#define MNPObject std::shared_ptr<MNObject>
#else
#define MAKE_OBJ(type, param) MNSharedPtr<MNObject>(new type (param))
#define MNPObject MNSharedPtr<MNObject>

#endif


namespace mn
{
class MNObject
{
public:
    MNObject();
    virtual ~MNObject(){}
    explicit MNObject(MNVariant value );
    void set_attr(const std::string& name, const MNPObject value);
    MNPObject get_attr(const std::string& name);
    bool has_attr(const std::string& name) const;
    void set_type(std::string type);
    std::unordered_map<std::string, MNPObject>* get_attrs();
    virtual std::string get_type();

    virtual bool is_true() {return true;}
    virtual MNPObject __cmp_ne__(MNPObject op);
    virtual MNPObject __cmp_e__(MNPObject op);
    virtual MNPObject __cmp_b__(MNPObject /*op*/) ;
    virtual MNPObject __cmp_a__(MNPObject /*op*/);
    virtual MNPObject __cmp_ae__(MNPObject /*op*/);
    virtual MNPObject __cmp_be__(MNPObject /*op*/);
    virtual MNPObject __add__(MNPObject /*op*/);
    virtual MNPObject __and__(MNPObject /*op*/);
    virtual MNPObject __sub__(MNPObject /*op*/);
    virtual MNPObject __mul__(MNPObject /*op*/);
    virtual MNPObject __tdiv__(MNPObject /*op*/);
    virtual MNPObject __fdiv__(MNPObject /*op*/);

    virtual MNPObject store_subscr(MNPObject index);
    virtual MNPObject load_subscr(MNPObject index);

    virtual std::string __str__();
    MNVariant& get_data();
protected:
    MNVariant data;
    std::unordered_map<std::string, MNPObject> attr;
    std::string type;
protected:
    MNPObject ret_err(std::string op, MNObject* left, MNObject* right);
};

class MNError: public MNObject
{
public:
    virtual std::string get_type() {return "error";}
    MNError(MNVariant value):MNObject(value){data.type = MN_ERROR;}
};

class MNIConstFloat: public MNObject
{
public:
    MNIConstFloat(MNVariant value ):MNObject(value){data.type = CONST_FLOAT;}
    virtual std::string get_type() {return "float";}
    virtual bool is_true();
    virtual MNPObject __cmp_e__(MNPObject op);
    virtual MNPObject __cmp_b__(MNPObject op);
    virtual MNPObject __cmp_a__(MNPObject op);
    virtual MNPObject __cmp_ae__(MNPObject op);
    virtual MNPObject __cmp_be__(MNPObject op);
    virtual MNPObject __cmp_ne__(MNPObject op);
    virtual MNPObject __add__(MNPObject op);
    virtual MNPObject __sub__(MNPObject op);
    virtual MNPObject __mul__(MNPObject op);
    virtual MNPObject __tdiv__(MNPObject /*op*/);
    virtual MNPObject __fdiv__(MNPObject /*op*/);
    virtual std::string __str__() {return mn::to_string(data.value.vfloat);}
};

class MNIConstInt: public MNObject
{
public:
    virtual std::string get_type() {return "int";}
    MNIConstInt(MNVariant value ):MNObject(value){data.type = CONST_INT;}
    virtual bool is_true();
    virtual MNPObject __cmp_e__(MNPObject op);
    virtual MNPObject __cmp_b__(MNPObject op);
    virtual MNPObject __cmp_a__(MNPObject op);
    virtual MNPObject __cmp_ae__(MNPObject op);
    virtual MNPObject __cmp_be__(MNPObject op);
    virtual MNPObject __cmp_ne__(MNPObject op);
    virtual MNPObject __add__(MNPObject op);
    virtual MNPObject __and__(MNPObject op);
    virtual MNPObject __sub__(MNPObject op);
    virtual MNPObject __mul__(MNPObject op);
    virtual MNPObject __tdiv__(MNPObject /*op*/);
    virtual MNPObject __fdiv__(MNPObject /*op*/);
    virtual std::string __str__() {return mn::to_string(data.value.v32);}
};


class MNConstStr: public MNObject
{
public:
    virtual std::string get_type() {return "str";}
    MNConstStr(MNVariant value):MNObject(value){data.type = CONST_STR;}
    virtual MNPObject __add__(MNPObject op);
    virtual MNPObject __cmp_b__(MNPObject op);
    virtual MNPObject __cmp_a__(MNPObject op);
    virtual MNPObject __cmp_ae__(MNPObject op);
    virtual MNPObject __cmp_be__(MNPObject op);
    virtual MNPObject __mul__(MNPObject op);
};

class MNContainer: public MNObject
{
public:
    MNContainer(MNVariant value):MNObject(value){
        success = MAKE_OBJ(MNIConstInt, MNVariant(1));
    }
    MNPObject add_object(MNPObject obj);
    std::string get_type() {return "container";}
    std::string __str__();
protected:
    std::vector<MNPObject> objs;
    MNPObject success;
};

class MNInterp;
struct MNFuncParamType
{
    int pos;
    enum TYPE{
        PPARAM,
        APARAM,
        KPARAM
    } type;
    bool default_value;
    MNPObject dparam;
    MNFuncParamType(int pos, TYPE type, bool default_value, MNPObject dparam)
    {
        this->pos = pos;
        this->type = type;
        this->default_value = default_value;
        this->dparam = dparam;
    }
    MNFuncParamType()
    {
       pos = 0;
       type = PPARAM;
       default_value = false;
    }
};

class MNFunction: public MNObject
{
public:
    MNFunction(std::vector<MNVariant*>* data);
    void add_param(std::string name, int pos, MNFuncParamType::TYPE type, bool default_value, MNPObject dparam);
    MNPObject call_function(unsigned int pos, unsigned int arg);
    MNPObject call_function_var(unsigned int pos, unsigned int arg);
    MNPObject call_function_kw(unsigned int pos, unsigned int arg);
    MNPObject call_function_var_kw(unsigned int pos, unsigned int arg);
protected:
    MNInterp* interp;
    std::unordered_map<std::string, MNFuncParamType> order;
    std::vector<std::string> name_order;
};
}
#endif // MNOBJECT_H
