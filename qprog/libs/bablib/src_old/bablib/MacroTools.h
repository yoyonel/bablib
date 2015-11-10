#ifndef __MACRO_TOO	LS__
#define __MACRO_TOOLS__

// use preprocessing tools from boost :
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/stringize.hpp>
//#include <boost/preprocessor/seq/for_each.hpp>
//#include <boost/preprocessor/seq/transfom.hpp>
//#include <boost/preprocessor/control/iif.hpp>
//#include <boost/preprocessor/facilities/expand.hpp>

//------------------------------------------
// definition of FOR_INT macro :
// usage : FOR_INT(imin, imax, macro)
//  > successively applies macro to integer values from imin (included) to imax (excluded)
/*
#define FOR_INT_GET_MIN(state_min, state_max, state_macro)    state_min
#define FOR_INT_GET_MAX(state_min, state_max, state_macro)    state_max
#define FOR_INT_GET_MACRO(state_min, state_max, state_macro)  state_macro

#define FOR_INT_PRED(r, state)      BOOST_PP_NOT_EQUAL(FOR_INT_GET_MIN state, FOR_INT_GET_MAX state)
#define FOR_INT_OP(r, state)        (BOOST_PP_INC(FOR_INT_GET_MIN state), FOR_INT_GET_MAX state, FOR_INT_GET_MACRO state)
#define FOR_INT_MACRO(r, state)     FOR_INT_GET_MACRO state (FOR_INT_GET_MIN state)
#define FOR_INT(value_min, value_max, macro) BOOST_PP_FOR((value_min, value_max, macro), FOR_INT_PRED, FOR_INT_OP, FOR_INT_MACRO)
*/
#define FOR_INT_GET_MIN(state)		BOOST_PP_TUPLE_ELEM(3, 0, state)
#define FOR_INT_GET_MAX(state)		BOOST_PP_TUPLE_ELEM(3, 1, state)
#define FOR_INT_GET_MACRO(state)	BOOST_PP_TUPLE_ELEM(3, 2, state)

#define FOR_INT_PRED(r, state)      BOOST_PP_NOT_EQUAL(FOR_INT_GET_MIN(state), FOR_INT_GET_MAX(state))
#define FOR_INT_OP(r, state)        (BOOST_PP_INC(FOR_INT_GET_MIN(state)), FOR_INT_GET_MAX(state), FOR_INT_GET_MACRO(state))
#define FOR_INT_MACRO(r, state)     FOR_INT_GET_MACRO(state) (FOR_INT_GET_MIN(state))

#define FOR_INT(value_min, value_max, macro) BOOST_PP_FOR((value_min, value_max, macro), FOR_INT_PRED, FOR_INT_OP, FOR_INT_MACRO)
//------------------------------------------
// safe concatenation of macro arguments :

#define CONCAT2(a,b)      BOOST_PP_CAT(a,b)
#define CONCAT3(a,b,c)    CONCAT2(CONCAT2(a, b), c)
#define CONCAT4(a,b,c,d)  CONCAT2(CONCAT3(a, b, c), d)

#endif

