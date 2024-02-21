/*
 *  bin2c - compresses data files & converts the result to C source code
 *  Copyright (C) 1998-2000  Anders Widell  <awl@hem.passagen.se>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * This command uses the zlib library to compress each file given on
 * the command line, and outputs the compressed data as C source code
 * to the file 'data.c' in the current directory
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_LIBZ
#include <zlib.h>
#else
typedef unsigned char Bytef;
typedef unsigned long uLongf;
#endif

#define BUFSIZE 16384 /* Increase buffer size by this amount */

#define SUFFIXLEN 8

static Bytef *source = NULL;	   /* Buffer containing uncompressed data */
static Bytef *dest = NULL;	   /* Buffer containing compressed data */
static uLongf source_buf_size = 0; /* Buffer size */
#ifdef USE_LIBZ
static uLongf dest_buf_size = 0; /* Buffer size */
#endif

static uLongf source_len; /* Length of uncompressed data */
static uLongf dest_len;	  /* Length of compressed data */

static FILE *infile = NULL;  /* The input file containing binary data */
static FILE *outfile = NULL; /* The output file 'data.c' */

static const char *program_name = "";

/*
 * Print error message and free allocated resources
 *
 */

static int error(char *msg1, char *msg2, char *msg3)
{
	fprintf(stderr, "%s: %s%s%s\n", program_name, msg1, msg2, msg3);

	if (infile != NULL) fclose(infile);
	if (outfile != NULL) fclose(outfile);
	remove("data.c");
	free(dest);
	free(source);

	return 1;
}

/*
 * Replacement for strrchr in case it isn't present in libc
 *
 */

static char *my_strrchr(char *s, int c)
{
	char *ptr = NULL;

	while (*s) {
		if (*s == c) ptr = s;
		s++;
	}

	return ptr;
}

#ifdef USE_LIBZ
/*
 * NOTE: my_compress2 is taken directly from zlib 1.1.3
 *
 * This is for compability with early versions of zlib that
 * don't have the compress2 function.
 *
 */

/*
     Compresses the source buffer into the destination buffer. The level
   parameter has the same meaning as in deflateInit.  source_len is the byte
   length of the source buffer. Upon entry, dest_len is the total size of the
   destination buffer, which must be at least 0.1% larger than source_len plus
   12 bytes. Upon exit, dest_len is the actual size of the compressed buffer.

     compress2 returns Z_OK if success, Z_MEM_ERROR if there was not enough
   memory, Z_BUF_ERROR if there was not enough room in the output buffer,
   Z_STREAM_ERROR if the level parameter is invalid.
*/
int my_compress2(Bytef *dest, uLongf *dest_len, const Bytef *source, uLong source_len, int level)
{
	z_stream stream;
	int err;

	stream.next_in = (Bytef *)source;
	stream.avail_in = (uInt)source_len;
#ifdef MAXSEG_64K
	/* Check for source > 64K on 16-bit machine: */
	if ((uLong)stream.avail_in != source_len) return Z_BUF_ERROR;
#endif
	stream.next_out = dest;
	stream.avail_out = (uInt)*dest_len;
	if ((uLong)stream.avail_out != *dest_len) return Z_BUF_ERROR;

	stream.zalloc = (alloc_func)0;
	stream.zfree = (free_func)0;
	stream.opaque = (voidpf)0;

	err = deflateInit(&stream, level);
	if (err != Z_OK) return err;

	err = deflate(&stream, Z_FINISH);
	if (err != Z_STREAM_END) {
		deflateEnd(&stream);
		return err == Z_OK ? Z_BUF_ERROR : err;
	}
	*dest_len = stream.total_out;

	err = deflateEnd(&stream);
	return err;
}
#endif

const char *usage =
    "\nUsage: ./bin2c -o <output-file> file1 [file2 [file3 [...]]]\n\n"
    "    Example: ./bin2c -o data.c a.bmp b.jpg c.png\n\n";

static char *outputfile;
static char **filelist;
static int file_list;

typedef struct _export_list {
	char *export_data;
	struct _export_list *next;
} export_list_t;

static export_list_t *exports_head = NULL;

