#include <iostream>
#include "interface.h"

using namespace std;
using namespace Eigen;

void example1() {
	// Assign the address to a .txt file containing full paths to the target images
	char* database_src = "C:/deploy-test/image_list.txt";

	// On declaration, the program will automatically look for and load bilateral_settings.txt and
	// siftParam_setting.txt in current folder. If not found, then default parameters are used
	CODE_Interface code_control;
	vector<string> img_names, sift_names, aux_names;
	code_control.parseInput_CODE(database_src, img_names);
	code_control.init_image_store_CODE(img_names);

	// Perform feature detection, the .sift and .aux files are outputted into the same folder
	// with the images. The names of the .sift and .aux files will be returned for future use
	code_control.detect_asfit_features_CODE(img_names, sift_names, aux_names);

	// Determine the image to match to other images (left side image)
	code_control.init_matching_Eigen_Query_BINARY_CODE(sift_names[0], aux_names[0]);

	// In this example, the first image is matched to all the rest images. Need to insert the feature
	// information of these images to the program
	for(int i=1; i<img_names.size(); i++)
		code_control.insert_candidate_BINARY_CODE(sift_names[i], aux_names[i]);

	// Compute the matches and filter them afterwards
	code_control.compute_matchings_CODE();
	code_control.compute_affine_CODE();
	code_control.compute_bilateral_function_CODE();

	// The filtered matches are stored inside code_control
	vector<RowVectorXi> filtered_index;
	vector<Matrix<int, 2, Dynamic>> matches_all;
	vector<Matrix<int, 2, Dynamic>> filtered_matches;
	code_control.return_filtered_index_CODE(filtered_index);
	code_control.return_matches_CODE(matches_all);
	assert(matches_all.size() == filtered_index.size());
	filtered_matches.reserve(matches_all.size());
	for(int i=0; i<filtered_index.size(); i++) {
		Matrix<int, 2, Dynamic> tmp_match;
		tmp_match.resize(2, filtered_index[i].cols());
		for(int j=0; j<filtered_index[i].cols(); j++) {
 			tmp_match(0,j)  = matches_all[i].coeff(0,filtered_index[i][j]);
 			tmp_match(1,j) =  matches_all[i].coeff(1,filtered_index[i][j]);
		}
		filtered_matches.push_back(tmp_match);
	}

	// Get image path, keypoints or 128d descriptors as you like
// 	string query_path;
// 	Matrix<float, 6, Dynamic> query_keypoint;
// 	Matrix<float, 128, Dynamic> desc_keypoint;
// 	code_control.return_path_query_CODE(query_path);
// 	code_control.return_keypoint_query_CODE(query_keypoint);
// 	code_control.return_desc_query_CODE(desc_keypoint);

	// Clear the database and release the GPU
	code_control.clear_database_CODE();
	code_control.releaseGPU_CODE();
}

void example2() {
	char* database_src = "C:/deploy-test/image_list.txt";
	CODE_Interface code_control;

	// Find the path to output matchings
	string desP, nameP, matchingP;
	code_control.splitFilename_CODE(string(database_src), desP, nameP);
	matchingP = desP + "/" + string("matchings.txt");

	vector<string> img_names, sift_names, aux_names;
	code_control.parseInput_CODE(database_src, img_names);
	
	// We assume you've already had sift and aff files ready in this example
	// You just need to extract the proper names for the sift and aff files
	code_control.extract_SIFTAUX_names_CODE(img_names, sift_names, aux_names);
	code_control.init_image_store_CODE(img_names);

	// Generate a minimum spanning tree
	vector<vector<int>> MST;
	code_control.set_MST_iteration_CODE(2);
	code_control.set_MST_neighbor_CODE(2);
	code_control.generate_MST_CODE(MST);

	// Engage the MST guided matching process
	code_control.MST_guided_matching_CODE(sift_names, aux_names, matchingP, MST);

	code_control.clear_database_CODE();
	code_control.releaseGPU_CODE();
}

void example3() {
	// This example demonstrates the functions with full pipeline
	// Note that REPORT option is only available in full pipeline functions
	char* database_src = "C:/deploy-test/image_list.txt";
	char* src_1 = "C:/deploy-test/image_list1.txt";
	char* src_2 = "C:/deploy-test/image_list2.txt";
	char* sift_1 = "C:/deploy-test/view_0000.sift";
	char* sift_2 = "C:/deploy-test/view_0001.sift";
	CODE_Interface code_control;

	// Pair-wise image matching. Suppose you have image1 image2 and image3, the matchings
	// will be performed for (image1,image2), (image1,image3), (image2, image3)
	code_control.pwis_CODE(database_src);

	// Detect affine-sift features for the images
//	code_control.sift_CODE(database_src);

	// Match the images in list1 to images in list2, duplicate images won't be matched
//	code_control.twoSide_Comparison_CODE(src_1, src_2);

	// Perform matching on a pair of images. The sift and aff files must be ready
//	code_control.match_CODE(sift_1, sift_2);

	// Calculate the bilateral functions. This step can only be performed after match_CODE
	// You must have the binary output from match_CODE, sift, and aff files ready
//	code_control.blat_CODE(sift_1, sift_2);

	// Perform matching and bilateral function computation in one step
//	code_control.matchAblat_CODE(sift_1, sift_2);
}

void main() {
	example1();
	// example2();
	// example3();

	system("pause");
}