/*���������������迹����*/

double fxy(double xi,double yi)               /*���庯��fxy*/
{
	double f;  
	f=yi;
	return(f); 
}

double gxy(double xi,double yi,double xr)				 /*���庯��gxy*/
{
	double b=10,k=1,m=1;
	double g;
	g=(-b*yi-k*xi+k*xr)/m;
	return(g);
}

double gun(double xi,double yi,double xr,double fe) /*���庯��gun*/
{
	double b=470,k=0.01,m=0.85,fd=-15;
	double g;
	g=(-b*yi-k*xi+k*xr+fd-fe)/m;
	return(g);
}

typedef struct Result{
	double xi;
	double yi;
}Result;

typedef struct res{
	double xi;
	double yi;
	double xri;
}res;

//δ�Ӵ�֮ǰ
Result hk(double h,double xi,double yi,double xr){
	double f1,g1,f2,g2,f3,g3,f4,g4,xi_1,yi_1;
	f1=fxy(xi,yi);
	g1=gxy(xi,yi,xr);
	f2=fxy(xi+h*f1/2,yi+h*g1/2);
	g2=gxy(xi+h*f1/2,yi+h*g1/2,xr);
	f3=fxy(xi+h*f2/2,yi+h*g2/2);
	g3=gxy(xi+h*f2/2,yi+h*g2/2,xr);
	f4=fxy(xi+h*f3,yi+h*g3);
	g4=gxy(xi+h*f3,yi+h*g3,xr);
	xi_1=xi+h*(f1+2*f2+2*f3+f4)/6;
	yi_1=yi+h*(g1+2*g2+2*g3+g4)/6;
	struct Result ret;
	ret.xi=xi_1;
	ret.yi=yi_1;
	return ret;
}

//�Ӵ�֮��
res iteration(double h,double xi,double yi,double xr,double fe){
	double f1,g1,f2,g2,f3,g3,f4,g4,xi_1,yi_1,xr_1,kd,fd;
	kd=0.5;fd=-15;
	f1=fxy(xi,yi);
	g1=gun(xi,yi,xr,fe);
	f2=fxy(xi+h*f1/2,yi+h*g1/2);
	g2=gun(xi+h*f1/2,yi+h*g1/2,xr,fe);
	f3=fxy(xi+h*f2/2,yi+h*g2/2);
	g3=gun(xi+h*f2/2,yi+h*g2/2,xr,fe);
	f4=fxy(xi+h*f3,yi+h*g3);
	g4=gun(xi+h*f3,yi+h*g3,xr,fe);
	xi_1=xi+h*(f1+2*f2+2*f3+f4)/6;
	yi_1=yi+h*(g1+2*g2+2*g3+g4)/6;
	xr_1=xr+kd*(fd-fe);
	struct res outc;
	outc.xi=xi_1;
	outc.yi=yi_1;
	outc.xri=xr_1;
	return outc;
}