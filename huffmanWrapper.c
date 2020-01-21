// huffmanWrapper.c
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "huffman.h"

static unsigned char huffbuf[HUFFHEAP_SIZE];

/*  */
PyObject* pyhuffman_compress(PyObject* self, PyObject* args) {
  const char* in_filename = NULL;
  const char* out_filename = NULL;
  unsigned char* in_buff = NULL;
  unsigned char* out_buff = NULL;
  long inlen = 0;
  long outlen = 0;
  unsigned long rst = 0;
  size_t readlen = 0;
  int rtn = 0;
  FILE *fp;

  if (!PyArg_ParseTuple(args, "ss", &in_filename, &out_filename)) {
    printf("parse args error...\n");
    return NULL;
  }
  fp = fopen(in_filename, "rb");
  if (NULL == fp) {
    printf("open file error... [%s] errno:[%d] errmsg:[%s]\n", in_filename, errno, strerror(errno));
    return Py_BuildValue("s", "in_file open error");
  }
  rtn = fseek(fp, 0, SEEK_END);
  if (rtn != 0) {
    fclose(fp);
    printf("fseek error[SEEK_END]... [%s] errno:[%d] errmsg:[%s]\n", in_filename, errno, strerror(errno));
    return Py_BuildValue("s", "in_file open error");
  }
  inlen = ftell(fp);
  if (inlen == -1L) {
    fclose(fp);
    printf("ftell error... [%s] errno:[%d] errmsg:[%s]\n", in_filename, errno, strerror(errno));
    return Py_BuildValue("");
  }
  rtn = fseek(fp, 0, SEEK_SET);
  if (rtn != 0) {
    fclose(fp);
    printf("fseek error[SEEK_SET]... [%s] errno:[%d] errmsg:[%s]\n", in_filename, errno, strerror(errno));
    return Py_BuildValue("");
  }
  outlen = inlen * 2 + 1;     // Just to prevent memory overflow, it is possible that the size becomes larger after compression.
  inlen += 1;
  in_buff = (unsigned char*)PyMem_Malloc(sizeof(unsigned char) * inlen);
  if (NULL == in_buff) {
    fclose(fp);
    printf("PyMem_Malloc(in_buff) error... [%s] errno:[%d] errmsg:[%s]\n", in_filename, errno, strerror(errno));
    return PyErr_NoMemory();
  }
  memset(in_buff, 0x00, sizeof(unsigned char) * inlen);
  out_buff = (unsigned char*)PyMem_Malloc(sizeof(unsigned char) * outlen);
  if (NULL == out_buff) {
    fclose(fp);
    PyMem_Free(in_buff); in_buff = 0x00;
    printf("PyMem_Malloc(out_buff) error... [%s] errno:[%d] errmsg:[%s]\n", in_filename, errno, strerror(errno));
    return PyErr_NoMemory();
  }
  memset(out_buff, 0x00, sizeof(unsigned char) * outlen);
  readlen = fread(in_buff, sizeof(unsigned char), inlen, fp);
  fclose(fp);
  rst = huffman_compress(in_buff, (unsigned long)readlen, out_buff, (unsigned long)outlen, huffbuf);
  if (rst) {
    // compress success
    fp = fopen(out_filename, "w+b");
    fwrite(out_buff, sizeof(unsigned char), rst, fp);
    fclose(fp);
  }
  PyMem_Free(in_buff); in_buff = 0x00;
  PyMem_Free(out_buff); out_buff = 0x00;
  return Py_BuildValue("k", rst);
}

PyObject* pyhuffman_decompress(PyObject* self, PyObject* args) {
  const char* in_filename = NULL;
  const char* out_filename = NULL;
  unsigned char* in_buff = NULL;
  unsigned char* out_buff = NULL;
  long inlen = 0;
  long outlen = 0;
  unsigned long rst = 0;
  size_t readlen = 0;
  int rtn = 0;
  FILE *fp;

  if (!PyArg_ParseTuple(args, "ss", &in_filename, &out_filename)) {
    printf("parse args error...\n");
    return NULL;
  }
  fp = fopen(in_filename, "rb");
  if (NULL == fp) {
    printf("open file error... [%s] errno:[%d] errmsg:[%s]\n", in_filename, errno, strerror(errno));
    return Py_BuildValue("");
  }
  rtn = fseek(fp, 0, SEEK_END);
  if (rtn != 0) {
    fclose(fp);
    printf("fseek error[SEEK_END]... [%s] errno:[%d] errmsg:[%s]\n", in_filename, errno, strerror(errno));
    return Py_BuildValue("");
  }
  inlen = ftell(fp);
  if (inlen == -1L) {
    fclose(fp);
    printf("ftell error... [%s] errno:[%d] errmsg:[%s]\n", in_filename, errno, strerror(errno));
    return Py_BuildValue("");
  }
  rtn = fseek(fp, 0, SEEK_SET);
  if (rtn != 0) {
    fclose(fp);
    printf("fseek error[SEEK_SET]... [%s] errno:[%d] errmsg:[%s]\n", in_filename, errno, strerror(errno));
    return Py_BuildValue("");
  }
  outlen = inlen * 4 + 1;     // Just to prevent memory overflow, it is possible that the size becomes larger after compression.
  inlen += 1;
  in_buff = (unsigned char*)PyMem_Malloc(sizeof(unsigned char) * inlen);
  if (NULL == in_buff) {
    fclose(fp);
    printf("PyMem_Malloc(in_buff) error... [%s] errno:[%d] errmsg:[%s]\n", in_filename, errno, strerror(errno));
    return PyErr_NoMemory();
  }
  memset(in_buff, 0x00, sizeof(unsigned char) * inlen);
  out_buff = (unsigned char*)PyMem_Malloc(sizeof(unsigned char) * outlen);
  if (NULL == out_buff) {
    fclose(fp);
    PyMem_Free(in_buff); in_buff = 0x00;
    printf("PyMem_Malloc(out_buff) error... [%s] errno:[%d] errmsg:[%s]\n", in_filename, errno, strerror(errno));
    return PyErr_NoMemory();
  }
  memset(out_buff, 0x00, sizeof(unsigned char) * outlen);
  readlen = fread(in_buff, sizeof(unsigned char), inlen, fp);
  fclose(fp);
  rst = huffman_decompress(in_buff, (unsigned long)readlen, out_buff, (unsigned long)outlen, huffbuf);
  if (rst) {
    // decompress success
    fp = fopen(out_filename, "w+b");
    fwrite(out_buff, sizeof(unsigned char), rst, fp);
    fclose(fp);
  }
  PyMem_Free((void *)in_buff); in_buff = 0x00;
  PyMem_Free((void *)out_buff); out_buff = 0x00;
  return Py_BuildValue("k", rst);
}

static PyMethodDef huffmanmethods[] = {
  {"compress", pyhuffman_compress, METH_VARARGS},
  {"decompress", pyhuffman_decompress, METH_VARARGS},
  {NULL},
};

static struct PyModuleDef huffmanmodule = {
  PyModuleDef_HEAD_INIT,
  "PycHuffman",
  "huffman compress decompress",
  -1,
  huffmanmethods
};

PyMODINIT_FUNC PyInit_PycHuffman(void) {
  return PyModule_Create(&huffmanmodule);
}
