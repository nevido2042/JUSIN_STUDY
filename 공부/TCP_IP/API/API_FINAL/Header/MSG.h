#pragma once

enum MSG_TYPE 
{
    ALLOC_ID, CREATE_STAR, DELETE_STAR, MOVE_STAR
};

struct CLIENT 
{
    int x = 0;
    int y = 0;
    int id = 0;
};

struct MSG_BASE
{
    int zero[4] = { 0, };
};

struct MSG_ALLOC_ID 
{
    int type = ALLOC_ID;
    int id = 0;
    int zero[2] = { 0, };
};

struct MSG_CREATE_STAR
{
    int type = CREATE_STAR;
    int id = 0;
    int x = 0;
    int y = 0;
};

struct MSG_DELETE_STAR 
{
    int type = DELETE_STAR;
    int id = 0;
    int zero[2] = { 0, };
};

struct MSG_MOVE_STAR 
{
    int type = MOVE_STAR;
    int id = 0;
    int x = 0;
    int y = 0;
};
