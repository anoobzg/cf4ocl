/*   
 * This file is part of cf4ocl (C Framework for OpenCL).
 * 
 * cf4ocl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or (at your option) any later version.
 * 
 * cf4ocl is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with cf4ocl. If not, see 
 * <http://www.gnu.org/licenses/>.
 * */
 
 /** 
 * @file
 * 
 * Definition of a wrapper class and its methods for OpenCL program
 * objects.
 * 
 * @author Nuno Fachada
 * @date 2014
 * @copyright [GNU Lesser General Public License version 3 (LGPLv3)](http://www.gnu.org/licenses/lgpl.html)
 * */
 
#ifndef _CCL_PROGRAM_WRAPPER_H_
#define _CCL_PROGRAM_WRAPPER_H_

#include <glib.h>
#include "oclversions.h"
#include "context_wrapper.h"
#include "abstract_wrapper.h"
#include "event_wrapper.h"
#include "queue_wrapper.h"
#include "kernel_wrapper.h"

/**
 * @defgroup PROGRAM_WRAPPER Program wrapper
 *
 * A wrapper object for OpenCL programs and functions to manage 
 * them.
 * 
 * @todo detailed description of module.
 * 
 * @{
 */
 
/**
 * Program wrapper class.
 * 
 * @extends ccl_dev_container
 */
typedef struct ccl_program CCLProgram;

/** Class which represents a binary object associated with a program
 * and a device. */
typedef struct ccl_program_binary CCLProgramBinary;

/**
 * Prototype of callback functions for program build, compile and 
 * link.
 * 
 * @public @memberof ccl_program
 * 
 * @param[in] program Program wrapper object.
 * @param[in] user_data A pointer to user supplied data.
 * */
typedef void (CL_CALLBACK* ccl_program_callback)(
	cl_program program, void* user_data);

/* *********** */
/* WRAPPER API */
/* *********** */

/* Get the program wrapper for the given OpenCL program. */
CCLProgram* ccl_program_new_wrap(cl_program program);

/* Decrements the reference count of the program wrapper object. 
 * If it reaches 0, the program wrapper object is destroyed. */
void ccl_program_destroy(CCLProgram* prg);

/* *********************** */
/* CREATE FROM SOURCES API */
/* *********************** */

/* Create a new program wrapper object from a source file. */
CCLProgram* ccl_program_new_from_source_file(CCLContext* ctx, 
	const char* filename, GError** err);

/* Create a new program wrapper object from several source files. */
CCLProgram* ccl_program_new_from_source_files(CCLContext* ctx, 
	cl_uint count, const char** filenames, GError** err);

/* Create a new program wrapper object from a null-terminated source 
 * string. */
CCLProgram* ccl_program_new_from_source(CCLContext* ctx, 
	const char* string, GError** err);

/* Create a new program wrapper object from several source code 
 * strings. */
CCLProgram* ccl_program_new_from_sources(CCLContext* ctx,
	cl_uint count, const char** strings, const size_t* lengths,
	GError** err);

/* ************************ */
/* CREATE FROM BINARIES API */
/* ************************ */

/* Create a new program wrapper object with binary code from a specific
 * device. */
CCLProgram* ccl_program_new_from_binary_file(CCLContext* ctx, 
	CCLDevice* dev, const char* filename, cl_int *binary_status, 
	GError** err);

/* Create a new program wrapper object from files containing binary 
 * code executable on the given device list, one file per device. */
CCLProgram* ccl_program_new_from_binary_files(CCLContext* ctx, 
	cl_uint num_devices, CCLDevice** devs, const char** filenames, 
	cl_int *binary_status, GError** err);

/* Create a new program wrapper object from binary code executable on a
 * specific device. */
CCLProgram* ccl_program_new_from_binary(CCLContext* ctx, CCLDevice* dev,
	CCLProgramBinary* binary, cl_int *binary_status, GError** err);

/* Create a new program wrapper object from a list of binary code
 * strings executable on the given device list, one binary string per 
 * device. */
CCLProgram* ccl_program_new_from_binaries(CCLContext* ctx,
	cl_uint num_devices, CCLDevice** devs, CCLProgramBinary** bins,
	cl_int *binary_status, GError** err);

/* ******************************** */
/* CREATE FROM BUILT-IN KERNELS API */
/* ******************************** */

#ifdef CL_VERSION_1_2

/* Create a new program wrapper object from device built-in kernels. */
CCLProgram* ccl_program_new_from_built_in_kernels(CCLContext* ctx,
	cl_uint num_devices, CCLDevice** devs, const char *kernel_names, 
	GError** err);

#endif

/* ************************ */
/* BUILD, COMPILE, LINK API */
/* ************************ */
 
/* Utility function which builds (compiles and links) a program 
 * executable from the program source or binary. */
cl_bool ccl_program_build(
	CCLProgram* prg, const char* options, GError** err);

/* Builds (compiles and links) a program executable from the program 
 * source or binary. This function wraps the clBuildProgram() OpenCL
 * function. */
