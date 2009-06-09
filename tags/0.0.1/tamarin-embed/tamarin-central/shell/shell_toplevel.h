/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe AS3 Team
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
/* machine generated file -- do not edit */
#define AVMTHUNK_VERSION 4
extern const uint32_t shell_toplevel_abc_class_count;
extern const uint32_t shell_toplevel_abc_script_count;
extern const uint32_t shell_toplevel_abc_method_count;
extern const uint32_t shell_toplevel_abc_length;
extern const uint8_t shell_toplevel_abc_data[];
AVMTHUNK_DECLARE_NATIVE_INITIALIZER(shell_toplevel)
/* classes */
const uint32_t abcclass_avmplus_ITest = 0;
const uint32_t abcclass_avmplus_System = 1;
const uint32_t abcclass_avmplus_File = 2;
const uint32_t abcclass_flash_system_Capabilities = 3;
const uint32_t abcclass_db_MySQL = 4;
const uint32_t abcclass_http__HTTP = 5;
const uint32_t abcclass_http__HTTPForm = 6;
const uint32_t abcclass_avmplus_Domain = 7;
const uint32_t abcclass_avmplus_StringBuilder = 8;
const uint32_t abcclass_flash_utils_ByteArray = 9;
const uint32_t abcclass_flash_sampler_StackFrame = 10;
const uint32_t abcclass_flash_sampler_Sample = 11;
const uint32_t abcclass_flash_sampler_NewObjectSample = 12;
const uint32_t abcclass_flash_sampler_DeleteObjectSample = 13;
const uint32_t abcclass_flash_trace_Trace = 14;
const uint32_t abcclass_flash_utils_Dictionary = 15;
const uint32_t abcclass_flash_utils_Endian = 16;
const uint32_t abcclass_avmplus_JObject = 17;
/* methods */
const uint32_t avmplus_System_exit = 13;
const uint32_t avmplus_System_exec = 14;
const uint32_t avmplus_System_getAvmplusVersion = 15;
const uint32_t avmplus_System_trace = 16;
const uint32_t avmplus_System_write = 17;
const uint32_t avmplus_System_debugger = 18;
const uint32_t avmplus_System_isDebugger = 19;
const uint32_t avmplus_System_getTimer = 20;
const uint32_t avmplus_System_private_getArgv = 21;
const uint32_t avmplus_System_readLine = 22;
const uint32_t avmplus_System_totalMemory_get = 23;
const uint32_t avmplus_System_freeMemory_get = 24;
const uint32_t avmplus_System_privateMemory_get = 25;
const uint32_t avmplus_System_ns_example_nstest = 26;
const uint32_t avmplus_System_isGlobal = 27;
const uint32_t avmplus_File_exists = 30;
const uint32_t avmplus_File_read = 31;
const uint32_t avmplus_File_write = 32;
const uint32_t db_MySQL_connect = 41;
const uint32_t db_MySQL_fetch = 42;
const uint32_t db_MySQL_exec = 43;
const uint32_t db_MySQL_escape = 44;
const uint32_t db_MySQL_getError = 45;
const uint32_t http__HTTP_write = 48;
const uint32_t http__HTTP_logError = 49;
const uint32_t http__HTTP_postData = 50;
const uint32_t http__HTTP_listEnv = 51;
const uint32_t http__HTTP_getEnv = 52;
const uint32_t http__HTTPForm_getFields = 55;
const uint32_t http__HTTPForm_listFileKeys = 56;
const uint32_t http__HTTPForm_isUploaded = 57;
const uint32_t http__HTTPForm_saveFile = 58;
const uint32_t avmplus_Domain_currentDomain_get = 62;
const uint32_t avmplus_Domain_MIN_DOMAIN_MEMORY_LENGTH_get = 63;
const uint32_t avmplus_Domain_private_init = 64;
const uint32_t avmplus_Domain_loadBytes = 66;
const uint32_t avmplus_Domain_getClass = 67;
const uint32_t avmplus_Domain_domainMemory_get = 69;
const uint32_t avmplus_Domain_domainMemory_set = 70;
const uint32_t avmplus_StringBuilder_append = 74;
const uint32_t avmplus_StringBuilder_capacity_get = 75;
const uint32_t avmplus_StringBuilder_charAt = 76;
const uint32_t avmplus_StringBuilder_charCodeAt = 77;
const uint32_t avmplus_StringBuilder_ensureCapacity = 78;
const uint32_t avmplus_StringBuilder_indexOf = 79;
const uint32_t avmplus_StringBuilder_insert = 80;
const uint32_t avmplus_StringBuilder_lastIndexOf = 81;
const uint32_t avmplus_StringBuilder_length_get = 82;
const uint32_t avmplus_StringBuilder_length_set = 83;
const uint32_t avmplus_StringBuilder_remove = 84;
const uint32_t avmplus_StringBuilder_removeCharAt = 85;
const uint32_t avmplus_StringBuilder_replace = 86;
const uint32_t avmplus_StringBuilder_reverse = 87;
const uint32_t avmplus_StringBuilder_setCharAt = 88;
const uint32_t avmplus_StringBuilder_substring = 89;
const uint32_t avmplus_StringBuilder_toString = 90;
const uint32_t avmplus_StringBuilder_trimToSize = 91;
const uint32_t flash_utils_ByteArray_readFile = 94;
const uint32_t flash_utils_ByteArray_writeFile = 95;
const uint32_t flash_utils_ByteArray_readBytes = 96;
const uint32_t flash_utils_ByteArray_writeBytes = 97;
const uint32_t flash_utils_ByteArray_writeBoolean = 98;
const uint32_t flash_utils_ByteArray_writeByte = 99;
const uint32_t flash_utils_ByteArray_writeShort = 100;
const uint32_t flash_utils_ByteArray_writeInt = 101;
const uint32_t flash_utils_ByteArray_writeUnsignedInt = 102;
const uint32_t flash_utils_ByteArray_writeFloat = 103;
const uint32_t flash_utils_ByteArray_writeDouble = 104;
const uint32_t flash_utils_ByteArray_writeUTF = 105;
const uint32_t flash_utils_ByteArray_writeUTFBytes = 106;
const uint32_t flash_utils_ByteArray_readBoolean = 107;
const uint32_t flash_utils_ByteArray_readByte = 108;
const uint32_t flash_utils_ByteArray_readUnsignedByte = 109;
const uint32_t flash_utils_ByteArray_readShort = 110;
const uint32_t flash_utils_ByteArray_readUnsignedShort = 111;
const uint32_t flash_utils_ByteArray_readInt = 112;
const uint32_t flash_utils_ByteArray_readUnsignedInt = 113;
const uint32_t flash_utils_ByteArray_readFloat = 114;
const uint32_t flash_utils_ByteArray_readDouble = 115;
const uint32_t flash_utils_ByteArray_readUTF = 116;
const uint32_t flash_utils_ByteArray_readUTFBytes = 117;
const uint32_t flash_utils_ByteArray_length_get = 118;
const uint32_t flash_utils_ByteArray_length_set = 119;
const uint32_t flash_utils_ByteArray_private_zlib_compress = 120;
const uint32_t flash_utils_ByteArray_private_zlib_uncompress = 122;
const uint32_t flash_utils_ByteArray_private__toString = 124;
const uint32_t flash_utils_ByteArray_bytesAvailable_get = 126;
const uint32_t flash_utils_ByteArray_position_get = 127;
const uint32_t flash_utils_ByteArray_position_set = 128;
const uint32_t flash_utils_ByteArray_endian_get = 129;
const uint32_t flash_utils_ByteArray_endian_set = 130;
const uint32_t flash_sampler_isGetterSetter = 133;
const uint32_t flash_sampler__getInvocationCount = 134;
const uint32_t flash_sampler_getSampleCount = 138;
const uint32_t flash_sampler_getSamples = 139;
const uint32_t flash_sampler_getMemberNames = 140;
const uint32_t flash_sampler_getSize = 141;
const uint32_t flash_sampler__setSamplerCallback = 142;
const uint32_t flash_sampler_sampleInternalAllocs = 145;
const uint32_t flash_sampler_pauseSampling = 146;
const uint32_t flash_sampler_stopSampling = 147;
const uint32_t flash_sampler_startSampling = 148;
const uint32_t flash_sampler_clearSamples = 149;
const uint32_t flash_sampler_NewObjectSample_object_get = 156;
const uint32_t flash_sampler_NewObjectSample_size_get = 157;
const uint32_t flash_trace_Trace_setLevel = 163;
const uint32_t flash_trace_Trace_getLevel = 164;
const uint32_t flash_trace_Trace_setListener = 165;
const uint32_t flash_trace_Trace_getListener = 166;
const uint32_t flash_utils_Dictionary_private_init = 170;
const uint32_t avmplus_JObject_create = 177;
const uint32_t avmplus_JObject_createArray = 178;
const uint32_t avmplus_JObject_toArray = 179;
const uint32_t avmplus_JObject_constructorSignature = 180;
const uint32_t avmplus_JObject_methodSignature = 181;
const uint32_t avmplus_JObject_fieldSignature = 182;
extern AvmBox shell_toplevel_s2a_oos_rest_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_JObject_methodSignature_thunk  shell_toplevel_s2a_oos_rest_thunk

