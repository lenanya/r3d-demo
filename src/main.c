#include "r3d.h"
#include "car.h"
#include <stdlib.h>
#include "raymath.h"
#include <stdio.h>

#define W_WIDTH 800
#define W_HEIGHT 600
#define W_TITLE "r3d demo"

#define CUBE_SPIN_SPEED 45

int main(void) {

  InitWindow(W_WIDTH, W_HEIGHT, W_TITLE);
  SetTargetFPS(60);

  R3D_Init(W_WIDTH, W_HEIGHT, 0);

  Model floor = {0};

  floor = LoadModelFromMesh(GenMeshPlane(1000, 1000, 1, 1));
  floor.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1;
  floor.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 1;
  floor.materials[0].maps[MATERIAL_MAP_METALNESS].value = 0;

  R3D_SetMaterialAlbedo(&floor.materials[0], NULL, (Color){0x10, 0x10, 0x10, 0xFF});

  Model cube = {0};

  cube = LoadModelFromMesh(GenMeshCube(1, 1, 1));
  cube.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1;
  cube.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 1;
  cube.materials[0].maps[MATERIAL_MAP_METALNESS].value = 0;

  Image car = LoadImageFromMemory(".png", car_image, car_size);

  ImageFlipVertical(&car);

  Texture texture = LoadTextureFromImage(car);
  
  GenTextureMipmaps(&texture);
	SetTextureFilter(texture, TEXTURE_FILTER_ANISOTROPIC_4X);

  R3D_SetMaterialAlbedo(&cube.materials[0], &texture, WHITE);

  Model wall = LoadModelFromMesh(GenMeshCube(10, 5, 0.5));
  wall.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1;
  wall.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 1;
  wall.materials[0].maps[MATERIAL_MAP_METALNESS].value = 0;

  R3D_SetMaterialAlbedo(&wall.materials[0], &texture, WHITE);

  Model ceiling = LoadModelFromMesh(GenMeshCube(10, 1, 10));
  ceiling.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1;

  R3D_SetMaterialAlbedo(&ceiling.materials[0], &texture, GRAY);

  Model lamp = LoadModelFromMesh(GenMeshSphere(0.3, 64, 64));
  lamp.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1;

  R3D_SetMaterialEmission(&lamp.materials[0], NULL, WHITE, 0.5);

  Model lamp_line = LoadModelFromMesh(GenMeshCylinder(0.05, 0.5, 64));
  lamp_line.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1;
  lamp_line.materials[0].maps[MATERIAL_MAP_METALNESS].value = 1;

  R3D_SetMaterialAlbedo(&lamp_line.materials[0], NULL, GRAY);
  
  Model player = LoadModelFromMesh(GenMeshCylinder(0.5, 1.5, 64));

  Camera3D camera = {
    .position = (Vector3) {0, 2, 3},
    .target = (Vector3) {0, 1.5, 0},
    .up = (Vector3) {0, 1, 0},
    .fovy = 90,
  };

  R3D_Light light = R3D_CreateLight(R3D_LIGHT_OMNI);
  {
    R3D_LightLookAt(light, (Vector3) { 0, 4.5, 0 }, (Vector3) { 0 });
    R3D_SetLightActive(light, true);
    R3D_EnableShadow(light, 4096);
  }

  float cube_angle = 0;

  SetMousePosition(W_WIDTH / 2, W_HEIGHT / 2);
  HideCursor();

  while(!WindowShouldClose()) {
    cube_angle = fmodf(cube_angle + CUBE_SPIN_SPEED * GetFrameTime(), 360);

    UpdateCamera(&camera, CAMERA_FIRST_PERSON);

    if(camera.position.x >= 4.49) {
      camera.position.x = 4.49;
    } 
    if(camera.position.x <= -4.49) {
      camera.position.x = -4.49;
    }
    if(camera.position.z >= 4.49) {
      camera.position.z = 4.49;
    }
    if(camera.position.z <= -4.49) {
      camera.position.z = -4.49;
    }

    SetMousePosition(W_WIDTH / 2, W_HEIGHT / 2);

    BeginDrawing();
    ClearBackground(RED);
    R3D_Begin(camera);

    R3D_DrawModel(floor, 
      (Vector3) {0, 0, 0}, 
      1.0f
    );

    R3D_DrawModel(
      wall, 
      (Vector3) {0, 2.5, -5}, 
      1
    );

    R3D_DrawModelEx(
      wall, 
      (Vector3) {5, 2.5, 0},
      (Vector3) {0, 1, 0},
      90,
      (Vector3) {1, 1, 1}
    ); 

    R3D_DrawModelEx(
      wall, 
      (Vector3) {-5, 2.5, 0},
      (Vector3) {0, 1, 0},
      90,
      (Vector3) {1, 1, 1}
    );

    R3D_DrawModelEx(
      wall, 
      (Vector3) {0, 2.5, 5},
      (Vector3) {0, 1, 0},
      0,
      (Vector3) {1, 1, 1}
    );

    R3D_DrawModelEx(
      cube, 
      (Vector3) {0, 1.5, 0}, 
      (Vector3) {1, 1, 1}, 
      cube_angle, 
      (Vector3) {1, 1, 1}
    );

    R3D_DrawModel(
      ceiling,
      (Vector3) {0, 5.5, 0},
      1
    );

    R3D_DrawModel(
      lamp,
      (Vector3) {0, 4.5, 0},
      1
    );

    R3D_DrawModel(
      lamp_line,
      (Vector3) {0, 4.5, 0},
      1
    );

    R3D_DrawModel(
      player,
      (Vector3) {camera.position.x, 0.75, camera.position.z},
      1
    );  

    R3D_End();
    EndDrawing();
  }

  UnloadModel(floor);
  UnloadModel(cube);
  R3D_Close();
  CloseWindow();
  return 0;
}