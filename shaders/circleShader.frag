void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    float rOuter = 0.05;
    float rInner = 0.04;
    float xdisp = 0.5;
    float ydisp = 0.5;
    float scale = 1.0;
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    vec3 col;
    scale = cos(iTime);
    
    // Time varying pixel color
    //vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));
    float rsq = (uv.x - xdisp)  * (uv.x - xdisp) + (uv.y - ydisp) * (uv.y - ydisp);
    if(rsq < rOuter*scale && rsq > rInner*scale)
        col = vec3(1, 0, 0);
    else
        col = vec3(0, 0, 0);
    // Output to screen
    fragColor = vec4(col,1.0);
}