extern double shell_toplevel_d2d_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_sampler_NewObjectSample_size_get_thunk  shell_toplevel_d2d_o_thunk
#define avmplus_System_freeMemory_get_thunk  shell_toplevel_d2d_o_thunk
#define avmplus_System_privateMemory_get_thunk  shell_toplevel_d2d_o_thunk
#define flash_sampler_getSampleCount_thunk  shell_toplevel_d2d_o_thunk
#define flash_utils_ByteArray_readFloat_thunk  shell_toplevel_d2d_o_thunk
#define avmplus_System_totalMemory_get_thunk  shell_toplevel_d2d_o_thunk
#define flash_utils_ByteArray_readDouble_thunk  shell_toplevel_d2d_o_thunk

extern AvmBox shell_toplevel_v2a_oouu_opti0_opti0_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_writeBytes_thunk  shell_toplevel_v2a_oouu_opti0_opti0_thunk
#define flash_utils_ByteArray_readBytes_thunk  shell_toplevel_v2a_oouu_opti0_opti0_thunk

extern AvmBox shell_toplevel_a2a_os_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_readFile_thunk  shell_toplevel_a2a_os_thunk
#define db_MySQL_fetch_thunk  shell_toplevel_a2a_os_thunk
#define avmplus_Domain_getClass_thunk  shell_toplevel_a2a_os_thunk

