const int size = 2000;
/* 
def _apow(v, p):
    return (v >= 0) and (v**p) or -((-v)**p)

def linearRGB_component_to_sRGB(c):
    if abs(c) <= 0.0031308:
        return 12.92 * c
    else:
        return 1.055 * _apow(c, 1/2.4) - 0.055

for i in range(17*8):
    lin_c = (1.0 - (i / (17*8-1))) * 0.9 + (1.0-0.9)/2
    print("%d," % round(linearRGB_component_to_sRGB(lin_c)*255))

 */
static const uint8_t linear_light_17_steps_to_sRGB[17] = {
/* white on black, full scale 
  0, 71, 99, 120, 137, 152, 165, 177, 188, 198, 207, 216, 225, 233, 240, 248, 255,
*/
249, 243, 236, 229, 221, 213, 205, 197, 188, 178, 167, 156, 143, 129, 112, 92, 63,
};
/*
static const uint8_t linear_light_17times8_steps_to_sRGB[17*8] = {
 249, 249, 248, 247, 246, 245, 245, 244, 243, 242, 241, 241, 240, 239, 238, 237, 237, 236, 235, 234, 233, 232, 232, 231, 230, 229, 228, 227, 226, 225, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 192, 191, 190, 189, 188, 187, 186, 185, 184, 182, 181, 180, 179, 178, 176, 175, 174, 173, 172, 170, 169, 168, 166, 165, 164, 162, 161, 160, 158, 157, 155, 154, 153, 151, 150, 148, 147, 145, 143, 142, 140, 139, 137, 135, 134, 132, 130, 128, 126, 125, 123, 121, 119, 117, 115, 112, 110, 108, 106, 103, 101, 98, 96, 93, 90, 88, 85, 82, 78, 75, 71, 67, 63, 
};
*/

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

const int particle_count = 20;

class World {
  public:

  uint16_t data[size/4*size/4];
  //uint16_t pos2mask[size/4*size/4];
  uint8_t film[size/4*size/4];

  int particle_x[particle_count];
  int particle_y[particle_count];
  int particle_vx[particle_count];
  int particle_vy[particle_count];

  World() {
    bzero(data, sizeof(data));
    bzero(film, sizeof(film));
    dab(size/2.0, size/2.0, 700);
    do_xor();
    for (int i=0; i<particle_count; i++) {
      //particle_x[i] = i*size/particle_count;
      particle_x[i] = size/2;
      particle_y[i] = size/2;
      particle_vx[i] = 0;
      particle_vy[i] = 0;
    }
  }


  void dab(int cx, int cy, int radius) {
    int radius2 = radius*radius;
    for (int y=0; y<size; y++) {
      for (int x=0; x<size; x++) {
        int rr = (x-cx)*(x-cx) + (y-cy)*(y-cy);
        if (rr < radius2) {
          uint16_t * p = data + y/4 * (size/4) + x/4;
          *p |= subpos2mask(x, y);
        }
      }
    }
  }

  void xor_particle(int cx, int cy, int radius) {
    if (cx - radius < 0) return;
    if (cy - radius < 0) return;
    if (cx + radius >= size) return;
    if (cy + radius >= size) return;
    int radius2 = radius*radius;
    for (int y=cy-radius; y<cy+radius+1; y++) {
      for (int x=cx-radius; x<cx+radius+1; x++) {
        int rr = (x-cx)*(x-cx) + (y-cy)*(y-cy);
        if (rr < radius2) {
          uint16_t * p = data + y/4 * (size/4) + x/4;
          *p ^= subpos2mask(x, y);
        }
      }
    }
  }

  void do_xor() {
    for (int i=0; i<particle_count; i++) {
      xor_particle(particle_x[i], particle_y[i], 40);
    }
  }
  void move_particles()
  {
    for (int i=0; i<particle_count; i++) {
      if (random()%10 == 1) particle_vx[i] += (random()%3)-1;
      if (random()%10 == 1) particle_vy[i] += (random()%3)-1;
      particle_x[i] += particle_vx[i];
      particle_y[i] += particle_vy[i];
      if (particle_y[i] > size) particle_y[i] -= size;
      if (particle_x[i] > size) particle_x[i] -= size;
      if (particle_y[i] < 0) particle_y[i] += size;
      if (particle_x[i] < 0) particle_x[i] += size;
    }

  }
  void move() {
    do_xor();
    move_particles();
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
    do_xor();
  }

  void expose()
  {
    for (int y=0; y<size/4; y++) {
      uint16_t * src = data + y*(size/4);
      uint8_t  * dst = film + y*(size/4);
      for (int x=0; x<size/4; x++) {
        *dst += count_bits(*src);
        dst += 1;
        src += 1;
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
      uint8_t * src = film + y*(size/4);
      uint8_t * dst = (uint8_t*)buf.buf + y*buf.strides[1];
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
          linear_light_17_steps_to_sRGB[((*src)+2)/4];
        dst += 4;
        src += 1;
      }
    }
    bzero(film, sizeof(film));
    Py_RETURN_NONE;
  }
};
