 h = 1000;
F = [100 150];
amp = [1 0];
A = [0.03 0.03];

[M,Wn,beta,FTYPE] = kaiserord(F,amp,A,h);
b = fir1(M,Wn,FTYPE,kaiser(M+1,beta),'noscale');
freqz(b,1,1024,h)