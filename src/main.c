#include "r3d.h"
#include "car.h"
#include <stdlib.h>
#include "raymath.h"

#define W_WIDTH 800
#define W_HEIGHT 600
#define W_TITLE "r3d demo"

#define CUBE_SPIN_SPEED 45

int main(void) {

  InitWindow(W_WIDTH, W_HEIGHT, W_TITLE);
  SetTargetFPS(60);

  R3D_Init(W_WIDTH, W_HEIGHT, 0);

  Model plane = {0};

  plane = LoadModelFromMesh(GenMeshPlane(1000, 1000, 1, 1));
  plane.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1;
  plane.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 1;
  plane.materials[0].maps[MATERIAL_MAP_METALNESS].value = 1;

  R3D_SetMaterialAlbedo(&plane.materials[0], NULL, (Color){0x10, 0x10, 0x10, 0xFF});

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

  Camera3D camera = {
    .position = (Vector3) {0, 1, 2},
    .target = (Vector3) {0, 0.5, 0},
    .up = (Vector3) {0, 1, 0},
    .fovy = 60,
  };

  R3D_Light light = R3D_CreateLight(R3D_LIGHT_SPOT);
  {
      R3D_LightLookAt(light, (Vector3) { 0, 10, 5 }, (Vector3) { 0 });
      R3D_SetLightActive(light, true);
      R3D_EnableShadow(light, 4096);
  }

  float cube_angle = 0;

  while(!WindowShouldClose()) {
    cube_angle = fmodf(cube_angle + CUBE_SPIN_SPEED * GetFrameTime(), 360);

    BeginDrawing();
    ClearBackground(GRAY);
    R3D_Begin(camera);

    R3D_DrawModel(plane, (Vector3) {0, -1, 0}, 1.0f);
    R3D_DrawModelEx(
      cube, 
      (Vector3) {0, 0.5, 0}, 
      (Vector3) {1, 1, 1}, 
      cube_angle, 
      (Vector3) {1, 1, 1}
    );

    R3D_End();
    EndDrawing();
  }

  UnloadModel(plane);
  UnloadModel(cube);
  R3D_Close();
  CloseWindow();
  return 0;
}