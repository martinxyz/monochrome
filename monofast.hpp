
class World {
  public:
  int radius = 700;
  PyObject * render(PyObject * arr) {

    Py_buffer buf;
    int res = PyObject_GetBuffer(arr, &buf, PyBUF_WRITABLE|PyBUF_STRIDES);
    if (res != 0) return NULL;
    assert(buf.ndim == 2);     // 2 dimensions (y, x)
    assert(buf.strides[0] == 4); // 4 bytes stride (unused alpha)

    int width  = buf.shape[0];
    int height = buf.shape[1];
    uint8_t * p = (uint8_t*)buf.buf;
    
    int radius2 = radius*radius;
    uint32_t one_over_radius2 = (1L<<32)/(radius*radius);
    int cx = 400;
    int cy = 300;

    for (int y=0; y<height; y++) {
      for (int x=0; x<width; x++) {
        int dist2 = (x-cx)*(x-cx) + (y-cy)*(y-cy);
        if (dist2 < radius2) {
          uint64_t fac = dist2 * one_over_radius2 / (1<<16);
          p[0] = (p[0] * fac) >> 16;
          p[1] = (p[1] * fac) >> 16;
          p[2] = (p[2] * fac) >> 16;
        }
        p += 4;
      }
    }
    Py_RETURN_NONE;
  }
};
