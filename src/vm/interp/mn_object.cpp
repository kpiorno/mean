 /*************************************************************************/
/*  mn_item.cpp                                                            */
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

#include <vm/interp/mn_object.h>
#include <vm/mn_interp.h>
#define C_STR(VALUE)  #VALUE
#define TYPE_OP_CHECK(COMP, OP, TYPE, FCTYPE, SCTYPE, V1, V2) \
if (op->get_data().type == COMP) \
    return MAKE_OBJ(TYPE, MNVariant((FCTYPE)data.value.V1 OP (SCTYPE)op->get_data().value.V2)) ;

#define BINARY_OP_CINT(FNAME, BASE, DVALUE, OP, FCTYPE, SCTYPE) \
_FORCE_INLINE_ MNPObject BASE :: FNAME (MNPObject op){ \
    TYPE_OP_CHECK(CONST_FLOAT, OP, MNIConstFloat, FCTYPE, SCTYPE, vint, vfloat) \
    else if (op->get_data().type != CONST_INT || data.type != CONST_INT) \
       return MAKE_OBJ(MNError, MNVariant("TypeError: unsupported operand type(s) for " + std::string(C_STR(OP)) + ": '" \
                                        + get_type() + "' and '" + op->get_type() + "'") ); \
    return MAKE_OBJ(BASE, MNVariant(data.value.DVALUE OP op->get_data().value.DVALUE) ); \
}

#define BINARY_OP_CFLOAT(FNAME, BASE, DVALUE, OP, FCTYPE, SCTYPE) \
MNPObject BASE :: FNAME (MNPObject op){ \
    TYPE_OP_CHECK(CONST_INT, OP, MNIConstFloat, FCTYPE, SCTYPE, vfloat, vint) \
    else if (op->get_data().type != CONST_FLOAT || data.type != CONST_FLOAT) \
       return MAKE_OBJ(MNError, MNVariant("TypeError: unsupported operand type(s) for " + std::string(C_STR(OP)) + ": '" \
                                        + get_type() + "' and '" + op->get_type() + "'") ); \
    return MAKE_OBJ(BASE, MNVariant(data.value.DVALUE OP op->get_data().value.DVALUE)); \
}

#define BINARY_OP_CSTRING(FNAME, BASE, DVALUE, OP, FCTYPE, SCTYPE) \
MNPObject BASE :: FNAME (MNPObject op){ \
    TYPE_OP_CHECK(CONST_INT, OP, MNIConstCSTRING, FCTYPE, SCTYPE, vfloat, vint) \
    return MAKE_OBJ(BASE, MNVariant(data.value.DVALUE OP op->get_data().value.DVALUE) ); \
}

#define CMP_OP_CHECK(COMP, OP, FCTYPE, SCTYPE) \
if (data.FCTYPE OP op->get_data().SCTYPE && op->get_data().type == COMP) \
    return MAKE_OBJ(MNIConstInt, MNVariant(1));

#define CMP_OP_CINT(FNAME, BASE, OP) \
_FORCE_INLINE_ MNPObject BASE :: FNAME (MNPObject op){ \
    CMP_OP_CHECK(CONST_INT, OP, value.vint, value.vint) \
    else \
    CMP_OP_CHECK(CONST_FLOAT, OP, value.vint, value.vfloat) \
    return MAKE_OBJ(MNIConstInt, MNIConstInt(MNVariant(0)));\
}

#define CMP_OP_CINT2(FNAME, BASE, OP) \
_FORCE_INLINE_ MNPObject BASE :: FNAME (MNPObject op){ \
    CMP_OP_CHECK(CONST_INT, OP, value.vint, value.vint) \
    else \
    CMP_OP_CHECK(CONST_FLOAT, OP, value.vint, value.vfloat) \
    return MAKE_OBJ(MNObject, MNVariant (0) );\
}

#define CMP_OP_CFLOAT(FNAME, BASE, OP) \
_FORCE_INLINE_ MNPObject BASE :: FNAME (MNPObject op){ \
    CMP_OP_CHECK(CONST_INT, OP, value.vfloat, value.vint) \
    else \
    CMP_OP_CHECK(CONST_FLOAT, OP, value.vfloat, value.vfloat) \
    return MAKE_OBJ(MNIConstInt, MNVariant(0) );\
}

#define CMP_OP_CFLOAT2(FNAME, BASE, OP) \
_FORCE_INLINE_ MNPObject BASE :: FNAME (MNPObject op){ \
    CMP_OP_CHECK(CONST_INT, OP, value.vfloat, value.vint) \
    else \
    CMP_OP_CHECK(CONST_FLOAT, OP, value.vfloat, value.vfloat) \
    return MAKE_OBJ(MNIConstInt, MNVariant(0) );\
}

