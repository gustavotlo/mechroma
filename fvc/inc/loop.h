#define QP 28
// ------ F U N C T I O N S -----------------------------

short int **partition(short int **frame,int h,int w);
short int **transform(short int ** block);
short int **quantize(short int **block);
short int **inverse_transform(short int ** block);
short int **inverse_quantize(short int **block);
short int **post_scale(short int **block);
short int **loop(short int **block);
short int **malloc_matrix(int w, int h);
float **malloc_matrix_float(int w, int h);
