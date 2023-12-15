#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

uniform float framesStill;
uniform float NUMBER_OF_SAMPLES;
uniform float MAX_REFLECTIONS;
uniform float apertureSize;
uniform float focusDistance;
uniform float zoom;
uniform float colorMultiplierWhenReachedMaxRef;
uniform bool showNormals;
uniform vec2 rSeed1;
uniform vec2 rSeed2;
uniform vec2 resolution;
uniform vec3 CameraRotation;
uniform vec3 CameraPosition;
uniform vec3 skyboxRotation;
uniform sampler2D skybox;
uniform sampler2D tex;
uniform bool planeGrid;
uniform vec3 gridCol2;
uniform float tileSize;
uniform bool useSkyboxColor;
uniform vec3 skyboxColor;
uniform float treshHoldIntensity;

const int MAX_OBJECTS = 30;
const int ELEMENTS_IN_1OBJ = 23;
uniform float objects[MAX_OBJECTS*ELEMENTS_IN_1OBJ];

#define MAX_DIST 1000.0
#define PI 3.141592

uvec4 R_STATE;
float degree = 2.0 * PI / 360.0;
vec3 bloomWeights = vec3(0.2126, 0.7152, 0.0722);

struct Sdf {
    float d;
    float d2;
    vec3 n;
    vec3 n2;
};

struct Ray {
    Sdf sdf;
    vec3 col;
    vec3 specularCol;
    vec3 pos;
    float percentSpecular;
    float roughness;
    float refractionIndex;
    float reflectivity;
    float isLight;
    float type;
};

uint TausStep(uint z, int S1, int S2, int S3, uint M) {
	uint b = (((z << S1) ^ z) >> S2);
	return (((z & M) << S3) ^ b);	
}

uint LCGStep(uint z, uint A, uint C) {
	return (A * z + C);	
}

vec2 hash22(vec2 p) {
	p += rSeed1.x;
	vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
	p3 += dot(p3, p3.yzx+33.33);
	return fract((p3.xx+p3.yz)*p3.zy);
}

float random() {
	R_STATE.x = TausStep(R_STATE.x, 4, 12, 9, uint(4294967294));
	R_STATE.y = TausStep(R_STATE.y, 2, 3, 4, uint(4294967288));
	R_STATE.z = TausStep(R_STATE.z, 3, 1, 10, uint(4294967280));
	R_STATE.w = LCGStep(R_STATE.w, uint(1664525), uint(1013904223));
	return 2.3283064365387e-10 * float((R_STATE.x ^ R_STATE.y ^ R_STATE.z ^ R_STATE.w));
}

vec3 randomOnSphere() {
	vec3 rand = vec3(random(), random(), random());
	float theta = rand.x * 2.0 * 3.14159265;
	float v = rand.y;
	float phi = acos(2.0 * v - 1.0);
	float r = pow(rand.z, 1.0 / 3.0);
	float x = r * sin(phi) * cos(theta);
	float y = r * sin(phi) * sin(theta);
	float z = r * cos(phi);
	return vec3(x, y, z);
}

mat3 Pitch(float a) {
    float Sin = sin(a);
    float Cos = cos(a);

    return mat3(1, 0, 0,
                0, Cos, -Sin,
                0, Sin, Cos);
}

mat3 Yaw(float a) {
    float Sin = sin(a);
    float Cos = cos(a);

    return mat3(Cos, 0, Sin,
                0, 1, 0,
                -Sin, 0, Cos);
}

mat3 Roll(float a) {
    float Sin = sin(a);
    float Cos = cos(a);

    return mat3(Cos, -Sin, 0,
                Sin, Cos, 0,
                0, 0, 1);
}

vec3 applySkyBox(vec3 rd, sampler2D skybox) {
    if (useSkyboxColor) return skyboxColor;
    rd *= Pitch(skyboxRotation[0] * degree);
    rd *= Yaw(skyboxRotation[1] * degree);
    rd *= Roll(skyboxRotation[2] * degree);
    // Convert the ray direction to 2D texture coordinates
    vec2 st;
    st.x = 0.5 + atan(rd.z, rd.x) / (2.0 * PI);
    st.y = 0.5 - asin(rd.y) / PI;
    // Sample the cube map using the texture coordinates
    vec3 col = texture(skybox, st).rgb;

    return col;
}

Ray Min(Ray minIt, Ray obj) {
    return (obj.sdf.d > 0.0 && obj.sdf.d < minIt.sdf.d) ? obj : minIt;
}