namespace mn
{
MNObject::MNObject()
{

}

MNObject::MNObject(MNVariant value):data(value)
{

}


MNVariant& MNObject::get_data()
{
    return data;
}


std::string MNObject::__str__()
{
    return data.svalue;
}

void MNObject::set_attr(const std::string& name, const MNPObject value)
{
    attr[name] = value;
}

MNPObject MNObject::get_attr(const std::string &name)
{
    if (has_attr(name))
        return attr[name];
    return MAKE_OBJ(MNError, MNVariant("NameError: name '" + name + "' is not defined" ));
}

std::unordered_map<std::string, MNPObject>* MNObject::get_attrs()
{
    return &attr;
}

_FORCE_INLINE_ bool MNObject::has_attr(const std::string &name) const
{
    if (attr.find(name) != attr.end())
        return true;
    return false;
}

std::string MNObject::get_type()
{
    return "object";
}

MNPObject MNObject::__cmp_ne__(MNPObject op){
    if (this != op.get())
         return MAKE_OBJ(MNIConstInt, MNVariant(1));
    return MAKE_OBJ(MNIConstInt, MNVariant(0));
}
MNPObject MNObject::__cmp_e__(MNPObject op){
    if (this == op.get())
        return MAKE_OBJ(MNIConstInt, MNVariant(1));
    return MAKE_OBJ(MNIConstInt, MNVariant(0));
}
MNPObject MNObject::__cmp_b__(MNPObject op){
    return ret_err("<", this, op.get());
}
MNPObject MNObject::__cmp_a__(MNPObject op) {
    return ret_err(">", this, op.get());
}
MNPObject MNObject::__cmp_ae__(MNPObject op) {
    return ret_err(">=", this, op.get());
}
MNPObject MNObject::__cmp_be__(MNPObject op) {
    return ret_err("<=", this, op.get());
}
MNPObject MNObject::__add__(MNPObject op){
    return ret_err("+", this, op.get());
}
MNPObject MNObject::__and__(MNPObject op){
    return ret_err("&", this, op.get());
}
MNPObject MNObject::__sub__(MNPObject op){
    return ret_err("-", this, op.get());
}
MNPObject MNObject::__mul__(MNPObject op){
    return ret_err("*", this, op.get());
}
MNPObject MNObject::__tdiv__(MNPObject op){
    return ret_err("/", this, op.get());
}
MNPObject MNObject::__fdiv__(MNPObject op){
    return ret_err("//", this, op.get());
}

MNPObject MNObject::store_subscr(MNPObject /*index*/)
{
    return MAKE_OBJ(MNError, MNVariant("TypeError: '" + get_type() + "' object is not subscriptable"));
}

MNPObject MNObject::load_subscr(MNPObject /*index*/)
{
    return MAKE_OBJ(MNError, MNVariant("TypeError: '" + get_type() + "' object is not subscriptable"));
}


MNPObject MNObject::ret_err(std::string op, MNObject* left, MNObject* right)
{
    return MAKE_OBJ(MNError, MNVariant("TypeError: unsupported operand type(s) for " + op + ": '"
                                                + left->get_type() + "' and '" + right->get_type() + "'"));
}



BINARY_OP_CINT(__add__, MNIConstInt, vint, +, float, float )
MNPObject MNIConstInt::__and__(MNPObject op)
{
    return MAKE_OBJ(MNIConstInt, MNVariant(data.value.vint & op->get_data().value.vint) );
}

CMP_OP_CINT(__cmp_e__, MNIConstInt, ==)
CMP_OP_CINT(__cmp_b__, MNIConstInt, <)
CMP_OP_CINT(__cmp_a__, MNIConstInt, >)
CMP_OP_CINT(__cmp_ae__, MNIConstInt, >=)
CMP_OP_CINT(__cmp_be__, MNIConstInt, >=)
CMP_OP_CINT(__cmp_ne__, MNIConstInt, !=)


_FORCE_INLINE_ bool MNIConstInt::is_true()
{
    if (data.value.vint == 0)
        return false;
    return true;
}


MNPObject MNIConstInt :: __mul__ (MNPObject op){
    if (op->get_data().type == CONST_FLOAT)
        return MAKE_OBJ(MNIConstFloat, MNVariant((float)data.value.v32 * op->get_data().value.vfloat) );
    if (op->get_data().type == CONST_STR)
    {
        std::string value = "";
        std::string tvalue = op->get_data().svalue;
        for (int i=0; i < get_data().value.v32; ++i)
            value += tvalue;
        return MAKE_OBJ(MNConstStr, MNVariant(value) );
    }
    else if (op->get_data().type != CONST_INT || data.type != CONST_INT)
        return MAKE_OBJ(MNError, MNVariant("TypeError: unsupported operand type(s) for *: '"
                                        + get_type() + "' and '" + op->get_type() + "'") );
    return MAKE_OBJ(MNIConstInt, MNVariant(data.value.v32 * op->get_data().value.v32) );
}

BINARY_OP_CINT(__sub__, MNIConstInt, vint, -, float, float )
BINARY_OP_CINT(__tdiv__, MNIConstInt, vint, /, float, float )
BINARY_OP_CINT(__fdiv__, MNIConstInt, vint, / (int), float, float )

CMP_OP_CFLOAT(__cmp_e__, MNIConstFloat, ==)
CMP_OP_CFLOAT(__cmp_b__, MNIConstFloat, <)
CMP_OP_CFLOAT(__cmp_a__, MNIConstFloat, >)
CMP_OP_CFLOAT(__cmp_ae__, MNIConstFloat, >=)
CMP_OP_CFLOAT(__cmp_be__, MNIConstFloat, <=)
CMP_OP_CFLOAT(__cmp_ne__, MNIConstFloat, !=)

_FORCE_INLINE_ bool MNIConstFloat::is_true()
{
    if (data.value.vfloat == 0)
        return false;
    return true;
}
BINARY_OP_CFLOAT(__add__, MNIConstFloat, vfloat, +, float, float )
BINARY_OP_CFLOAT(__mul__, MNIConstFloat, vfloat, *, float, float )
BINARY_OP_CFLOAT(__sub__, MNIConstFloat, vfloat, -, float, float )
BINARY_OP_CFLOAT(__tdiv__, MNIConstFloat, vfloat, /, float, float )
BINARY_OP_CFLOAT(__fdiv__, MNIConstFloat, vfloat, / (int), float, float )


MNPObject MNConstStr::__add__(MNPObject op)
{
    if (op->get_data().type != CONST_STR || data.type != CONST_STR)
           return MAKE_OBJ(MNError, MNVariant("TypeError: unsupported operand type(s) for +: '"
                                            + get_type() + "' and '" + op->get_type() + "'") );
    return MAKE_OBJ(MNConstStr, MNVariant(data.svalue + op->get_data().svalue));
}

MNPObject MNConstStr::__cmp_b__(MNPObject op)
{
    if (op->get_data().type != CONST_STR || data.type != CONST_STR)
           return MAKE_OBJ(MNError, MNVariant("TypeError: unsupported operand type(s) for <: '"
                                            + get_type() + "' and '" + op->get_type() + "'") );
    return MAKE_OBJ(MNConstStr, MNVariant(data.svalue < op->get_data().svalue));
}

MNPObject MNConstStr::__cmp_be__(MNPObject op)
{
    if (op->get_data().type != CONST_STR || data.type != CONST_STR)
           return MAKE_OBJ(MNError, MNVariant("TypeError: unsupported operand type(s) for <=: '"
                                            + get_type() + "' and '" + op->get_type() + "'") );
    return MAKE_OBJ(MNConstStr, MNVariant(data.svalue <= op->get_data().svalue));
}

MNPObject MNConstStr::__cmp_a__(MNPObject op)
{
    if (op->get_data().type != CONST_STR || data.type != CONST_STR)
           return MAKE_OBJ(MNError, MNVariant("TypeError: unsupported operand type(s) for >: '"
                                            + get_type() + "' and '" + op->get_type() + "'") );
    return MAKE_OBJ(MNConstStr, MNVariant(data.svalue > op->get_data().svalue));
}

MNPObject MNConstStr::__cmp_ae__(MNPObject op)
{
    if (op->get_data().type != CONST_STR || data.type != CONST_STR)
           return MAKE_OBJ(MNError, MNVariant("TypeError: unsupported operand type(s) for >=: '"
                                            + get_type() + "' and '" + op->get_type() + "'") );
    return MAKE_OBJ(MNConstStr, MNVariant(data.svalue >= op->get_data().svalue));
}

MNPObject MNConstStr :: __mul__ (MNPObject op)
{
    if (op->get_data().type == CONST_INT)
    {
        std::string value = "";
        std::string tvalue = get_data().svalue;
        for (int i=0; i < op->get_data().value.v32; ++i)
            value += tvalue;
        return MAKE_OBJ(MNConstStr, MNVariant(value) );
    }
    return MAKE_OBJ(MNError, MNVariant("TypeError: unsupported operand type(s) for *: '"
                                    + get_type() + "' and '" + op->get_type() + "'") );
}


MNPObject MNContainer::add_object(MNPObject obj)
{
    objs.push_back(obj);
    return success;
}

std::string MNContainer::__str__()
{
    std::string res = "[";
    for (unsigned int i=0; i < objs.size(); ++i)
        res += objs[i]->__str__() + (i < objs.size()-1 ? ", " : "");
    res += "]";
    return res;
}


MNFunction::MNFunction(std::vector<MNVariant*>* data)
{
    interp = new MNInterp(data);
}

void MNFunction::add_param(std::string name, int pos, MNFuncParamType::TYPE type, bool default_value, MNPObject dparam)
{
    name_order.push_back(name);
    order[name] = MNFuncParamType(pos, type, default_value, dparam);
}

}
