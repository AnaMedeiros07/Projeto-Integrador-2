u_sat_plus=6;	u_sat_minus=-6
J=0.02;Bf=0.0121;Km=0.312;R=3.47;Kw=1
A	=	[-Bf/J-Km^2/(R*J)	0	;1	0]; B	=	[Km*Kw/(R*J)	1/J;	0	0];
Bu	=	B(:,1)	//B	column	corresponding	to	command	
C	=	[1	0;	0	1];		
sys_c=syslin('c',A,B,C);
tf=4													//Final	instant	of	simulaCon	
x0=[1	0]'				//IniCal	state	
A_dis=A;B_dis=B;Bu_dis=Bu;	C_dis=C	
sysdis_c=syslin('c',A_dis,B_dis,C_dis);	//This	is	needed	for	simulaCon	in	general	
sysdis_c_csim=syslin('c',A_dis,Bu_dis,C_dis);	//This	is	needed	for	the	csim
h=0.06
sys_d=dscr(sys_c,h);	Fi=sys_d(2);Gama=sys_d(3)	
Gamau=Gama(:,1)	//Gama	column	for	the	command	input	
p_d=spec(Fi);
sysdis_d=dscr(sysdis_c,h);Fi_dis=sysdis_d(2);Gama_dis=sysdis_d(3)	
sysdis_c=syslin('c',A_dis,B_dis,C_dis);
poles_s=[-3	-3];
poles_z=exp(poles_s*h);disp(poles_z,"Feedback	system	poles	in	the	z	plane:")
K=ppol(Fi,Gamau,poles_z);	disp(K,"Feedback	gains	k1	and	k2	for	the	regulator:")	
tk=0:h:tf//Vector	of	Cme	instants	
x0 =[1 1]';
p_amp=0;
tk_size=size(tk)	
N=tk_size(2)	//Number	of	sampling	instants	in	simulaCon	
p=p_amp*ones(1,N)											//Disturbance	
//IniCal	values	
xk=x0
xmk=x0
encoder_resolution=960	
tetamq=2*%pi/encoder_resolution
for	k=0:1:N-1	
    pk=p(k+1)	
    uk=-K*xmk
    if	uk>u_sat_plus	then	uk=u_sat_plus	end	
    if	uk<u_sat_minus	then	uk=u_sat_minus	end	
    xkplus1=Fi_dis*xk+Gama_dis*[uk pk]'	
    u(1,k+1)=uk
    x(:,k+1)=xk
    xk=xkplus1	
    xm(:,k+1)=xmk
    omegak=xk(1,1)	
    tetak=xk(2,1)	
    if k==0 then
        tetamk=tetak
        omegamk=omegak
    else
        tetakminus1=x(2,k)
        tetamkminus1=xm(2,k)				
    if modulo(tetak,tetamq)==0 then tetamk=tetak
    else
        Qhigh=ceil(tetak/tetamq)*tetamq
        Qlow=floor(tetak/tetamq)*tetamq
        if Qlow <tetakminus1 &tetakminus1<Qhigh then
            tetamk=tetamkminus1
        else
            if tetak>tetakminus1 then
                tetamk=Qlow
            else 
                tetamk=Qhigh
            end
               
        end
    end
    omegamk=omegak
end
xmk=[omegamk tetamk]'
end

i=x(1,:); //velocidade
i=(u-(Km*i))/R;
scf(0)	
clf
UfScale=0.1	
drawlater()	
a=gca();	
plot(tk',xm',tk',	x',tk',i',tk',(u*UfScale)')
a.children.children.mark_mode='on'		//Desenha pontos
a.children.children.mark_style=0				//Marca	de	ponto é círculo
a.children.children.mark_size=3	
a.children.children(1).mark_foreground=5					//Cor	da	marca	de	ponto		
a.children.children(1).mark_background=5				//Cor	da	marca	de	ponto
a.children.children(2).mark_foreground=2					//Cor	da	marca	de	ponto		
a.children.children(2).mark_background=2
a.children.children(3).mark_foreground=3					//Cor	da	marca	de	ponto		
a.children.children(3).mark_background=3		
a.children.children(4).mark_foreground=4					//Cor	da	marca	de	ponto		
a.children.children(4).mark_background=4	
a.children.children(5).mark_foreground=4					//Cor	da	marca	de	ponto		
a.children.children(5).mark_background=4
a.children.children(6).mark_foreground=6					//Cor	da	marca	de	ponto		
a.children.children(6).mark_background=6		
a.children.children(1).foreground=5		
a.children.children(2).foreground=2
a.children.children(3).foreground=3
a.children.children(4).foreground=4
a.children.children(5).foreground=4
a.children.children(6).foreground=6
a.grid=[0	0];		
a.x_label.text='t';	
a.x_label.font_size=3;	
a.y_label.font_size=3;	
legend(['omegam','tetam','omega','teta','i','u/'+string(1/UfScale)'],1)
drawnow()	
