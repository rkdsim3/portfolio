%% left Image data input

[file,path] = uigetfile('*.txt','Select image data file');

if isequal(file,0) % 대화상자에서 x 또는 취소 선택시 예외처리
    disp('User selected Cancel');
    return
else 
    File_ID = fopen(fullfile(path,file),'r'); % 불러온 파일을 읽기권한으로 열기
    IMG_data = fscanf(File_ID, '%g %g %g %g %g %g %g %g %g',[9 inf]); % 파일 데이터 읽기
end    

[Num_of_imagedata, idim] = size(IMG_data);
xl = IMG_data(1,1)
yl = IMG_data(2,1)
%% right Image data input

[file,path] = uigetfile('*.txt','Select image data file');

if isequal(file,0) % 대화상자에서 x 또는 취소 선택시 예외처리
    disp('User selected Cancel');
    return
else 
    File_ID = fopen(fullfile(path,file),'r'); % 불러온 파일을 읽기권한으로 열기
    IMG_data = fscanf(File_ID, '%g %g %g %g %g %g %g %g %g',[9 inf]); % 파일 데이터 읽기
end    

[Num_of_imagedata, idim] = size(IMG_data);
xr = IMG_data(1,1)
yr = IMG_data(2,1)
%% left EOP input

[file,path] = uigetfile('*.txt','Select EOP data file');

if isequal(file,0) % 대화상자에서 x 또는 취소 선택시 예외처리
    disp('User selected Cancel');
    return
else 
    File_ID = fopen(fullfile(path,file),'r'); % 불러온 파일을 읽기권한으로 열기
    left_EOP = fscanf(File_ID, '%g %g %g %g %g %g',[6 inf]); % 파일 데이터 읽기
    left_EOP = left_EOP'; 
end 

X0l=left_EOP(1,1) ;
Y0l=left_EOP(1,2) ;
Z0l=left_EOP(1,3) ;
ol=left_EOP(1,4) ;
pl=left_EOP(1,5) ;
kl=left_EOP(1,6) ;
%% rightEOP input

[file,path] = uigetfile('*.txt','Select EOP data file');

if isequal(file,0) % 대화상자에서 x 또는 취소 선택시 예외처리
    disp('User selected Cancel');
    return
else 
    File_ID = fopen(fullfile(path,file),'r'); % 불러온 파일을 읽기권한으로 열기
    right_EOP = fscanf(File_ID, '%g %g %g %g %g %g',[6 inf]); % 파일 데이터 읽기
    right_EOP = right_EOP'; 
end  
X0r=right_EOP(1,1) ;
Y0r=right_EOP(1,2) ;
Z0r=right_EOP(1,3) ;
or=right_EOP(1,4) ;
pr=right_EOP(1,5) ;
kr=right_EOP(1,6) ;
%% IOP Input 

[file,path] = uigetfile('*.txt','load IOP data file');
if (file~=0)
    file_name = sprintf('%s%s',path,file)
    fileID = fopen(file_name,'r');
    IOP_data = fscanf(fileID, '%g %g %g %g %g',[4,inf]); 
    IOP_data = IOP_data';
else
    disp('Plz input your IOP data!!'); 
    return
end
xp = IOP_data(1,1); 
yp = IOP_data(1,2);
c = IOP_data(1,3);
%% Object point
Rl=[cos(pl)*cos(kl) -cos(pl)*sin(kl) sin(pl);...
   cos(ol)*sin(kl)+sin(ol)*sin(pl)*cos(kl) cos(ol)*cos(kl)-sin(ol)*sin(pl)*sin(kl) -sin(ol)*cos(pl);...
   sin(ol)*sin(kl)-cos(ol)*sin(pl)*cos(kl) sin(ol)*cos(kl)+cos(ol)*sin(pl)*sin(kl) cos(ol)*cos(pl)];
Rr=[cos(pr)*cos(kr) -cos(pr)*sin(kr) sin(pr);...
   cos(or)*sin(kr)+sin(or)*sin(pr)*cos(kr) cos(or)*cos(kr)-sin(or)*sin(pr)*sin(kr) -sin(or)*cos(pr);...
   sin(or)*sin(kr)-cos(or)*sin(pr)*cos(kr) sin(or)*cos(kr)+cos(or)*sin(pr)*sin(kr) cos(or)*cos(pr)];

syms lambda mu
T=[X0r-X0l; Y0r-Y0l; Z0r-Z0l];
R_1=Rl*[xl-xp;yl-yp;-c];
R_2=Rr*[xr-xp;yr-yp;-c];
K(1,1)=R_1(1,1);
K(2,1)=R_1(2,1);
K(3,1)=R_1(3,1);
K(1,2)=R_2(1,1);
K(2,2)=R_2(2,1);
K(3,2)=R_2(3,1);
Result=linsolve(K,T);
lambda=Result(1,1);
OP = [X0l; Y0l; Z0l]+lambda*Rl*[xl-xp;yl-yp;-c]
X = OP(1,1);
Y = OP(2,1);
Z = OP(3,1);

%% Display Results 

disp('X 좌표')
disp(X)
disp('Y 좌표')
disp(Y)
disp('Z 좌표')
disp(Z)