cl_bool ccl_program_build_full(CCLProgram* prg, 
	cl_uint num_devices, CCLDevice** devices, const char *options, 
	ccl_program_callback pfn_notify, void *user_data, GError** err);

/**
 * @class ccl_program
 * @todo Add support for compiling and linking.
 * */

/* ******************************* */
/* KERNEL RELATED HELPER FUNCTIONS */
/* ******************************* */

/* Get the kernel wrapper object for the given program kernel 
 * function. */
CCLKernel* ccl_program_get_kernel(
	CCLProgram* prg, const char* kernel_name, GError** err);

/* Enqueues a program kernel function for execution on a device. */
CCLEvent* ccl_program_enqueue_kernel(CCLProgram* prg, 
	const char* kernel_name, CCLQueue* cq, cl_uint work_dim, 
	const size_t* global_work_offset, const size_t* global_work_size, 
	const size_t* local_work_size, CCLEventWaitList* evt_wait_lst, 
	GError** err, ...) G_GNUC_NULL_TERMINATED;

/* Enqueues a program kernel function for execution on a device. */
CCLEvent* ccl_program_enqueue_kernel_v(CCLProgram* prg, 
	const char* kernel_name, CCLQueue* cq, cl_uint work_dim, 
	const size_t* global_work_offset, const size_t* global_work_size, 
	const size_t* local_work_size, CCLEventWaitList* evt_wait_lst, 
	CCLArg** args, GError** err);

/* ************************* */
/* BINARY HANDLING FUNCTIONS */
/* ************************* */

/* Get the program's binary object for the the specified device. */
CCLProgramBinary* ccl_program_get_binary(
	CCLProgram* prg, CCLDevice* dev, GError** err);

/* Save the program's binary code for a specified device to a file. */
cl_bool ccl_program_save_binary(CCLProgram* prg, CCLDevice* dev,
	const char* filename, GError** err);

/* Save the program binaries for all associated devices to files, one
 * file per device. */
cl_bool ccl_program_save_all_binaries(CCLProgram* prg, 
	const char* file_prefix, const char* file_suffix, GError** err);

/* ***************************************** */
/* DEVICE CONTAINER FUNCTION IMPLEMENTATIONS */
/* ***************************************** */

/* Get ::CCLDevice wrapper at given index. */
CCLDevice* ccl_program_get_device(
	CCLProgram* prg, cl_uint index, GError** err);

/* Return number of devices in program. */
cl_uint ccl_program_get_num_devices(CCLProgram* prg, GError** err);

/* Get all device wrappers in program. */
CCLDevice* const* ccl_program_get_all_devices(CCLProgram* prg, 
	GError** err);
	
/* ************************************************* */
/* ABSTRACT WRAPPER MACROS (INFO, REF/UNREF, UNWRAP) */
/* ************************************************* */

/**
 * Get a ::CCLWrapperInfo program information object. To get the 
 * program binaries use the ::ccl_program_get_binary() function instead, 
 * as this macro will return NULL when the CL_PROGRAM_BINARIES parameter 
 * is requested.
 * 
 * @param[in] prg The program wrapper object.
 * @param[in] param_name Name of information/parameter to get.
 * @param[out] err Return location for a GError, or `NULL` if error
 * reporting is to be ignored.
 * @return The requested program information object. This object will
 * be automatically freed when the program wrapper object is 
 * destroyed.  If an error occurs or if the CL_PROGRAM_BINARIES 
 * parameter is requested, NULL is returned.
 * */
#define ccl_program_get_info(prg, param_name, err) \
	(param_name == CL_PROGRAM_BINARIES) \
	? NULL \
	: ccl_wrapper_get_info((CCLWrapper*) prg, NULL, param_name, \
		(ccl_wrapper_info_fp) clGetProgramInfo, CL_TRUE, err)

/** 
 * Macro which returns a scalar program information value. 
 * 
 * Use with care. In case an error occurs, zero is returned, which 
 * might be ambiguous if zero is a valid return value. In this case, it
 * is necessary to check the error object. 
 * 
 * @param[in] prg The program wrapper object.
 * @param[in] param_name Name of information/parameter to get value of.
 * @param[in] param_type Type of parameter (e.g. cl_uint, size_t, etc.).
 * @param[out] err Return location for a GError, or `NULL` if error
 * reporting is to be ignored.
 * @return The requested program information value. This value will be 
 * automatically freed when the program wrapper object is destroyed. 
 * If an error occurs or if the CL_PROGRAM_BINARIES parameter is 
 * requested, zero is returned.
 * */
#define ccl_program_get_scalar_info(prg, param_name, param_type, err) \
	(param_name == CL_PROGRAM_BINARIES) \
	? (param_type) 0 \
	: *((param_type*) ccl_wrapper_get_info_value((CCLWrapper*) prg, \
		NULL, param_name, (ccl_wrapper_info_fp) clGetProgramInfo, \
		CL_TRUE, err))

