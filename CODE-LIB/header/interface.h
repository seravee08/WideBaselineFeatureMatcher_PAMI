#pragma once
#ifndef INTERFACE_H
#define INTERFACE_H

#include "demo_lib_sift.h"
#include "Eigen/Dense"

class CODE_Interface {
private:
	int start_index;
	int iter_mst, neighbor_mst;
	int matchCount[2];
	float timeRecord[16];
	Img_Store img_control;
	mtchParam match_settings;
	matching_Eigen matchings_control;
	vector<Eigen::RowVectorXi> ind_chosen_control;

	bool No_Affine;			// Normal sift feature rather than affine-sift, default: false
	bool DISP;				// Display the matches, default: false
	bool REPORT;			// Give full timing report, default: true
	bool USE_MST;			// Use minimum spanning tree to accelerate the process, default: false
	bool Normal_Matchings;	// Output normal matchings (x,y), default: false
	bool use_gist;			// Use gist as pre-filter in twoSide_Comparison mode, default: false
	bool sift_ready;		// Skip the sift detection, default: false
	bool gist_ready;		// Skip the gist detection, default: false
public:
	CODE_Interface();
	void twoSide_Comparison_CODE(char*, char*);
	int matchAblat_CODE(char*, char*);
	int blat_CODE(char*, char*);
	void match_CODE(char*, char*);
	void sift_CODE(char*);
	void pwis_CODE(char*);

	void set_MST_CODE(bool, int=2, int=2);
	void set_startIndex_CODE(int);
	void set_noAffine_CODE(bool);
	void set_disp_CODE(bool);
	void set_report_CODE(bool);
	void set_normalMatchOutput_CODE(bool);
	void set_useGIST_CODE(bool);
	void set_siftReady_CODE(bool);
	void set_gistReady_CODE(bool);
	void set_MST_iteration_CODE(int);
	void set_MST_neighbor_CODE(int);

public:
	void releaseGPU_CODE();
	void splitFilename_CODE(const string&, string&, string&);
	void generate_MST_CODE(vector<vector<int>>&);
	void init_image_store_CODE(vector<string>);
	void set_vsfm_matches_limit_CODE(int);
	void init_matching_Eigen_Query_BINARY_CODE(string&, string&);
	void insert_candidate_BINARY_CODE(string&, string&);
	void compute_matchings_CODE();
	void compute_affine_CODE();
	void compute_bilateral_function_CODE();
	void clear_database_CODE();
	void parseInput_CODE(char*, vector<string>&);
	void detect_asfit_features_CODE(vector<string>&, vector<string>&, vector<string>&);
	void extract_SIFTAUX_names_CODE(vector<string>&, vector<string>&, vector<string>&);
	void MST_guided_matching_CODE(vector<string>&, vector<string>&, string, vector<vector<int>>&);

public:
	void return_path_query_CODE(string&);
	void return_path_database_CODE(vector<string>&);
	void return_matches_CODE(vector<Eigen::Matrix<int, 2, Eigen::Dynamic>>&);
	void return_keypoint_query_CODE(Eigen::Matrix<float, 6, Eigen::Dynamic>&);
	void return_keypoint_database_CODE(vector<Eigen::Matrix<float, 6, Eigen::Dynamic>>&);
	void return_desc_query_CODE(Eigen::Matrix<float, 128, Eigen::Dynamic>&);
	void return_desc_database_CODE(vector<Eigen::Matrix<float, 128, Eigen::Dynamic>>&);
	void return_filtered_index_CODE(vector<Eigen::RowVectorXi>&);
};

#endif