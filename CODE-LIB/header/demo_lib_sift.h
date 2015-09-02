// Authors: Unknown. Please, if you are the author of this file, or if you 
// know who are the authors of this file, let us know, so we can give the 
// adequate credits and/or get the adequate authorizations.

// WARNING: 
// This file implements an algorithm possibly linked to the patent
//
// David Lowe  "Method and apparatus for identifying scale invariant 
// features in an image and use of same for locating an object in an 
// image",  U.S. Patent 6,711,293.
//
// This file is made available for the exclusive aim of serving as
// scientific tool to verify of the soundness and
// completeness of the algorithm description. Compilation,
// execution and redistribution of this file may violate exclusive
// patents rights in certain countries.
// The situation being different for every country and changing
// over time, it is your responsibility to determine which patent
// rights restrictions apply to you before you compile, use,
// modify, or redistribute this file. A patent lawyer is qualified
// to make this determination.
// If and only if they don't conflict with any patent terms, you
// can benefit from the following license terms attached to this
// file.
//
// This program is provided for scientific and educational only:
// you can use and/or modify it for these purposes, but you are
// not allowed to redistribute this work or derivative works in
// source or executable form. A license must be obtained from the
// patent right holders for any other use.


#ifndef _CLIBSIFT_H_
#define _CLIBSIFT_H_



///////////// Description
/// For each octave:
///    	- Divide in par.Scales scales
/// 	- Convolve and compute differences of convolved scales
///	- Look for a 3x3 multiscale extrema and contraste enough and with no predominant direction (no 1d edge)

/// For each extrema
///	- Compute orientation histogram in neighborhood.
///	- Generate a keypoint for each mode with this orientation


/// For each keypoint
///	 - Create vector 



///////////// Possible differences with MW
/// Gaussian convolution


#include <stdlib.h>
#include <assert.h>

#include "numerics1.h"
#include "library.h"
#include "filter.h"
#include "domain.h"
#include "splines.h"
#include "flimage.h"
#include "Eigen/Core"
#include "cv.h"
#include "highgui.h"


#include <vector>
// BASIC STRUCTURES:

// Keypoints:
#define OriSize  8
#define IndexSize  4
#define VecLength  IndexSize * IndexSize * OriSize
#define Default_Max_Matches 4096
#define Match_Slice_Limit_GLSL 13000
#define Match_Slice_Limit_CUDA 2000
#define GPU_MATCH 1
#define FLANN_MATCH 2

struct keypoint {
	float x;
	float y;
	float scale;
	float angle;
	float tilt;
	float rot;
	float vec[VecLength];
};

/* Keypoint structure:
 position:	x,y
 scale:		s
 orientation:	angle
 descriptor:	array of gradient orientation histograms in a neighbors */
struct keypoint_char {
	float	x,y,
	scale,
	angle;
	unsigned char	vec[VecLength];
};

/* Keypoint structure:
 position:	x,y
 scale:		s
 orientation:	angle
 descriptor:	array of gradient orientation histograms in a neighbors */
struct keypoint_short {
	float	x,y,
	scale,
	angle;
	unsigned short	vec[VecLength];
};

/* Keypoint structure:
 position:	x,y
 scale:		s
 orientation:	angle
 descriptor:	array of gradient orientation histograms in a neighbors */
struct keypoint_int {
	float	x,y,
	scale,
	angle;
	unsigned int	vec[VecLength];
};

/* List of keypoints: just use the standard class vector: */
typedef std::vector<keypoint> keypointslist;


/* List of keypoints: just use the standard class vector: */
typedef std::vector<keypoint_char> keypointslist_char;
typedef std::vector<keypoint_short> keypointslist_short;
typedef std::vector<keypoint_int> keypointslist_int;



/* Matching: just use the standard class pair: */
#if defined(MATCH_SCORE_OUT)
typedef std::pair<keypoint,keypoint> matching_pair;
class matching : public matching_pair
{
public:
	matching(keypoint kp1, keypoint kp2) : matching_pair(kp1, kp2){};
	matching() : matching_pair(){};

public:
	float ratio;
	float best;
	float second_best;
	int first_index;
	int second_index;
};
#else
typedef std::pair<keypoint,keypoint> matching;  
#endif


/* List of matchings: just use the standard class vector: */
typedef std::vector<matching> matchingslist;

class siftParam
{
public:
	siftParam(bool);
	~siftParam();
	void set_from_file(char*);
public:
	bool     use_cuda;
	char**   sift_param;
	int	     num_param;
	int		 num_tilt;
};

