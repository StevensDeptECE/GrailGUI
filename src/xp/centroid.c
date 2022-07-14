/*
	Written by Joseph O'Rourke
	orourke@cs.smith.edu
	October 27, 1995

	Computes the centroid (center of gravity) of an arbitrary
	simple polygon via a weighted sum of signed triangle areas,
	weighted by the centroid of each triangle.
	Reads x,y coordinates from stdin.  
	NB: Assumes points are entered in ccw order!  
	E.g., input for square:
		0	0
		10	0
		10	10
		0	10
	This solves Exercise 12, p.47, of my text,
	Computational Geometry in C.  See the book for an explanation
	of why this works. Follow links from
		http://cs.smith.edu/~orourke/

*/
#include	<stdio.h>

#define DIM     2               /* Dimension of points */
typedef int     tPointi[DIM];   /* type integer point */
typedef double  tPointd[DIM];   /* type double point */

#define PMAX    1000            /* Max # of pts in polygon */
typedef tPointi tPolygoni[PMAX];/* type integer polygon */

int     Area2( tPointi a, tPointi b, tPointi c );
void    FindCG( int n, tPolygoni P, tPointd CG );
int	ReadPoints( tPolygoni P );
void    Centroid3( tPointi p1, tPointi p2, tPointi p3, tPointi c );
void	PrintPoint( tPointd p );

int main()
{
	int	n;
	tPolygoni	P;
	tPointd CG;

	n = ReadPoints( P );
	FindCG( n, P ,CG);
	printf("The cg is ");
	PrintPoint( CG );
}

/* 
        Returns twice the signed area of the triangle determined by a,b,c,
        positive if a,b,c are oriented ccw, and negative if cw.
*/
int     Area2( tPointi a, tPointi b, tPointi c )
{
	return
		(b[0] - a[0]) * (c[1] - a[1]) -
		(c[0] - a[0]) * (b[1] - a[1]);
}

/*      
        Returns the cg in CG.  Computes the weighted sum of
	each triangle's area times its centroid.  Twice area
	and three times centroid is used to avoid division
	until the last moment.
*/
void     FindCG( int n, tPolygoni P, tPointd CG)
{
        int     i;
        double  A2, Areasum2 = 0;        /* Partial area sum */    
	tPointi Cent3;

	CG[0] = 0;
	CG[1] = 0;
        for (i = 1; i < n-1; i++) {
	        Centroid3( P[0], P[i], P[i+1], Cent3 );
	        A2 =  Area2( P[0], P[i], P[i+1]);
		CG[0] += A2 * Cent3[0];
		CG[1] += A2 * Cent3[1];
		Areasum2 += A2;
	      }
        CG[0] /= 3 * Areasum2;
        CG[1] /= 3 * Areasum2;
	return;
}
/*
	Returns three times the centroid.  The factor of 3 is
	left in to permit division to be avoided until later.
*/
void    Centroid3( tPointi p1, tPointi p2, tPointi p3, tPointi c )
{
        c[0] = p1[0] + p2[0] + p3[0];
        c[1] = p1[1] + p2[1] + p3[1];
	return;
}

void	PrintPoint( tPointd p )
{
        int i;

        putchar('(');
        for ( i=0; i<DIM; i++) {
		printf("%f",p[i]);
		if (i != DIM - 1) putchar(',');
        }
        putchar(')');
	putchar('\n');
}

/*
	Reads in the coordinates of the vertices of a polygon from stdin,
	puts them into P, and returns n, the number of vertices.
	The input is assumed to be pairs of whitespace-separated coordinates,
	one pair per line.  The number of points is not part of the input.
*/
int  ReadPoints( tPolygoni P )
{
    int	n = 0;
    
    printf("Polygon:\n");
    printf("  i   x   y\n");      
    while ( (n < PMAX) && 
	    (scanf("%d %d",&P[n][0],&P[n][1]) != EOF) ) {
	printf("%3d%4d%4d\n", n, P[n][0], P[n][1]);
	++n;
    }
    if (n < PMAX)
	printf("n = %3d vertices read\n",n);
    else	printf("Error in ReadPoints:\too many points; max is %d\n", 
		       PMAX);
    putchar('\n');
    
    return	n;
}
