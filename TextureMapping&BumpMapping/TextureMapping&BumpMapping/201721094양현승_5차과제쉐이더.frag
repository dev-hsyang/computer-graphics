#version 150 core
//shader.frag:::fragment shader
out vec4 out_Color;

in vec3 worldPosition;
in vec3 normal;
in vec2 texCoord;

uniform vec3 lightPosition;
uniform vec3 lightInt;
uniform vec3 ambInt;
uniform vec3 color;
uniform vec3 cameraPosition;
uniform float shininess;
//main에서 쉐이더로 uniform으로 texture map과 bump map 받아온다
uniform sampler2D colorTexture;
uniform sampler2D bumpTexture;
//uniform float specularColor

mat3 getTBN(vec3 N){
    vec3 Q1 = dFdx(worldPosition), Q2 = dFdy(worldPosition);
    vec2 st1 = dFdx(texCoord), st2 = dFdy(texCoord);
    float D = st1.s * st2.t - st2.s * st1.t;
    return mat3( normalize( (Q1*st2.t - Q2 * st1.t) * D ),
                 normalize( (-Q1*st2.s + Q2 * st1.s) * D),
                 N );
}

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
    
    vec4 albelo = texture(colorTexture, texCoord);
    
    vec3 diffuse = lightInt * max(0,dot(N, L)) * albelo.rgb;
    vec3 ambient = ambInt * albelo.rgb;
    vec3 specular = lightInt * pow(max(0, dot(R, V)), shininess);
    out_Color = vec4(diffuse + ambient + specular, 1.0);    
}
