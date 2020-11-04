function [ dxdX_0, dxdY_0, dxdZ_0, dxdomega, dxdphi, dxdkappa, dydX_0, dydY_0, dydZ_0, dydomega, dydphi, dydkappa ] = Linearization_Co_Eq( c, X, Y, Z, X_0, Y_0, Z_0, omega, phi, kappa, M )


m11 = cos(phi)*cos(kappa);                                            % m11 = M(1,1);
m12 = cos(omega) * sin(kappa) + sin(omega) * sin(phi) * cos(kappa);   % m12 = M(1,2);
m13 = sin(omega)*sin(kappa)-cos(omega)*sin(phi)*cos(kappa);           % m13 = M(1,3);
m21 = -cos(phi)*sin(kappa);                                           % m21 = M(2,1);
m22 = cos(omega)*cos(kappa)-sin(omega)*sin(phi)*sin(kappa);           % m22 = M(2,2);
m23 = sin(omega)*cos(kappa)+cos(omega)*sin(phi)*sin(kappa);           % m23 = M(2,3);
m31 = sin(phi);                                                       % m31 = M(3,1);
m32 = -sin(omega)*cos(phi);                                           % m32 = M(3,2);
m33 = cos(omega)*cos(phi);                                            % m33 = M(3,3);

% R=M^(-1)이므로, D, N_x, N-y를 M matrix로 표현

D = m31 * ( X - X_0 ) + m32 * ( Y - Y_0 ) + m33 * ( Z - Z_0 );
N_x = m11 * ( X - X_0 ) + m12 * ( Y - Y_0 ) + m13 * ( Z - Z_0 );
N_y = m21 * ( X - X_0 ) + m22 * ( Y - Y_0 ) + m23 * ( Z - Z_0 );

% The x coordinate equation

dxdX_0 = c * ( ( m11 * D - m31 * N_x ) / (D^2) );

dxdY_0 = c * ( ( m12 * D - m32 * N_x ) / (D^2) );

dxdZ_0 = c * ( ( m13 * D - m33 * N_x ) / (D^2) );

% The y coordinate equation

dydX_0 = c * ( ( m21 * D - m31 * N_y ) / (D^2) );

dydY_0 = c * ( ( m22 * D - m32 * N_y ) / (D^2) );

dydZ_0 = c * ( ( m23 * D - m33 * N_y ) / (D^2) );

% Partial derivatives with respect to EOP

dxdomega = -c * ( D * ( -m13 * ( Y - Y_0 ) + m12 * ( Z - Z_0 )) + N_x * ( -m33 * ( Y - Y_0 ) + m32 * ( Z - Z_0 ) ) ) / (D^2);
    
dxdphi = -c * ( -D^2 * cos(kappa) + N_x * ( -N_x * cos(kappa) + N_y * sin(kappa))) / (D^2);

dxdkappa = -c * N_y / D;

dydomega = -c * ( D * ( -m23 * ( Y - Y_0 ) + m22 * ( Z - Z_0 )) + N_y * ( -m33 * ( Y - Y_0 ) - m32 * ( Z - Z_0 ) ) ) / (D^2);
    
dydphi = -c * ( D^2 * sin(kappa) + N_y * ( N_x * cos(kappa) + N_y * sin(kappa))) / (D^2);
  
dydkappa = c * N_x / D;


end

