#version 150 core
//shader.frag:::fragment shader
out vec4 out_Color;

in vec3 worldPosition;
in vec3 normal;
in vec2 texCoord;
in vec4 shadow_Position;

uniform vec3 lightPosition;
uniform vec3 lightInt;
uniform vec3 ambInt;
uniform vec3 color;
uniform vec3 cameraPosition;
uniform vec3 specularColor;
uniform float shininess;
//main에서 쉐이더로 uniform으로 texture map과 bump map 받아온다
uniform sampler2D colorTexture;
uniform sampler2D bumpTexture;
uniform sampler2D shadowMap;
//uniform float specularColor

mat3 getTBN(vec3 N){
    vec3 Q1 = dFdx(worldPosition), Q2 = dFdy(worldPosition);
    vec2 st1 = dFdx(texCoord), st2 = dFdy(texCoord);
    float D = st1.s * st2.t - st2.s * st1.t;
    return mat3( normalize( (Q1*st2.t - Q2 * st1.t) * D ),
                 normalize( (-Q1*st2.s + Q2 * st1.s) * D),
                 N );
}

const vec2 poissonDisk[4] = vec2[](
    vec2( -0.94201624, -0.39906216 ),
    vec2( 0.94558609, -0.76890725 ),
    vec2( -0.094184101, -0.92938870 ),
    vec2( 0.34495938, 0.29387760 ));


void main(void){
    vec3 L = normalize(lightPosition-worldPosition);
    vec3 N = normalize(normal);
    
    //bump map을 적용한 normal 새로 계산
    mat3 TBN = getTBN(N);
    float Bu = texture(bumpTexture, texCoord+vec2(0.0001, 0)).r - texture(bumpTexture, texCoord+vec2(-0.0001, 0)).r;
    float Bv = texture(bumpTexture, texCoord+vec2(0, 0.0001)).r - texture(bumpTexture, texCoord+vec2(0, -0.0001)).r;
    vec3 bumpVec = vec3( -Bu * 15, -Bv * 15, 1);
    N = normalize(TBN * bumpVec);
    
    vec3 R = 2  * dot(N, L) * N - L;
    vec3 V = normalize (cameraPosition - worldPosition);
    vec4 albedo = texture(colorTexture, texCoord);
    vec3 diffuse = lightInt * max(0,dot(N, L)) * albedo.rgb;
    vec3 ambient = ambInt * albedo.rgb;
    vec3 specular = lightInt * pow(max(0, dot(R, V)), shininess) * max(0,dot(N,L)) * specularColor;
    
    vec3 shadowCoord = shadow_Position.xyz/shadow_Position.w * 0.5 + 0.5;
    //float depth = texture( shadowMap, shadowCoord.xy + poissonDisk[]).r; //조명이 봤을때 가장 가까운 점의 depth
    float shadowFactor = 1;
    float bias = 0.001;
    for(int i=0; i<4; i++){
        if( shadowCoord.z - bias > texture( shadowMap, shadowCoord.xy + poissonDisk[i]/700.).r ) shadowFactor -= 0.25;
    }
    out_Color = vec4( pow((diffuse + specular) * shadowFactor + ambient, vec3(1/2.2)), 1.0);
}