extern AvmBox shell_toplevel_u2a_ou_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_StringBuilder_charCodeAt_thunk  shell_toplevel_u2a_ou_thunk

extern AvmBox shell_toplevel_a2a_ou_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_StringBuilder_length_set_thunk  shell_toplevel_a2a_ou_thunk

extern AvmBox shell_toplevel_v2a_oi_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_writeByte_thunk  shell_toplevel_v2a_oi_thunk
#define avmplus_System_exit_thunk  shell_toplevel_v2a_oi_thunk
#define flash_utils_ByteArray_writeInt_thunk  shell_toplevel_v2a_oi_thunk
#define flash_utils_ByteArray_writeShort_thunk  shell_toplevel_v2a_oi_thunk

extern AvmBox shell_toplevel_s2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_System_readLine_thunk  shell_toplevel_s2a_o_thunk
#define http__HTTP_postData_thunk  shell_toplevel_s2a_o_thunk
#define flash_utils_ByteArray_endian_get_thunk  shell_toplevel_s2a_o_thunk
#define flash_utils_ByteArray_readUTF_thunk  shell_toplevel_s2a_o_thunk
#define flash_utils_ByteArray_private__toString_thunk  shell_toplevel_s2a_o_thunk
#define avmplus_System_getAvmplusVersion_thunk  shell_toplevel_s2a_o_thunk
#define avmplus_StringBuilder_toString_thunk  shell_toplevel_s2a_o_thunk
#define db_MySQL_getError_thunk  shell_toplevel_s2a_o_thunk

extern AvmBox shell_toplevel_v2a_oss_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_File_write_thunk  shell_toplevel_v2a_oss_thunk

extern AvmBox shell_toplevel_b2a_oao_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_sampler_isGetterSetter_thunk  shell_toplevel_b2a_oao_thunk

extern AvmBox shell_toplevel_i2a_oi_opti2_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_trace_Trace_getLevel_thunk  shell_toplevel_i2a_oi_opti2_thunk

extern AvmBox shell_toplevel_v2a_ous_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_StringBuilder_setCharAt_thunk  shell_toplevel_v2a_ous_thunk

extern AvmBox shell_toplevel_u2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_readUnsignedShort_thunk  shell_toplevel_u2a_o_thunk
#define flash_utils_ByteArray_bytesAvailable_get_thunk  shell_toplevel_u2a_o_thunk
#define flash_utils_ByteArray_length_get_thunk  shell_toplevel_u2a_o_thunk
#define avmplus_StringBuilder_length_get_thunk  shell_toplevel_u2a_o_thunk
#define flash_utils_ByteArray_readUnsignedInt_thunk  shell_toplevel_u2a_o_thunk
#define flash_utils_ByteArray_readUnsignedByte_thunk  shell_toplevel_u2a_o_thunk
#define avmplus_System_getTimer_thunk  shell_toplevel_u2a_o_thunk
#define avmplus_Domain_MIN_DOMAIN_MEMORY_LENGTH_get_thunk  shell_toplevel_u2a_o_thunk
#define avmplus_StringBuilder_capacity_get_thunk  shell_toplevel_u2a_o_thunk
#define flash_utils_ByteArray_position_get_thunk  shell_toplevel_u2a_o_thunk