Sdf sdSphere( in vec3 ro, in vec3 rd, in vec3 ce, float ra ) {
    vec3 oc = ro - ce;
    float b = dot( oc, rd );
    vec3 qc = oc - b*rd;
    float h = ra*ra - dot( qc, qc );
    // no intersection
    if( h<0.0 ) return Sdf(MAX_DIST, MAX_DIST, vec3(-1.0), vec3(0));
    h = sqrt( h );
    float d = -b - h;
    float d2 = -b + h;
    // normal of the nearest hit
    vec3 n = normalize((ro + rd * (d - 0.001)) - ce);
    // normal of the farest hit
    vec3 n2 = normalize((ro + rd * (d2 - 0.001)) - ce);

    return Sdf(d, d2, n, n2);
}

Sdf sdCube( in vec3 ro, in vec3 rd, in vec3 ce, vec3 boxSize, float IOR) {   
    ro -= ce;
    vec3 m = 1.0/rd;
    vec3 n = m*ro;
    vec3 k = abs(m)*boxSize;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    float tN = max( max( t1.x, t1.y ), t1.z );
    float tF = min( min( t2.x, t2.y ), t2.z );
    // no intersection
    if( tN>tF || tF<0.0) return Sdf(MAX_DIST, MAX_DIST, vec3(0), vec3(0));
    // normal of the nearest hit
    vec3 N = (tN>0.0) ? step(vec3(tN),t1) : step(t2,vec3(tF));
    N *= -sign(rd);
    // normal of the farest hit
    // there is no need to calculate object's N2 if it is not a lense as it will not be used
    vec3 N2 = vec3(0);
    if (IOR != -1.0){
        N2 = (tF > 0.0) ? step(t1, vec3(tN)) : step(vec3(tF), t2);
        N2 *= sign(rd);
    }

    return Sdf(tN, tF, N, N2);
}

Sdf sdPlane(in vec3 ro, in vec3 rd, in vec4 p) {
	float d = -(dot(ro, p.xyz) + p.w) / dot(rd, p.xyz);
    return Sdf(d, d, p.xyz, vec3(0));
}

float fresnelReflectAmount (vec3 rd, vec3 normal, float n1, float n2, float object_reflectivity) {
        // Schlick aproximation
        float r0 = (n1-n2) / (n1+n2);
        r0 *= r0;
        float cosX = -dot(normal, rd);
        if (n1 > n2)
        {
            float n = n1/n2;
            float sinT2 = n*n*(1.0-cosX*cosX);
            // Total internal reflection
            if (sinT2 > 1.0)
                return 1.0;
            cosX = sqrt(1.0-sinT2);
        }
        float x = 1.0-cosX;
        float ret = r0+(1.0-r0)*x*x*x*x*x;
 
        ret = (object_reflectivity + (1.0-object_reflectivity) * ret);
        return ret;
}

Ray GetClosestObj(vec3 ro, vec3 rd) {
    Ray minIt;
    minIt.sdf.d = MAX_DIST;
    // x, y, z, r, g, b, type, radius, cubeSize, isLigjt, reflectivity, refract, specularPercent, roughtness, powerOfLight, rotation, specularColour
	// 1  2  3  4  5  6    7     8     9 10 11     12         13          14          15             16            17       18 19 20     21 22 23
    for (int i=0; i<MAX_OBJECTS; i++){
        if (objects[6 + i * ELEMENTS_IN_1OBJ] != 0.0){
            Ray object;
            int ind = i * ELEMENTS_IN_1OBJ;

            // common properties
            vec3 pos = vec3(objects[ind], objects[1 + ind], objects[2 + ind]);
            object.col = vec3(objects[3 + ind], objects[4 + ind], objects[5 + ind]);
            object.specularCol = vec3(objects[20 + ind], objects[21 + ind], objects[22 + ind]);

            // choosing the material
            object.isLight = objects[11 + ind];
            if (object.isLight == 1.0) object.col *= objects[16 + ind];
            object.refractionIndex = objects[13 + ind];
            object.reflectivity = objects[12 + ind];
            object.roughness = objects[15 + ind];
            object.percentSpecular = objects[14 + ind];
            
            // choosing the sdf
            if (objects[6 + ind] == 1.0) {
                // sphere
                object.sdf = sdSphere(ro, rd, pos, objects[7 + ind]);
                object.type = 1.0;
            } 
            else if (objects[6 + ind] == 2.0) {
                // cube
                mat3 rotMat = Pitch(objects[17 + ind] * degree) * Yaw(objects[18 + ind] * degree) * Roll(objects[19 + ind] * degree);
                object.sdf = sdCube((ro-pos) * rotMat, rd * rotMat, vec3(0), vec3(objects[8 + ind], objects[9 + ind], objects[10 + ind]), object.refractionIndex);
                rotMat = transpose(inverse(rotMat));
                object.sdf.n = normalize(rotMat * object.sdf.n);
                if (object.refractionIndex != -1.0){
                    object.sdf.n2 = normalize(rotMat * object.sdf.n2);
                }
                object.type = 2.0;
            } 
            else if (objects[6 + ind] == 3.0) {
                // plane
                object.sdf = sdPlane(ro, rd, vec4(0, 1, 0, -1.0 * pos.y));
                object.type = 3.0;
            }
            minIt = Min(minIt, object);
            minIt.pos = pos;
        }
    }

    return minIt;
}

