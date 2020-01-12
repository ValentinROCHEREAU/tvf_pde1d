#include <iostream>
#include "closed_form.hpp"
#include "model.hpp"
#include "solver.hpp"
#include <algorithm>


int main(int argc, char* argv[])
{	
	// Exemple d'utilisation de la class payoff

	//dauphine::payoff pp = dauphine::payoff("s", { 95 , 105 }, [](double d2) {return d2 * 2; }); //if the client wants to input his own function
	payoff pp = payoff("strangle", { 95 , 105 }); //if we want to use the function already input in the class
	std::cout << pp.getparameters()[0] << std::endl; // get the parameters
	std::cout << pp.getname() << std::endl; // get the name
	std::cout << pp.getpayoff()(90) << std::endl; //get the payoff function and evaluate it at 90
	
	
	model model_pde(10, 10, 0.2, 0.05, 1./10., pp);
	
	std::vector<double> r(10);
	std::vector<std::vector<double>> sigma(10, std::vector<double>(10));
	
	for(int i=0; i<10; ++i)
	{	
		for (int j=0; j<10; ++j)
		{
			sigma[i][j] = i*1./100. + 0.2;
		}
		r[i]=i*3./100.;
	}
	
	model model_pde_r(10, 10, 0.2, r, 1./10., pp);
	model model_pde_sigmar(10, 10, sigma, r, 1./10., pp);
	model model_pde_sigma(10, 10, sigma, 0.05, 1./10., pp);
	
	solver_edp solver_model(model_pde);
	
	std::vector<double> sol(solver_model.solve_pde());
	
	std::vector<std::vector<double>> mat(3, std::vector<double>(3));
	std::vector<double> d(3);
	
	for(int i=0; i<3; i++)
	{
		d[i] = 3-i;
	}
	
	mat[0][0] = 1;
	mat[1][1] = 2;
	mat[2][2] = 3;
	
	mat[0][1] = -1;
	mat[1][2] = -2;
	
	mat[1][0] = 1;
	mat[2][1] = 2;
	
	std::vector<double> test_inv(solver_model.product_inverse(mat, d));
	
	for(int i=0; i<test_inv.size();i++)
	{
		std::cout << test_inv[i] << ",";
	}
	
	return 0;
}