/** 
 * Macro which returns an array program information value. To get 
 * the program binaries use the ::ccl_program_get_binary() function 
 * instead, as this macro will return NULL when the CL_PROGRAM_BINARIES 
 * parameter is requested.
 * 
 * Use with care. In case an error occurs, NULL is returned, which 
 * might be ambiguous if NULL is a valid return value. In this case, it
 * is necessary to check the error object. 
 * 
 * @param[in] prg The program wrapper object.
 * @param[in] param_name Name of information/parameter to get value of.
 * @param[in] param_type Type of parameter (e.g. char*, size_t*, etc.).
 * @param[out] err Return location for a GError, or `NULL` if error
 * reporting is to be ignored.
 * @return The requested program information value. This value will be 
 * automatically freed when the program wrapper object is destroyed. 
 * If an error occurs or if the CL_PROGRAM_BINARIES parameter is 
 * requested, NULL is returned.
 * */
#define ccl_program_get_array_info(prg, param_name, param_type, err) \
	(param_name == CL_PROGRAM_BINARIES) \
	? NULL \
	: (param_type) ccl_wrapper_get_info_value((CCLWrapper*) prg, \
		NULL, param_name, (ccl_wrapper_info_fp) clGetProgramInfo, \
		CL_TRUE, err)

/**
 * Get a ::CCLWrapperInfo program build information object.
 * 
 * @param[in] prg The program wrapper object.
 * @param[in] dev The device wrapper object.
 * @param[in] param_name Name of information/parameter to get.
 * @param[out] err Return location for a GError, or `NULL` if error
 * reporting is to be ignored.
 * @return The requested program build information object. This object 
 * will be automatically freed when the program wrapper object is 
 * destroyed. If an error occurs, NULL is returned.
 * */
#define ccl_program_get_build_info(prg, dev, param_name, err) \
	ccl_wrapper_get_info((CCLWrapper*) prg, (CCLWrapper*) dev, \
		param_name, (ccl_wrapper_info_fp) clGetProgramBuildInfo, \
		CL_FALSE, err)

/** 
 * Macro which returns a scalar program build information value. 
 * 
 * Use with care. In case an error occurs, zero is returned, which 
 * might be ambiguous if zero is a valid return value. In this case, it
 * is necessary to check the error object. 
 * 
 * @param[in] prg The program wrapper object.
 * @param[in] dev The device wrapper object.
 * @param[in] param_name Name of information/parameter to get value of.
 * @param[in] param_type Type of parameter (e.g. cl_uint, size_t, etc.).
 * @param[out] err Return location for a GError, or `NULL` if error
 * reporting is to be ignored.
 * @return The requested program build information value. This value 
 * will be automatically freed when the program wrapper object is 
 * destroyed. If an error occurs, zero is returned.
 * */
#define ccl_program_get_scalar_build_info(prg, dev, param_name, \
	param_type, err) \
	*((param_type*) ccl_wrapper_get_info_value((CCLWrapper*) prg, \
		(CCLWrapper*) dev, param_name, \
		(ccl_wrapper_info_fp) clGetProgramBuildInfo, CL_FALSE, err))

/** 
 * Macro which returns an array program build information value. 
 * 
 * Use with care. In case an error occurs, NULL is returned, which 
 * might be ambiguous if NULL is a valid return value. In this case, it
 * is necessary to check the error object. 
 * 
 * @param[in] prg The program wrapper object.
 * @param[in] dev The device wrapper object.
 * @param[in] param_name Name of information/parameter to get value of.
 * @param[in] param_type Type of parameter (e.g. char*, size_t*, etc.).
 * @param[out] err Return location for a GError, or `NULL` if error
 * reporting is to be ignored.
 * @return The requested program build information value. This value 
 * will be automatically freed when the program wrapper object is 
 * destroyed. If an error occurs, NULL is returned.
 * */
#define ccl_program_get_array_build_info(prg, dev, param_name, \
	param_type, err) \
	(param_type) ccl_wrapper_get_info_value((CCLWrapper*) prg, \
		(CCLWrapper*) dev, param_name, \
		(ccl_wrapper_info_fp) clGetProgramBuildInfo, CL_FALSE, err)

/** 
 * Increase the reference count of the program object.
 * 
 * @param[in] prg The program wrapper object. 
 * */
#define ccl_program_ref(prg) \
	ccl_wrapper_ref((CCLWrapper*) prg)

/**
 * Alias to ccl_program_destroy().
 * 
 * @param[in] prg Program wrapper object to destroy if reference count 
 * is 1, otherwise just decrement the reference count.
 * */
#define ccl_program_unref(prg) ccl_program_destroy(prg)

/**
 * Get the OpenCL program object.
 * 
 * @param[in] prg The program wrapper object.
 * @return The OpenCL program object.
 * */
#define ccl_program_unwrap(prg) \
	((cl_program) ccl_wrapper_unwrap((CCLWrapper*) prg))

/** @} */

#endif


