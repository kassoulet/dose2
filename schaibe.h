typedef struct _Vector {
  float x, y, z; 
} Vector;
typedef struct _Matrix {
  Vector i, j, k, l;
} Matrix ;
extern Vector v_c0, v_ci, v_cj, v_ck;
#define pi 3.14159265358979323846264338
void *getmem(int bytes);
void mark();
void release();

#define fist2(x) ((int)((x)+100000.5)-100000)

Vector *v_sub(Vector *d, Vector *a, Vector *b);
Vector *v_norm(Vector *d);
Vector *v_cross(Vector *d, Vector *a, Vector *b);
Vector *v_mul(Vector *d, Vector *a, float k);
Vector *v_create(Vector *d, float x, float y, float z);
Vector *new_v();

int add_mask(unsigned int *a, unsigned int b);

//int ne_mask(int, int);
//int b_mask(int, int);
#define b_mask(a, b) ((unsigned)(a)<(unsigned)(b)?-1:0)
#define ne_mask(a, b) ((unsigned)(a)!=(unsigned)(b)?-1:0)

#define a_mask(a, b) (b_mask((b), (a)))
#define l_mask(a, b) (b_mask((a)+2147483648U, (b)+2147483648U))
#define g_mask(a, b) (a_mask((a)+2147483648U, (b)+2147483648U))
#define e_mask(a, b) (~ne_mask((a), (b)))
#define nb_mask(a, b) (~b_mask((a), (b)))
#define na_mask(a, b) (~a_mask((a), (b)))
#define nl_mask(a, b) (~l_mask((a), (b)))
#define ng_mask(a, b) (~g_mask((a), (b)))
#define btw_mask(a, b, c) (b_mask((a)-(b), (c)-(b)))
#define nbtw_mask(a, b, c) (a_mask((a)-(b), (c)-(b)))
