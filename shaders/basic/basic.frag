#version 450

layout(location=0)in vec3 fragColor;
layout(location=1)in vec3 position;

layout(location=0)out vec4 outColor;

float roundedBoxSDF(vec2 CenterPosition,vec2 Size,vec4 Radius)
{
    vec4 radius=Radius;
    
    //Radius.xy = (CenterPosition.x>0.0) ? Radius.xy : Radius.zw;
    if(CenterPosition.x>0.)
    {
        radius.x=Radius.x;
        radius.y=Radius.y;
    }else
    {
        radius.x=Radius.z;
        radius.y=Radius.w;
    }
    
    //Radius.x  = (CenterPosition.y>0.0) ? Radius.x  : Radius.y;
    if(CenterPosition.y>0.)
    {
        radius.x=Radius.x;
    }else
    {
        radius.x=Radius.y;
    }
    
    vec2 q=abs(CenterPosition)-Size+radius.x;
    return min(max(q.x,q.y),0.)+length(max(q,vec2(0.)))-radius.x;
}

vec4 RoundedBox()
{
    // The pixel space scale of the rectangle.
    vec2 size=vec2(.5,.5);
    
    // The pixel space location of the rectangle.
    vec2 location=vec2(0.,0.);
    
    // How soft the edges should be (in pixels). Higher values could be used to simulate a drop shadow.
    float edgeSoftness=.01;
    
    // The radius of the corners (in pixels) clockwise starting in the top left.
    vec4 radius=vec4(0.,0.,0.,0.);
    
    // Calculate distance to edge.
    float distance=roundedBoxSDF(position.xy-location.xy,size.xy/2.,radius);
    
    // Smooth the result (free antialiasing).
    float smoothedAlpha=1.-smoothstep(0.,edgeSoftness,distance);
    
    // Border
    float borderThickness=0.;
    float borderSoftness=.1;
    float borderAlpha=1.-smoothstep(borderThickness-borderSoftness,borderThickness,abs(distance));
    
    // Colors
    vec4 rectColor=vec4(1.,0.,0.,1.);
    vec4 borderColor=vec4(1.,0.,0.,1.);
    vec4 bgColor=vec4(.0941,.6471,.3529,0.);
    
    return mix(bgColor,mix(rectColor,borderColor,borderAlpha),smoothedAlpha);
}

void main(){
    outColor=RoundedBox();
    //outColor=vec4(fragColor,1.);
}