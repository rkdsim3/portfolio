clear;
clc;
clear all;
%% File Input
% cd('C:\Users\KSK\Desktop\HW_SPR');

disp('--------------------SPR ����--------------------')

[file,path] = uigetfile('*.txt','Select GCP data file'); % GCP ���ϼ��� ��ȭ���� Ȱ��ȭ

if isequal(file,0) % ��ȭ���ڿ��� x �Ǵ� ��� ���ý� ����ó��
    disp('User selected Cancel');
    return
else 
    File_ID = fopen(fullfile(path,file),'r'); % �ҷ��� ������ �б�������� ����
    GCP_data = fscanf(File_ID, '%g %g %g %g %g %g %g %g %g',[9 inf]); % ���� ������ �б�
    disp('GCP data loaded successfully!'); % ������ �б� ���� �̺�Ʈ�޽��� ���
    GCP_data = GCP_data';
end    

[Num_of_GCP, gdim] = size(GCP_data); 

%% Image data input

[file,path] = uigetfile('*.txt','Select image data file');

if isequal(file,0) % ��ȭ���ڿ��� x �Ǵ� ��� ���ý� ����ó��
    disp('User selected Cancel');
    return
else 
    File_ID = fopen(fullfile(path,file),'r'); % �ҷ��� ������ �б�������� ����
    IMG_data = fscanf(File_ID, '%g %g %g %g %g %g %g %g %g',[9 inf]); % ���� ������ �б�
    disp('IMG data loaded successfully!'); % ������ �б� ���� �̺�Ʈ�޽��� ���
end    

[Num_of_imagedata, idim] = size(IMG_data); 

%% IOP Input 

[file,path] = uigetfile('*.txt','Select IOP data file');

if isequal(file,0) % ��ȭ���ڿ��� x �Ǵ� ��� ���ý� ����ó��
    disp('User selected Cancel');
    return
else 
    File_ID = fopen(fullfile(path,file),'r'); % �ҷ��� ������ �б�������� ����
    IOP_data = fscanf(File_ID, '%g %g %g',[3 inf]); % ���� ������ �б�
    disp('IOP data loaded successfully!'); % ������ �б� ���� �̺�Ʈ�޽��� ���
end  

IOP_xp = IOP_data(1,1);
IOP_yp = IOP_data(2,1);
IOP_f = IOP_data(3,1);  % focal length

%% initial_EOP input

[file,path] = uigetfile('*.txt','Select initial_EOP data file');

if isequal(file,0) % ��ȭ���ڿ��� x �Ǵ� ��� ���ý� ����ó��
    disp('User selected Cancel');
    return
else 
    File_ID = fopen(fullfile(path,file),'r'); % �ҷ��� ������ �б�������� ����
    initial_EOP = fscanf(File_ID, '%g %g %g %g %g %g',[6 inf]); % ���� ������ �б�
    disp('initial EOP data loaded successfully!'); % ������ �б� ���� �̺�Ʈ�޽��� ���
    initial_EOP = initial_EOP'; 
end  

%% Taylor Series & LESS  

X0_initial= initial_EOP(1,1);   Y0_initial= initial_EOP(1,2);   Z0_initial= initial_EOP(1,3);   
omega_initial= initial_EOP(1,4);   phi_initial= initial_EOP(1,5);   kappa_initial= initial_EOP(1,6);   

dX0 = 1; 
dY0 = 1;
dZ0 = 1;
domega = 1;
dphi = 1;
dkappa = 1;
iteration = 0; 


