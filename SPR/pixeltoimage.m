
SPR=input("SPR1 or SPR2?: ");


if SPR==1
    
%%SPR1

GCP1_pixel=[1398,248];
GCP3_pixel=[863,2194];
GCP5_pixel=[7615,311];
GCP6_pixel=[4160,3583];
GCP9_pixel=[4930,2350];
GCP11_pixel=[1351,4297];
GCP_pixel=[GCP1_pixel;GCP3_pixel;GCP5_pixel;GCP6_pixel;GCP9_pixel;GCP11_pixel];

elseif SPR==2
  
%%SPR2

GCP1_pixel=[1398,248];
GCP2_pixel=[7002,730];
GCP3_pixel=[863,2194];
GCP5_pixel=[7615,311];
GCP6_pixel=[4160,3583];
GCP7_pixel=[7390,4869];
GCP9_pixel=[4930,2350];
GCP11_pixel=[1351,4297];
GCP_pixel=[GCP1_pixel;GCP2_pixel;GCP3_pixel;GCP5_pixel;GCP6_pixel;GCP7_pixel;GCP9_pixel;GCP11_pixel];


end

pixelsize=4.4*10^-6;
num_row=7952;
num_col=5304;


for i=1:size(GCP,1)
    GCP_image(i,1)=(GCP_pixel(i,1)-(num_row-1)/2)*pixelsize;
    GCP_image(i,2)=((num_col-1)/2-GCP_pixel(i,2))*pixelsize;
    
    
end
