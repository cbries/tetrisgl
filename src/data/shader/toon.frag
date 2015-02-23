// simple toon fragment shader
// www.lighthouse3d.com

varying vec3 normal, lightDir;

// void main()
// {

// 	float intensity;
// 	vec3 n;
// 	vec4 color;
// 
// 	n = normalize(normal);
// 	intensity = max(dot(lightDir,n),0.0); 
// 
// 	if( intensity > 0.98 )
// 		color = vec4(0.98,0.98,0.98,1.0);
// 	else if( intensity > 0.75 )
// 		color = vec4(0.75,0.75,0.75,1.0);
// 	else if( intensity > 0.5 )
// 		color = vec4(0.5,0.5,0.5,1.0);
// 	else if( intensity > 0.25 )
// 		color= vec4(0.25,0.25,0.25,1.0);
// 	else if( intensity > 0.1 )
// 		color = vec4(0.1,0.1,0.1,1.0);
// 	else	
// 		color = gl_Color;

// 	if (intensity > 0.98)
// // 		color = vec4(1.0,1.0,1.0,1.0);
// 		color = gl_Color * 0.95;
// 	else if (intensity > 0.5)
// // 		color = vec4(0.4,0.4,0.4,1.0);	
// 		color = gl_Color * 0.55;
// 	else if (intensity > 0.25)
// // 		color = vec4(0.2,0.2,0.2,1.0);
// 		color = gl_Color * 0.25;
// 	else if( intensity > 0.1 )
// 		color = gl_Color * 0.1;
// 	else
// 		color = gl_Color;

// 	gl_FragColor = color;
// }


void main()
{
	// Faktor der Farbe berechnen.
	/// Wert zwischen 0 - 1 (schwarz - weiß)
	
	color =  gl_Color * 0.7;
	gl_FragColor = color;
}