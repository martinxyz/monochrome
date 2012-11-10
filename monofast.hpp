const int size = 2000;
/* 
def _apow(v, p):
    return (v >= 0) and (v**p) or -((-v)**p)

def linearRGB_component_to_sRGB(c):
    if abs(c) <= 0.0031308:
        return 12.92 * c
    else:
        return 1.055 * _apow(c, 1/2.4) - 0.055

for i in range(17):
    lin_c = i / 16.0
    print("%d," % round(linearRGB_component_to_sRGB(lin_c)*255))

 */
static const uint8_t linear_light_17_steps_to_sRGB[17] = {
  0, 71, 99, 120, 137, 152, 165, 177, 188, 198, 207, 216, 225, 233, 240, 248, 255,
};


static inline uint8_t subpos2bit(uint8_t x, uint8_t y)
{
  return (y%4)*4 + (x%4);
}
static inline uint16_t subpos2mask(uint8_t x, uint8_t y)
{
  return ((uint16_t)1) << subpos2bit(x, y);
}

static const uint16_t leftrow_mask = subpos2mask(0, 0)
                          | subpos2mask(0, 1)
                          | subpos2mask(0, 2)
                          | subpos2mask(0, 3);

// from http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetTable
static const unsigned char BitsSetTable256[256] = 
{
#   define B2(n) n,     n+1,     n+1,     n+2
#   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
    B6(0), B6(1), B6(1), B6(2)
};
static inline uint8_t count_bits(uint16_t v)
{
  return BitsSetTable256[v & 0xff] + BitsSetTable256[v >> 8];
}

class World {
  public:

  uint16_t data[size/4*size/4];
  //uint16_t pos2mask[size/4*size/4];

  World() {
    bzero(data, sizeof(data));
    int radius = 700;
    int radius2 = radius*radius;
    for (int y=0; y<size; y++) {
      for (int x=0; x<size; x++) {
        int rr = (x-(size/2))*(x-(size/2)) + (y-(size/2))*(y-(size/2));
        if (rr < radius2) {
          uint16_t * p = data + y/4 * (size/4) + x/4;
          *p |= subpos2mask(x, y);
        }
      }
    }
  }


  void move() {
    for (int y=0; y<size/4; y++) {
      uint16_t last = 0;
      uint16_t * p = data + y * (size/4);
      for (int x=0; x<size/4; x++) {
        uint16_t cur = p[x];

        // rotate left by one bit (gcc produces ROL)
        uint16_t high_bit = (cur & (1<<15)) >> 15;
        cur = cur << 1;
        cur |= high_bit;

        uint16_t next_last = cur & leftrow_mask;
        cur = cur & ~leftrow_mask;
        cur = cur | last;
        p[x] = cur;
        last = next_last;
      }
    }
  }

  PyObject * render(PyObject * arr) {

    Py_buffer buf;
    int res = PyObject_GetBuffer(arr, &buf, PyBUF_WRITABLE|PyBUF_STRIDES);
    if (res != 0) return NULL;
    assert(buf.ndim == 2);     // 2 dimensions (y, x)
    assert(buf.strides[0] == 4); // 4 bytes stride (unused alpha)
    /*
    int width  = buf.shape[0];
    int height = buf.shape[1];
    */
    
    for (int y=0; y<size/4; y++) {
      uint16_t * src = data + y*(size/4);
      uint8_t  * dst = (uint8_t*)buf.buf + y*buf.strides[1];
      for (int x=0; x<size/4; x++) {
        /*
        if (*src) {
          dst[0] = 200;
          dst[1] = 200;
          dst[2] = 200;
        } else {
          dst[0] = 50;
          dst[1] = 50;
          dst[2] = 50;
        }
        */
        dst[0] = dst[1] = dst[2] = 
          //  count_bits(*src) * 15;
          linear_light_17_steps_to_sRGB[count_bits(*src)];
        dst += 4;
        src += 1;
      }
    }
    Py_RETURN_NONE;
  }
};
