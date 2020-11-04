function [ output ] = Mathematical_Model( xp, yp, c, M, X, Y, Z, X0_initial, Y0_initial, Z0_initial )

x0 = xp - c * ( M(1,1) * (X - X0_initial) + M(1,2) * (Y - Y0_initial) + M(1,3) * (Z - Z0_initial) ) / ( M(3,1) * (X - X0_initial) + M(3,2) * (Y - Y0_initial) + M(3,3) * (Z - Z0_initial) );
y0 = yp - c * ( M(2,1) * (X - X0_initial) + M(2,2) * (Y - Y0_initial) + M(2,3) * (Z - Z0_initial) ) / ( M(3,1) * (X - X0_initial) + M(3,2) * (Y - Y0_initial) + M(3,3) * (Z - Z0_initial) );

output = [ x0; y0 ];

end

