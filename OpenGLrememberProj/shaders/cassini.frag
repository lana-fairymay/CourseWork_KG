#version 120

varying vec2 texCoord;  
uniform vec2 size;
uniform float Time;



//    https://en.wikipedia.org/wiki/Cassini_oval
bool curve(in float x, in float y, in float start, out float r)
{
    float a = 1.0;
    float c = (abs(fract(start + Time*0.012)*1.3)+0.7)*a;
    r=abs( (x*x+y*y)*(x*x+y*y)-2.0*c*c*(x*x-y*y) - c*c*c*c+a*a*a*a) ;
    if  (r <= 0.013)
       return true;
    else 
        return false;
    
}

void main( void )
{
	
	vec2 uv = texCoord.xy;
    uv.x*=size.x/size.y;


    
    float u = uv.x*2-1*size.x/size.y;
    float v = uv.y*2-1;
      
    gl_FragColor = vec4(0,0,0,1);
    
    for (float t=0.0; t<1.0;t+=0.04)
    {
        float r;
    	curve(u,v,t,r);
        float col = 1.0/(r*300.0);
        gl_FragColor += vec4(col*(sin(t*3.14)+2.0),
		                     col*(cos(t*3.14)+2.0),
							 col,
							 1.0);
						//*vec4(uv+Time, 1.0, 1.0);
    }
    
}