extern AvmBox shell_toplevel_v2a_ouu_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_StringBuilder_remove_thunk  shell_toplevel_v2a_ouu_thunk

extern AvmBox shell_toplevel_a2a_ooio_optakAvmThunkNull_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_JObject_createArray_thunk  shell_toplevel_a2a_ooio_optakAvmThunkNull_thunk

extern AvmBox shell_toplevel_s2a_os_rest_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_JObject_constructorSignature_thunk  shell_toplevel_s2a_os_rest_thunk

extern AvmBox shell_toplevel_i2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_readInt_thunk  shell_toplevel_i2a_o_thunk
#define flash_utils_ByteArray_readShort_thunk  shell_toplevel_i2a_o_thunk
#define flash_utils_ByteArray_readByte_thunk  shell_toplevel_i2a_o_thunk

extern AvmBox shell_toplevel_v2a_oua_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_StringBuilder_insert_thunk  shell_toplevel_v2a_oua_thunk

extern AvmBox shell_toplevel_s2a_oos_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_JObject_fieldSignature_thunk  shell_toplevel_s2a_oos_thunk

extern AvmBox shell_toplevel_v2a_ouus_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_StringBuilder_replace_thunk  shell_toplevel_v2a_ouus_thunk

extern AvmBox shell_toplevel_i2a_os_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_System_exec_thunk  shell_toplevel_i2a_os_thunk
#define db_MySQL_exec_thunk  shell_toplevel_i2a_os_thunk

extern AvmBox shell_toplevel_b2a_oa_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_System_isGlobal_thunk  shell_toplevel_b2a_oa_thunk

extern AvmBox shell_toplevel_s2a_os_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define http__HTTP_getEnv_thunk  shell_toplevel_s2a_os_thunk
#define db_MySQL_escape_thunk  shell_toplevel_s2a_os_thunk
#define avmplus_File_read_thunk  shell_toplevel_s2a_os_thunk

extern AvmBox shell_toplevel_s2a_ou_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_StringBuilder_charAt_thunk  shell_toplevel_s2a_ou_thunk
#define flash_utils_ByteArray_readUTFBytes_thunk  shell_toplevel_s2a_ou_thunk

extern AvmBox shell_toplevel_a2a_os_rest_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_JObject_create_thunk  shell_toplevel_a2a_os_rest_thunk

extern AvmBox shell_toplevel_b2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_readBoolean_thunk  shell_toplevel_b2a_o_thunk
#define avmplus_System_isDebugger_thunk  shell_toplevel_b2a_o_thunk

extern AvmBox shell_toplevel_b2a_os_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_File_exists_thunk  shell_toplevel_b2a_os_thunk
#define http__HTTPForm_isUploaded_thunk  shell_toplevel_b2a_os_thunk

extern AvmBox shell_toplevel_a2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_sampler_getSamples_thunk  shell_toplevel_a2a_o_thunk
#define http__HTTPForm_listFileKeys_thunk  shell_toplevel_a2a_o_thunk
#define avmplus_Domain_currentDomain_get_thunk  shell_toplevel_a2a_o_thunk
#define avmplus_System_private_getArgv_thunk  shell_toplevel_a2a_o_thunk
#define http__HTTP_listEnv_thunk  shell_toplevel_a2a_o_thunk
#define http__HTTPForm_getFields_thunk  shell_toplevel_a2a_o_thunk
#define flash_trace_Trace_getListener_thunk  shell_toplevel_a2a_o_thunk
#define flash_sampler_NewObjectSample_object_get_thunk  shell_toplevel_a2a_o_thunk
#define avmplus_Domain_domainMemory_get_thunk  shell_toplevel_a2a_o_thunk

extern double shell_toplevel_d2d_oaou_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_sampler__getInvocationCount_thunk  shell_toplevel_d2d_oaou_thunk

extern AvmBox shell_toplevel_b2a_ossss_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define db_MySQL_connect_thunk  shell_toplevel_b2a_ossss_thunk

extern AvmBox shell_toplevel_i2a_osu_optu4294967295U_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_StringBuilder_lastIndexOf_thunk  shell_toplevel_i2a_osu_optu4294967295U_thunk

extern AvmBox shell_toplevel_v2a_od_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_writeFloat_thunk  shell_toplevel_v2a_od_thunk
#define flash_utils_ByteArray_writeDouble_thunk  shell_toplevel_v2a_od_thunk

