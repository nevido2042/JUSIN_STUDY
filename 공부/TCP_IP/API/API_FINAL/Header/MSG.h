#pragma once

#define MSG_SIZE 16

enum MSG_TYPE 
{
    ALLOC_ID, CREATE_PLAYER, DELETE_PLAYER,
    MOVE_RIGHT_PLAYER,
    MOVE_LEFT_PLAYER,
    STOP_RIGHT_PLAYER,
    STOP_LEFT_PLAYER
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

struct MSG_ID
{
    int type{ 0 };
    int iID{ 0 };
    int zero[2] = { 0, };
};

struct MSG_ALLOC_ID 
{
    int type = ALLOC_ID;
    int id = 0;
    int zero[2] = { 0, };
};

struct MSG_CREATE_PLAYER
{
    int type = CREATE_PLAYER;
    int id = 0;
    int x = 0;
    int y = 0;
};

struct MSG_DELETE_PLAYER
{
    int type = DELETE_PLAYER;
    int id = 0;
    int zero[2] = { 0, };
};

struct MSG_MOVE_RIGHT_PLAYER
{
    int type = MOVE_RIGHT_PLAYER;
    int id = 0;
    int x = 0;
    int y = 0;
};

struct MSG_MOVE_LEFT_PLAYER
{
    int type = MOVE_LEFT_PLAYER;
    int id = 0;
    int x = 0;
    int y = 0;
};

struct MSG_STOP_LEFT_PLAYER
{
    int type = STOP_LEFT_PLAYER;
    int id = 0;
    int x = 0;
    int y = 0;
};

struct MSG_STOP_RIGHT_PLAYER
{
    int type = STOP_RIGHT_PLAYER;
    int id = 0;
    int x = 0;
    int y = 0;
};