Ray CastRay(inout vec3 ro, inout vec3 rd){
    Ray minIt = GetClosestObj(ro, rd);
    if (showNormals) minIt.col = minIt.sdf.n;

    // if hit sky
    if (minIt.sdf.d == MAX_DIST) {
        minIt.col = applySkyBox(rd, skybox);
        return minIt;
    }
    
    // if hit a light source
    if (minIt.isLight == 1.0){
        minIt.sdf.d = MAX_DIST;
        return minIt;
    };
        
    // refraction
    if (minIt.refractionIndex != -1.0 && !showNormals) {
        ro += rd * (minIt.sdf.d - 0.001);  // move ro to the hit pos

        // calculate how much light is reflected
        float rFloat = fract(random());
        float fresnel = fresnelReflectAmount(rd, minIt.sdf.n, 1.01, minIt.refractionIndex, minIt.reflectivity);
        if (rFloat < fresnel * fresnel){
            minIt.col = minIt.specularCol;
            rd = reflect(rd, minIt.sdf.n);
            return minIt;
        }

        // refract the rd
        rd = refract(rd, minIt.sdf.n, 1.01 / minIt.refractionIndex);
            
        // get the second distance, the hit we cant see
        minIt = GetClosestObj(ro, rd);

        // and move the ro there
        ro += rd * (minIt.sdf.d2 + 0.001);
        // refrect the distance again as light isrefracted twice, both when enters the glass and leaves it
        rd = refract(rd, -minIt.sdf.n2, minIt.refractionIndex / 1.01);

        return minIt;
    }
    // move the ray origin to the point of hit
    ro += rd * (minIt.sdf.d - 0.001);

    // plane grid
    if (minIt.type == 3.0 && planeGrid){
        float tile = mod(floor(ro.x / tileSize) + floor(ro.z / tileSize), 2.0);
        if (tile == 1.0) minIt.col = gridCol2;
    }

    vec3 rOnSphere = randomOnSphere(); // random ray direction
    bool doSpecular = fract(random()) < minIt.percentSpecular;

    //  if specular reflection
    if (doSpecular){
        vec3 specular = reflect(rd, minIt.sdf.n);
        vec3 diffuse = normalize(rOnSphere * dot(rOnSphere, minIt.sdf.n));
        rd = mix(specular, diffuse, minIt.roughness);
        if (minIt.type != 3.0) minIt.col = minIt.specularCol; // specular colour
    } else {
        rd = normalize(rOnSphere * dot(rOnSphere, minIt.sdf.n));
    }

    return minIt;
}

vec3 RayTrace(vec3 ro, vec3 rd) {
	vec3 col = vec3(1);
    float i = 0.0;
	for(i; i < MAX_REFLECTIONS; i++)
	{
        Ray refCol = CastRay(ro, rd);
		col *= refCol.col;

		if (refCol.sdf.d == MAX_DIST) break;
        if (dot(col, vec3(1)) < 0.3) return col * 0.5;
    }
    if (i == MAX_REFLECTIONS && !showNormals) return col * colorMultiplierWhenReachedMaxRef;

	return col;
}

void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5 * resolution) / resolution.y;
    // random
    vec2 uvRes = hash22(uv + 1.0) * resolution + resolution;
	R_STATE.x = uint(rSeed1.x + uvRes.x);
	R_STATE.y = uint(rSeed1.y + uvRes.x);
	R_STATE.z = uint(rSeed2.x + uvRes.y);
	R_STATE.w = uint(rSeed2.y + uvRes.y);
    // color
    vec3 col = vec3(0);
    // ray direction
    vec3 rd = normalize(vec3(uv.x, uv.y, zoom));
    vec3 ro = CameraPosition;
    // camera rotation
    rd *= Pitch(CameraRotation.y * degree);
    rd *= Yaw  (CameraRotation.x * degree);
    rd *= Roll (CameraRotation.z * degree);

    // ray tracing
    for (float i=0.0; i<NUMBER_OF_SAMPLES; i++){
        vec3 offset = vec3(randomOnSphere() * 0.5 * apertureSize);
        vec3 nRo = ro + offset;
        vec3 nRd = normalize(focusDistance * rd - offset);

        col += RayTrace(nRo, nRd);
    } col /= NUMBER_OF_SAMPLES;
    

    vec3 sampleCol = texture(tex, gl_FragCoord.xy / resolution).rgb;
    col = mix(sampleCol, col, 1.0 / (framesStill));

    FragColor = vec4(col, 1.0);

    // calculate bright pixels
    float brightness = dot(FragColor.rgb, bloomWeights * treshHoldIntensity);
    if (brightness > 1.0) BloomColor = FragColor;
    else BloomColor = vec4(0.0);
} 
