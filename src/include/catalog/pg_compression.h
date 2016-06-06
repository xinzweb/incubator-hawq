/*-------------------------------------------------------------------------
 *
 * pg_compress.h
 * 
 * 		Representation of compression algorithms for user extensibility of
 * 		compression used by the storage layer.
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_COMPRESSION
#define PG_COMPRESSION

#include "catalog/genbki.h"

#include "utils/relcache.h"


/* TIDYCAT_BEGINDEF

   CREATE TABLE pg_compression
   with (relid=3056, reltype_oid=3057, CamelCase=Compression)
   (
   compname				name, 
   compconstructor		regproc,
   compdestructor		regproc,
   compcompressor		regproc,
   compdecompressor		regproc,
   compvalidator		regproc,
   compowner			oid
   );

   create unique index on pg_compression(oid) with (indexid=3058, CamelCase=CompressionOid);
   create unique index on pg_compression(compname) with (indexid=3059, CamelCase=CompressionCompname);

   alter table pg_compression add fk compconstructor on pg_proc(oid);
   alter table pg_compression add fk compdestructor on pg_proc(oid);
   alter table pg_compression add fk compcompressor on pg_proc(oid);
   alter table pg_compression add fk compdecompressor on pg_proc(oid);
   alter table pg_compression add fk compvalidator on pg_proc(oid);
   alter table pg_compression add fk compowner on pg_authid(oid);

   TIDYCAT_ENDDEF
*/
/* TIDYCAT_BEGIN_CODEGEN 

   WARNING: DO NOT MODIFY THE FOLLOWING SECTION: 
   Generated by ./tidycat.pl version 31
   on Thu Sep  1 16:43:17 2011
*/


/*
 TidyCat Comments for pg_compression:
  Table has an Oid column.
  Table has static type (see pg_types.h).

*/

/* ----------------
 *		pg_compression definition.  cpp turns this into
 *		typedef struct FormData_pg_compression
 * ----------------
 */
#define CompressionRelationId	3056

CATALOG(pg_compression,3056)
{
	NameData	compname;			
	regproc		compconstructor;	
	regproc		compdestructor;		
	regproc		compcompressor;		
	regproc		compdecompressor;	
	regproc		compvalidator;		
	Oid			compowner;			
} FormData_pg_compression;


/* ----------------
 *		Form_pg_compression corresponds to a pointer to a tuple with
 *		the format of pg_compression relation.
 * ----------------
 */
typedef FormData_pg_compression *Form_pg_compression;


/* ----------------
 *		compiler constants for pg_compression
 * ----------------
 */
#define Natts_pg_compression					7
#define Anum_pg_compression_compname			1
#define Anum_pg_compression_compconstructor		2
#define Anum_pg_compression_compdestructor		3
#define Anum_pg_compression_compcompressor		4
#define Anum_pg_compression_compdecompressor	5
#define Anum_pg_compression_compvalidator		6
#define Anum_pg_compression_compowner			7


/* TIDYCAT_END_CODEGEN */

/* Initial contents */
DATA(insert OID = 3069 ( snappy gp_snappy_constructor gp_snappy_destructor gp_snappy_compress gp_snappy_decompress gp_snappy_validator PGUID ));

DATA(insert OID = 3060 ( zlib gp_zlib_constructor gp_zlib_destructor gp_zlib_compress gp_zlib_decompress gp_zlib_validator PGUID ));

DATA(insert OID = 3061 ( quicklz gp_quicklz_constructor gp_quicklz_destructor gp_quicklz_compress gp_quicklz_decompress gp_quicklz_validator PGUID ));

DATA(insert OID = 3062 ( rle_type gp_rle_type_constructor gp_rle_type_destructor gp_rle_type_compress gp_rle_type_decompress gp_rle_type_validator PGUID ));

DATA(insert OID = 3063 ( none gp_dummy_compression_constructor gp_dummy_compression_destructor gp_dummy_compression_compress gp_dummy_compression_decompress gp_dummy_compression_validator PGUID ));

#define NUM_COMPRESS_FUNCS 5

#define COMPRESSION_CONSTRUCTOR 0
#define COMPRESSION_DESTRUCTOR 1
#define COMPRESSION_COMPRESS 2
#define COMPRESSION_DECOMPRESS 3
#define COMPRESSION_VALIDATOR 4

typedef struct CompressionState
{
	/*
	 * Allows a constructor to tell the calling level the maximum storage
	 * required for input of the given size. Different algorithms need
	 * different maximum buffers. For example, quicklz's compression
	 * algorithm needs at most input size + 400 bytes.
	 */
	size_t (*desired_sz)(size_t input);

	void *opaque; /* algorithm specific stuff opaque to the caller */
} CompressionState;

typedef struct StorageAttributes
{
	char *comptype; /* compresstype field */
	int complevel; /* compresslevel field */
	size_t blocksize; /* blocksize field */
	Oid	typid; /* Oid of the type being compressed */
} StorageAttributes;

extern CompressionState *callCompressionConstructor(PGFunction constructor,
										TupleDesc tupledesc,
										StorageAttributes *sa,
										bool is_compress);

extern void callCompressionDestructor(PGFunction destructor, CompressionState *state);

extern void callCompressionActuator(PGFunction func, const void *src,
									int32 src_sz, char *dst, int32 dst_sz,
									int32 *dst_used, CompressionState *state);

extern void callCompressionValidator(PGFunction func, char *comptype,
									 int32 complevel, int32 blocksize,
									 Oid typid);

extern bool compresstype_is_valid(char *compresstype);
extern List *default_column_encoding_clause(void);
extern PGFunction *GetCompressionImplementation(char *comptype);
extern bool is_storage_encoding_directive(char *name);

#endif   /* PG_COMPRESSION */
