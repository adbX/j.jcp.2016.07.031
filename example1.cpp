#include <iostream>
#include "FAI_2DFSDEsolver.h"
#include"gamma.h"
#include <time.h>
#include <iomanip>
#include <cmath>
const uint_32 level=2;
const uint_32 N=8192;
const double X_L=0, X_R=M_PI, Y_Low=0, Y_Upp=M_PI, T=0.5, alpha=0.01;
const double eps=0.5e-8;
const double tol=1.0e-8;
double inf_norm_real(double* vec, uint_32 veclen);
int main() {
	uint_32 M, i, j, Ms;
	clock_t t1, t2;
	double s;
	t1=clock();
	M=_Pow_int(2, level)-1;
	Ms=M*M;
	double tau=T/(double)N, miu, oneovermiu;
	miu=pow(tau, alpha)*Gamma(2-alpha);
	double h1, h2;
	h1=(X_R-X_L)/(double)(M+1);
	h2=(Y_Upp-Y_Low)/(double)(M+1);
	oneovermiu=1.0/miu;
	double onema=1-alpha, apkp1, apk;
	double* ak=new double[N];
	apk=0;
	apkp1=1;

	for(i=0; i<N; i++) {
		ak[i]=apkp1-apk;
		apk=apkp1;
		apkp1=pow(i+2, onema);
	}

	double* Frac_div_appro_coeff=new double[N];
	Frac_div_appro_coeff[0]=oneovermiu;

	for (i=1; i<N; i++) {
		Frac_div_appro_coeff[i]=oneovermiu*(ak[i]-ak[i-1]);
	}

	delete[]ak;
	complex** rhs=new complex*[N];
	double gammafac=1./Gamma(3-alpha);
	double t;
	double* exactsol=new double[Ms];
	double mh1, mh2, tmalpha=2-alpha, oneoverh1s=1/(h1*h1), oneoverh2s=1/(h2*h2),
	                 ts, tp2ma;
	double x_s=X_L+0.5*h1, x_e=X_L+(M+0.5)*h1, y_s=Y_Low+0.5*h2,
	       y_e=Y_Low+(M+0.5)*h2;
	uint_32 Mm1=M-1, Mmmm1=Mm1*M;

	uint_32 indy, n;

	for (j=0; j<M; j++) {
		indy=j*M;
		double y = Y_Low+(j+1)*h2;

		for (i=0; i<M; i++) {
			double x = X_L+(i+1)*h1;
			exactsol[indy+i]=sin(x)*sin(y);
		}
	}

	for (n=0; n<N; n++) {
		t=(n+1)*tau;
		ts=t*t;
		tp2ma=pow(t, tmalpha);
		rhs[n]=new complex[Ms];

		for (j=0; j<M; j++) {
			mh2=Y_Low+(j+1)*h2;
			indy=j*M;

			for (i=0; i<M; i++) {
				mh1=X_L+(i+1)*h1;
				rhs[n][indy+i].r=2*exactsol[indy+i]*(tp2ma*gammafac+ts);
				rhs[n][indy+i].i=0;
			}

			rhs[n][indy].r+=(oneoverh1s*postv_func(x_s, mh2)*ts*sin(X_L)*sin(mh2));
			rhs[n][indy+Mm1].r+=(oneoverh1s*postv_func(x_e, mh2)*ts*sin(X_R)*sin(mh2));
		}

		for (i=0; i<M; i++) {
			mh1=X_L+(i+1)*h1;
			rhs[n][i].r+=(oneoverh2s*postv_func(mh1, y_s)*ts*sin(mh1)*sin(Y_Low));
			rhs[n][Mmmm1+i].r+=(oneoverh2s*postv_func(mh1, y_e)*ts*sin(mh1)*sin(Y_Upp));
		}
	}

	double iter_num_arr[1];
	Time_frac_diffusion_2Deq_solver(N, level, X_L, X_R, Y_Low, Y_Upp,
	                                Frac_div_appro_coeff, rhs, eps, iter_num_arr, tol);
	t2=clock();
	delete[]Frac_div_appro_coeff;
	double norm1, error, *tempsolver=new double[Ms], max=0, max1;
	norm1=inf_norm_real(exactsol, Ms);
	norm1*=(T*T);

	for (n=0; n<N; n++) {
		t=(n+1)*tau;
		ts=t*t;

		for (i=0; i<Ms; i++) {
			tempsolver[i]=rhs[n][i].r-ts*exactsol[i];
		}

		max1=inf_norm_real(tempsolver, Ms);

		if(max<max1) {
			max=max1;
		}
	}

	std::cout<<"averageiter: "<<iter_num_arr[0]<<std::endl;
	error=max/norm1;
	std::cout.setf(std::ios::scientific);
	std::cout<<"the relative error under infinite norm is: "<<error<<std::endl;
	std::cout.unsetf(std::ios::scientific);
	std::cout.setf(std::ios::fixed);
	s=(double)(t2-t1)/CLOCKS_PER_SEC;
	std::cout<<std::setprecision(7)<<"the running time is : "<<s<<std::endl;
	/*
	 for (i=0;i<Ms;i++)
	 {
		 std::cout<<exactsol[i]*pow(T,onepalpha)-rhs[N-1][i].r<<std::endl;
	 }
	*/
	delete[]exactsol;
	delete[]tempsolver;

	for (i=0; i<N; i++) {
		delete[]rhs[i];
	}
}
double inf_norm_real(double* vec, uint_32 veclen) {
	double max=fabs(vec[0]);

	for (uint_32 i=1; i<veclen; i++) {
		if (max<fabs(vec[i])) {
			max=fabs(vec[i]);
		}
	}

	return max;
}
inline double postv_func(double x, double y) {
	return 1.;
}