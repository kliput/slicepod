/*
 * Special header to ignore some macros in Eclipse CDT parser.
 */

#ifndef ECLIPSE_IGNORE_HPP_
#define ECLIPSE_IGNORE_HPP_

#ifdef __CDT_PARSER__
   #define QX_REGISTER_HPP_QX_SLICEPOD(a, b, c)
#endif

#endif /* ECLIPSE_IGNORE_HPP_ */