class match_data
{
public:
	Eigen::RowVectorXi ind_weak;
	Eigen::RowVectorXi problem_pts;
	Eigen::Matrix<float, 4, Eigen::Dynamic> affine_all;
};

class mtchParam
{
public:
	mtchParam();
	void set_from_file(char*);
public:
	// Note: dis_disN_threshold doesn't have much effects on the performance
	int minimum_matching_num;
	int minimum_strong_ind_num;
	int minimum_unique_num;
	int num_of_centroids;
	int minimum_required_samples;
	int p_all_truncate_limit;
	int ind_chosen_compensate_limit;
	int ind_chosen_compensate_limit_high;
	int minimum_required_samples_verify;
	int minimum_truncate_verify;
	int scale_adjust_image_width;
	int scale_adjust_image_height;
	float dis_disN_threshold;
	float thres_spatial;
	float likelihood_threshold;
	float global_threshold;
	float min_lambda_one;
	float min_lambda_two;
	float grad_des_threshold;
	float dist_scale;
	float grad_thresh_affine_verify;
	float min_spatial;
	// Threshold parameters for warp
	int warp_minimum_matching_num;
	int warp_minimum_strong_ind_num;
	float warp_dis_disN_threshold;
};

class matching_Eigen
{
public:
	matching_Eigen();
	matching_Eigen(mtchParam&);
	matching_Eigen(float d_d_t, float t_s, float l_t, float g_t, float m_l_o, float m_l_t, float g_d_t, float d_s, float g_t_a_v,
		int m_m_n, int m_s_n, int m_u_n, int n_c, int m_r_s, int m_r_p_a, int i_c_c_l, int i_c_c_l_h, int m_r_s_v, int m_t_v):
	dis_disN_threshold(d_d_t),thres_spatial(t_s), likelihood_threshold(l_t), global_threshold(g_t),
	min_lambda_one(m_l_o), min_lambda_two(m_l_t), grad_des_threshold(g_d_t), dist_scale(d_s), grad_thresh_affine_verify(g_t_a_v),
	minimum_matching_num(m_m_n), minimum_strong_ind_num(m_s_n), minimum_unique_num(m_u_n), num_centroids(n_c),
	minimum_required_samples(m_r_s), p_all_truncate_limit(m_r_p_a), ind_chosen_compensate_limit(i_c_c_l),
	ind_chosen_compensate_limit_high(i_c_c_l_h), minimum_required_samples_verify(m_r_s_v), minimum_truncate_verify(m_t_v)
	{database_size = 0; matching_method = FLANN_MATCH;};
	void init_from_mtchParam(mtchParam&);
	void init_matching_Eigen_Database(int,int*,int*,int*,int*,string*,vector<vector<vector<keypointslist>>>&);
	void init_matching_Eigen_Query(int,int,int,string,vector<vector<keypointslist>>&);
	void init_matching_Eigen_Query_ASCII(string&, string&, string&);
	void init_matching_Eigen_Query_BINARY(string&, string&, bool, float[16]);
	void validate_matching_or_strong_num_and_release(int);
	void insert_candidate(vector<vector<keypointslist>>&,string,int,int,int);
	void insert_candidate_ASCII(string&, string&, string&);
	void insert_candidate_BINARY(string&, string&, bool, float[16]);
	void delete_candidate(int);
	void clear_database();
	void write_matches_BINARY(const char* name, int index);
	void read_matches_BINARY(const char* name, int index);
	void set_matching_method(int method);
	void set_vsfm_matches_limit(int lim);
public:
	// Matching Validate Parameters
	float	dis_disN_threshold;
	float	thres_spatial;
	float	likelihood_threshold;
	float	global_threshold;
	float	min_lambda_one;
	float	min_lambda_two;
	float	grad_des_threshold;
	float	dist_scale;
	float	grad_thresh_affine_verify;
	float   min_spatial;
	int		minimum_matching_num;
	int		minimum_strong_ind_num;
	int		minimum_unique_num;
	int		num_centroids;
	int		minimum_required_samples;
	int		p_all_truncate_limit;
	int		ind_chosen_compensate_limit;
	int		ind_chosen_compensate_limit_high;
	int		minimum_required_samples_verify;
	int		minimum_truncate_verify;
	int		matching_method;
	int		scale_adjust_image_width;
	int		scale_adjust_image_height;
	int		vsfm_matching_number_limit;

	// Query Parameters
	int					asift_num_query;
	int					width_query;
	int					height_query;
	string				path_query;
	vector<vector<int>> structure_query;
	Eigen::Matrix<float, 128, Eigen::Dynamic>	desc_query;
	Eigen::Matrix<float, 6, Eigen::Dynamic>		keypoint_query;