extern AvmBox shell_toplevel_s2a_ouu_optu4294967295U_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_StringBuilder_substring_thunk  shell_toplevel_s2a_ouu_optu4294967295U_thunk

extern AvmBox shell_toplevel_v2a_ob_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_Dictionary_private_init_thunk  shell_toplevel_v2a_ob_thunk
#define flash_utils_ByteArray_writeBoolean_thunk  shell_toplevel_v2a_ob_thunk
#define flash_sampler_sampleInternalAllocs_thunk  shell_toplevel_v2a_ob_thunk

extern AvmBox shell_toplevel_v2a_oa_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_StringBuilder_append_thunk  shell_toplevel_v2a_oa_thunk

extern double shell_toplevel_d2d_oa_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_sampler_getSize_thunk  shell_toplevel_d2d_oa_thunk

extern AvmBox shell_toplevel_v2a_oo_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_System_trace_thunk  shell_toplevel_v2a_oo_thunk
#define avmplus_Domain_private_init_thunk  shell_toplevel_v2a_oo_thunk
#define flash_sampler__setSamplerCallback_thunk  shell_toplevel_v2a_oo_thunk

extern AvmBox shell_toplevel_i2a_osu_opti0_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_StringBuilder_indexOf_thunk  shell_toplevel_i2a_osu_opti0_thunk

extern AvmBox shell_toplevel_v2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_sampler_startSampling_thunk  shell_toplevel_v2a_o_thunk
#define flash_utils_ByteArray_private_zlib_compress_thunk  shell_toplevel_v2a_o_thunk
#define flash_sampler_pauseSampling_thunk  shell_toplevel_v2a_o_thunk
#define flash_sampler_clearSamples_thunk  shell_toplevel_v2a_o_thunk
#define flash_sampler_stopSampling_thunk  shell_toplevel_v2a_o_thunk
#define avmplus_System_ns_example_nstest_thunk  shell_toplevel_v2a_o_thunk
#define avmplus_System_debugger_thunk  shell_toplevel_v2a_o_thunk
#define avmplus_StringBuilder_trimToSize_thunk  shell_toplevel_v2a_o_thunk
#define flash_utils_ByteArray_private_zlib_uncompress_thunk  shell_toplevel_v2a_o_thunk
#define avmplus_StringBuilder_reverse_thunk  shell_toplevel_v2a_o_thunk

extern AvmBox shell_toplevel_a2a_oii_opti2_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_trace_Trace_setLevel_thunk  shell_toplevel_a2a_oii_opti2_thunk

extern AvmBox shell_toplevel_v2a_ou_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_StringBuilder_ensureCapacity_thunk  shell_toplevel_v2a_ou_thunk
#define flash_utils_ByteArray_position_set_thunk  shell_toplevel_v2a_ou_thunk
#define avmplus_StringBuilder_removeCharAt_thunk  shell_toplevel_v2a_ou_thunk
#define flash_utils_ByteArray_writeUnsignedInt_thunk  shell_toplevel_v2a_ou_thunk
#define flash_utils_ByteArray_length_set_thunk  shell_toplevel_v2a_ou_thunk

extern AvmBox shell_toplevel_v2a_os_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_endian_set_thunk  shell_toplevel_v2a_os_thunk
#define flash_utils_ByteArray_writeUTF_thunk  shell_toplevel_v2a_os_thunk
#define flash_utils_ByteArray_writeUTFBytes_thunk  shell_toplevel_v2a_os_thunk
#define http__HTTP_write_thunk  shell_toplevel_v2a_os_thunk
#define avmplus_System_write_thunk  shell_toplevel_v2a_os_thunk
#define http__HTTP_logError_thunk  shell_toplevel_v2a_os_thunk
#define flash_utils_ByteArray_writeFile_thunk  shell_toplevel_v2a_os_thunk

extern AvmBox shell_toplevel_b2a_oss_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define http__HTTPForm_saveFile_thunk  shell_toplevel_b2a_oss_thunk

extern AvmBox shell_toplevel_a2a_oab_optbfalse_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_sampler_getMemberNames_thunk  shell_toplevel_a2a_oab_optbfalse_thunk

extern AvmBox shell_toplevel_a2a_oo_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_JObject_toArray_thunk  shell_toplevel_a2a_oo_thunk
#define avmplus_Domain_loadBytes_thunk  shell_toplevel_a2a_oo_thunk
#define avmplus_Domain_domainMemory_set_thunk  shell_toplevel_a2a_oo_thunk
#define flash_trace_Trace_setListener_thunk  shell_toplevel_a2a_oo_thunk
