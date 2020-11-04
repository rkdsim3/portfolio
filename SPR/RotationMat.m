function [ M ] = RotationMat(omega, phi, kappa)

omega_M = [1, 0, 0
           0, cos(omega), sin(omega)
           0, -sin(omega), cos(omega)];
phi_M   = [cos(phi), 0, -sin(phi)
           0, 1, 0
           sin(phi), 0, cos(phi)];
kappa_M = [cos(kappa), sin(kappa), 0
           -sin(kappa), cos(kappa), 0
            0, 0, 1];

omega_R = omega_M^(-1);
phi_R   = phi_M^(-1);
kappa_R = kappa_M^(-1);

M = kappa_M * phi_M * omega_M;

end