static const char *add_export(const char *filename) {
	int idx = 0;
	int i;
	const char *begin;
	const char *ext;
	static char strname[1024];
	begin = strrchr(filename, '/');
	if (begin == NULL) begin = filename;

	ext = strrchr(begin, '.');
	i = (ext ? (ext - begin) : strlen(begin)) + 10;
	// char* strname = (char*)malloc(i);

	strname[idx++] = '_';
	if (ext) {
		for (i = 1; ext[i]; i++) strname[idx++] = ext[i];
		strname[idx++] = '_';
	}

	for (i = 0; (ext && &begin[i] < ext) || (!ext && begin[i]); i++) {
		if (isalnum(begin[i]))
			strname[idx++] = begin[i];
		else
			strname[idx++] = '_';
	}

	if (strname[idx - 1] == '_')
		strcpy(strname + idx, "data");
	else
		strcpy(strname + idx, "_data");

	export_list_t *els = (export_list_t *)calloc(1, sizeof(export_list_t));
	els->export_data = strname;
	els->next = exports_head;
	exports_head = els;

	return strname;
}

static void print_exports(FILE *f) {
	export_list_t *els = exports_head;
	while (els) {
		fprintf(f, "\t%s\n", els->export_data);
		els = els->next;
	}
}

static int parser_args(int argc, char *argv[]) {
	int i;
	int list_idx = 0;
	if (argc < 4) {
		printf(usage);
		return 0;
	}
	filelist = (char **)calloc(argc - 3, sizeof(char *));
	// try find outputfile
	i = 1;
	while (i < argc) {
		switch (argv[i][0]) {
			case '-':
				if (argv[i][1] == 'o') {
					outputfile = argv[++i];
					break;
				} else if (argv[i][1] == 'h' ||
					   argv[i][1] == '?') {
					printf(usage);
					return 0;
				}
			default:
				filelist[list_idx++] = argv[i];
				break;
		}
		i++;
	}

	return 1;
}

int main(int argc, char **argv)
{
	int i;
	char suffix[SUFFIXLEN];
#ifdef USE_LIBZ
	int result;
#endif
	unsigned j;
	char *ptr;
	int position;

	program_name = argv[0];
	if (!parser_args(argc, argv)) return 1;

	outfile = fopen(outputfile, "w");
	if (outfile == NULL) {
		fprintf(stderr, "%s: can't open 'data.c' for writing\n",
			argv[0]);
		return 1;
	}

	/* Process each file given on command line */
	for (i = 0; i < argc - 3; i++) {
		infile = fopen(filelist[i], "rb");
		if (infile == NULL)
			return error("can't open '", argv[i], "' for reading");

		/* Read infile to source buffer */
		source_len = 0;
		while (!feof(infile)) {
			if (source_len + BUFSIZE > source_buf_size) {
				source_buf_size += BUFSIZE;
				source = realloc(source, source_buf_size);
				if (source == NULL)
					return error("memory exhausted", "",
						     "");
			}
			source_len +=
			    fread(source + source_len, 1, BUFSIZE, infile);
			if (ferror(infile))
				return error("error reading '", argv[i], "'");
		}
		fclose(infile);

#ifdef USE_LIBZ

		/* (Re)allocate dest buffer */
		dest_len = source_buf_size + (source_buf_size + 9) / 10 + 12;
		if (dest_buf_size < dest_len) {
			dest_buf_size = dest_len;
			dest = realloc(dest, dest_buf_size);
			if (dest == NULL)
				return error("memory exhausted", "", "");
		}

		/* Compress dest buffer */
		dest_len = dest_buf_size;
		result = my_compress2(dest, &dest_len, source, source_len, 9);
		if (result != Z_OK)
			return error("error compressing '", argv[i], "'");

#else

		dest_len = source_len;
		dest = source;

#endif

		/* Output dest buffer as C source code to outfile */

		fprintf(outfile, "static const unsigned char %s[] = {\n",
			add_export(filelist[i]));

		for (j = 0; j < dest_len - 1; j++) {
			switch (j % 8) {
				case 0:
					fprintf(outfile, "  0x%02x, ",
						((unsigned)dest[j]) & 0xffu);
					break;
				case 7:
					fprintf(outfile, "0x%02x,\n",
						((unsigned)dest[j]) & 0xffu);
					break;
				default:
					fprintf(outfile, "0x%02x, ",
						((unsigned)dest[j]) & 0xffu);
					break;
			}
		}

		if ((dest_len - 1) % 8 == 0)
			fprintf(outfile, "  0x%02x\n};\n\n",
				((unsigned)dest[dest_len - 1]) & 0xffu);
		else
			fprintf(outfile, "0x%02x\n};\n\n",
				((unsigned)dest[dest_len - 1]) & 0xffu);
	}

	fprintf(outfile, "/*********************************************\n");
	fprintf(outfile, "Export:\n");
	print_exports(outfile);
	fprintf(outfile, "**********************************************/\n");

	fclose(outfile);
#ifdef USE_LIBZ
	free(dest);
#endif
	free(source);

	return 0;
}
