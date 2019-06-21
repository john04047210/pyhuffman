// huffmanWrapper.c
#include "Python.h"
#include "huffman.h"

static unsigned char huffbuf[HUFFHEAP_SIZE];

/*  */
PyObject* pyhuffman_compress(PyObject* self, PyObject* args) {
  unsigned char* in_filename = NULL;
  unsigned char* out_filename = NULL;
  unsigned char* in_buff = NULL;
  unsigned char* out_buff = NULL;
  unsigned long inlen = 0;
  unsigned long outlen = 0;
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
  in_buff = (char*)malloc(sizeof(char) * inlen);
  memset(in_buff, 0x00, sizeof(char) * inlen);
  out_buff = (char*)malloc(sizeof(char) * outlen);
  memset(out_buff, 0x00, sizeof(char) * outlen);
  readlen = fread(in_buff, sizeof(char), inlen, fp);
  fclose(fp);
  rst = huffman_compress(in_buff, inlen, out_buff, outlen, huffbuf);
  if (rst) {
    // compress success
    fp = fopen(out_filename, "w+b");
    fwrite(out_buff, sizeof(char), rst, fp);
    fclose(fp);
  }
  free(in_buff); in_buff = 0x00;
  free(out_buff); out_buff = 0x00;
  return Py_BuildValue("l", rst);
}

PyObject* pyhuffman_decompress(PyObject* self, PyObject* args) {
  unsigned char* in_filename = NULL;
  unsigned char* out_filename = NULL;
  unsigned char* in_buff = NULL;
  unsigned char* out_buff = NULL;
  unsigned long inlen = 0;
  unsigned long outlen = 0;
  unsigned long rst = 0;
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
  outlen = inlen * 2 + 1;     // Just to prevent memory overflow, it is possible that the size becomes larger after compression.
  inlen += 1;
  in_buff = (char*)malloc(sizeof(char) * inlen);
  memset(in_buff, 0x00, sizeof(char) * inlen);
  out_buff = (char*)malloc(sizeof(char) * outlen);
  memset(out_buff, 0x00, sizeof(char) * outlen);
  fread(in_buff, sizeof(char), inlen, fp);
  fclose(fp);
  rst = huffman_decompress(in_buff, inlen, out_buff, outlen, huffbuf);
  if (rst) {
    // decompress success
    fp = fopen(out_filename, "w+b");
    fwrite(out_buff, sizeof(char), rst - 1, fp);
    fclose(fp);
  }
  free(in_buff); in_buff = 0x00;
  free(out_buff); out_buff = 0x00;
  return Py_BuildValue("l", rst);
}

static PyMethodDef huffmanmethods[] = {
  {"compress", pyhuffman_compress, METH_VARARGS},
  {"decompress", pyhuffman_decompress, METH_VARARGS},
  {NULL},
};

static struct PyModuleDef huffmanmodule = {
  PyModuleDef_HEAD_INIT,
  "pyhuffman",
  "huffman compress decompress",
  -1,
  huffmanmethods
};

PyMODINIT_FUNC PyInit_pyhuffman(void) {
  return PyModule_Create(&huffmanmodule);
}
