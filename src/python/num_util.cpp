// Copyright 2006  Phil Austin (http://www.eos.ubc.ca/personal/paustin)
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define IMPORT_ARRAY
#include "python/num_util.hpp"
#include <boost/python/extract.hpp>
#include <boost/python/numpy/dtype.hpp>
#include <cassert>

/* num_util.h and num_util.cpp were obtained from:
 * http://www.eos.ubc.ca/research/clouds/num_util.html on 2003-12-17 under the
 * terms and conditions of the Boost license version 1.0.  num_util was written
 * by Rhys Goldstein, Chris Seymour and Phil Austin.

 * Questions or comments about num_util should be directed to Phil Austin at
 * paustin@eos.ubs.ca.
 */

// 2011/01/05 obtained additional components of latest num_util from:
// Copyright 2006  Phil Austin (http://www.eos.ubc.ca/personal/paustin)
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/*
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

namespace {
	const char* rcsid = "$Id: num_util.cpp,v 1.17 2011/01/11 07:18:53 bsherwood Exp $";
}

  // Local function prototypes
  using namespace boost::python;
  namespace cvisual { namespace python {
    static int rank(const numpy::ndarray& arr);
  } } // !namespace cvisual::python

namespace cvisual{ namespace python {


#if PY_MAJOR_VERSION >= 3
int
#else
void
#endif
init_numpy()
{
	Py_Initialize();
	boost::python::numpy::initialize();
	import_array();
}

size_t
typesize( NPY_TYPES t)
{
	size_t result = 0;
	switch (t) {
	    case NPY_BYTE:
	    	result = sizeof (char);
	    	break;
	    case NPY_UBYTE:
	    	result = sizeof (unsigned char);
	    	break;
	    case NPY_SHORT:
	    	result = sizeof (short);
	    	break;
	    case NPY_INT:
	    	result = sizeof (int);
	    	break;
	    case NPY_LONG:
	    	result = sizeof (long);
	    	break;
	    case NPY_FLOAT:
	    	result = sizeof (float);
	    	break;
	    case NPY_DOUBLE:
	    	result = sizeof (double);
	    	break;
	    case NPY_CFLOAT:
	    	result = sizeof (float)*2;
	    	break;
	    case NPY_CDOUBLE:
	    	result = sizeof (double)*2;
	    	break;
		default:
			bool type_is_recognized = false;
			assert( type_is_recognized == true);
	}
	return result;
}



typedef KindStringMap::value_type  KindStringMapEntry;
KindStringMapEntry kindStringMapEntries[] =
  {
    KindStringMapEntry(NPY_CHAR,   "NPY_CHAR"),
    KindStringMapEntry(NPY_UBYTE,  "NPY_UBYTE"),
    KindStringMapEntry(NPY_BYTE,  "NPY_BYTE"),
    KindStringMapEntry(NPY_SHORT,  "NPY_SHORT"),
    KindStringMapEntry(NPY_INT,    "NPY_INT"),
    KindStringMapEntry(NPY_LONG,   "NPY_LONG"),
    KindStringMapEntry(NPY_FLOAT,  "NPY_FLOAT"),
    KindStringMapEntry(NPY_DOUBLE, "NPY_DOUBLE"),
    KindStringMapEntry(NPY_CFLOAT, "NPY_CFLOAT"),
    KindStringMapEntry(NPY_CDOUBLE,"NPY_CDOUBLE"),
    KindStringMapEntry(NPY_OBJECT, "NPY_OBJECT"),
    KindStringMapEntry(NPY_NTYPES, "NPY_NTYPES"),
    KindStringMapEntry(NPY_NOTYPE ,"NPY_NOTYPE")
  };

typedef KindCharMap::value_type  KindCharMapEntry;
KindCharMapEntry kindCharMapEntries[] =
  {
    KindCharMapEntry(NPY_CHAR,   'c'),
    KindCharMapEntry(NPY_UBYTE,  'b'),
    KindCharMapEntry(NPY_BYTE,  '1'),
    KindCharMapEntry(NPY_SHORT,  's'),
    KindCharMapEntry(NPY_INT,    'i'),
    KindCharMapEntry(NPY_LONG,   'l'),
    KindCharMapEntry(NPY_FLOAT,  'f'),
    KindCharMapEntry(NPY_DOUBLE, 'd'),
    KindCharMapEntry(NPY_CFLOAT, 'F'),
    KindCharMapEntry(NPY_CDOUBLE,'D'),
    KindCharMapEntry(NPY_OBJECT, 'O')
  };

typedef KindTypeMap::value_type  KindTypeMapEntry;
KindTypeMapEntry kindTypeMapEntries[] =
  {
    KindTypeMapEntry('c',NPY_CHAR),
    KindTypeMapEntry('b',NPY_UBYTE),
    KindTypeMapEntry('1',NPY_BYTE),
    KindTypeMapEntry('s',NPY_SHORT),
    KindTypeMapEntry('i',NPY_INT),
    KindTypeMapEntry('l',NPY_LONG),
    KindTypeMapEntry('f',NPY_FLOAT),
    KindTypeMapEntry('d',NPY_DOUBLE),
    KindTypeMapEntry('F',NPY_CFLOAT),
    KindTypeMapEntry('D',NPY_CDOUBLE),
    KindTypeMapEntry('O',NPY_OBJECT)
  };


int numStringEntries = sizeof(kindStringMapEntries)/sizeof(KindStringMapEntry);
int numCharEntries = sizeof(kindCharMapEntries)/sizeof(KindCharMapEntry);
int numTypeEntries = sizeof(kindTypeMapEntries)/sizeof(KindTypeMapEntry);


using namespace boost::python;

static KindStringMap kindstrings(kindStringMapEntries,
                                   kindStringMapEntries + numStringEntries);

static KindCharMap kindchars(kindCharMapEntries,
                                   kindCharMapEntries + numCharEntries);

static KindTypeMap kindtypes(kindTypeMapEntries,
                                   kindTypeMapEntries + numTypeEntries);

//Create a numpy ndarray with dimensions dimens and Numeric type t
numpy::ndarray makeNum(const std::vector<npy_intp>& dimens, NPY_TYPES t){
  object obj(handle<>(PyArray_SimpleNew(dimens.size(), const_cast<npy_intp*>(&dimens[0]), t)));
  return extract<numpy::ndarray>(obj);
}


NPY_TYPES type(const numpy::ndarray arr){
  return NPY_TYPES (PyArray_TYPE((PyArrayObject*)arr.ptr()));
}

void check_type(numpy::ndarray arr,
		NPY_TYPES expected_type){
  NPY_TYPES actual_type = type(arr);
  if (actual_type != expected_type) {
    std::ostringstream stream;
    stream << "expected Numeric type " << kindstrings[expected_type]
	   << ", found Numeric type " << kindstrings[actual_type] << std::ends;
    PyErr_SetString(PyExc_TypeError, stream.str().c_str());
    throw_error_already_set();
  }
  return;
}

// Return the number of dimensions
static int
rank(const numpy::ndarray& arr)
{
   if(!PyArray_Check(arr.ptr())){
      PyErr_SetString(PyExc_ValueError, "expected a PyArrayObject");
      throw_error_already_set();
	}
	return PyArray_NDIM((PyArrayObject*)arr.ptr());
//	return ((PyArrayObject*) arr.ptr())->nd;
}

std::vector<npy_intp> shape(numpy::ndarray arr){
  std::vector<npy_intp> out_dims;
  if(!PyArray_Check(arr.ptr())){
    PyErr_SetString(PyExc_ValueError, "expected a PyArrayObject");
    throw_error_already_set();
  }
  npy_intp* dims_ptr = PyArray_DIMS((PyArrayObject*)arr.ptr());
  int the_rank = rank(arr);
  for (int i = 0; i < the_rank; i++){
    out_dims.push_back(*(dims_ptr + i));
  }
  return out_dims;
}

bool iscontiguous(numpy::ndarray arr)
{
  //  return arr.iscontiguous();
  return PyArray_ISCONTIGUOUS((PyArrayObject*)arr.ptr());
}

void check_contiguous(numpy::ndarray arr)
{
  if (!iscontiguous(arr)) {
    PyErr_SetString(PyExc_RuntimeError, "expected a contiguous array");
    throw_error_already_set();
  }
  return;
}

char*
data(const numpy::ndarray& arr){
  if(!PyArray_Check(arr.ptr())){
    PyErr_SetString(PyExc_ValueError, "expected a PyArrayObject");
    throw_error_already_set();
  }
  return PyArray_BYTES((PyArrayObject*)arr.ptr());
}

//Return a clone of this array with a new type
numpy::ndarray astype(numpy::ndarray arr, NPY_TYPES t){
  return arr.astype(numpy::dtype(object(t)));
}

std::string type2string(NPY_TYPES t_type){
  return kindstrings[t_type];
}

char type2char(NPY_TYPES t_type){
  return kindchars[t_type];
}

NPY_TYPES char2type(char e_type){
  return kindtypes[e_type];
}

} } //cvisual python

