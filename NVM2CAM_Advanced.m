%% Convert .nvm file and _P.txt file into .cam files for texturing
%% !!!! Warning: Use this after CMPMVS !!!
%% ================= Dataset dependent parameters ==================
nvm_directory = 'C:/Users/wang.fan/Desktop/Mac_VSFM/cmp_comp.nvm';
cmpmvs_output = 'C:/Users/wang.fan/Desktop/Mac_VSFM/cmp_comp.nvm.cmp/00/data';
%% =================================================================
%% ======================== Read .nvm file =========================
nvm_file = fopen(nvm_directory, 'r');
dummy    = textscan(nvm_file, '%s', 2, 'Delimiter', '\n');
cam_num  = textscan(nvm_file, '%d', 1, 'Delimiter', '\n');
cam_info = textscan(nvm_file, '%s %f %f %f %f %f %f %f %f %f %f', cam_num{1}(1));
%% ======================== Read _P.txt file =======================
mkdir([cmpmvs_output '/scene']);
p_list   = dir([cmpmvs_output '/*.txt']);
img_list = dir([cmpmvs_output '/*.jpg']);
p_name   = {p_list.name};
img_name = {img_list.name};
p_num    = length(p_name);
img_num  = length(img_name);
if p_num ~= img_num disp('Did you put something else in this folder?'); end
if p_num ~= cam_num{1}(1) disp('Did you do something funny ?'); end
for c = 1 : 1 : p_num
    cur_img = imread([cmpmvs_output '/' img_name{c}]);
    height = size(cur_img,1);
    width = size(cur_img,2);
    cur_K = [cam_info{2}(c) 0 width/2; 0 cam_info{2}(c) height/2; 0 0 1];
    output_direc = [cmpmvs_output '/scene'];
    if(c - 1 < 10) output_direc = [output_direc '/view_000' int2str(c-1) '.jpg']; end
    if(c - 1 >= 10 && c - 1 < 100) output_direc = [output_direc '/view_00' int2str(c-1) '.jpg']; end
    if(c - 1 >= 100 && c - 1 < 1000) output_direc = [output_direc '/view_0' int2str(c-1) '.jpg']; end
    if(c - 1 >= 1000 && c - 1 < 10000) output_direc = [output_direc '/view_' int2str(c-1) '.jpg']; end
    imwrite(cur_img, output_direc);
    P_fileID = fopen([cmpmvs_output '/' p_name{c}], 'r');
    proj_cell = textscan(P_fileID, '%f %f %f %f');
    projection = [proj_cell{1}(1) proj_cell{2}(1) proj_cell{3}(1) proj_cell{4}(1);
    proj_cell{1}(2) proj_cell{2}(2) proj_cell{3}(2) proj_cell{4}(2);
    proj_cell{1}(3) proj_cell{2}(3) proj_cell{3}(3) proj_cell{4}(3)];
    P_rectified = inv(cur_K) * projection;
    max_dimension = max(height, width);
    cam_output_direc = [cmpmvs_output '/scene'];
    if(c - 1 < 10) cam_output_direc = [cam_output_direc '/view_000' int2str(c-1) '.CAM']; end
    if(c - 1 >= 10 && c - 1 < 100) cam_output_direc = [cam_output_direc '/view_00' int2str(c-1) '.CAM']; end
    if(c - 1 >= 100 && c - 1 < 1000) cam_output_direc = [cam_output_direc '/view_0' int2str(c-1) '.CAM']; end
    if(c - 1 >= 1000 && c - 1 < 10000) cam_output_direc = [cam_output_direc '/view_' int2str(c-1) '.CAM']; end
    cam_fileID = fopen(cam_output_direc, 'w');
    for outer=1:3
        fprintf(cam_fileID, '%f ', P_rectified(outer, 4));
    end
    for outer=1:3
        for inner=1:3
            fprintf(cam_fileID, '%f ', P_rectified(outer,inner));
        end
    end
    fprintf(cam_fileID, '\n');
    fprintf(cam_fileID, '%f 0 0 1 0.5 0.5', cam_info{2}(c)/max_dimension);
    fclose(cam_fileID);
    fclose(P_fileID);
end
fclose(nvm_file);





































