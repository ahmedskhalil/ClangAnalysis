// Sample file to semantic analyse


double i = 4;
double i = 27;
double r = 2.8, x = 33;

typedef double (*fp_t)(double, double);
typedef double func_t(double, int);
int (*pfunc)(double, double);

__typeof(pfunc) pfunc11;
fp_t pfunc22;
func_t ft;
__typeof(ft) f;

int main()
{
	int i=4; int c=i;
    struct s {} x;
    struct xx {};
	return 0;
}
