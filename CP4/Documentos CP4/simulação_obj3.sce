//------- Definir Variaveis-----
u_sat_plus=6;	u_sat_minus=-6
J=0.02;Bf=0.0121;Km=0.3120;R=3.47;Kw=1
A	=	[-Bf/J-Km^2/(R*J)	0	;1	0]; B	=	[Km*Kw/(R*J)	1/J;	0	0];
Bu	=	B(:,1)	//B	column	corresponding	to	command	
C	=	[1	0;	0	1];		
sys_c=syslin('c',A,B,C);
//-------Polos-----------------
p_c=spec(A)	//Calculates	A	eigenvalues	
disp(p_c,"Nominal	model	poles	in	s:")
//-------condições iniciais------
tf=4													//Final	instant	of	simulaCon	
x0=[1	0]'				//IniCal	state	
//-----------preturbação----------
A_dis=A;B_dis=B;Bu_dis=Bu;	C_dis=C	
sysdis_c=syslin('c',A_dis,B_dis,C_dis);	//This	is	needed	for	simulaCon	in	general	
sysdis_c_csim=syslin('c',A_dis,Bu_dis,C_dis);	//This	is	needed	for	the	csim
//-------- Simulação resposta livre------
tk=0:0.01:tf		//Vector	of	Cme	instants	
[y,x]=csim(zeros(tk),tk,sysdis_c_csim,x0)										//instrucCon
//---------Simulação resposta forçada-----
[y,x]=csim(zeros(tk),tk,sysdis_c_csim,x0)	
[y,x]=csim("step",tk,sysdis_c_csim)	
//--------Desenho dos Graficos-------
scf(0);	clf;	drawlater();a=gca();	
plot(tk',x')	
a.children.children.mark_mode='on'		//Desenha pontos
a.children.children.mark_style=0				//Marca	de	ponto é círculo
a.children.children.mark_size=3					//Tamanho	da	marca	de	pont
a.children.children(1).mark_foreground=5					//Cor	da	marca	de	ponto		
a.children.children(1).mark_background=5				//Cor	da	marca	de	ponto
a.children.children(2).mark_foreground=2					//Cor	da	marca	de	ponto		
a.children.children(2).mark_background=2				//Cor	da	marca	de	ponto
a.children.children(1).foreground=5		
a.children.children(2).foreground=2		
a.grid=[0	0];		
a.x_label.text='t';	
a.x_label.font_size=3;	
a.y_label.font_size=3;	
legend(['omega','teta'],1)	
drawnow()
