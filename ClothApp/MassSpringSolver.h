#pragma once
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>

struct mass_spring_system { 
	typedef Eigen::SparseMatrix<float> SparseMatrix;
	typedef Eigen::VectorXf VectorXf;
	typedef std::vector<std::pair<unsigned int, unsigned int> > EdgeList;
	
	// parameters
	unsigned int n_points; // number of points
	unsigned int n_springs; // number of springs
	float time_step; // time step
	EdgeList spring_list; // spring edge list
	VectorXf rest_lengths; // spring rest lengths
	VectorXf fext; // external forces
	float damping_factor; // damping factor
	

	// system matrices
	SparseMatrix M;
	SparseMatrix L;
	SparseMatrix J;
	
	mass_spring_system(
		unsigned int n_points,       // number of points
		unsigned int n_springs,      // number of springs
		float time_step,             // time step
		EdgeList spring_list,        // spring edge list
		VectorXf rest_lengths,       // spring rest lengths
		VectorXf stiffnesses,        // spring stiffnesses
		VectorXf M,                  // node masses
		VectorXf fext,               // external forces
		float damping_factor         // damping factor
	);
};

class MassSpringSolver {
private:
	typedef Eigen::Vector3f Vector3f;
	typedef Eigen::VectorXf VectorXf;
	typedef Eigen::SparseMatrix<float> SparseMatrix;
	typedef Eigen::SimplicialCholesky<Eigen::SparseMatrix<float> > Cholesky;
	typedef Eigen::Map<Eigen::VectorXf> Map;

	// system
	mass_spring_system* system;
	Cholesky system_matrix;

	// state
	Map current_state;
	VectorXf prev_state;
	VectorXf spring_directions;
	VectorXf inertial_term;

	// steps
	void globalStep();
	void localStep();

public:
	MassSpringSolver(mass_spring_system* system, float* vbuff);

	// solve iterations
	void solve(unsigned int n);
	void timedSolve(unsigned int ms);
};


class MassSpringBuilder {
public:
	static mass_spring_system* UniformGrid(
		unsigned int n,          // grid size
		float time_step,         // time step
		float rest_length,       // spring rest length (non-diagonal)
		float stiffness,         // spring stiffness
		float mass,              // node mass
		float damping_factor,    // damping factor
		float gravity,           // gravitationl force (-z axis)
		float* points            // vertex buffer
	);
};
