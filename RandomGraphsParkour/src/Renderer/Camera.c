#include "Renderer/Camera.h"
#include "ILDA_matrix/ILDA_matrix.h"

void RGP_CameraInit(RGP_2DCamera_t* cam, float near, float far, float width, float height)
{
    RGP_ASSERT(cam != NULL, "Camera must not be NULL");

    cam->far = far;
    cam->near = near;
    cam->zoom = 1.0f;

    RGP_CameraResizeBound(cam, width, height);

    RGP_LOG_SUCCESS("Camera created");
}

void RGP_CameraShutdown(RGP_2DCamera_t* cam)
{
    RGP_ASSERT(cam != NULL, "Camera must not be NULL");

    RGP_LOG_SUCCESS("Camera destroyed");
}

void RGP_CameraZoom(RGP_2DCamera_t* cam, float zoomValue)
{
    RGP_ASSERT(cam != NULL, "Camera must not be NULL");

    cam->zoom = ILDA_clamp(zoomValue, 0.0f, 10.f);
    RGP_CameraUpdateView(cam);
}

void RGP_CameraUpdateView(RGP_2DCamera_t* cam)
{
    RGP_ASSERT(cam != NULL, "Camera must not be NULL");

    cam->projection = ILDA_ortho_r(cam->aspectRatio * cam->zoom, -cam->aspectRatio * cam->zoom, -cam->zoom, cam->zoom, cam->near, cam->far);

    ILDA_vector3f up = {0.0f, 1.0f, 0.0f};
    ILDA_vector3f target = { .x = cam->position.x, .y = cam->position.y, .z = cam->position.z + 1 };
    cam->view = ILDA_look_at(&cam->position, &target, &up);
}

void RGP_CameraUpdatePosition(RGP_2DCamera_t* cam, ILDA_vector3f* position)
{
    RGP_ASSERT(cam != NULL && position != NULL, "Camera and location must not be NULL");
    cam->position = *position;
    RGP_CameraUpdateView(cam);
}

void RGP_CameraMove(RGP_2DCamera_t* cam, ILDA_vector3f* position)
{
    RGP_ASSERT(cam != NULL && position != NULL, "Camera and location must not be NULL");
    ILDA_vector3f_add(&cam->position, position);
    RGP_CameraUpdateView(cam);
}

void RGP_CameraResizeBound(RGP_2DCamera_t* cam, float width, float height)
{
    RGP_ASSERT(cam != NULL, "Camera must not be NULL");

    cam->width = width;
    cam->height = height;
    cam->aspectRatio = width / height;
    RGP_CameraUpdateView(cam);
}
