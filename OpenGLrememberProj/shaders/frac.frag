#version 120

varying vec2 texCoord;  
uniform vec2 size;
uniform float uZoom;
uniform vec2 uOffset;
uniform float Time;

vec2 umn(vec2 a, vec2 b)  //умножение комплексных чисел
{
     vec2 r;
     r.x = a.x*b.x - a.y*b.y;
     r.y = a.x*b.y + a.y*b.x;
     return r;
}

vec2 powC(vec2 a, int p)   //возведение комплексных чисел в степень
{
	vec2 o = a;
	for (int i=2; i<=p; i++)
	{
		o = umn (a,o);
	}
	return o;
}


vec3 toRgb(int c, int max)    //HSV to RGB 
{
	int new_c = int(mod(c,max));
	float d = 1.0*max/6;
	int n = int(floor(1.0*new_c / d));
	vec3 outrgb;
	float val;
	if (new_c==n*d)
		val=0;
	else
		val = 1.0*( new_c-n*d ) / d;
	if (n==0)
	   outrgb = vec3 (1,val,0);
	if (n==1)
	   outrgb = vec3 (1-val,1,0);
	if (n==2)
	   outrgb = vec3 (0,1,val);
	if (n==3)
	   outrgb = vec3 (0,1-val,1);
	if (n==4)
	   outrgb = vec3 (val,0,1);
	if (n==5)
	   outrgb = vec3 (1,0,1-val);
    //if (n>=6)
		//outrgb = vec3(0,0,0);
	return outrgb;
}

void main(void)
{
	
	float aspect = size.x/size.y;
	vec2 uv = texCoord;
	
	//gl_FragColor = vec4(toRgb(uv.x,1),1.0);

		
		uv-=vec2(0.5,0.5);
				
		uv.x*=aspect;
				
		
		uv/=uZoom;
		uv-=uOffset;	
		
		gl_FragColor = vec4(0.0,0.0,0.0,1.0);
		int i;
		vec2 R;
		float MAX = sqrt(uZoom)*20;
		for (i=0; i<MAX; i++)
		{
			R = powC(R,2)+uv;
			if ( (R.x*R.x+R.y*R.y) > 2 )
			{ 
				gl_FragColor = vec4 (toRgb(i, int(MAX)),1.0);
				break;
			}
		}
	

}




