#include "rastrizer.h"
// function draw_model(p,qt,vertices,vt,vm,faces,f,tc,uv,textures,calc_light,tex_size,fast,tsts)
//     for i=1,3*faces,3 do
//         local a,b,c,xab,yab,zab,xac,yac,zac,nv,l_dir,l_cos,l_int;
//         a,b,c,l_int=f[i],f[i+1],f[i+2],0.9;
//         if(calc_light==true) then
//             -- flat shading
//             -- normal vector
//             xab,yab,zab,xac,yac,zac=vm[b*3+1]-vm[a*3+1],vm[b*3+2]-vm[a*3+2],vm[b*3+3]-vm[a*3+3],vm[c*3+1]-vm[a*3+1],vm[c*3+2]-vm[a*3+2],vm[c*3+3]-vm[a*3+3]
//             nv={yab*zac-zab*yac,zab*xac-xab*zac,xab*yac-yab*xac}
//             vec_len=v3_len({nv[1],nv[2],nv[3]})
//             nv[1],nv[2],nv[3]=nv[1]/vec_len,nv[2]/vec_len,nv[3]/vec_len
//             nv_len=v3_len(nv)
//             -- light direction
//             local tx,ty,tz=(vm[a*3+1]+vm[b*3+1]+vm[c*3+1])/3,(vm[a*3+2]+vm[b*3+2]+vm[c*3+2])/3,(vm[a*3+3]+vm[b*3+3]+vm[c*3+3])/3
//             l_dir={50-tx,50-ty,50-tz}
//             l_len=v3_len(l_dir)
//             -- cos
//             l_dir_nv=v3_len({l_dir[1]-nv[1],l_dir[2]-nv[2],l_dir[3]-nv[3]})
//             x=nv_len*nv_len+l_len*l_len-l_dir_nv*l_dir_nv
//             y=l_len*nv_len*2
//             l_cos=x/y
//             l_int=max(0.1,l_cos)
//         end
//         local tex_i=1
//         if #textures==6 then tex_i=flr(i/3)%6+1 end
//         tric(
//             vt[a*3+1],
//             vt[a*3+2],
//             vt[b*3+1],
//             vt[b*3+2],
//             vt[c*3+1],
//             vt[c*3+2],
//             {tc[uv[i]*2+1],tc[uv[i]*2+2]},{tc[uv[i+1]*2+1],tc[uv[i+1]*2+2]},{tc[uv[i+2]*2+1],tc[uv[i+2]*2+2]},l_int,tex_size,textures[tex_i],fast,tsts)
//     end
// end
// void draw_cube(p)
// 	local qt,vertices,faces,vt,vm=t*0.01,8,12,{},{}
// 	local v=split("0.35,0.35,-0.35,0.35,-0.35,-0.35,0.35,0.35,0.35,0.35,-0.35,0.35,-0.35,0.35,-0.35,-0.35,-0.35,-0.35,-0.35,0.35,0.35,-0.35,-0.35,0.35")
// 	local f=split("0,2,4,3,7,2,7,5,6,5,7,1,1,3,0,5,1,4,2,6,4,7,6,2,5,4,6,7,3,1,3,2,0,1,0,4")
//     local tc=split("1.0,0.0,0.0,1.0,0.0,0.0,1.0,1.0")
//     local uv=split("2,1,0,2,1,0,1,3,2,0,2,3,2,1,0,1,3,2,1,3,0,1,3,0,3,0,2,2,1,3,1,3,0,3,0,2")
//     for j=1,vertices*3,3 do
//         local x,y,z=v[j],v[j+1],v[j+2];
//         y,z=rotate(y,z,qt);
//         x,z=rotate(x,z,qt*1.5);
//         x,y=inf(qt+p,x,y)
//         y-=1
//         add(vm,x);
//         add(vm,y);
//         add(vm,z);
//         z=z+5;
//         x=x*96/z+64;
//         y=y*96/z+64;
//         vt[j],vt[j+1],vt[j+1]=flr(x),flr(y),flr(z);
//     end
//     draw_model(p,qt,vertices,vt,vm,faces,f,tc,uv,{tex},true,16,false,256)
// end

typedef struct
{
    uint16_t diffuse;
    uint16_t * texture;
} Material;

typedef struct 
{
    uint16_t verticesCounter;
    uint16_t facesCounter;
    uint16_t * vertices;
    uint16_t * faces;
    uint16_t * texture_coords;
    uint16_t * uv;
    Material * mat;
} Mesh;