while max( max( abs( dX0 ), abs( dY0 ) ), abs( dZ0 ) ) > 0.000001 && max( max( abs( domega ), abs( dphi ) ), abs( dkappa ) ) > 0.0000001 && iteration<100

    M = RotationMat(omega_initial, phi_initial, kappa_initial);

    % Linearization of Collinearity Equation
    for i = 1:Num_of_GCP
        [ dxdX_0(i,1), dxdY_0(i,1), dxdZ_0(i,1), dxdomega(i,1), dxdphi(i,1), dxdkappa(i,1), dydX_0(i,1), dydY_0(i,1), dydZ_0(i,1), dydomega(i,1), dydphi(i,1), dydkappa(i,1) ] ...
                                                = Linearization_Co_Eq( IOP_f, GCP_data(i,1), GCP_data(i,2), GCP_data(i,3), X0_initial, Y0_initial, Z0_initial, omega_initial, phi_initial, kappa_initial, M );
                                            

        % Design Matrix for Least Squares Adjustment                                    
        A(2*i-1,:) = [dxdX_0(i,1), dxdY_0(i,1), dxdZ_0(i,1), dxdomega(i,1), dxdphi(i,1), dxdkappa(i,1)];    
        A(2*i,:) = [dydX_0(i,1), dydY_0(i,1), dydZ_0(i,1), dydomega(i,1), dydphi(i,1), dydkappa(i,1)];      
    end
    
    % Mathematical Model
    for i = 1:Num_of_GCP
        initial(i,:) = Mathematical_Model(IOP_xp, IOP_yp, IOP_f, M, GCP_data(i,1), GCP_data(i,2), GCP_data(i,3), X0_initial, Y0_initial, Z0_initial);
    
        x0_initial = initial(:,1);
        y0_initial = initial(:,2);
  
        y_Mat(2*i-1,1) = IMG_data(i,1) - x0_initial(i,1) ;     % y-yo in Observation Equation
        y_Mat(2*i,1) = IMG_data(i,2) - y0_initial(i,1) ;       % x-xo in Observation Equation
    end
  
    N = A'*A;    % N = A'PA , P = I�� ����(���߷� ���ٰ� ����)

    Estimate = pinv(A) * y_Mat;  % Parameter vector to estimate / c = A'Py , P = I�� ����(���߷� ���ٰ� ����)
    
    % Parameter vector to estimate
    dX0 = Estimate(1,1);
    dY0 = Estimate(2,1);
    dZ0 = Estimate(3,1);
    domega = Estimate(4,1);
    dphi = Estimate(5,1);
    dkappa = Estimate(6,1);

    % initial EOP �� ����
    X0_initial = X0_initial+dX0;          
    Y0_initial = Y0_initial+dY0;          
    Z0_initial = Z0_initial+dZ0;          
    omega_initial = omega_initial+domega ;    
    phi_initial = phi_initial+dphi ;         
    kappa_initial = kappa_initial+dkappa ;      
    
    iteration = iteration + 1;
    
    residual = y_Mat - A * Estimate; % ���� / e^~ = y - A*Parameter vector to estimate
        
    sigma = residual' * residual / (Num_of_GCP - gdim); % Standard deviation 
    
end

% Estimated initial EOP matrix
est_EOP = [X0_initial Y0_initial Z0_initial omega_initial phi_initial kappa_initial];

error = initial_EOP'-est_EOP'; % ����
var_comp = (error'*error)/(size(A,1)-size(A,2)); % �л�
D = var_comp^2*(eye(size(A,1))-A*N*A'); % ���л�
%% Display Results 

fprintf('\n\n --------------------SPR ���-------------------- \n\n');
fprintf('X_O ��ǥ : %s \n\n', X0_initial);
fprintf('Y_0 ��ǥ : %s \n\n', Y0_initial);
fprintf('Z_0 ��ǥ : %s \n\n', Z0_initial);
fprintf('��(radian) : %s \n\n', omega_initial*180/pi());
fprintf('��(radian) : %s \n\n', phi_initial*180/pi());
fprintf('��(radian) : %s \n\n', kappa_initial*180/pi());
disp('Residual :')
for a = 1:size(residual)
    fprintf('%d \n',residual(a));
end
fprintf('\n');
fprintf('Standard deviation : %s \n\n', sigma);
fprintf('iteration Ƚ�� : %d \n\n', iteration);
fprintf('Variance components : %s \n\n', var_comp);
disp('Covariance matrices :');
for a = 1:size(D)
    fprintf('%d \n',D(a));
end