	// Database Parameters
	int							database_size;
	vector<bool>				thres_unique_validation;
	vector<int>					asift_num_database;
	vector<int>					width_query_database;
	vector<int>					height_query_database;
	vector<int>					matching_num_database;
	vector<string>				path_database;
	vector<match_data>			match_data_database;
	vector<vector<vector<int>>> structure_database;
	vector<Eigen::Matrix<float, 128, Eigen::Dynamic>>	desc_database;
	vector<Eigen::Matrix<float, 6, Eigen::Dynamic>>		keypoint_database;
	vector<Eigen::Matrix<int, 2, Eigen::Dynamic>>		matches_all_database;
	vector<Eigen::RowVectorXf>							score_best_database;
	vector<Eigen::RowVectorXf>							score_bestN_database;
};

class warp_unity
{
public:
	void _ini_bsc_param(int _w, int _h, cv::Mat _w_i);
	void _clear();
public:
	int _feat_num;
	int _matching_num;
	int _width;
	int _height;
	cv::Mat _warped_image;
	match_data _match_data;
	Eigen::Matrix<float, 128, Eigen::Dynamic> _descp;
	Eigen::Matrix<float, 6	, Eigen::Dynamic> _keyp;
	Eigen::Matrix<int  , 2	, Eigen::Dynamic> _matches;
	Eigen::RowVectorXf _score_best;
	Eigen::RowVectorXf _score_bestN;
};

class matching_warp
{
public:
	matching_warp();
	matching_warp(mtchParam&);
	matching_warp(float d_d_t, int m_m_n, int m_s_i_n): dis_disN_threshold(d_d_t), minimum_matching_num(m_m_n),
		minimum_strong_ind_num(m_s_i_n){warp_database_size = 0;}
	void init_from_mtchParam(mtchParam&);
	void coordinated_validation(matching_Eigen&, int);
	void resize_warp_database(int s);
	void delete_candidate(int);
public:
	int		minimum_matching_num;
	int		minimum_strong_ind_num;
	int		warp_database_size;
	int		scale_adjust_image_width;
	int		scale_adjust_image_height;
	float	dis_disN_threshold;
	float	min_spatial;
	vector<bool>		warp_thres_unique_validation;
	vector<warp_unity>	warp_entities;
};

class Img_Color {
public:
	cv::Mat img;
	int width;
	int height;
public:
	void set_img(std::string file_name);
	void release_mem();
};

class Img_Store {
public:
	int imageNumber;
	std::vector<Img_Color> img_store;
public:
	void init_store(std::vector<std::string> name_list);
};

