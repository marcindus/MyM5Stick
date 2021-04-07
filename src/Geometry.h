#pragma once

typedef struct point_3d
{
    double x;
    double y;
    double z;
} point_3d_t;

typedef struct line_3d
{
    point_3d_t start_point;
    point_3d_t end_point;
} line_3d_t;

typedef struct point_2d
{
    double x;
    double y;
} point_2d_t;



