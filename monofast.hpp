
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
    int cx = 400;
    int cy = 300;

    for (int y=0; y<height; y++) {
      for (int x=0; x<width; x++) {
        int dist2 = (x-cx)*(x-cx) + (y-cy)*(y-cy);
        if (dist2 < radius2) {
          p[0] = p[0] * dist2 / radius2;
          p[1] = p[1] * dist2 / radius2;
          p[2] = p[2] * dist2 / radius2;
        }
        p += 4;
      }
    }
    Py_RETURN_NONE;
  }
};