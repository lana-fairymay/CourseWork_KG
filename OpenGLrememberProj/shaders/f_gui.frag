varying vec2 texCoord;  // значение текстурных координат 
// именно этого фрагмента

void main(void)
{
float x=texCoord.x;
float y=texCoord.y;
 
if(gl_FragColor.r == 1 && 
   gl_FragColor.g == 1 &&
   gl_FragColor.b == 1 )
   {
	   gl_FragColor.a = 1;
	   gl_FragColor.r = 1;
   }
 

}