struct siftPar
{

int OctaveMax;

int DoubleImSize;

int order;

/* InitSigma gives the amount of smoothing applied to the image at the
   first level of each octave.  In effect, this determines the sampling
   needed in the image domain relative to amount of smoothing.  Good
   values determined experimentally are in the range 1.2 to 1.8.
*/
float  InitSigma /*= 1.6*/;
 

/* Peaks in the DOG function must be at least BorderDist samples away
   from the image border, at whatever sampling is used for that scale.
   Keypoints close to the border (BorderDist < about 15) will have part
   of the descriptor landing outside the image, which is approximated by
   having the closest image pixel replicated.  However, to perform as much
   matching as possible close to the edge, use BorderDist of 4.
*/
int BorderDist /*= 5*/;


/* Scales gives the number of discrete smoothing levels within each octave.
   For example, Scales = 2 implies dividing octave into 2 intervals, so
   smoothing for each scale sample is sqrt(2) more than previous level.
   Value of 2 works well, but higher values find somewhat more keypoints.
*/

int Scales /*= 3*/;


/// Decreasing PeakThresh allows more non contrasted keypoints
/* Magnitude of difference-of-Gaussian value at a keypoint must be above
   this threshold.  This avoids considering points with very low contrast
   that are dominated by noise.  It is divided by Scales because more
   closely spaced scale samples produce smaller DOG values.  A value of
   0.08 considers only the most stable keypoints, but applications may
   wish to use lower values such as 0.02 to find keypoints from low-contast
   regions.
*/

//#define  PeakThreshInit  255*0.04 
//#define  PeakThresh      PeakThreshInit / Scales
float PeakThresh  /*255.0 * 0.04 / 3.0*/;

/// Decreasing EdgeThresh allows more edge points
/* This threshold eliminates responses at edges.  A value of 0.08 means
   that the ratio of the largest to smallest eigenvalues (principle
   curvatures) is below 10.  A value of 0.14 means ratio is less than 5.
   A value of 0.0 does not eliminate any responses.
   Threshold at first octave is different.
*/
float  EdgeThresh  /*0.06*/;
float  EdgeThresh1 /*0.08*/;


/* OriBins gives the number of bins in the histogram (36 gives 10
   degree spacing of bins).
*/
int OriBins  /*36*/;


/* Size of Gaussian used to select orientations as multiple of scale
     of smaller Gaussian in DOG function used to find keypoint.
     Best values: 1.0 for UseHistogramOri = FALSE; 1.5 for TRUE.
*/
float OriSigma  /*1.5*/;


/// Look for local (3-neighborhood) maximum with valuer larger or equal than OriHistThresh * maxval
///  Setting one returns a single peak
/* All local peaks in the orientation histogram are used to generate
   keypoints, as long as the local peak is within OriHistThresh of
   the maximum peak.  A value of 1.0 only selects a single orientation
   at each location.
*/
float OriHistThresh  /*0.8*/;


/// Feature vector is normalized to has euclidean norm 1.
/// This threshold avoid the excessive concentration of information on single peaks
/* Index values are thresholded at this value so that regions with
   high gradients do not need to match precisely in magnitude.
   Best value should be determined experimentally.  Value of 1.0
   has no effect.  Value of 0.2 is significantly better.
*/
float  MaxIndexVal  /*0.2*/;


/* This constant specifies how large a region is covered by each index
   vector bin.  It gives the spacing of index samples in terms of
   pixels at this scale (which is then multiplied by the scale of a
   keypoint).  It should be set experimentally to as small a value as
   possible to keep features local (good values are in range 3 to 5).
*/
int  MagFactor   /*3*/;


/* Width of Gaussian weighting window for index vector values.  It is
   given relative to half-width of index, so value of 1.0 means that
   weight has fallen to about half near corners of index patch.  A
   value of 1.0 works slightly better than large values (which are
   equivalent to not using weighting).  Value of 0.5 is considerably
   worse.
*/
float   IndexSigma  /*1.0*/;

/* If this is TRUE, then treat gradients with opposite signs as being
   the same.  In theory, this could create more illumination invariance,
   but generally harms performance in practice.
*/
int  IgnoreGradSign  /*0*/;



float ratiomax  /*0.6*/;
float distmax;			/*0*/
/*
   In order to constrain the research zone for matches.
   Useful for example when looking only at epipolar lines
*/

float MatchXradius /*= 1000000.0f*/;
float MatchYradius /*= 1000000.0f*/;
float MatchRatio;

int noncorrectlylocalized;

int UseOrsa;	//do filtering	/*1*/
int UseN1;		// do n_1 filtering	/*1*/
int Use1N;		// do 1_nfiltering	/*1*/
float MatchRatio_low;
int lib_verbal;					/*0*/
};

//////////////////////////////////////////////////////////
/// SIFT 
//////////////////////////////////////////////////////////

void readCudaDevice();

void default_sift_parameters(siftPar &par);

void compute_sift_keypoints(float *input,  keypointslist& keypoints,int width, int height, siftPar &par);

// MATCHING DETECTION FUNCTION:
void compute_sift_matches( keypointslist& keys1, keypointslist& keys2, matchingslist& matchings, siftPar &par);

void compute_sift_matches_flann(int num1, int num2, matching_Eigen& matchings_info, int index);

// FOR SIFTGPU
#if defined(USE_SIFTGPU)
typedef std::vector<unsigned char> siftgpu_keypointslist;
void compute_sift_matches_gpu(
	int num1,  int num2, matching_Eigen& matchings_info,
	siftPar &par, int index, bool use_cuda);

void compute_sift_matches_gpu(
	matching_warp& warps, matching_Eigen& matchings_info,
	siftPar& par, int index, bool use_cuda);

double compute_sift_keypoints_gpu(
	unsigned char *input, keypointslist& keypoints,
	int width, int height, siftPar &par);

int GetSiftMatch_HACK_Slice(
	int, int,
	siftgpu_keypointslist&, siftgpu_keypointslist&,
	bool, int**&, float*&, float*&);

int AsiftGen_Cuda(
	const float const*,
	int, int, int,
	vector<vector<keypointslist>>&);

int AsiftGen_Cuda_Filtered(
	const float const*,
	int,
	int,
	int,
	vector<vector<keypointslist>>&
	);
#endif


#endif // _LIBSIFT_H_



